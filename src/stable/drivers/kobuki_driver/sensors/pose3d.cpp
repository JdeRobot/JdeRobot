#include "pose3d.h"

Pose3D::Pose3D(KobukiManager *kobuki)
{
    this->kobuki = kobuki;
}

int Pose3D::setPose3DData(const jderobot::Pose3DDataPtr&  pose3dData,
                             const Ice::Current&)
{

}


jderobot::Pose3DDataPtr Pose3D::getPose3DData(const Ice::Current&)
{

    mutex.lock();
    jderobot::Pose3DDataPtr pose3dData(new jderobot::Pose3DData());
    pose3dData->x = kobuki->getRobotX();
    pose3dData->y = kobuki->getRobotY();
    pose3dData->robottheta = 0;
    mutex.unlock();
    return pose3dData;
}
