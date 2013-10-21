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

#include <colorspaces/imagecv.h>
#include <jderobot/image.h>
#include <tr1/memory>

namespace colorspacesice{
  class Image: public colorspaces::Image {
  public:
    Image(Image& i);
    /**
     * Creates a Image from ImageData.
     */
    Image(const jderobot::ImageDataPtr& img);

    /**
     * Gets a ImageData representation for this image.
     * Image data should be continuous, otherwise returned pointer will be 0.
     * Data is not copied if this instance was created from a ImageData, the same instance will be returned with timeStamp updated to now
     */
    operator jderobot::ImageDataPtr() const;
  private:
    mutable jderobot::ImageDataPtr imgImageData;
  };
  typedef std::tr1::shared_ptr<Image> ImagePtr;

} //colorspacesice

#endif //IMAGE_COLORSPACESICE_H
