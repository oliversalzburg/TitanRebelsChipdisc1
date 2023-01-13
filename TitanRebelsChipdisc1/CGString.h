/********************************************
 * CGString.h - Dynamic String Class        *
 * (c) Oliver Salzburg - CPP Studios 2004   *
 ********************************************/

#pragma once

//Forward declarations
class CGStringBase;
typedef const class CGStringBase CGStringConst;
class CGString;
template <int SIZE> class CGStringStatic;

class CGCharSet {
protected:
  bool m_Flags[256];

public:
  //some standard character sets
  static CGCharSet Letters;
  static CGCharSet Numbers;
  static CGCharSet Standard;
  
  //Construct from a string.
  CGCharSet(const char* str=NULL) {
    memset(m_Flags, 0, sizeof(m_Flags));
    AddChars(str);
  }

  //Construct from exisiting set plus optional string.
  CGCharSet(CGCharSet &src, const char* str=NULL) {
    *this=src;
    AddChars(str);
  }

  //Add characters from a string.
  void AddChars(const char* str) {
    if (str==NULL) return;
    while (*str != '\0') {
      m_Flags[(unsigned char)(*str)]=true;
      str++;
    }
  }

  //Returns whether a certain character is included in the set.
  bool Includes(long chr) const {
    if (chr>255 || chr<0) return false;
    return m_Flags[chr];
  }
};

//Base class for all strings.
class CGStringBase {
  friend class CGString;
  //friend class CGStringStatic;
protected:
  
  //The pointer to our string data.
  char* m_Ptr;
  
  //The amount of memory reserved for a dynamic string object.
  long m_Reserved;
  
  //The length of the real string.
  long m_Length;
  
  //Static strings will not use memory allocation.
  bool m_IsStatic;

  //A global empty string. We set the m_Ptr pointer to this address when it should be empty.
  static char m_Empty[];

public:
  //This should never be used.
  inline CGStringBase(void) {}

  //Construct a string from a char pointer (or a part of it).
  CGStringBase(const char *ptr, long start=0, long length=-1) {
    m_IsStatic=true;
    if (ptr != NULL) {
      if (length < 0) length=(long)strlen(ptr+start);
      if (length > 0) {
         m_Length = length;
         m_Reserved = length;
         m_Ptr=(char*)ptr+start;
         return;
      }
      m_Ptr=m_Empty;
      m_Reserved=0;
      m_Length=0;
    }
  }

  //Index operator for all integer types
  const char& operator [] (char i) const { return m_Ptr[i]; }
  char& operator [] (char i) { return m_Ptr[i]; }
  const char& operator [] (unsigned char i) const { return m_Ptr[i]; }
	char& operator [] (unsigned char i) { return m_Ptr[i]; }
	const char& operator [] (short i) const { return m_Ptr[i]; }
	char& operator [] (short i) { return m_Ptr[i]; }
	const char& operator [] (unsigned short i) const { return m_Ptr[i]; }
	char& operator [] (unsigned short i) { return m_Ptr[i]; }
	const char& operator [] (long i) const { return m_Ptr[i]; }
	char& operator [] (long i) { return m_Ptr[i]; }
	const char& operator [] (unsigned long i) const { return m_Ptr[i]; }
	char& operator [] (unsigned long i) { return m_Ptr[i]; }
	const char& operator [] (int i) const { return m_Ptr[i]; }
	char& operator [] (int i) { return m_Ptr[i]; }
	const char& operator [] (unsigned int i) const { return m_Ptr[i]; }
	char& operator [] (unsigned int i) { return m_Ptr[i]; }

	//Binary + operator for all integer types
	const char* operator + (char i) const { return m_Ptr+i; }
	char* operator + (char i) { return m_Ptr+i; }
	const char* operator + (unsigned char i) const { return m_Ptr+i; }
	char* operator + (unsigned char i) { return m_Ptr+i; }
	const char* operator + (short i) const { return m_Ptr+i; }
	char* operator + (short i) { return m_Ptr+i; }
	const char* operator + (unsigned short i) const { return m_Ptr+i; }
	char* operator + (unsigned short i) { return m_Ptr+i; }
	const char* operator + (long i) const { return m_Ptr+i; }
	char* operator + (long i) { return m_Ptr+i; }
	const char* operator + (unsigned long i) const { return m_Ptr+i; }
	char* operator + (unsigned long i) { return m_Ptr+i; }
	const char* operator + (int i) const { return m_Ptr+i; }
	char* operator + (int i) { return m_Ptr+i; }
	const char* operator + (unsigned int i) const { return m_Ptr+i; }
	char* operator + (unsigned int i) { return m_Ptr+i; }
		

