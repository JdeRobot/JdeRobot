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

#ifndef IMAGE_COLORSPACESICE_H
#define IMAGE_COLORSPACESICE_H

#include <colorspaces/image++.h>
#include <jderobot/varcolor.h>
#include <tr1/memory>

namespace colorspacesice{
  
  /*derive from Shared to use Handle*/
  class ImageDescription: public colorspaces::ImageDescriptionpp {
  public:
    ImageDescription(const int width,
		     const int height,
		     const int size,
		     const Format *fmt);
    ImageDescription(const ::ImageDescription& i);
    ImageDescription(const jderobot::ImageDescriptionPtr& iDesc);

    operator jderobot::ImageDescriptionPtr() const;//cast to jderobot::ImageDescriptionPtr
  };
  typedef std::tr1::shared_ptr<ImageDescription> ImageDescriptionPtr;

  class Image: public colorspaces::Imagepp {
  public:
    Image(const ::ImageDescription &desc, char *const data = 0);
    Image(::Image& i);
    Image(const jderobot::ImageDataPtr& img);

    operator jderobot::ImageDataPtr() const;//cast to jderobot::ImageDataPtr.it copies data
  private:
    /*if data come from ImageDataPtr we keep a reference to avoid deallocation while
     this instance is alreaady using the data. 
     FIXME: If data is changed reference is not released*/
    const jderobot::ImageDataPtr imgPtr;
  };
  typedef std::tr1::shared_ptr<Image> ImagePtr;

} //colorspacesice

#endif //IMAGE_COLORSPACESICE_H
