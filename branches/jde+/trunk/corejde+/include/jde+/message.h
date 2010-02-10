#ifndef MESSAGE_HPP
#define MESSAGE_HPP
#include <jde+/data.h>
#include <jde+/schematypes.h>
#include <string>
#include <iostream>

class Message {
public:
  sid_t src;
  sid_t dst;
  double tin;
  double tout;

  Message(const sid_t src, const sid_t dst);
  virtual ~Message();
  virtual std::ostream& put(std::ostream& os) const;
};

std::ostream& operator<<(std::ostream& os, const Message& msg);

class Perception : public Message {
public:
  std::string name;
  Data data;

  Perception(const sid_t src,
	     const sid_t dst,
	     const std::string& name, const Data& perception = Data::Null);
  virtual ~Perception();
  virtual std::ostream& put(std::ostream& os) const;
};

class Modulation : public Message {
public:
  std::string name;
  Data data;

  Modulation(const sid_t src,
	     const sid_t dst,
	     const std::string& name, const Data& modulation = Data::Null);
  virtual ~Modulation();
  virtual std::ostream& put(std::ostream& os) const;
};

class  Arbitrate_Req : public Message {
public:

  Arbitrate_Req(const sid_t src,
		const sid_t dst);
  virtual ~Arbitrate_Req();
  virtual std::ostream& put(std::ostream& os) const;
};

class State_Change : public Message {
public:
  state_enum state;

  State_Change(const sid_t src,
	       const sid_t dst,
	       const state_enum change_state);
  virtual ~State_Change();
  virtual std::ostream& put(std::ostream& os) const;
};

class New_State : public Message {
public:
  state_enum state;

  New_State(const sid_t src,
	    const sid_t dst,
	    const state_enum new_state);
  virtual ~New_State();
  virtual std::ostream& put(std::ostream& os) const;
};

#endif /*MESSAGE_HPP*/
