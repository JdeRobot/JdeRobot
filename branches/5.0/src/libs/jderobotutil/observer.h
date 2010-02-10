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
  typedef std::tr1::shared_ptr<ObserverArg> ObserverArgPtr;

  class Subject
    : public std::tr1::enable_shared_from_this<Subject>{
  public:
    Subject();
    virtual void addObserver(ObserverPtr o);
    virtual int countObservers() const;
    virtual void deleteObserver(ObserverPtr o);
    virtual void deleteObservers();
    virtual bool hasChanged() const;
    virtual void notifyObservers();
    virtual void notifyObservers(ObserverArgPtr arg);
  protected:
    virtual void clearChanged();
    virtual void setChanged();
  private:
    typedef std::list<ObserverPtr> ObserverList;
    ObserverList observers;
    bool changed;
  };
  typedef std::tr1::shared_ptr<Subject> SubjectPtr;
    

  class Observer{
  public:
    virtual void update(const SubjectPtr o, ObserverArgPtr arg = ObserverArgPtr()) = 0;
  };
} //namespace
    

#endif /*JDEROBOTUTIL_OBSERVER_H*/