	//Cast operator to char pointer
  inline operator const char*() const {
    return m_Ptr;
	}
	inline operator char*() {
		return m_Ptr;
	}

  //Cast operator to LPTSTR
	inline operator LPTSTR() const {
		return (LPTSTR)m_Ptr;
	}

  //The length of our string.
  long Length() const {
    return m_Length;
  }

  //The maximum length that this string can have.
  inline long MaxLength() const {
    return m_Reserved;
  }

  //Recalculate and return length.
  //If the data behind our char pointer was modified, we have to recalculate it's length.
  long RecalcLength() {
    m_Length=(long)strlen(m_Ptr);
    return m_Length;
  }

  //Returns the starting position of a desired substring.
  long FindString(CGStringConst& src, long start=0) const {
    if (start<0 || start>m_Length) return -1;
    char *p=strstr(m_Ptr+start, src.m_Ptr);
    if (p) return (long)(p - m_Ptr);
    return -1;
  }

  //Returns the first position of a desired character.
  long FindChar(long chr, long start=0) {
    if (start<0) start=0;
    if (start>=m_Length) return -1;
    while (start<m_Length) {
      if ((long)m_Ptr[start]==chr) break;
      start++;
    }
    return start;
  }

  //Returns the last position of a desired character.
  long FindCharRev(long chr, long start=0x7fffffff) const {
    if (start<0) return -1;
		if (start>=m_Length) start=m_Length-1;
		while (start>=0) {
			if ((long)m_Ptr[start]==chr) break;
			start--;
		}
		return start;
  }

  //Go through the string until a position is found which is included in the given set.
  long FindStart(CGCharSet &set, long start=0) const {
    while (start<m_Length && set.Includes(m_Ptr[start]))
      start++;
    return start;
  }

  //Go through the string until a position is found which is not included in the given set.
  long FindEnd(CGCharSet &set, long start=0) const {
    while (start<m_Length && !(set.Includes(m_Ptr[start])))
      start++;
    return start;
  }

  //Compare two strings.
  long Compare(CGStringConst &src) const {
    long j=min(m_Length, src.m_Length);
    char *p1=m_Ptr, *p2=src.m_Ptr;
    for (long i=0; i<j; i++, p1++, p2++) {
      if (*p1 < *p2) return -1;
      if (*p1 > *p2) return 1;
    }
    if (m_Length < src.m_Length) return -1;
		if (m_Length > src.m_Length) return 1;
		return 0;
  }

  //Return temporary substring object from start and length.
	CGString SubString(long start, long length=0x7fffffff) const;

	//Return temporary substring object from start and character set.
	CGString SubString(long start, CGCharSet &set) const;
	
	//Split into string array by a break character.
	long Split(long chr, CGArray<CGString>& OutArr) const;
};

//Class for strings with dynamic memory management.
class CGString : public CGStringBase {
public:
  //Default constructor.
  inline CGString() {
    m_IsStatic=false;
    m_Ptr=m_Empty;
    m_Reserved=0;
    m_Length=0;
  }

  //Constructor which also reserves memory (to avoid later reallocs).
  explicit CGString(long len) {
    m_IsStatic=false;
    m_Ptr=m_Empty;
    m_Reserved=0;
    m_Length=0;
    Reserve(len);
  }

  //Partial constructor used by a sub class for statically allocated strings.
	explicit inline CGString(long reserved, char *ptr) {
		m_IsStatic=true;
		m_Ptr=ptr;
		m_Reserved=reserved;
		ptr[reserved]='\0';
	}

  //Construct from a char pointer (or from a part of it).
  explicit CGString(const char *ptr, long start=0, long length=-1) {
    m_IsStatic=false;
    m_Ptr=m_Empty;
    m_Reserved=0;
    m_Length=0;

    if (ptr!=NULL) {
      if (length<0) length=(long)strlen(ptr+start);
      if (length>0) {
        Reserve(length);
        memcpy(m_Ptr, ptr+start, length);
        m_Ptr[length]='\0';
        m_Length=length;
      }
    }
  }

  //Construct from a baseclass instance.
  CGString (CGStringConst& src) {
    m_IsStatic=false;
		m_Ptr=m_Empty;
		m_Reserved=0;
		m_Length=0;
    if (src.m_Length) {
      Reserve(src.m_Length);
      memcpy(m_Ptr, src.m_Ptr, src.m_Length);
      m_Ptr[src.m_Length] = '\0';
      m_Length=src.m_Length;
    }
  }

