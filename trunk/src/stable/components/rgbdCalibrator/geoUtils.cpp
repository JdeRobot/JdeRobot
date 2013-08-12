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

#include "geoUtils.h"

namespace rgbdCalibrator
{

  GeoUtils::Line3D::Line3D ()
  {
  }

  GeoUtils::Line3D::Line3D (HPoint3D p, HPoint3D v)
  {
    point = p;
    vector = v;
  }

  bool GeoUtils::Line3D::isPointInLine (HPoint3D p)
  {
    return ((p.X - point.X)/vector.X) == 
      ((p.Y - point.Y)/vector.Y) ==
      ((p.Z - point.Z)/vector.Z);
  }

  HPoint3D GeoUtils::Line3D::getPointByZ (float z)
  {
    HPoint3D res;
    t = (z - point.Z)/ vector.Z;

    res.X = point.X + t*vector.X;
    res.Y = point.Y + t*vector.Y;
    res.Z = z;
    res.H = 1.0;
    
    return res;
  }


  GeoUtils::GeoUtils ()
  {
  }

  HPoint3D GeoUtils::getVector (HPoint3D p0, HPoint3D p1)
  {
    HPoint3D vector;
    vector.X = p0.X - p1.X;
    vector.Y = p0.Y - p1.Y;
    vector.Z = p0.Z - p1.Z;
    vector.H = 1.;

    return vector;
  }



}
