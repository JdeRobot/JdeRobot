/*
 *
 *  Copyright (C) 1997-2010 JDERobot Developers Team
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

namespace bgfgview {
  Model::Model(gbxutilacfr::Tracer& tracer, const colorspaces::Image& initialImg) throw ()
    : _tracer(tracer), currentImage(initialImg), bgImage(initialImg), 
      fgMaskImage(initialImg.width, initialImg.height, colorspaces::ImageGRAY8:FORMAT_GRAY8),
      bg_model(0) {
  }

  Model::~Model(){
    if (bg_model != 0)
      cvReleaseBGStatModel(&bg_model);
  }
  
  void Model::updateBGModel(const colorspaces::Image& img) throw () {
    currentImage = img;
    if (bg_model != 0) {
      cvUpdateBGStatModel(&bg_model, &currentImage);
      /*data isn't copied*/
      bgImage = colorspaces::Image(cv::Mat(bg_model->background), colorspaces::ImageRGB888:FORMAT_RGB888);
      fgMaskImage = colorspaces::Image(cv::Mat(bg_model->foreground), colorspaces::ImageGRAY8:FORMAT_GRAY8);
    }
    notifyObservers();
  }

  void Model::setBGModel(const CvBGStatModel* newBGModel) throw()
  { 
    if (bg_model != 0)
      cvReleaseBGStatModel(&bg_model);

    bg_model = newBGModel;
    notifyObservers(); 
  }

}//namespace
