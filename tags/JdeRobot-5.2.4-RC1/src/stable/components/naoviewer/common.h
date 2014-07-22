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

#ifndef NAOOPERATOR_COMMON_H
#define NAOOPERATOR_COMMON_H

typedef enum Hinge {
    PITCH_LEFT_SHOULDER,
    ROLL_LEFT_SHOULDER,
    YAWPITCH_LEFT_HIP,
    PITCH_LEFT_HIP,
    ROLL_LEFT_HIP,
    YAWPITCH_RIGHT_HIP,
    PITCH_RIGHT_HIP,
    ROLL_RIGHT_HIP,
    PITCH_RIGHT_SHOULDER,
    ROLL_RIGHT_SHOULDER,
    YAW_LEFT_ELBOW,
    ROLL_LEFT_ELBOW,
    PITCH_LEFT_KNEE,
    PITCH_RIGHT_KNEE,
    YAW_RIGHT_ELBOW,
    ROLL_RIGHT_ELBOW,
    PITCH_LEFT_ANKLE,
    ROLL_LEFT_ANKLE,
    PITCH_RIGHT_ANKLE,
    ROLL_RIGHT_ANKLE,
    HINGE_END
} Hinge;

typedef enum Motion {
    HEADMOTORS,
    HEADSPEED,
    LEFTSHOULDERMOTORS,
    RIGHTSHOULDERMOTORS,
    LEFTELBOWMOTORS,
    RIGHTELBOWMOTORS,
    LEFTHIPMOTORS,
    RIGHTHIPMOTORS,
    LEFTKNEEMOTORS,
    RIGHTKNEEMOTORS,
    LEFTANKLEMOTORS,
    RIGHTANKLEMOTORS,
    MOTORS
} Motion;

typedef enum Action {
    RIGHTKICK,
    LEFTKICK,
    STANDUP_BACK,
    STANDUP_FRONT,
    INTERCEPT,
    CHANGECAMERA,
    RESETNAOQI
} Action;

const float PI = 3.14159265;

# endif // NAOOPERATOR_COMMON_H