  //Copy constructor.
  CGString (const CGString& src) {
    m_IsStatic=false;
    //Share data with other CGString object?
    if (!src.m_IsStatic && src.m_Ptr!= m_Empty) {
      //Increase reference counter.
      (*(((long*)src.m_Ptr)-1))++;
      m_Ptr=src.m_Ptr;
      m_Length=src.m_Length;
      m_Reserved=src.m_Reserved;
      return;
    }
    m_Ptr=m_Empty;
    m_Reserved=0;
    m_Length=0;
    if (src.m_Length) {
      Reserve(src.m_Length);
      memcpy(m_Ptr,src.m_Ptr, src.m_Length);
      m_Ptr[src.m_Length]='\0';
      m_Length=src.m_Length;
      return;
    }
  }

  //Construct from a number. (int)
  CGString (const int num) {
    m_Ptr=m_Empty;
    m_Length=0;
    m_Reserved=0;
		m_IsStatic=false;

    char *buffer;
    buffer=(char*)malloc(255);
    _itoa_s(num, buffer, 255, 10);
    if (strlen(buffer)) {
      Reserve((long)strlen(buffer));
      memcpy(m_Ptr, buffer, strlen(buffer));
      m_Ptr[strlen(buffer)] = '\0';
      m_Length=(long)strlen(buffer);
    }
    free(buffer);
  }

	 //Construct from a number. (float)
  CGString (const float num) {
    m_Ptr=m_Empty;
    m_Length=0;
    m_Reserved=0;
		m_IsStatic = false;

		Reserve(256);
		memset(m_Ptr,0,256);
		sprintf_s(m_Ptr,256,"%.3f",num);
    m_Length=(long)strlen(m_Ptr);
  }

  //Destructor
  ~CGString() {
    if (!m_IsStatic && m_Ptr!=m_Empty) {
      if (--(*(((long*)m_Ptr)-1)) <= 0)
				free(((long*)m_Ptr)-1);
			m_Ptr=m_Empty;
    }
  }

  //Assignment operator from other CGString
	CGString& operator = (const CGString& src) {
		//Share data with other CGString object?
		if (!m_IsStatic && !src.m_IsStatic && src.m_Ptr!=m_Empty) {
			//Increase reference counter.
      (*(((long*)src.m_Ptr)-1))++;
			if (m_Ptr!=m_Empty)
				if (--(*(((long*)m_Ptr)-1)) <= 0)
					free(((long*)m_Ptr)-1);
			m_Ptr=src.m_Ptr;			
			m_Length=src.m_Length;
			m_Reserved=src.m_Reserved;
			return *this;
		}
		if (src.m_Length) {	
			Reserve(src.m_Length);
			memcpy(m_Ptr, src.m_Ptr, src.m_Length);
			m_Ptr[src.m_Length]='\0';
			m_Length=src.m_Length;
		} else
			Empty();
		return *this;
	}

  //Assignment operator from char*
	CGString& operator = (const char* src) {
		if (!m_IsStatic && strlen(src)) {
			Empty();
      Reserve((long)strlen(src));
			memcpy(m_Ptr, src, strlen(src));
			m_Ptr[strlen(src)]='\0';
			m_Length=(long)strlen(src);
		} else
			Empty();
		return *this;
	}

  //Empty string
	void Empty() {
		if (m_IsStatic) {
			m_Ptr[0]='\0';
		} else {
			if (m_Ptr!=m_Empty)	{
				if (--(*(((long*)m_Ptr)-1)) <= 0)
					free(((long*)m_Ptr)-1);
				m_Ptr=m_Empty;
				m_Reserved=0;
			}
		  m_Length=0;
		}
  }

  //Reserve memory for a string with max Len characters
	long Reserve(long length=0, bool GrowExp=true);

  //Shrink reserved memory to minimum
	void Shrink() {
		if (m_Reserved <= m_Length) return;
		Reserve();
		m_Ptr=(char*)(((long*)realloc(((long*)m_Ptr)-1, sizeof(long)+m_Length+1))+1);
		m_Reserved=m_Length;
	}

  //Add (concatenate) to string
	CGString& Add(CGStringConst& src) {
		if (src.m_Length>0) {
			Reserve(m_Length + src.m_Length);
			long l = m_Reserved - m_Length;
			if (src.m_Length < l) l = src.m_Length;
			memcpy(m_Ptr + m_Length, src.m_Ptr, l);
			m_Length+=l;
			m_Ptr[m_Length]='\0';
		}
		return *this;
	}

  //Insert into string at certain position pos
	CGString& Insert(CGStringConst& src, long pos=0) {
		if (src.m_Length>0) {
			if (pos<0) pos=0;
			if (pos>m_Length) pos=m_Length;
			Reserve(m_Length + src.m_Length);
			long l = m_Reserved - m_Length;
			if (src.m_Length < l) l = src.m_Length;
			memmove(m_Ptr + pos + l, m_Ptr + pos, m_Length - pos);
			memcpy(m_Ptr + pos, src.m_Ptr, l);
			m_Length+=l;
			m_Ptr[m_Length]='\0';
		}
		return *this;
	}

