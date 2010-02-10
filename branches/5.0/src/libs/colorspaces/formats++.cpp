#include "formats++.h"

namespace colorspaces {
}

std::ostream &operator<<(std::ostream &stream, const Format& fmt){
  char str[256];

  Format_snprintf(str,256,&fmt);
  stream << str;
  return stream;
}
