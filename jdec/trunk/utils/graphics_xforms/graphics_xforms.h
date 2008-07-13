/**
 *  jdec graphics_xforms driver header.
 *
 *  @file graphics_xforms.h
 *  @author Jose Antonio Santos Cadenas <santoscadenas@gmail.com>
 *  @version 1.0
 *  @date 2007-11-21
 */

#ifndef __GRAPHICS_XFORMS_H__
#define __GRAPHICS_XFORMS_H__
/**
 * Generic display function's type definition
 * @return void
 */
typedef void (*gui_function)(void);
/**
 * Generic display callback function's type definition
 * @param f function to register
 * @return 1 if the callback was successful, othewise 0
 */
typedef int (*callback)(gui_function f);

/**
 * Generic buttons callback function's type definition
 * @param obj Pointer to the objet wich generate an event.
 * @return void
 */
typedef void (*guibuttons)(void *obj);
/**
 * Generic display refresh callback function's type definition
 * @return void
 */
typedef void (*guidisplay)(void);

/**
 * Type definition of the generic suscribe function to buttons callback
 * @param guibuttons The buttons callback fucntion implemented by the schema
 * @return 1 if the callback was registered correctly, othewise 0
 */
typedef int (*registerbuttons)(guibuttons f);
/**
 * Type definition of the generic suscribe function to display callback
 * @param guidisplay The display callback fucntion implemented by the schema
 * @return 1 if the callback was registered correctly, othewise 0
 */
typedef int (*registerdisplay)(guidisplay f);
/**
 * Type definition of the generic unsuscribe function to buttons callback
 * @param guibuttons The buttons callback fucntion implemented by the schema
 * @return 1 if the callback was unregistered correctly, othewise 0
 */
typedef int (*deletebuttons)(guibuttons f);
/**
 * Type definition of the generic suscribe function to display callback
 * @param guidisplay The display callback fucntion implemented by the schema
 * @return 1 if the callback was unregistered correctly, othewise 0
 */
typedef int (*deletedisplay)(guidisplay f);
#endif
