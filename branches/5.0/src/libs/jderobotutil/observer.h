#ifndef JDEROBOTUTIL_OBSERVER_H
#define JDEROBOTUTIL_OBSERVER_H

#include <list>
#include <tr1/memory>

namespace jderobotutil{
  class Observer;
  typedef std::tr1::shared_ptr<Observer> ObserverPtr;

  class ObserverArg {
  public:
    ObserverArg(void *arg);
    void *arg_;
  };

  class Subject {
  public:
    Subject();
    virtual void addObserver(ObserverPtr o);
    virtual int countObservers() const;
    virtual void deleteObserver(ObserverPtr o);
    virtual void deleteObservers();
    virtual bool hasChanged() const;
    virtual void notifyObservers();
    virtual void notifyObservers(ObserverArg* arg);
  protected:
    virtual void clearChanged();
    virtual void setChanged();
  private:
    typedef std::list<ObserverPtr> ObserverList;
    ObserverList observers;
    bool changed;
  };
    

  class Observer{
  public:
    virtual void update(const Subject* o, ObserverArg* arg = 0) = 0;
  };
} //namespace
    

#endif /*JDEROBOTUTIL_OBSERVER_H*/
