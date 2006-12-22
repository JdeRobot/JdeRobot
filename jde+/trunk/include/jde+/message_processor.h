#ifndef MESSAGE_PROCESSOR_HPP
#define MESSAGE_PROCESSOR_HPP

#include <jde+/message.h>
#include <jde+/pointer.h>

/*Interface*/
class Message_Processor {
 public:
  virtual ~Message_Processor() {}
  virtual void process(Pointer<Perception> p) = 0;
  virtual void process(Pointer<Modulation> m) = 0;
  virtual void process(Pointer<Arbitrate_Req> a) = 0;
  virtual void process(Pointer<State_Change> s) = 0;
  virtual void process(Pointer<New_State> n) = 0;
};

#endif /*MESSAGE_PROCESSOR_HPP*/
