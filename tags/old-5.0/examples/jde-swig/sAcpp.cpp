/*Example schema written in C++*/
#include <random_iface.h>
#include <loader.h>
#include <schema.h>
#include <iostream>
#include <cstdlib>

class sA: public Schema {
public:
  sA(const int sid, const int father_sid)
    : Schema(sid,father_sid), seed(46486422),
      child(0),child_sr(0),child_random(0) {}

  virtual ~sA() {
    if (this->child) {
      delete this->child_random;
      this->child_sr->delete_instance(this->child);
    }
  }

  /*schema base*/
  virtual int init() {
    std::cerr << "executing init on schema " << this->sid << "...\n";
    
    if (this->child == 0) {
      this->child_sr = Schema_reg::search(RANDOM_IFACE_NAME);

      if (this->child_sr == 0) {
	std::cerr << "can't find child on schema " << this->sid
		  << "...\n";
	return 1;
      }
      this->child = this->child_sr->instance(this->sid*10,
					     this->sid);
      //this->child_random = new Random(this->child);
      this->child_random = cast_Random(this->child);
      std::cerr << "\tset_seed(" << this->seed << ") on child\n";
      this->child_random->set_seed(this->seed);
    }

    std::cerr << "\tget_random() from " << RANDOM_IFACE_NAME << ":"
	      << this->child_random->get_random() << "\n";
    return 0;
  }

  void* cast(const char* interface_name) {
    return 0;
  }
private:
  const int seed;
  Schema* child;
  Schema_reg* child_sr;
  Random* child_random;
};


/*sA ctor*/
Schema* new_sA(Schema_reg* sr, const int sid, const int father_sid) {
  return new sA(sid,father_sid);
}

/*sA dtor*/
void delete_sA(Schema_reg* sr, Schema* s) {
  delete s;
}


/*register schema definition*/
ADD_SCHEMA_REG("sA","null",&new_sA,&delete_sA,0)

