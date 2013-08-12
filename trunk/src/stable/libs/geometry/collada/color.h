#ifndef COLOR_H
#define COLOR_H

#include <iostream>
namespace files_3D {
    class Color
    {
    public:
        Color();
        void set(float r, float g, float b, float a);


        public: float r, g, b, a;

        public: friend std::istream &operator>> (std::istream &_in, Color &_pt)
        {
          // Skip white spaces
          _in.setf(std::ios_base::skipws);
          _in >> _pt.r >> _pt.g >> _pt.b >> _pt.a;
          return _in;
        }

    };
}

#endif // COLOR_H
