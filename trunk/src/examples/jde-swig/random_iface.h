#ifndef RANDOM_H
#define RANDOM_H
#include <schema.h>

#define RANDOM_IFACE_NAME "random"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Random* Random_p;
typedef float (*Random_get_random_f)(Random_p const i);
typedef void (*Random_set_seed_f)(Random_p const i, const int seed);

/*Random struct*/
typedef struct Random {
  Random_get_random_f get_random_cb;
  void* get_random_cbdata;
  Random_set_seed_f set_seed_cb;
  void* set_seed_cbdata;
#ifdef __cplusplus
  Random();//new
  //Random(Schema* const s);//cast
  Random(Random_get_random_f const get_random_cb,
	 void* const get_random_cbdata,
	 Random_set_seed_f const set_seed_cb,
	 void* const set_seed_cbdata);
  virtual ~Random();
  virtual float get_random() = 0;
  virtual void set_seed(const int seed) = 0;
private:
  static float sget_random(Random* const i);
  static void sset_seed(Random* const i, const int seed);
#endif
}Random;

/*Random methods*/
Random* new_Random(Random_get_random_f const get_random_cb,
		   void* const get_random_cbdata,
		   Random_set_seed_f const set_seed_cb,
		   void* const set_seed_cbdata);
  //Random* copy_Random(Random* dst, const Random* src);
Random* cast_Random(Schema* const s);
void delete_Random(Random* i);

float Random_get_random(Random* const i);
void Random_set_seed(Random* const i, const int seed);

#ifdef __cplusplus
} /*extern "C"*/
#endif /*__cplusplus*/
#endif /*RANDOM_H*/
