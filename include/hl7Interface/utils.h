#pragma once


#include <cassert>
//#include <type_traits>

///**
//	\brief 简单的锁，封装了CriticalSection
//*/
//class CSimpleLock
//{
//public:
//	CSimpleLock()
//	{
//		::InitializeCriticalSection(&m_Lock);
//	}
//
//	~CSimpleLock()
//	{
//		::DeleteCriticalSection(&m_Lock);
//	}
//
//    CSimpleLock(const CSimpleLock& lock)
//    {
//        ::InitializeCriticalSection(&m_Lock);
//    }
//    CSimpleLock& operator = (const CSimpleLock &lock)//使用赋值无效
//    {
//        return *this;
//    }
//	void Lock()
//	{
//		::EnterCriticalSection(&m_Lock);
//	}
//
//	void UnLock()
//	{
//		::LeaveCriticalSection(&m_Lock);
//	}
//
//	BOOL TestLock()
//	{
//		return ::TryEnterCriticalSection(&m_Lock);
//	}
//
//
//private:
//	CRITICAL_SECTION m_Lock;
//};
//
//
//
//class CSimpleAutoLock
//{
//	//这个类需要改进
//	//1:兼容临界区
//	//2:私有锁成员改为引用
//public:
//	CSimpleAutoLock(CSimpleLock * pSimpleLock)
//	{
//		m_pSimpleLock = pSimpleLock;
//		pSimpleLock->Lock();
//	}
//
//	~CSimpleAutoLock()
//	{
//		m_pSimpleLock->UnLock();
//	}
//
//private:
//	CSimpleLock * m_pSimpleLock;
//};



//for simple memory
template<typename T>
struct DefaultPtrDeleter
{
	inline void operator()(T  t) const
	{
		enum { type_must_be_complete = sizeof(t) };
		delete t;
	}
};

/**
	作用域内智能指针 code from chromium
*/
template <typename T, 
			typename D>
class ScopedPtrT{
public:

	typedef T element_type;

	// Constructor.  Defaults to intializing with NULL.
	// There is no way to create an uninitialized scoped_ptr.
	// The input parameter must be allocated with new.
	explicit ScopedPtrT(T* p = NULL) : ptr_(p) { }

	// Destructor.  If there is a C object, delete it.
	// We don't need to test ptr_ == NULL because C++ does that for us.
	~ScopedPtrT() {
		d(ptr_);
	}

	// Reset.  Deletes the current owned object, if any.
	// Then takes ownership of a new object, if given.
	// this->reset(this->get()) works.
	void Reset(T* p = NULL) {
		if (p != ptr_) {
			d(ptr_);
			ptr_ = p;
		}
	}

	// Accessors to get the owned object.
	// operator* and operator-> will assert() if there is no current object.
	T& operator*() const {
		assert(ptr_ != NULL);
		return *ptr_;
	}
	T* operator->() const  {
		assert(ptr_ != NULL);
		return ptr_;
	}
	T* Get() const { return ptr_; }

	// Comparison operators.
	// These return whether two scoped_ptr refer to the same object, not just to
	// two different but equal objects.
	bool operator==(T* p) const { return ptr_ == p; }
	bool operator!=(T* p) const { return ptr_ != p; }

	// Swap two scoped pointers.
	void Swap(ScopedPtrT& p2) {
		T* tmp = ptr_;
		ptr_ = p2.ptr_;
		p2.ptr_ = tmp;
	}

	// Release a pointer.
	// The return value is the current pointer held by this object.
	// If this object holds a NULL pointer, the return value is NULL.
	// After this operation, this object will hold a NULL pointer,
	// and will not own the object any more.
	T* Release()  {
		T* retVal = ptr_;
		ptr_ = NULL;
		return retVal;
	}

private:
	T* ptr_;
	D d;

	// Forbid comparison of scoped_ptr types.  If C2 != C, it totally doesn't
	// make sense, and if C2 == C, it still doesn't make sense because you should
	// never have the same object owned by two different scoped_ptrs.
	template <typename T2, typename D2> bool operator==(ScopedPtrT<T2, D2> const& p2) const;
	template <typename T2, typename D2> bool operator!=(ScopedPtrT<T2, D2> const& p2) const;

	// Disallow evil constructors
	ScopedPtrT(const ScopedPtrT&);
	void operator=(const ScopedPtrT&);
};

// Free functions
template <typename T,  typename D>
void Swap(ScopedPtrT<T, D>& p1, ScopedPtrT<T, D>& p2) {
	p1.Swap(p2);
}

template <typename T, typename D>
bool operator==(T* p1, const ScopedPtrT<T, D>& p2) {
	return p1 == p2.get();
}

template <typename T,typename D>
bool operator!=(T* p1, const ScopedPtrT<T, D>& p2) {
	return p1 != p2.get();
}


/**
	\brief VOID * 智能指针，VOID*指针转换成特定的POD类型
  \note 
*/
template< typename T>
class CVoidPSP
{
public:
	explicit CVoidPSP(void * p )
	{
    // check pod type
   // assert( std::tr1::is_pod<T>::value );
		assert( p != NULL );
		m_pBuffer = p;
	}


	~CVoidPSP() 
	{
		
	}

	void * get()
	{
		return m_pBuffer;
	}

	void * operator& ()
	{
		assert( m_pBuffer != NULL);
		return m_pBuffer;
	}

	operator T*() const 
	{
		assert( m_pBuffer != NULL);

		return (T *)m_pBuffer;
	}

	T* operator->() const 
	{
		assert( m_pBuffer != NULL);

		return (T *)m_pBuffer;
	}
private:
	void * m_pBuffer;
};
