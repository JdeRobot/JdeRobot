/*Example schema in C++*/
#include <random_iface.h>
#include <loader.h>
#include <schema.h>
#include <iostream>
#include <cstdlib>

class sC: public Schema, public Random {
public:
  sC(const int sid, const int father_sid)
    : Schema(sid,father_sid), Random() {}

  /*schema base*/
  virtual int init() {
    std::cerr << "executing init on schema sC(" << this->sid << ")...\n";
    return 0;
  }

  virtual void* cast(const char* interface_name) {
    return dynamic_cast<Random*>(this);
  }

  virtual float get_random() {
    std::cerr << "call get_random() inside sC(" << this->sid <<  ")\n";
    return (float)rand();
  }

  virtual void set_seed(const int seed) {
    std::cerr << "call set_seed(" << seed
	      << ") inside sC(" << this->sid <<  ")\n";
    srand(seed);
  }
};


/*sC ctor*/
Schema* new_sC(Schema_reg* sr, const int sid, const int father_sid) {
  return new sC(sid,father_sid);
}

/*sC dtor*/
void delete_sC(Schema_reg* sr, Schema* s) {
  delete s;
}

/*register schema definition*/
static Schema_reg sC_reg("sC",
			 RANDOM_IFACE_NAME,
			 &new_sC,&delete_sC);
static int loaded = Schema_reg::add(&sC_reg);
