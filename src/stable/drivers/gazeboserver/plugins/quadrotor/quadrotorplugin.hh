//=================================================================================================
// Copyright (c) 2012, Johannes Meyer, TU Darmstadt
// All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the Flight Systems and Automatic Control group,
//       TU Darmstadt, nor the names of its contributors may be used to
//       endorse or promote products derived from this software without
//       specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//=================================================================================================

#ifndef QUADROTORPLUGIN_HH
#define QUADROTORPLUGIN_HH

#include <boost/bind.hpp>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/transport/transport.hh>

#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>

//ICE interfaces
//#include <quadrotor.h>
#include <pose3d.h>
#include <remoteConfig.h>
#include <camera.h>
#include <cmdvel.h>
#include <ardroneextra.h>

#include "quadrotor_parser.h"
#include "quadrotor_config.h"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "interfaces/navdatai.h"
#include <visionlib/colorspaces/colorspacesmm.h>

#include <sstream>

namespace gazebo {

void* thread_QuadrotorPluginICE ( void* v );
void* thread_state_controller (void *v);

enum {Unknown, Initialize, Flying, Landed, Landing, TakingOff, ToFixPoint};

class QuadrotorPlugin : public ModelPlugin {
public:
    QuadrotorPlugin();
    ~QuadrotorPlugin();

    void setVelocityCommand(const jderobot::CMDVelDataPtr&);
    common::Time getSimTime() const;
    void setConfiguration(QuadrotorConfig*);
    void configureDrone();
    void RunController();

    mutable pthread_mutex_t quadrotor_mtx;

    std::string cfgfile_QuadrotorPlugin;

    //State control members:
    unsigned int navi_state;
    bool m_isFlying, m_takeOff;
    double m_timeAfterTakeOff;
    std::string toggle_topic_;

    cv::Mat *image;
    pthread_mutex_t img_mutex;

protected:
    virtual void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf);
    virtual void Init();
    virtual void OnUpdate();
    virtual void Reset();

private:
    event::ConnectionPtr updateConnection;
    physics::WorldPtr world;
    physics::ModelPtr model;
    sdf::ElementPtr sdf;
    physics::LinkPtr link;

    math::Pose pose;
    math::Vector3 euler, velocity, acceleration, angular_velocity;

    double max_force_;
    double motion_small_noise_;
    double motion_drift_noise_;
    double motion_drift_noise_time_;
    std::string img_topic_;

	float eulerMax;
	
	//int set_navdata_demo_value;
	int cam_state;
	int altMax;
	int altMin;

	float vzMax;
	float yawSpeed;
	int isOutdoor;
	int withoutShell;
	int frameSize;

    class PIDController {
    public:
        PIDController();
        virtual ~PIDController();
        virtual void Load(sdf::ElementPtr _sdf, const std::string& prefix = "");

        double gain_p;
        double gain_i;
        double gain_d;
        double time_constant;
        double limit;

        double input;
        double dinput;
        double output;
        double p, i, d;

        double update(double input, double x, double dx, double dt);
        void reset();
    };

    struct Controllers {
        PIDController roll;
        PIDController pitch;
        PIDController yaw;
        PIDController velocity_x;
        PIDController velocity_y;
        PIDController velocity_z;
    } controllers_;

    jderobot::CMDVelDataPtr velocity_command_;

    math::Vector3 inertia;
    double mass;

    common::Time last_time;
    
    QuadrotorConfig *conf;
    transport::SubscriberPtr sub_;

    void ImageCallback(const boost::shared_ptr<const msgs::Image> &_msg);
    void AddNoiseToVel();

    class StateController {
    public:
        StateController(QuadrotorPlugin *adp) {
            this->adp = adp;
            this->last_time = adp->getSimTime();
            std::cout << "Constructor StateController\n";
        }

        void updateController() {
            // Get simulator time
            common::Time sim_time = adp->getSimTime();
            double dt = (sim_time - last_time).Double();

            // process robot operation information
            pthread_mutex_lock(&adp->quadrotor_mtx);
            if((adp->m_takeOff)&&(adp->navi_state == Landed)) {
                adp->m_timeAfterTakeOff = 0;
                adp->m_takeOff = false;
                adp->navi_state = TakingOff;
                std::cout << "State: TakingOff\n";
            } else if(adp->navi_state == TakingOff) {
                // take off phase need more power
                adp->m_timeAfterTakeOff += dt;
                if(adp->m_timeAfterTakeOff > 0.5) {
                    adp->navi_state = Flying;
                    std::cout << "State: Flying\n";
                }
                if(adp->m_isFlying == false) {
                    adp->m_timeAfterTakeOff = 0;
                    adp->navi_state = Landing;
                    std::cout << "State: Landing\n";
                }
            } else if((adp->navi_state == Flying) ||
                      (adp->navi_state == ToFixPoint)) {
                if(adp->m_isFlying == false) {
                    adp->m_timeAfterTakeOff = 0;
                    adp->navi_state = Landing;
                    std::cout << "State: Landing\n";
                }
            } else if(adp->navi_state == Landing) {
                adp->m_timeAfterTakeOff += dt;
                if(adp->m_timeAfterTakeOff > 1.5) {
                    adp->navi_state = Landed;
                    std::cout << "State: Landed\n";
                }
                if(adp->m_isFlying == true) {
                    adp->m_timeAfterTakeOff = 0;
                    adp->m_takeOff = false;
                    adp->navi_state = TakingOff;
                    std::cout << "State: TakingOff\n";
                }
            }
            pthread_mutex_unlock(&adp->quadrotor_mtx);
            // save last time stamp
            last_time = sim_time;
        }

    private:
        QuadrotorPlugin *adp;
        common::Time last_time;
    };

    StateController* sc;
};

} //gazebo
#endif
