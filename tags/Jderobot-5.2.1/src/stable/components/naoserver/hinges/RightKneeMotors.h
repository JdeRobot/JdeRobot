/*
 *  Copyright (C) 1997-2013 JDERobot Developers Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 *  Authors : Borja Menéndez <borjamonserrano@gmail.com>
 *
 */

#ifndef RIGHTKNEEMOTORS_H
#define RIGHTKNEEMOTORS_H

#include "HingeMotors.h"

#include "Singleton.h"

class RightKneeMotors : public Singleton<RightKneeMotors>, public HingeMotors {
    public:
        // Constructor (overriding)
        RightKneeMotors ();
};
#endif // RIGHTKNEEMOTORS_H