/*
 *
 *  Copyright (C) 1997-2009 JDERobot Developers Team
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
 *  Authors : David Lobato Bravo <dav.lobato@gmail.com>
 *
 */


#include "model.h"
#include <cmath>
#include <algorithm>
#include <tr1/memory>

namespace motiondetection {
  Model::Model(gbxutilacfr::Tracer& tracer, const colorspaces::Image& initialImg) throw()
    : motionThreshold(3),algorithm(new OpticalFlowAlgorithm(tracer,initialImg)),_tracer(tracer) {}

  void Model::setImage(const colorspaces::Image& img) throw(){
    algorithm->calcMotion(img);
    notifyObservers();
  }
  
  const MotionItem2DSeq& Model::getMotionDetected() const throw (){
    return algorithm->getMotionDetected();
  }

  bool Model::isMotionDetected(MotionItem2D& max) const throw(){
    MotionItem2D item(algorithm->getMaxMotionDetected());

    max = item;
    return item.motion >= motionThreshold;
  }
}//namespace
