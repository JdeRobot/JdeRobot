#ifndef SCHEMA_H
#define SCHEMA_H

#define MAX_NAMES 128 /*names maximun lenght*/

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Schema* Schema_p;
typedef int (*Schema_init_f)(Schema_p const s);
typedef void* (*Schema_cast_f)(Schema_p const s,
			       const char* interface_name);

typedef struct Schema{
  int sid;
  int father_sid;
  Schema_init_f init_cb;
  void* init_cbdata;
  Schema_cast_f cast_cb;
  void* cast_cbdata;
  void* private_data;
#ifdef __cplusplus
  Schema(const int sid, const int father_sid);
  Schema(const int sid, const int father_sid,
	 Schema_init_f const init_cb,void* const init_cbdata,
	 Schema_cast_f const cast_cb,
	 void* const cast_cbdata,
	 void* const private_data = 0);
  virtual ~Schema();
  virtual int init() = 0;
  virtual void* cast(const char* interface_name) = 0;
private:
  static int sinit(Schema* const s);
  static void* scast(Schema* const s, const char* interface_name);
#endif /*__cplusplus*/
}Schema;

Schema* new_Schema(const int sid, const int father_sid,
		   Schema_init_f const init_cb,void* const init_cbdata,
		   Schema_cast_f const cast_cb,
		   void* const cast_cbdata,
		   void* const private_data = 0);
  //Schema* copy_Schema(Schema* dst, const Schema* src);
void delete_Schema(Schema* const s);

/*Schema API*/
int Schema_init(Schema* const s);
void* Schema_cast(Schema* const s, const char* interface_name);

#ifdef __cplusplus
} /*extern "C"*/

// /*
//   SchemaWrap can wrap a Schema* struct and call
//   its methods in C++ fashion
// */
// class SchemaWrap {
// public:
//   SchemaWrap(Schema* const s);
//   virtual ~SchemaWrap();
//   virtual int init();
//   virtual void* cast(char* const interface_name);
//   operator Schema& ();
//   const int& sid;
//   const int& father_sid;
// protected:
//   Schema* const wrapped_s;
// };


// /*
//   SchemaBase is the base class for Schema implementations.
//   It is an abstract class so it must be inherited to be used.
// */
// class SchemaBase: public SchemaWrap {
// public:
//   SchemaBase(const int sid, const int father_sid);
//   virtual ~SchemaBase();
//   virtual int init() = 0;
//   virtual void* cast(char* const interface_name) = 0;
// private:
//   static int sinit(Schema* const s);
//   static void* scast(Schema* const s, char* const interface_name);
// };
#endif /*__cplusplus*/
#endif /*SCHEMA_H*/
