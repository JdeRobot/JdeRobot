#ifndef CLASSFACTORY_HPP
#define CLASSFACTORY_HPP

/**
   Base class for factories
*/
class Classfactory {
public:
  /**
     Destructor.
  */
  virtual ~Classfactory() {}

  /**
     Create a new instance.
     \param i pointer to pointer where instance is attached
  */
  virtual void create_instance(void** i) const = 0;
};


#endif /*CLASSFACTORY_HPP*/
