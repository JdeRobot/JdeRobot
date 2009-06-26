#ifndef INTERFACE_H
#define INTERFACE_H
#include <jde.h>

#ifdef __cplusplus
extern "C" {
#endif

/*forward declaration for private data*/
typedef struct JDEInterface_p JDEInterface_p;
typedef struct JDEInterfacePrx_p JDEInterfacePrx_p;

typedef struct JDEInterface{
  char interface_name[MAX_NAME];
  JDESchema* supplier;
  JDEInterface_p* priv;
} JDEInterface;
  
typedef struct JDEInterfacePrx{
  JDEInterface* refers_to;
  JDESchema* user;
  JDEInterfacePrx_p* priv;
}JDEInterfacePrx;

#define INTERFACEDEF(typename,attr)\
typedef struct typename{\
  attr\
  JDEInterface* super;\
}typename;

#define IFPROXYDEF(typename,typenameprx) \
typedef struct typenameprx{ \
  typename* refers_to;/*if 0->backward compatibility*/ \
  JDEInterfacePrx* super; \
} typenameprx;

#define SUPER(i) ((i)->super)
#define PRX_REFERS_TO(prx) ((prx)->refers_to)
#define INTEFACE_NAME(i) ((i)->super->interface_name)
#define INTEFACE_NAME_PRX(prx) ((prx)->super->refers_to->interface_name)

/**
 * JDEInterface constructor
 * 
 * Allocate a new interface structure given the name and the schema
 * that supplies (implements) this interface.
 *
 * @param interface_name the name the interface will be known in the
 * system
 * @paran supplier the schema suplying (implementing) this interface
 * @return the new allocated interface
 *
 */
JDEInterface* new_JDEInterface(const char* interface_name,
			       JDESchema* const supplier);

/**
 * JDEInterface destructor
 *
 * It destroys the interface instance.
 * If this is NULL nothing happens.
 *
 * @param this interface to destroy
 * @return void
 */
void delete_JDEInterface(JDEInterface* const this);


/**
 * JDEInterfacePrx constructor
 *
 * It creates a proxy to access an interface with the given name.
 * Optionally, it is possible to give a pointer to a interface
 * structure to register it.
 * @param interface_name name to the referred interface
 * @param user schema using this proxy
 * @param refers_to interface instance, proxy will free it when the
 * last proxy is deleted
 * @return proxy instance
 */
JDEInterfacePrx* new_JDEInterfacePrx(const char* interface_name,
				     JDESchema* const user,
				     JDEInterface* const refers_to);

/**
 * JDEInterfacePrx destructor
 *
 * It destroys the proxy instance.
 * If this is NULL nothing happens.
 * @param this instance to destroy
 * @return void
 */
void delete_JDEInterfacePrx(JDEInterfacePrx* const this);

/**
 * Add a reference to an interface instance
 *
 * @param this interface instance with the new reference
 * @param referral proxy instance referring to this interface
 * @return void
 */
void JDEInterface_addref(JDEInterface* const this,
			 JDEInterfacePrx* const referral);

/**
 * Delete a reference to an interface instance
 *
 * @param this interface instance to delete the reference
 * @param referral proxy instance referring to this interface
 * @return void
 */
void JDEInterface_delref(JDEInterface* const this,
			 JDEInterfacePrx* const referral);

/**
 *  Reference count an interface instance has
 *
 * @param this interface instance
 * @return the reference count
 */
unsigned int JDEInterface_refcount(JDEInterface* const this);


/**
 *  Run the schema supplying this interface
 *
 * @param this interface proxy instance
 * @return void
 */
void JDEInterfacePrx_run(const JDEInterfacePrx* this);

/**
 *  Stop the schema supplying this interface
 *
 * @param this interface proxy instance
 * @return void
 */
void JDEInterfacePrx_stop(const JDEInterfacePrx* this);
  

  //void JDEInterfacePrx_atomic_start(JDEInterfacePrx* const this);
  //void JDEInterfacePrx_atomic_end(JDEInterfacePrx* const this);
  //void JDEInterfacePrx_invalidate(JDEInterfacePrx* const this);
  //int JDEInterfacePrx_isvalid(JDEInterfacePrx* const this);
#ifdef __cplusplus
}
#endif
#endif /*INTERFACE_H*/
