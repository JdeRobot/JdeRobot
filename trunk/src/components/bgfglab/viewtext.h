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

#ifndef BGFGLAB_VIEWTEXT_H
#define BGFGLAB_VIEWTEXT_H

#include <gbxutilacfr/exceptions.h>
#include <colorspaces/colorspacesmm.h>
#include <memory>
#include <vector>
#include "controller.h" //class View
#include "bgmodelfactory.h"

namespace bgfglab {
  class ViewText: public View {
  public:
    ViewText(Controller &controller) throw();
    virtual ~ViewText() throw() {}
    virtual void update(const jderobotutil::Subject* o,
			jderobotutil::ObserverArg* arg = 0)
      throw(gbxutilacfr::Exception);
  private:
    /* void drawImage(Gtk::DrawingArea* drawingArea,  */
    /* 		   const colorspaces::Image& image);     */

    /* void setBGModel(BGModelFactoryPtr m); */
    

    /* class PImpl; */
    /* std::auto_ptr<PImpl> pImpl; */
  };
  
}//namespace

#endif /*BGFGLAB_VIEWTEXT_H*/
