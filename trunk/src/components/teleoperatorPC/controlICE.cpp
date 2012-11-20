/*
 *  Copyright (C) 1997-2011 JDERobot Developers Team
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
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *  Authors : Maikel González <m.gonzalezbai@gmail.com>,
 *              José María Cañas <jmplaza@gsyc.es>
 *
 */

#include "controlICE.h"

controlICE::controlICE(SharedMemory *interfacesData) {
    this->interfacesData = interfacesData;

    interfacesData->motorsInterface.activated = FALSE;
    interfacesData->motorsInterface.checkInit = FALSE;
    interfacesData->motorsInterface.checkEnd = FALSE;
    interfacesData->encodersInterface.activated = FALSE;
    interfacesData->encodersInterface.checkInit = FALSE;
    interfacesData->encodersInterface.checkEnd = FALSE;
    interfacesData->laserInterface.activated = FALSE;
    interfacesData->laserInterface.checkInit = FALSE;
    interfacesData->laserInterface.checkEnd = FALSE;
    interfacesData->camerasInterface.activated = FALSE;
    interfacesData->camerasInterface.checkInit = FALSE;
    interfacesData->camerasInterface.checkEnd = FALSE;
    interfacesData->pose3DEncodersInterface.activated = FALSE;
    interfacesData->pose3DEncodersInterface.checkInit = FALSE;
    interfacesData->pose3DEncodersInterface.checkEnd = FALSE;
    interfacesData->pose3DMotorsInterface.activated = FALSE;
    interfacesData->pose3DMotorsInterface.checkInit = FALSE;
    interfacesData->pose3DMotorsInterface.checkEnd = FALSE;

    interfacesData->Pose3DMotorsDataToSendLeft.tilt = 0;
    interfacesData->Pose3DMotorsDataToSendLeft.pan = 0;
    interfacesData->motorsDataToSend.v = 0;
    interfacesData->motorsDataToSend.w = 0;

}

controlICE::controlICE(const controlICE& orig) {
}

controlICE::~controlICE() {
}

void controlICE::getDataGazebo() {
    if (interfacesData->motorsInterface.activated) {
        interfacesData->motorsDataReceived.v = mprx->getV();
        interfacesData->motorsDataReceived.w = mprx->getW();
        interfacesData->motorsDataReceived.l = mprx->getL();
    }
    if (interfacesData->laserInterface.activated) {
        interfacesData->laserDataReceived = lprx->getLaserData();
    }
    if (interfacesData->pose3DEncodersInterface.activated) {
        interfacesData->Pose3DEncodersDataReceivedLeft = p3deprxLeft->getPose3DEncodersData();
        interfacesData->Pose3DEncodersDataReceivedRight = p3deprxRight->getPose3DEncodersData();
    }
    if (interfacesData->camerasInterface.activated) {
        pthread_mutex_lock(&interfacesData->imagesData_mutex);
        interfacesData->imageDataLeftReceived = cprxLeft->getImageData();
        interfacesData->imageDataRightReceived = cprxRight->getImageData();
        pthread_mutex_unlock(&interfacesData->imagesData_mutex);
    }

    if (interfacesData->encodersInterface.activated) {
        interfacesData->encodersDataReceived = eprx->getEncodersData();
    }
}

void controlICE::sendDataGazebo() {


    if (interfacesData->motorsInterface.activated) {
        mprx->setV(interfacesData->motorsDataToSend.v);
        mprx->setW(interfacesData->motorsDataToSend.w);
    }

    if (interfacesData->pose3DMotorsInterface.activated) {

        interfacesData->Pose3DMotorsDataLeft = new jderobot::Pose3DMotorsData();

        interfacesData->Pose3DMotorsDataLeft->tilt = interfacesData->Pose3DMotorsDataToSendLeft.tilt;
        interfacesData->Pose3DMotorsDataLeft->pan = interfacesData->Pose3DMotorsDataToSendLeft.pan;
        p3dmprxLeft->setPose3DMotorsData(interfacesData->Pose3DMotorsDataLeft);

        interfacesData->Pose3DMotorsDataRight = new jderobot::Pose3DMotorsData();

        interfacesData->Pose3DMotorsDataRight->tilt = interfacesData->Pose3DMotorsDataToSendRight.tilt;
        interfacesData->Pose3DMotorsDataRight->pan = interfacesData->Pose3DMotorsDataToSendRight.pan;
        p3dmprxRight->setPose3DMotorsData(interfacesData->Pose3DMotorsDataRight);
    }





}

