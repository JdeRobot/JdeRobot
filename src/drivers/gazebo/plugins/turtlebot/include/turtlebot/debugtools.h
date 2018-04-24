/*
 *  Copyright (C) 1997-2015 JDE Developers Team
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
 *  CLONE of https://github.com/jderobot-varribas/gazeboplugin-quadrotor2/blob/2.1.0/include/quadrotor/interfaces/cameraibase.h
 *  Victor Arribas Raigadas <v.arribas.urjc@gmai.com>
 */


#ifndef DEBUGTOOLS_H
#define DEBUGTOOLS_H

#define DEBUG

#ifdef DEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif



#ifdef DEBUG
/* Debug Levels
 * Critical 1
 * Error 2
 * Warning 3
 * Info 4
 * Verbose 5
 */

#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL 4
#endif

#if DEBUG_LEVEL < 1
#define ONDEBUG_CRITICAL(x)
#else
#define ONDEBUG_CRITICAL(x) x
#endif

#if DEBUG_LEVEL < 2
#define ONDEBUG_ERROR(x)
#else
#define ONDEBUG_ERROR(x) x
#endif

#if DEBUG_LEVEL < 3
#define ONDEBUG_WARNING(x)
#else
#define ONDEBUG_WARNING(x) x
#endif

#if DEBUG_LEVEL < 4
#define ONDEBUG_INFO(x)
#else
#define ONDEBUG_INFO(x) x
#endif

#if DEBUG_LEVEL < 5
#define ONDEBUG_VERBOSE(x)
#else
#define ONDEBUG_VERBOSE(x) x
#endif

#endif

#endif // DEBUGTOOLS_H

