#include <jde+/schematypes.h>
#include <cstring>
#include <cstdio>

const std::string special_sid_enum_str[] = {
  std::string("NULL_SID"),
  std::string("ROOT")
};

const std::string state_enum_str[] = {
  std::string("NULLSTATE"),
  std::string("SLEPT"),
  std::string("ALERT"),
  std::string("READY"),
  std::string("ACTIVE")
};

const std::string schema_type_enum_str[] = {
  std::string("PSCHEMA"),
  std::string("MSCHEMA")
};


std::string sid_to_str(const sid_t sid) {
  char sid_str[8];

  if (sid == NULL_SID || sid == ROOT)
    return special_sid_enum_str[sid];
  else {
    snprintf(sid_str,sizeof(sid_str)-1,"%d",sid);
    return std::string(sid_str);
  }
}

std::string state_to_str(const state_enum state) {
  return state_enum_str[state];
}

std::string schema_type_to_str(const schema_type_enum stype) {
  return schema_type_enum_str[stype];
}
