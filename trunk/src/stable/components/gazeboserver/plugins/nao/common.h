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

#ifndef NAOCOMMON_H
#define NAOCOMMON_H

struct encoders_t {
    float x;
    float y;
    float z;
    float pan;
    float tilt;
    float roll;
    int clock;
    float maxPan;
    float maxTilt;
    float minPan;
    float minTilt;
};

struct motorsdata_t {
    float x;
    float y;
    float z;
    float pan;
    float tilt;
    float roll;
    float panSpeed;
    float tiltSpeed;
};

struct motorsparams_t {
    float maxPan;
    float minPan;
    float maxTilt;
    float minTilt;
    float maxPanSpeed;
    float maxTiltSpeed;
};

#endif // NAOCOMMON_H