  //Removes a certain part of the string
	CGString& Remove(long pos=0, long length=-1) {
		if (pos+length <= m_Length) {
			if (pos<0) pos=0;
			if (pos+length>m_Length) pos=m_Length-length;
			memmove(m_Ptr + pos, m_Ptr + pos + length, m_Length-pos);
      m_Length -= length;
      m_Ptr[m_Length]='\0';
		}
		return *this;
	}

  //Cut string before position pos
	CGString& CutBefore(long pos) {
		if (pos>=0 && pos<m_Length) {
			Reserve();
			m_Ptr[pos]='\0';
			m_Length=pos;
		}
		return *this;
	}

	//Cut string after position i (-1 is valid)
	CGString& CutAfter(long pos) {
		if (pos>=-1 && pos<(m_Length-1)) {
			Reserve();
			m_Ptr[pos+1]='\0';
			m_Length=pos+1;
		}
		return *this;
	}

  //Convert to lowercase
	CGString& Lower() {
		Reserve();
		char *p=m_Ptr;
		for (long i=0; i<m_Length; i++) {
			if (*p >= 'A' && *p <='Z') *p+=32;
			if ((unsigned char)*p >= 0xc0 && (unsigned char)*p <=0xde && (unsigned char)*p!=0xd7) *p=(char)((unsigned char)*p+32);
			p++;
		}
		return *this;
	}

	//Convert to uppercase
	CGString& Upper() {
		Reserve();
		char *p=m_Ptr;
		for (long i=0; i<m_Length; i++) {
			if (*p >= 'a' && *p <='z') *p-=32;
			if ((unsigned char)*p >= 0xe0 && (unsigned char)*p <=0xfe && (unsigned char)*p!=0xf7) *p=(char)((unsigned char)*p-32);
			p++;
		}
		return *this;
	}

	//Replace search string with replace string.
	CGString& Replace(const CGString search, const CGString replace) {
		long start=0;
		long hit=this->FindString(search,start);
		while (hit > -1) {
			this->Remove(hit,search.Length());
			this->Insert(replace, hit);
			start++;
			hit=this->FindString(search,start);
		}
		return *this;
	}
};

template <int SIZE>
class CGStringStatic : CGString {
protected:
  //The data for our string.
  char m_Data[SIZE+1];

public:
  //Default constructor
	inline CGStringStatic():
		CGString(SIZE+1, m_Data) {
		m_Length=0;
		m_Data[0]='\0';
	}

	//Constructor from base class
	inline CGStringStatic(CGStringConst& src):
		CNString(SIZE, m_Data) {
		m_Length=min(SIZE, src.m_Length);
		memcpy(m_Data, src.m_Ptr, m_Length);
		m_Data[m_Length]='\0';
	}

	//Constructor from CGString
	inline CGStringStatic(const CGString& src):
		CGString(SIZE, m_Data) {
		m_Length=min(SIZE, src.m_Length);
		memcpy(m_Data, src.m_Ptr, m_Length);
		m_Data[m_Length]='\0';
	}

	//Copy constructor
	inline CGStringStatic(const CGStringStatic& src):
		CGString(SIZE, m_Data) {
		m_Length=min(SIZE, src.m_Length);
		memcpy(m_Data, src.m_Ptr, m_Length);
		m_Data[m_Length]='\0';
	}

  //Construct from a number.
  CGStringStatic (const int num):
		CGString(SIZE, m_Data) {
    char *buffer;
    buffer=(char*)malloc(255);
    itoa(num, buffer, 10);
    if (strlen(buffer)) {
      memcpy(m_Data, buffer, strlen(buffer));
      m_Ptr[strlen(buffer)] = '\0';
      m_Length=(long)strlen(buffer);
    }
    free(buffer);
  }

	//Assignment operator from base class
	inline CGStringStatic& operator = (CGStringConst& src) {
		m_Length=min(SIZE, src.m_Length);
		memcpy(m_Data, src.m_Ptr, m_Length);
		m_Data[m_Length]='\0';
		return *this;
	}

	//Assignment operator from CGString
	inline CGStringStatic& operator = (const CGString& src) {
		m_Length=min(SIZE, src.m_Length);
		memcpy(m_Data, src.m_Ptr, m_Length);
		m_Data[m_Length]='\0';
		return *this;
	}

	//Assignment operator from other CGStringStatic
	inline CGStringStatic& operator = (const CGStringStatic& src) {
		m_Length=min(SIZE, src.m_Length);
		memcpy(m_Data, src.m_Ptr, m_Length);
		m_Data[m_Length]='\0';
		return *this;
	}
};