void controlICE::initLaser() {
    try {
        std::string nameLaser;
        nameLaser = std::string("--Ice.Config=teleoperatorPC.cfg");
        char* name = (char*) nameLaser.c_str();
        char* argv[] = {name};
        int argc = 1;

        this->icLaser = Ice::initialize(argc, argv);

        Ice::ObjectPrx baseLaser = this->icLaser->propertyToProxy("introrob.Laser.Proxy");
        if (0 == baseLaser)
            throw "Could not create proxy with laser";

        // Cast to laser
        this->lprx = jderobot::LaserPrx::checkedCast(baseLaser);
        if (0 == this->lprx)
            throw "Invalid proxy introrob.Laser.Proxy";

        std::cout << "Laser added" << std::endl;


    } catch (const Ice::Exception& ex) {
        std::cerr << ex << std::endl;
    } catch (const char* msg) {
        std::cerr << msg << std::endl;
    }

}

void controlICE::endLaser() {
    try {

        this->icLaser->shutdown();
        this->icLaser->destroy();
        std::cout << "Laser removed" << std::endl;
    } catch (const Ice::Exception& ex) {
        std::cerr << ex << std::endl;
    } catch (const char* msg) {
        std::cerr << msg << std::endl;
    }

}

void controlICE::initPose3DEncoders() {
    try {
        std::string namePose3DEncoders;
        namePose3DEncoders = std::string("--Ice.Config=teleoperatorPC.cfg");
        char* name = (char*) namePose3DEncoders.c_str();
        char* argv[] = {name};
        int argc = 1;

        this->icPose3DEncoders = Ice::initialize(argc, argv);

        Ice::ObjectPrx basePose3DEncoders = this->icPose3DEncoders->propertyToProxy("introrob.Pose3DEncodersLeft.Proxy");
        if (0 == basePose3DEncoders)
            throw "Could not create proxy with Pose3DEncodersLeft";

        // Cast to Pose3DencodersLeft
        this->p3deprxLeft = jderobot::Pose3DEncodersPrx::checkedCast(basePose3DEncoders);
        if (0 == this->p3deprxLeft)
            throw "Invalid proxy introrob.Pose3DEncodersLeft.Proxy";

        Ice::ObjectPrx basePose3DEncoders2 = this->icPose3DEncoders->propertyToProxy("introrob.Pose3DEncodersRight.Proxy");
        if (0 == basePose3DEncoders2)
            throw "Could not create proxy with Pose3DEncodersRight";

        // Cast to Pose3DencodersRight
        this->p3deprxRight = jderobot::Pose3DEncodersPrx::checkedCast(basePose3DEncoders2);
        if (0 == this->p3deprxRight)
            throw "Invalid proxy introrob.Pose3DEncodersRight.Proxy";


        std::cout << "Pose3Dencoders added" << std::endl;


    } catch (const Ice::Exception& ex) {
        std::cerr << ex << std::endl;
    } catch (const char* msg) {
        std::cerr << msg << std::endl;
    }

}

void controlICE::endPose3DEncoders() {
    try {

        this->icPose3DEncoders->shutdown();
        this->icPose3DEncoders->destroy();
        std::cout << "Pose3Dencoders removed" << std::endl;

    } catch (const Ice::Exception& ex) {
        std::cerr << ex << std::endl;
    } catch (const char* msg) {
        std::cerr << msg << std::endl;
    }

}

