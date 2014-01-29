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
 *  Author:     Borja Menéndez Moreno <b.menendez.moreno@gmail.com>
 *  Co-author:  José María Cañas Plaza <jmplaza@gsyc.es>
 *
 */

#include <boost/bind.hpp>
#include "gazebo.hh"
#include "physics/physics.hh"
#include "common/common.hh"
#include "transport/transport.hh"

// ICE utils includes
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>

// JDErobot general ice component includes
#include <jderobot/pose3dencoders.h>
#include <jderobot/pose3dmotors.h>

#include "common.h"

#ifndef POSELEFTHIP_H
#define POSELEFTHIP_H

namespace gazebo {

    void* thread_LeftHipICE ( void* v );

    class PoseLeftHip : public ModelPlugin {
    public:

        PoseLeftHip ();
        virtual void Load ( physics::ModelPtr _model, sdf::ElementPtr _sdf );
        virtual void Init ();

        std::string cfgfile_lefthip;
        pthread_mutex_t mutex_lefthipencoders;
        pthread_mutex_t mutex_lefthipmotors;

        struct lefthip_t {
            physics::JointPtr joint_yaw, joint_pitch, joint_roll;
            encoders_t encoders;
            motorsdata_t motorsdata;
            motorsparams_t motorsparams;
        };

        lefthip_t lefthip;

    private:
        void OnUpdate ();
        
        event::ConnectionPtr updateConnection;
        double stiffness;
        int cycle;
        
        float maxYaw, maxPitch, maxRoll, minYaw, minPitch, minRoll;
        std::string modelYaw, modelPitch, modelRoll;
    };
}

#endif // POSERIGHTHIP_H
