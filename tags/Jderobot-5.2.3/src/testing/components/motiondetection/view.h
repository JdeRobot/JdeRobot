#ifndef MOTIONDETECTION_VIEW_H
#define MOTIONDETECTION_VIEW_H

#include <gbxutilacfr/exceptions.h>
#include <jderobotutil/observer.h>
#include <tr1/memory>
#include "controller.h"

namespace motiondetection {
  class View: public jderobotutil::Observer
  {
  public:
    View(Controller &controller) throw();
    virtual ~View() throw();
    virtual void update(const jderobotutil::Subject* o,
			jderobotutil::ObserverArg* arg = 0)
      throw(gbxutilacfr::Exception);
    bool isVisible();
  private:
    class PImpl;
    std::tr1::shared_ptr<PImpl> pImpl;
  };
  typedef std::tr1::shared_ptr<View> ViewPtr;
}//namespace

#endif /*MOTIONDETECTION_VIEW_H*/
