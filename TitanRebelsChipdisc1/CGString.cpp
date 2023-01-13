/********************************************
 * CGString.cpp - Dynamic String Class      *
 * (c) Oliver Salzburg - CPP Studios 2004   *
 ********************************************/

#include "StdAfx.h"
#include "Main.h"

//Some standard character sets
CGCharSet CGCharSet::Letters("ABCDEFGHIJKLMNOPQRSTUWVXYZabcdefghijklmnopqrstuvwxyz");
CGCharSet CGCharSet::Numbers=("0123456789");
CGCharSet CGCharSet::Standard=("ABCDEFGHIJKLMNOPQRSTUWVXYZabcdefghijklmnopqrstuvwxyz_0123456789");

//Global empty string
char CGStringBase::m_Empty[]="\0";

//Return temporary substring object from start and length
CGString CGStringBase::SubString(long Start, long Len) const {
	if (Start<0) {Len+=Start; Start=0;}
	if (Start>m_Length) {Start=m_Length; Len=0;}
	if (Len<0) Len=0;
	if (Len > m_Length-Start) Len=m_Length-Start;
	return CGString(m_Ptr, Start, Len);
}

//Return temporary substring object from start and character set
CGString CGStringBase::SubString(long Start, CGCharSet& Set) const {
	if (Start<0) Start=0;
	if (Start>m_Length) Start=m_Length;
	long End=FindEnd(Set, Start);
	return CGString(m_Ptr, Start, End-Start);	
}

//Split into string array by a break character
long CGStringBase::Split(long ChrBrk, CGArray<CGString>& OutArr) const {
	OutArr.DeleteAllElements();
	if (m_Length==0) return 0;
	char *p1=m_Ptr;
	char *p2=m_Ptr;
	for (long c=m_Length ; c>=0; c--) {
		if ((long)(*p2)==ChrBrk || c==0) {
			if (p2-p1 >= 0) {
				CGString* ps=OutArr.AddElements(1);
				(*ps).CGString::CGString(p1, 0, (long)(p2-p1));
			}
			p1=p2+1;
		}
		p2++;
	}
  
	return OutArr.Length();
}

//Reserve memory for a string with max Len characters
long CGString::Reserve(long length, bool GrowExp) {
	if (m_IsStatic)
		return (m_Reserved < length)? -1 : 0;

	// Make the string unique if shared
	if (m_Ptr!=m_Empty) {
		if ((*(((long*)m_Ptr)-1)) > 1) {
			if (length < m_Length) length=m_Length;
			char *pnew=(char*)(((long*)malloc(sizeof(long)+length+1))+1);
			memcpy(pnew, m_Ptr, m_Length);
			pnew[m_Length]='\0';
			(*(((long*)m_Ptr)-1))--;
			m_Ptr=pnew;
			(*(((long*)m_Ptr)-1))=1;
			m_Reserved=length;
			return 0;
		}
	}
	
	if (length > m_Reserved) {
		if (m_Ptr==m_Empty) {
			m_Ptr=(char*)(((long*)malloc(sizeof(long)+length+1))+1);
			m_Ptr[0]='\0';
			(*(((long*)m_Ptr)-1))=1;
		} else {
			if (GrowExp) length=length*2; // Grow exponential to reduce number of successive reallocs
			m_Ptr=(char*)(((long*)realloc(((long*)m_Ptr)-1, sizeof(long)+length+1))+1);
		}
		m_Reserved=length;
	}
	return 0;
}

