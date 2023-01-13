#pragma once
#include <new.h>

template<class ClsType> class CGArray {
	
	protected:
	ClsType*	m_pFirst;						//Pointer to first element
	long			m_ElementCount;			//Number of elements
	long			m_ReservedCount;		//Number of elements we reserved memory for

	public:
	CGArray(long lSize = 0) {
		m_pFirst = NULL;
		m_ElementCount = 0;
		m_ReservedCount = 0;

		if (lSize > 0) AddElements(lSize);
	}

	~CGArray() {
		DeleteAllElements();
	}

	inline ClsType* First() const {
		return m_pFirst;
	}

	inline ClsType* Last() const {
		return m_pFirst + m_ElementCount + 1;
	}

	inline long Length() const {
		return m_ElementCount;
	}

	inline long Size() const {
		return m_ElementCount * sizeof(ClsType);
	}

	void ConstructElements(ClsType* p, long num_Elements) {		//Calls the constructor for n elements
		memset(p, 0, sizeof(ClsType) * num_Elements);
		for (; num_Elements--; p++) ::new((void*)p) ClsType;
	}

	void DestructElements(ClsType* p, long num_Elements) {	//Calls the destructor for n elements
		for (; num_Elements--; p++) p->~ClsType();
	}

	long ReserveMemory(long num_NewElementCount, bool GrowExp = true) {
		if (num_NewElementCount > m_ReservedCount) {
			if (m_pFirst == NULL) {																
				m_pFirst = (ClsType*)malloc(num_NewElementCount * sizeof(ClsType));				//There has no memory been reserved yet (use malloc)
			} else {
				if (GrowExp) num_NewElementCount *= 2;											//There's already memory reserved (use realloc)
				m_pFirst = (ClsType*)realloc(m_pFirst, num_NewElementCount * sizeof(ClsType));
			}
			m_ReservedCount = num_NewElementCount;
		}
		return 0;
	}

	ClsType* InsertElements(long pos, long num_Elements = 1) {
		ReserveMemory(m_ElementCount + num_Elements);						//For adding elements, we start off by reserving 
																														//memory for all needed elements
		if (pos < m_ElementCount) memmove(m_pFirst + pos + num_Elements, m_pFirst + pos, (m_ElementCount - pos) * sizeof(ClsType));
		ConstructElements(m_pFirst + pos, num_Elements);				//Construct new elements
		m_ElementCount += num_Elements;													//Increase element count

		return (m_pFirst + pos);
	}

	ClsType* AddElements(long num_Elements = 1) {
		return InsertElements(m_ElementCount, num_Elements);		//Call InsertElements to add new elements to the end of the array
	}
	
	void DeleteElements(long pos, long num_Elements = 1) {
		if (pos > m_ElementCount) return;
		DestructElements(m_pFirst + pos, num_Elements);					//Call destructor of all elements we want to delete
		if (pos + num_Elements < m_ElementCount)								//If the part to remove is not at the end of the array, shift the memory
			memmove(m_pFirst + pos, m_pFirst + pos + num_Elements, (m_ElementCount - (pos + num_Elements)) * sizeof(ClsType));
		m_ElementCount -= num_Elements;													//Decrease the length of the array to our new size
	}

	void DeleteAllElements() {
		if (m_pFirst != NULL) {
			DestructElements(m_pFirst, m_ElementCount);						//Call destructor for all elements
			free(m_pFirst);																				//Free all memory reserved for our array
			m_pFirst = NULL;																			//Set pointer to NULL
		}
		m_ElementCount = 0;																			//Reset variables
		m_ReservedCount = 0;
	}

	//Shrink reduces the allocated memory to the size of the array
	void Shrink() {
		if (m_ReservedCount > m_ElementCount) {
			m_pFirst = (ClsType*)realloc(m_pFirst, sizeof(ClsType) * m_ElementCount);
			m_ReservedCount = m_ElementCount;
		}
	}

	void TransferFrom(CGArray& source) {
		if (this == &source) return;
		DeleteAllElements();

		m_pFirst = source.m_pFirst;
		m_ElementCount = source.m_ElementCount;
		m_ReservedCount = source.m_ReservedCount;
		
		source.m_pFirst = NULL;
		source.m_ElementCount = 0;
		source.m_ReservedCount = 0;

		//source.CGArray<ClsType>::CGArray();
	}

	//Assignment operator copys data from another array and overwrites the current data
	CGArray& operator = (const CGArray& source) {
		if (this != &source) {
			DeleteAllElements();
			AddElements(source.m_ElementCount);
			for (long i = 0; i < m_ElementCount; i++) m_pFirst[i] = source[i];
		}
		return *this;
	}

	//Index operator returns reference to index
	inline ClsType& operator [] (short Idx) const {
		return *(m_pFirst+Idx);
	}

	inline ClsType& operator [] (unsigned short Idx) const {
		return *(m_pFirst+Idx);
	}

	inline ClsType& operator [] (long Idx) const {
		return *(m_pFirst+Idx);
	}

	inline ClsType& operator [] (unsigned long Idx) const {
		return *(m_pFirst+Idx);
	}

	inline ClsType& operator [] (int Idx) const {
		return *(m_pFirst+Idx);
	}

	inline ClsType& operator [] (unsigned int Idx) const {
		return *(m_pFirst+Idx);
	}

	//Binary + operator returns ptr to index
	inline ClsType* operator + (short Idx) const {
		return m_pFirst+Idx;
	}

	inline ClsType* operator + (unsigned short Idx) const {
		return m_pFirst+Idx;
	}

	inline ClsType* operator + (long Idx) const {
		return m_pFirst+Idx;
	}

	inline ClsType* operator + (unsigned long Idx) const {
		return m_pFirst+Idx;
	}

	inline ClsType* operator + (int Idx) const {
		return m_pFirst+Idx;
	}

	inline ClsType* operator + (unsigned int Idx) const {
		return m_pFirst+Idx;
	}
		
	//Cast operator to ClsType pointer
	inline operator ClsType*() const {
		return m_pFirst;
	}
};