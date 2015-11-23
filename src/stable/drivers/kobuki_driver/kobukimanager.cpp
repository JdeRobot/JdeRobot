#include "kobukimanager.h"

KobukiManager::KobukiManager() :
    dx(0.0), dth(0.0),
    slot_stream_data(&KobukiManager::update, *this)
{
    kobuki::Parameters parameters;
    parameters.sigslots_namespace = "/kobuki";
    parameters.device_port = "/dev/ttyUSB0";
    parameters.enable_acceleration_limiter = false;
    kobuki.init(parameters);
    kobuki.enable();
    slot_stream_data.connect("/kobuki/stream_data");

    v = 0;
    w = 0;

}

KobukiManager::~KobukiManager()
{
  kobuki.setBaseControl(0,0); // linear_velocity, angular_velocity in (m/s), (rad/s)
  kobuki.disable();
}

void KobukiManager::setV(float v)
{
    mutex.lock();
    std::cout << "recieved v: " << v  << std::endl;
    this->v = v;
    mutex.unlock();
}

double KobukiManager::getRobotX()
{
    double result;
    mutex.lock();
    result = pose.x();
    mutex.unlock();
    return result;
}

double KobukiManager::getRobotY()
{
    double result;
    mutex.lock();
    result = pose.y();
    mutex.unlock();
    return result;
}

double KobukiManager::getRobotTheta()
{
    double result;
    mutex.lock();
    result = pose.heading();
    mutex.unlock();
    return result;
}

void KobukiManager::setW(float w)
{
    mutex.lock();
    std::cout << "recieved w: " << w  << std::endl;
    this->w = w;
    mutex.unlock();
}

void KobukiManager::processMotion()
{
    mutex.lock();
    kobuki.setBaseControl(v, w);
    mutex.unlock();
}


void KobukiManager::update()
{
    mutex.lock();
    ecl::Pose2D<double> pose_update;
    ecl::linear_algebra::Vector3d pose_update_rates;
    kobuki.updateOdometry(pose_update, pose_update_rates);
    pose *= pose_update;
    dx += pose_update.x();
    dth += pose_update.heading();
    mutex.unlock();
    processMotion();
    kobuki::Battery batery = kobuki.batteryStatus();
    kobuki::CoreSensors::Data d = kobuki.getCoreSensorData();

    kobuki::DockIR::Data ir = kobuki.getDockIRData();

    std::cout << "ir: " << ir.docking.size()<< std::endl;

    for(int  i =0; i < ir.docking.size(); i++){
        std::cout << (unsigned char)ir.docking[i] << ", ";
    }

    printf("--[%03d | %03d | %03d]\n", ir.docking[2], ir.docking[1], ir.docking[0] );
    std::cout << std::endl;

    std::cout << "Bumper: " << (int)d.bumper << std::endl;
    std::cout << "Bateria: " << batery.percent() << " " << batery.capacity << std::endl;
}

ecl::Pose2D<double> KobukiManager::getPose()
{
    ecl::Pose2D<double> pose_result;
    mutex.lock();
    pose_result = pose;
    mutex.unlock();
    return pose_result;

}
