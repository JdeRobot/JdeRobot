#include <random_iface2.h>
#include <stdlib.h>
#include <assert.h>

Random* new_Random(Random_get_random_f const get_random_cb,
		   void* const get_random_cbdata,
		   Random_set_seed_f const set_seed_cb,
		   void* const set_seed_cbdata){
  Random* i;

  i = (Random*)malloc(sizeof(Random));
  i->get_random_cb = get_random_cb;
  i->get_random_cbdata = get_random_cbdata;
  i->set_seed_cb = set_seed_cb;
  i->set_seed_cbdata = set_seed_cbdata;
  return i;
}

Random* copy_Random(Random* dst, const Random* src) {
  dst->get_random_cb = src->get_random_cb;
  dst->get_random_cbdata = src->get_random_cbdata;
  dst->set_seed_cb = src->set_seed_cb;
  dst->set_seed_cbdata = src->set_seed_cbdata;
  return dst;
}

Random* cast_Random(Schema* const s) {
  return (Random*)Schema_cast(s,RANDOM_IFACE_NAME);
}

void delete_Random(Random* i) {
  free(i);
}

float Random_get_random(Random* const i) {
  assert(i != 0);
  assert(i->get_random_cb != 0);
  return i->get_random_cb(i);
}

void Random_set_seed(Random* const i, const int seed) {
  assert(i != 0);
  assert(i->set_seed_cb != 0);
  i->set_seed_cb(i,seed);
}

#ifdef __cplusplus
#include <iostream>

Random::Random()
  : get_random_cb(Random::sget_random), get_random_cbdata(this),
    set_seed_cb(Random::sset_seed),set_seed_cbdata(this) {}

Random::Random(Schema& s) {
  Random* r_casted = (Random*)s.cast(RANDOM_IFACE_NAME);
  
  copy_Random(this,r_casted);
}

Random::Random(Random_get_random_f const get_random_cb,
	       void* const get_random_cbdata,
	       Random_set_seed_f const set_seed_cb,
	       void* const set_seed_cbdata)
  : get_random_cb(get_random_cb), get_random_cbdata(get_random_cbdata),
    set_seed_cb(set_seed_cb),set_seed_cbdata(set_seed_cbdata) {}

Random::~Random() {}

float Random::get_random() {
  if (this->get_random_cb == Random::sget_random) {
    std::cerr << "Random::get_random you must overload this method in your subclass\n";
    return 0.0;
  }
  return Random_get_random(this);
}

void Random::set_seed(const int seed) {
  if (this->set_seed_cb != Random::sset_seed) {
    std::cerr << "Random::get_random you must overload this method in your subclass\n";
  }
  Random_set_seed(this,seed);
}

float Random::sget_random(Random* const i) {
  return i->get_random();
}

void Random::sset_seed(Random* const i, const int seed) {
  return i->set_seed(seed);
}
#endif /*__cplusplus*/
