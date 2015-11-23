/*
 *
 *  Copyright (C) 1997-2013 JDERobot Developers Team
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see http://www.gnu.org/licenses/. 
 *
 *  Authors : Roberto Calvo <rocapal [at] gsyc [dot] urjc [dot] es>
 *
 */

#include <iostream>
#include "viewer.h"
#include <progeo/progeo.h>

namespace rgbdCalibrator
{

  class GeoUtils
  {

  public:

    class Line3D
    {
    public:

      Line3D();

      Line3D(HPoint3D p, HPoint3D v);

      bool isPointInLine (HPoint3D p);
      
      HPoint3D getPointByZ (float z);
      

    private:
      // parametric equations
      // (x,y,z) = (x0,y0,z0) + t(x1,y1,z1)
      // 
      // (x0,y0,z0) is a point
      // (x1,y1,z1) is a director vector
      
      // catesian equations
      // x = x0 + t*x1
      // y = y0 + t*y1
      // z = z0 + t*z1

      HPoint3D point;
      HPoint3D vector;
      
      float t;

    };
    

    GeoUtils ();
    HPoint3D getVector (HPoint3D p0, HPoint3D p1);

  private:

  };



}
