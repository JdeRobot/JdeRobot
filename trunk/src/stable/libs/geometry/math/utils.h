#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <math.h>
#include <limits>

#include <boost/math/special_functions.hpp>

namespace math
{

    template<typename T>
    inline bool equal(const T &_a, const T &_b,
                      const T &_epsilon = 1e-6)
    {
      return std::fabs(_a - _b) <= _epsilon;
    }

    inline double parseFloat(const std::string& _input)
    {
      const char *p = _input.c_str();
      if (!*p || *p == '?')
        return std::numeric_limits<double>::quiet_NaN();
      int s = 1;
      while (*p == ' ')
        p++;

      if (*p == '-')
      {
        s = -1;
        p++;
      }

      double acc = 0;
      while (*p >= '0' && *p <= '9')
        acc = acc * 10 + *p++ - '0';

      if (*p == '.')
      {
        double k = 0.1;
        p++;
        while (*p >= '0' && *p <= '9')
        {
          acc += (*p++ - '0') * k;
          k *= 0.1;
        }
      }
      if (*p == 'e')
      {
        int es = 1;
        int f = 0;
        p++;
        if (*p == '-')
        {
          es = -1;
          p++;
        }
        else if (*p == '+')
        {
          es = 1;
          p++;
        }
        while (*p >= '0' && *p <= '9')
          f = f * 10 + *p++ - '0';

        acc *= pow(10, f*es);
      }

      if (*p)
      {
        std::cerr << "Invalid format[" << _input << "]\n";
        return 0.0;
      }
      return s * acc;
    }

    inline int parseInt(const std::string& _input)
        {
          const char *p = _input.c_str();
          if (!*p || *p == '?')
            return  std::numeric_limits<int>::quiet_NaN();

          int s = 1;
          while (*p == ' ')
            p++;

          if (*p == '-')
          {
            s = -1;
            p++;
          }

          double acc = 0;
          while (*p >= '0' && *p <= '9')
            acc = acc * 10 + *p++ - '0';

          if (*p)
          {
            std::cerr << "Invalid int numeric format[" << _input << "]\n";
            return 0.0;
          }

          return s * acc;
        }

    template<typename T>
    inline T precision(const T &_a, const unsigned int &_precision)
    {
      return boost::math::round(_a * pow(10, _precision)) / pow(10, _precision);
    }
}
#endif // UTILS_H