void controlICE::initPose3DMotors() {
    try {
        std::string namePose3DMotors;
        namePose3DMotors = std::string("--Ice.Config=teleoperatorPC.cfg");
        char* name = (char*) namePose3DMotors.c_str();
        char* argv[] = {name};
        int argc = 1;

        this->icPose3DMotors = Ice::initialize(argc, argv);

        Ice::ObjectPrx basePose3DMotors = this->icPose3DMotors->propertyToProxy("introrob.Pose3DMotorsLeft.Proxy");
        if (0 == basePose3DMotors)
            throw "Could not create proxy with Pose3DMotorsLeft";

        // Cast to Pose3DMotorsLeft
        this->p3dmprxLeft = jderobot::Pose3DMotorsPrx::checkedCast(basePose3DMotors);
        if (0 == this->p3dmprxLeft)
            throw "Invalid proxy introrob.Pose3DMotorsLeft.Proxy";

        Ice::ObjectPrx basePose3DMotors2 = this->icPose3DMotors->propertyToProxy("introrob.Pose3DMotorsRight.Proxy");
        if (0 == basePose3DMotors2)
            throw "Could not create proxy with Pose3DMotorsRight";

        // Cast to Pose3DMotorsRight
        this->p3dmprxRight = jderobot::Pose3DMotorsPrx::checkedCast(basePose3DMotors2);
        if (0 == this->p3dmprxRight)
            throw "Invalid proxy introrob.Pose3DMotorsRight.Proxy";


        std::cout << "Pose3DMotors added" << std::endl;


    } catch (const Ice::Exception& ex) {
        std::cerr << ex << std::endl;
    } catch (const char* msg) {
        std::cerr << msg << std::endl;
    }

}

void controlICE::endPose3DMotors() {
    try {

        this->icPose3DMotors->shutdown();
        this->icPose3DMotors->destroy();
        std::cout << "Pose3DMotors removed" << std::endl;

    } catch (const Ice::Exception& ex) {
        std::cerr << ex << std::endl;
    } catch (const char* msg) {
        std::cerr << msg << std::endl;
    }

}

void controlICE::initMotors() {
    try {
        std::string nameMotors;
        nameMotors = std::string("--Ice.Config=teleoperatorPC.cfg");
        char* name = (char*) nameMotors.c_str();
        char* argv[] = {name};
        int argc = 1;

        this->icMotors = Ice::initialize(argc, argv);

        Ice::ObjectPrx baseMotors = this->icMotors->propertyToProxy("introrob.Motors.Proxy");
        if (0 == baseMotors)
            throw "Could not create proxy with Motors";

        // Cast to Motors
        this->mprx = jderobot::MotorsPrx::checkedCast(baseMotors);
        if (0 == this->mprx)
            throw "Invalid proxy introrob.Motors.Proxy";

        std::cout << "Motors added" << std::endl;


    } catch (const Ice::Exception& ex) {
        std::cerr << ex << std::endl;
    } catch (const char* msg) {
        std::cerr << msg << std::endl;
    }

}

void controlICE::endMotors() {
    try {

        this->icMotors->shutdown();
        this->icMotors->destroy();
        std::cout << "Motors removed" << std::endl;

    } catch (const Ice::Exception& ex) {
        std::cerr << ex << std::endl;
    } catch (const char* msg) {
        std::cerr << msg << std::endl;
    }

}

void controlICE::initEncoders() {
    try {
        std::string nameEncoders;
        nameEncoders = std::string("--Ice.Config=teleoperatorPC.cfg");
        char* name = (char*) nameEncoders.c_str();
        char* argv[] = {name};
        int argc = 1;

        this->icEncoders = Ice::initialize(argc, argv);

        Ice::ObjectPrx baseEncoders = this->icEncoders->propertyToProxy("introrob.Encoders.Proxy");
        if (0 == baseEncoders)
            throw "Could not create proxy with Encoders";

        // Cast to Encoders
        this->eprx = jderobot::EncodersPrx::checkedCast(baseEncoders);
        if (0 == this->eprx)
            throw "Invalid proxy introrob.Encoders.Proxy";

        std::cout << "Encoders added" << std::endl;


    } catch (const Ice::Exception& ex) {
        std::cerr << ex << std::endl;
    } catch (const char* msg) {
        std::cerr << msg << std::endl;
    }

}

void controlICE::endEncoders() {
    try {

        this->icEncoders->shutdown();
        this->icEncoders->destroy();
        std::cout << "Encoders removed" << std::endl;

    } catch (const Ice::Exception& ex) {
        std::cerr << ex << std::endl;
    } catch (const char* msg) {
        std::cerr << msg << std::endl;
    }

}

