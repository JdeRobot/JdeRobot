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

#include "controller.h"

namespace bgfglab {
  class Controller::PImpl{
  public:
    PImpl()
      : running(true) {}
    bool running;
  };

  Controller::Controller(gbxutilacfr::Tracer& tracer, Model &m)
    : _tracer(tracer), _model(m), _views(), pImpl(new PImpl()) {}

  Controller::~Controller() {
    std::vector<ViewPtr>::iterator v_it;
    for (v_it = _views.begin();
	 v_it != _views.end(); v_it++)
      _model.deleteObserver(*v_it);//jderobotutil::ObserverPtr(view));
  }

  void Controller::setBGModel(const std::string modelName, const jderobotutil::ParamDict& param) throw() { 
    _model.setBGModel(modelName,param);
  }

  void Controller::addView(ViewPtr v) throw(){
    _views.push_back(v);//FIXME: check for duplicated views!?
    _model.addObserver(v);
  }

  void Controller::exit() throw(){
    pImpl->running = false;
  }
} /*namespace*/

