#ifndef SINGLETON_H
#define SINGLETON_H
#include <cc++/thread.h>
#ifdef CCXX_NAMESPACES
using ost::Mutex;
using ost::MutexLock;
#endif

/**
   Singleton template class. A singleton is a class that only creates
   one instance, and every body use it.
   To use this template, and create a singleton class:
   \code
      class A : public Singleton<A> {
         <method1>
         <method2>
         ...
         friend class Singleton<A>;
      };
   \endcode
   To access the unique instance:
   \code
      A* i = A::instance();
   \endcode

   For convenience a ref count mechanish is implemented, so unique
   instance can be released and deleted when nobody use it. Next call
   to instance create a new instance.
*/
template <typename T>
class Singleton {
 public:
  /**
     Create the unique instance. Each time we call it ref count is incremented.
  */
  static T* instance() {
    MutexLock l(singleton_lock);

    if (myinstance == NULL)
      myinstance = new T;

    instance_count++;
    return myinstance;
  };

  /**
     Release the instance. If ref count go 0 delete it.
  */
  static void releaseInstance() {
    MutexLock l(singleton_lock);
    
    if (--instance_count == 0){
      delete myinstance;
      myinstance = NULL;
    }
  };

  /**
     Destroy instance.
  */
  static void destroyInstance() {
    MutexLock l(singleton_lock);
    
    delete myinstance;
    myinstance = NULL;
    instance_count = 0;
  }

 protected:
  Singleton() {};

  virtual ~Singleton() {};

 private:
  Singleton(const Singleton& source) {};

  static T* myinstance;
  static Mutex singleton_lock;
  static int instance_count;
};


template <typename T>
T* Singleton<T>::myinstance = NULL;

template <typename T>
Mutex Singleton<T>::singleton_lock;

template <typename T>
int Singleton<T>::instance_count = 0;


#endif /*SINGLETON_H*/