void controlICE::initCameras() {
    try {
        std::string nameCameras;
        nameCameras = std::string("--Ice.Config=teleoperatorPC.cfg");
        char* name = (char*) nameCameras.c_str();
        char* argv[] = {name};
        int argc = 1;

        this->icCameras = Ice::initialize(argc, argv);

        Ice::ObjectPrx baseCameras = this->icCameras->propertyToProxy("introrob.CameraLeft.Proxy");
        if (0 == baseCameras)
            throw "Could not create proxy with CameraLeft";

        // Cast to CameraLeft
        this->cprxLeft = jderobot::CameraPrx::checkedCast(baseCameras);
        if (0 == this->cprxLeft)
            throw "Invalid proxy introrob.CameraLeft.Proxy";

        Ice::ObjectPrx baseCameras2 = this->icCameras->propertyToProxy("introrob.CameraRight.Proxy");
        if (0 == baseCameras2)
            throw "Could not create proxy with CameraRight";

        // Cast to CameraRight
        this->cprxRight = jderobot::CameraPrx::checkedCast(baseCameras2);
        if (0 == this->cprxRight)
            throw "Invalid proxy introrob.CameraRight.Proxy";


        std::cout << "Cameras added" << std::endl;


    } catch (const Ice::Exception& ex) {
        std::cerr << ex << std::endl;
    } catch (const char* msg) {
        std::cerr << msg << std::endl;
    }

}

void controlICE::endCameras() {
    try {

        this->icCameras->shutdown();
        this->icCameras->destroy();
        std::cout << "Cameras removed" << std::endl;

    } catch (const Ice::Exception& ex) {
        std::cerr << ex << std::endl;
    } catch (const char* msg) {
        std::cerr << msg << std::endl;
    }

}

void controlICE::checkInterfaces() {

    if (interfacesData->laserInterface.checkInit) {
        this->initLaser();
        interfacesData->laserInterface.activated = TRUE;
        interfacesData->laserInterface.checkInit = FALSE;
    }
    if (interfacesData->laserInterface.checkEnd) {
        this->endLaser();
        interfacesData->laserInterface.activated = FALSE;
        interfacesData->laserInterface.checkEnd = FALSE;
    }

    if (interfacesData->camerasInterface.checkInit) {
        this->initCameras();
        interfacesData->camerasInterface.activated = TRUE;
        interfacesData->camerasInterface.checkInit = FALSE;
    }
    if (interfacesData->camerasInterface.checkEnd) {
        this->endCameras();
        interfacesData->camerasInterface.activated = FALSE;
        interfacesData->camerasInterface.checkEnd = FALSE;
    }

    if (interfacesData->pose3DEncodersInterface.checkInit) {
        this->initPose3DEncoders();
        interfacesData->pose3DEncodersInterface.activated = TRUE;
        interfacesData->pose3DEncodersInterface.checkInit = FALSE;
    }
    if (interfacesData->pose3DEncodersInterface.checkEnd) {
        this->endPose3DEncoders();
        interfacesData->pose3DEncodersInterface.activated = FALSE;
        interfacesData->pose3DEncodersInterface.checkEnd = FALSE;
    }

    if (interfacesData->pose3DMotorsInterface.checkInit) {
        this->initPose3DMotors();
        interfacesData->pose3DMotorsInterface.activated = TRUE;
        interfacesData->pose3DMotorsInterface.checkInit = FALSE;
    }
    if (interfacesData->pose3DMotorsInterface.checkEnd) {
        this->endPose3DMotors();
        interfacesData->pose3DMotorsInterface.activated = FALSE;
        interfacesData->pose3DMotorsInterface.checkEnd = FALSE;
    }

    if (interfacesData->motorsInterface.checkInit) {
        this->initMotors();
        interfacesData->motorsInterface.activated = TRUE;
        interfacesData->motorsInterface.checkInit = FALSE;
    }
    if (interfacesData->motorsInterface.checkEnd) {
        this->endMotors();
        interfacesData->motorsInterface.activated = FALSE;
        interfacesData->motorsInterface.checkEnd = FALSE;
    }

    if (interfacesData->encodersInterface.checkInit) {
        this->initEncoders();
        interfacesData->encodersInterface.activated = TRUE;
        interfacesData->encodersInterface.checkInit = FALSE;
    }
    if (interfacesData->encodersInterface.checkEnd) {
        this->endEncoders();
        interfacesData->encodersInterface.activated = FALSE;
        interfacesData->encodersInterface.checkEnd = FALSE;
    }
    
}
