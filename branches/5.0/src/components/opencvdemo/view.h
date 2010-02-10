#ifndef OPENCVDEMO_VIEW_H
#define OPENCVDEMO_VIEW_H

#include <jderobotutil/observer.h>

namespace opencvdemo {
  class privImpl;

  class View: public virtual jderobotutil::Observer
  {
  public:
    View();
    virtual ~View();
    virtual void update(jderobotutil::Subject *o, jderobotutil::ObserverArg *arg = 0);
  private:
    detail::privImpl priv;
  };
}//namespace

#endif /*OPENCVDEMO_VIEW_H*/
