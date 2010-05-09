/*
 *
 *  Copyright (C) 1997-2008 JDE Developers Team
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
 *  Authors : Jose Antonio Santos Cadenas <santoscadenas@gmail.com>
 *            Jose Maria Cañas <jmplaza@gsyc.escet.urjc.es>
 *
 *
 */

#ifndef __GRAPHICS_GTK_H__
#define __GRAPHICS_GTK_H__

#include <glade/glade-xml.h>

#ifndef __GUI_DISPLAY
#define __GUI_DISPLAY
/**
 * Generic display refresh callback function's type definition
 * @return void
 */
typedef void (*guidisplay)(void);

#endif

#ifndef __REGISTER_DISPLAY
#define __REGISTER_DISPLAY
/**
 * Type definition of the generic suscribe function to display callback
 * @param guidisplay The display callback fucntion implemented by the schema
 * @return 1 if the callback was registered correctly, othewise 0
 */
typedef int (*registerdisplay)(guidisplay f);

#endif

#ifndef __DELETE_DISPLAY
#define __DELETE_DISPLAY
/**
 * Type definition of the generic suscribe function to display callback
 * @param guidisplay The display callback fucntion implemented by the schema
 * @return 1 if the callback was unregistered correctly, othewise 0
 */
typedef int (*deletedisplay)(guidisplay f);

#endif

/**
 * Search in the path the .glade file and loads it
 * @param file_name The name of the .glade file
 * @returns the newly created GladeXML object, or NULL on failure.
 */
typedef GladeXML* (*loadglade) (char * file_name);

#endif
