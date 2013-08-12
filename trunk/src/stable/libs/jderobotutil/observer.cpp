#include "observer.h"
#include <algorithm>

namespace jderobotutil {
  

  ObserverArg::ObserverArg(void *arg)
    : arg_(arg) {}

  Subject::Subject()
    : observers(0) {}

  void Subject::addObserver(ObserverPtr o){
    observers.push_back(o);
  }

  int Subject::countObservers() const{
    return observers.size();
  }

  void Subject::deleteObserver(ObserverPtr o){
    ObserverList::iterator it;

    if (observers.size() > 0){
      it = std::find(observers.begin(),observers.end(),o);
      if (it != observers.end())
	observers.erase(it);
    }
  }

  void Subject::deleteObservers(){
    observers.clear();
  }

  bool Subject::hasChanged() const{
    return changed;
  }

  void Subject::notifyObservers(){
    notifyObservers(0);
  }

  void Subject::notifyObservers(ObserverArg* arg){
    ObserverList::iterator it;

    for (it=observers.begin(); it != observers.end(); it++)
      (*it)->update(this,arg);
  }

  void Subject::clearChanged(){
    changed = false;
  }

  void Subject::setChanged(){
    changed = true;
  }
  
} //namespace
