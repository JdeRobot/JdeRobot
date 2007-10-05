#ifndef MESSAGE_PROCESSOR_HPP
#define MESSAGE_PROCESSOR_HPP
#include <cstdlib>
#include <queue>
#include <jde+/message.h>
#include <jde+/pointer.h>
#include <cc++/thread.h>
#ifdef CCXX_NAMESPACES
using ost::Mutex;
using ost::MutexLock;
#endif

/*Interface*/
class Message_Processor {
 public:
  virtual ~Message_Processor() throw() {}
  virtual void process(Pointer<Perception> p) throw() = 0;
  virtual void process(Pointer<Modulation> m) throw() = 0;
  virtual void process(Pointer<Arbitrate_Req> a) throw() = 0;
  virtual void process(Pointer<State_Change> s) throw()= 0;
  virtual void process(Pointer<New_State> n) throw() = 0;
};

template <class M>
class Message_Queue {
 public:
  Message_Queue() throw() {}

  Message_Queue(const Message_Queue& q) throw() {
    MutexLock l(queue_lock);
    MutexLock ql(q.queue_lock);

    queue = q.queue;
  }

  inline void push(M& m) throw() {
    MutexLock l(queue_lock);
    
    queue.push(m);
  }

  inline M& front() throw() {
    MutexLock l(queue_lock);
    
    return queue.front();
  }

  inline M& front() const throw() {
    MutexLock l(queue_lock);
    
    return queue.front();
  }

  inline void pop() throw() {
    MutexLock l(queue_lock);
    
    queue.pop();
  }

  inline bool empty() const throw() {
    MutexLock l(queue_lock);

    return queue.empty();
  }

  inline size_t size() const throw() {
    MutexLock l(queue_lock);

    return queue.size();
  }

  inline void clear() throw() {
    MutexLock l(queue_lock);

    queue.clear();
  }

 private:
  std::queue<M> queue;
  mutable Mutex queue_lock;
};


#endif /*MESSAGE_PROCESSOR_HPP*/
