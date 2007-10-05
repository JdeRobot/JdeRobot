#ifndef SCHEMATYPES_H
#define SCHEMATYPES_H
#include <string>
#include <vector>


/**
   Schema identifier type.
*/
typedef unsigned int sid_t;

/**
   Sids with special meanings
*/
enum special_sid_enum{
  NULL_SID = 0,
  ROOT
};


/**
   Translate sid_t to string.
*/
std::string sid_to_str(const sid_t sid);

/*state type*/
enum state_enum{
  NULLSTATE,
  SLEPT,
  ALERT,
  READY,
  ACTIVE,
  NSTATES = 5
};

/**
   Translate state_enum to string.
*/
std::string state_to_str(const state_enum state);

/*schema type*/
enum schema_type_enum{
   PSCHEMA,
   MSCHEMA,
   NSCHEMATYPES
};

/**
   Translate schema_type_enum to string.
*/
std::string schema_type_to_str(const schema_type_enum stype);

/**
   Timeout type
*/
typedef unsigned long timeout_t;


#endif /*SCHEMATYPES_H*/
