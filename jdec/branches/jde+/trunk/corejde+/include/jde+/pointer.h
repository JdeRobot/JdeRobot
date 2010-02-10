/*
  Based on http://ootips.org/yonat/4dev/smart-pointers.html 
  smart pointers implementations.
  Added thread-safe operations.
*/

#ifndef SMART_PTR_H
#define SMART_PTR_H
#include <stdexcept>
#include <cc++/thread.h>
#ifdef CCXX_NAMESPACES
using ost::AtomicCounter;
#endif

/**
  Maintains a count of references and auto-delete when it is 0
  Operations are thread-safe
*/
template <class X>
class Pointer {
 public:
  /**
     Constructor. Creates a counted smart-pointer
     \param p pointer managed. Auto delete when nobody points to it
     \param isarray used to indicate that memory pointed by p is an
     array allocated with new[], so must be released with delete[]
     \param counter used to replicate a pointer counter. Do not used
     unless you know what are doing.
  */
  explicit Pointer(X* p = 0, bool isarray = false, 
		   AtomicCounter* counter = 0) // allocate a new
                                                  // counter if not especified
    : ptr(p),isarray(isarray),itsCounter(counter) {
    if (p != 0 && counter == 0){
      itsCounter = new AtomicCounter();
      *itsCounter = 1;//init value
    }
  }
   
  /**
     Copy constructor. Increase reference counter to pointer r
     \param r pointer to copy
  */
  Pointer(const Pointer& r) throw() 
    : ptr(r.get()),isarray(r.isarray),itsCounter(0){
    acquire(r.itsCounter);
  }
   
  /** 
      Destructor. Release memory if las reference
  */
  virtual ~Pointer() {
    release();
  }
   
  /**
    Assign operator. Copy rigth operand into left
  */ 
  inline Pointer& operator=(const Pointer& r) {
    if (this != &r) {
      release();
      acquire(r.itsCounter);
      ptr = r.ptr;
      isarray = r.isarray;
    }
    return *this;
  }

  /**
     Dereference operand.
     \return data pointed
  */
  inline X& operator*()  const throw()   {
    return *ptr;
  }
  
  /**
     Dereference operand.
     \return pointer to data
  */
  inline X* operator->() const throw()   {
    return ptr;
  }
   
  /**
     Get pointer.
     \return pointer to data if not null
  */
  inline X* get()        const throw()   {
    return ptr ? ptr : 0;
  }
   
  /**
     Unique test.
     \return true if ref count is 1
  */
  inline bool unique()   const throw() {
    return (itsCounter ? (*itsCounter) == 1 : true);
  }

  /**
     Conversion operator
  */
  template<class X2> operator Pointer<X2> () {
    /*use same counter*/
    Pointer<X2> p2(this->ptr,this->isarray,this->itsCounter);
    ++(*this->itsCounter);
    return p2;
  }

 private:
  X* ptr;
  bool isarray;
  AtomicCounter* itsCounter;

  void acquire(AtomicCounter* c) throw() {
    /*increment the count*/

    itsCounter = c;
    if (itsCounter != 0)
      ++(*itsCounter);
  }

  void release() {
    /* decrement the count, delete if it is 0*/
    if (itsCounter != 0) {
      int counter = --(*itsCounter);

      if (counter < 0)
	throw std::runtime_error("Pointer: itsCounter become < 0 on release call");

      if (counter == 0){
	if (isarray)
	  delete[] ptr;
	else
	  delete ptr;
	delete itsCounter;
      }
      itsCounter = 0;
    }
  }
};


#endif /*SMART_PTR_H*/
