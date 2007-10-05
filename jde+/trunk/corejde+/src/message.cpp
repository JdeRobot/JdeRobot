#include <jde+/message.h>
#include <jde+/util.h>



Message::Message(const sid_t src,
		 const sid_t dst) 
  :src(src),dst(dst),tin(gettimeofdaysec()) {}

Message::~Message() {}

std::ostream& Message::put(std::ostream& os) const {
  os << '?' << src << ',' << dst;
    
  return os;
}

/*call put virtual method*/
std::ostream& operator<<(std::ostream& os, const Message& msg) {
  return msg.put(os);
}

Perception::Perception(const sid_t src,
		       const sid_t dst,
		       const std::string& name, 
		       const Data& perception)
  :Message(src,dst),
   name(name),data(perception) {}

Perception::~Perception() {}

std::ostream& Perception::put(std::ostream& os) const {
  os << 'P' << src << ',' << dst << ',' << name
     << '(' << data.get_size() << ')';
    
  return os;
}

Modulation::Modulation(const sid_t src,
		       const sid_t dst,
		       const std::string& name, 
		       const Data& modulation)
    :Message(src,dst),
     name(name),data(modulation) {}

Modulation::~Modulation() {}

std::ostream& Modulation::put(std::ostream& os) const {
  os << 'M' << src << ',' << dst << ',' << name
     << '(' << data.get_size() << ')';
    
  return os;
}

Arbitrate_Req::Arbitrate_Req(const sid_t src,
			     const sid_t dst)
  :Message(src,dst) {}

Arbitrate_Req::~Arbitrate_Req() {}

std::ostream& Arbitrate_Req::put(std::ostream& os) const {
  os << 'A' << src << ',' << dst;

  return os;
}

State_Change::State_Change(const sid_t src,
			   const sid_t dst,
			   const state_enum change_state)
  :Message(src,dst),
   state(change_state) {}

State_Change::~State_Change() {}

std::ostream& State_Change::put(std::ostream& os) const {
  std::string cstate = state_to_str(state);

  os << 'S' << src << ',' << dst << ',' << cstate;

  return os;
}

New_State::New_State(const sid_t src,
		     const sid_t dst,
		     const state_enum new_state)
  :Message(src,dst),
   state(new_state) {}

New_State::~New_State() {}

std::ostream& New_State::put(std::ostream& os) const {
  std::string nstate = state_to_str(state);

  os << 'N' << src << ',' << dst << ',' << nstate;

  return os;
}
