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

#include "generate.h"

/*************************************************************
 * CONSTRUCTOR
 *************************************************************/
Generate::Generate ( std::list<SubAutomata> subautomataList, std::string cpppath,
										std::string cfgpath, std::string cmakepath,
										std::string configfile ) {
	this->subautomataList = subautomataList;
	this->path = cpppath;
	this->cfgpath = cfgpath;
	this->cmakepath = cmakepath;
	this->configfile = configfile;

	this->mapTab[T_ZERO] = std::string();
	this->mapTab[T_ONE] = std::string("\t");
	this->mapTab[T_TWO] = std::string("\t\t");
	this->mapTab[T_THREE] = std::string("\t\t\t");
	this->mapTab[T_FOUR] = std::string("\t\t\t\t");
	this->mapTab[T_FIVE] = std::string("\t\t\t\t\t");
	this->mapTab[T_SIX] = std::string("\t\t\t\t\t\t");
}

/*************************************************************
 * DESTRUCTOR
 *************************************************************/
Generate::~Generate () {}

/*************************************************************
 * ANOTHER FUNCTIONS
 *************************************************************/
int Generate::init () {
	this->fs.open(this->path.c_str(), std::fstream::out);
	if (this->fs.is_open()) {
		this->generateHeaders();
		this->generateEnums();
		this->generateVariables();
		this->generateFunctions();
		this->generateSubautomatas();
		this->generateMain();
		this->fs.close();

		this->fs.open(this->cfgpath.c_str(), std::fstream::out);
		if (this->fs.is_open()) {
			this->fs << this->configfile << std::endl;
			this->fs.flush();
			// this->generateCfg();
			this->fs.close();
		}
		
		this->fs.open(this->cmakepath.c_str(), std::fstream::out);
		if (this->fs.is_open()) {
			this->generateCmake();
			this->fs.close();
		}
		return 0;
	} else
		return 1;
}

void Generate::generateHeaders () {
	this->generateGenericHeaders();
	this->generateSpecificHeaders();
}

void Generate::generateGenericHeaders () {
	this->fs << "#include <iostream>" << std::endl;
	this->fs << "#include <stdio.h>" << std::endl;
	this->fs << std::endl;
	this->fs << "#include <Ice/Ice.h>" << std::endl;
	this->fs << "#include <IceUtil/IceUtil.h>" << std::endl;
	this->fs << std::endl;
	this->fs.flush();
}

void Generate::generateSpecificHeaders () {
	for ( std::list<SubAutomata>::iterator subListIterator = this->subautomataList.begin();
            subListIterator != this->subautomataList.end(); subListIterator++ ) {
		std::list<std::string>* interfacesList = subListIterator->getInterfaces();
		for ( std::list<std::string>::iterator intListIterator = interfacesList->begin();
				intListIterator != interfacesList->end(); intListIterator++ )
			this->fs << "#include " << *intListIterator << std::endl;
		this->fs << std::endl;
		this->fs.flush();
	}
}

void Generate::generateEnums () {
	for ( std::list<SubAutomata>::iterator subListIterator = this->subautomataList.begin();
            subListIterator != this->subautomataList.end(); subListIterator++ ) {
		int id = subListIterator->getId();

		this->fs << "typedef enum State_Sub_" << id << " {" << std::endl;
		std::list<Node> nodeList = subListIterator->getNodeList();
		for ( std::list<Node>::iterator nodeListIterator = nodeList.begin();
				nodeListIterator != nodeList.end(); nodeListIterator++ ) {
			if (id != 1) {
				this->fs << "\t" << nodeListIterator->getName() << "," << std::endl;
				this->fs << "\t" << nodeListIterator->getName() << "_ghost," << std::endl;
			} else
				this->fs << "\t" << nodeListIterator->getName() << "," << std::endl;
		}
		this->fs << "} State_Sub_" << id << ";" << std::endl;
		
		this->fs << std::endl;

		this->fs << "const char* Names_Sub_" << id << "[] = {" << std::endl;
		for ( std::list<Node>::iterator nodeListIterator = nodeList.begin();
				nodeListIterator != nodeList.end(); nodeListIterator++ ) {
			this->fs << "\t\"" << nodeListIterator->getName() << "\"," << std::endl;
			if (id != 1) {
				std::string ghost = std::string(nodeListIterator->getName() + "_ghost");
				this->fs << "\t\"" << ghost << "\"," << std::endl;
			}
		}
		this->fs << "};" << std::endl;

		this->fs << std::endl;
		this->fs.flush();
	}
}

void Generate::generateVariables () {
	for ( std::list<SubAutomata>::iterator subListIterator = this->subautomataList.begin();
            subListIterator != this->subautomataList.end(); subListIterator++ )
		this->fs << "pthread_t thr_sub_" << subListIterator->getId() << ";" << std::endl;
	this->fs << std::endl;

	for ( std::list<SubAutomata>::iterator subListIterator = this->subautomataList.begin();
            subListIterator != this->subautomataList.end(); subListIterator++ ) {
		int id = subListIterator->getId();

		std::list<Node> nodeList = subListIterator->getNodeList();
		std::list<Node>::iterator nodeListIterator = nodeList.begin();
		while ( (!nodeListIterator->isInitial()) &&
				(nodeListIterator != nodeList.end()) )
			nodeListIterator++;

		std::string nameState;
		if (id != 1)
			nameState = std::string(nodeListIterator->getName() + "_ghost");
		else
			nameState = std::string(nodeListIterator->getName());

		this->fs << "State_Sub_" << id << " sub_" << id << " = " << nameState.c_str() << ";" << std::endl;
	}
	this->fs << std::endl;
	this->fs.flush();

	this->fs << "jderobot::CameraPrx cameraprx;" << std::endl;
	this->fs << "jderobot::MotorsPrx motorsprx;" << std::endl;
	this->fs << "jderobot::NaoMotionsPrx motions;" << std::endl;
	this->fs << "jderobot::Pose3DMotorsPrx head;" << std::endl;
	this->fs << "jderobot::Pose3DMotorsPrx leftshoulder;" << std::endl;
	this->fs << "jderobot::Pose3DMotorsPrx rightshoulder;" << std::endl;
	this->fs << "jderobot::Pose3DMotorsPrx leftelbow;" << std::endl;
	this->fs << "jderobot::Pose3DMotorsPrx rightelbow;" << std::endl;
	this->fs << "jderobot::Pose3DMotorsPrx lefthip;" << std::endl;
	this->fs << "jderobot::Pose3DMotorsPrx righthip;" << std::endl;
	this->fs << "jderobot::Pose3DMotorsPrx leftknee;" << std::endl;
	this->fs << "jderobot::Pose3DMotorsPrx rightknee;" << std::endl;
	this->fs << "jderobot::Pose3DMotorsPrx leftankle;" << std::endl;
	this->fs << "jderobot::Pose3DMotorsPrx rightankle;" << std::endl;
	this->fs << std::endl;
	this->fs.flush();
}
	
void Generate::generateFunctions () {
	for ( std::list<SubAutomata>::iterator subListIterator = this->subautomataList.begin();
            subListIterator != this->subautomataList.end(); subListIterator++ ) {		
		this->fs << subListIterator->getFunctions() << std::endl;
		this->fs << std::endl;
		this->fs.flush();
	}
}

void Generate::generateSubautomatas () {
	for ( std::list<SubAutomata>::iterator subListIterator = this->subautomataList.begin();
            subListIterator != this->subautomataList.end(); subListIterator++ ) {
       	int id = subListIterator->getId();		
		this->fs << "void* subautomata_" << id << " ( void* ) {" << std::endl;
		this->fs << "\tstruct timeval a, b;" << std::endl;
		this->fs << "\tint cycle = " << subListIterator->getTime() << ";" << std::endl;
		this->fs << "\tlong totala, totalb;" << std::endl;
		this->fs << "\tlong diff;" << std::endl;
		this->fs << "\ttime_t t_ini;" << std::endl;
		this->fs << "\ttime_t t_fin;" << std::endl;
		this->fs << "\tdouble secs;" << std::endl;
		this->fs << "\tbool t_activated;" << std::endl;
		this->fs << std::endl;

		int countNodes = 0;
		std::list<Node> nodeList = subListIterator->getNodeList();
		for ( std::list<Node>::iterator nodeListIterator = nodeList.begin();
				nodeListIterator != nodeList.end(); nodeListIterator++ )
			countNodes++;

		std::map<std::string, float> mapNameTime;
		std::list<Transition> transList = subListIterator->getTransList();
		if (id != 1) {
			for ( std::list<Transition>::iterator transListIterator = transList.begin();
					transListIterator != transList.end(); transListIterator++ )	{
				int idorigin = transListIterator->getIdOrigin();
				if (transListIterator->getType().compare("time") == 0) {
					std::list<Node>::iterator nodeListIterator = nodeList.begin();
					while ( (nodeListIterator->getId() != idorigin) &&
							(nodeListIterator != nodeList.end()) )
						nodeListIterator++;

					float ms = atof(transListIterator->getCode().c_str());
					this->fs << mapTab[T_ONE] << "t_" << nodeListIterator->getName() << "_max = " << (ms/1000.0) << ";" << std::endl;

					mapNameTime[nodeListIterator->getName()] = ms/1000.0;
				}
			}	
		}

		std::istringstream f(subListIterator->getVariables());
		std::string line;
		while (std::getline(f, line))
			this->fs << "\t" << line << std::endl;
		this->fs << std::endl;

		this->fs << "\twhile (true) {" << std::endl;
		this->fs << "\t\tgettimeofday(&a, NULL);" << std::endl;
		this->fs << "\t\ttotala = a.tv_sec * 1000000 * a.tv_usec;" << std::endl;
		this->fs << std::endl;

		if (id != 1) {
			int idfather = subListIterator->getIdFather();

			std::list<SubAutomata>::iterator subFatherListIterator = this->subautomataList.begin();
			while ( (subFatherListIterator->getId() != idfather) &&
					(subFatherListIterator != this->subautomataList.end()) )
				subFatherListIterator++;
			
			std::list<Node> nodeFatherList = subFatherListIterator->getNodeList();
			std::list<Node>::iterator nodeFatherListIterator = nodeFatherList.begin();
			while ( (nodeFatherListIterator->getIdSubautomataSon() != id) &&
					(nodeFatherListIterator != nodeFatherList.end()) )
				nodeFatherListIterator++;

			this->fs << "\t\tif (sub_" << idfather << " == " << nodeFatherListIterator->getName() << ") {" << std::endl;
			this->fs << "\t\t\tif (";

			int count = 0;
			for ( std::list<Transition>::iterator nodeListIterator = transList.begin();
					nodeListIterator != transList.end(); nodeListIterator++ ) {
				this->fs << " sub_" << id << " == " << nodeListIterator->getName();
				count++;
				if (count != countNodes) {
					this->fs << " ||";
				}
			}
			this->fs << ") {" << std::endl;

			this->fs << "\t\t\t\tsub_" << id << "(State_Sub_" << id << ")(sub_" << id << " - 1);" << std::endl;
			this->fs << "\t\t\t\tt_ini = time(NULL);" << std::endl;
			this->fs << "\t\t\t}" << std::endl;
		}

		this->fs << "\t\t// Evaluation switch" << std::endl;
		this->fs << "\t\tswitch (sub_" << id << ") {" << std::endl;
		for ( std::list<Node>::iterator nodeListIterator = nodeList.begin();
				nodeListIterator != nodeList.end(); nodeListIterator++ ) {
			int idNode = nodeListIterator->getId();
			this->fs << "\t\t\tcase " << nodeListIterator->getName() << ": {" << std::endl;
			
			for ( std::list<Transition>::iterator transListIterator = transList.begin();
					transListIterator != transList.end(); transListIterator++ ) {
				if (transListIterator->getIdOrigin() == idNode) {
					int idDestiny = transListIterator->getIdDestiny();
					if (transListIterator->getType().compare("condition") == 0) {
						this->fs << "\t\t\t\tif (" << transListIterator->getCode().c_str() << ") {" << std::endl;
						this->fs << "\t\t\t\t\tsub_" << id << " = " << subListIterator->getNodeName(idDestiny) << ";" << std::endl;
						this->fs << "\t\t\t\t}" << std::endl;
					} else {
						this->fs << "\t\t\t\tif (!t_activated) {" << std::endl;
						this->fs << "\t\t\t\t\tt_ini = time(NULL);" << std::endl;
						this->fs << "\t\t\t\t\tt_activated = true;" << std::endl;
						this->fs << "\t\t\t\t} else {" << std::endl;
						this->fs << "\t\t\t\t\tt_fin = time(NULL);" << std::endl;
						this->fs << "\t\t\t\t\tsecs = difftime(t_fin, t_ini);" << std::endl;
						if (id == 1) {
							float ms = atof(transListIterator->getCode().c_str());
							this->fs << "\t\t\t\t\tif (secs > (double) " << (ms / 1000.0) << ") {" << std::endl;
						} else
							this->fs << "\t\t\t\t\tif (secs > (double) t_" << subListIterator->getNodeName(idNode) << "_max) {" << std::endl;
						this->fs << "\t\t\t\t\t\tsub_" << id << " = " << subListIterator->getNodeName(idDestiny) << ";" << std::endl;
						this->fs << "\t\t\t\t\t\tt_activated = false;" << std::endl;
						if (id != 1)
							this->fs << "\t\t\t\t\t\tt_" << subListIterator->getNodeName(idNode) << "_max = " << mapNameTime[subListIterator->getNodeName(idNode)] << ";" << std::endl;
						this->fs << "\t\t\t\t\t}" << std::endl;
						this->fs << "\t\t\t\t}" << std::endl;
					}
					this->fs << std::endl;
				}
			}
			this->fs << "\t\t\t\tbreak;" << std::endl;
			this->fs << "\t\t\t}" << std::endl;
			this->fs.flush();
		}
		this->fs << "\t\t}" << std::endl;
		this->fs << std::endl;

		this->fs << "\t\t// Actuation switch" << std::endl;
		this->fs << "\t\tswitch (sub_" << id << ") {" << std::endl;
		for ( std::list<Node>::iterator nodeListIterator = nodeList.begin();
				nodeListIterator != nodeList.end(); nodeListIterator++ ) {
			this->fs << "\t\t\tcase " << nodeListIterator->getName() << ": {" << std::endl;
			std::istringstream f(nodeListIterator->getCode());
			std::string line;
			while (std::getline(f, line))
				this->fs << "\t\t\t\t" << line << std::endl;
			this->fs << "\t\t\t\tbreak;" << std::endl;
			this->fs << "\t\t\t}" << std::endl;
			this->fs.flush();
		}
		this->fs << "\t\t}" << std::endl;
		if (id != 1) {
			this->fs << "\t\t} else {" << std::endl;
			this->fs << "\t\t\tswitch (sub_" << id << ") {" << std::endl;
			for ( std::list<Node>::iterator nodeListIterator = nodeList.begin();
					nodeListIterator != nodeList.end(); nodeListIterator++ ) {
				if (mapNameTime.find(nodeListIterator->getName()) != mapNameTime.end()) {
					this->fs << "\t\t\t\tcase " << nodeListIterator->getName() << ":" << std::endl;
					this->fs << "\t\t\t\t\tt_" << nodeListIterator->getName() << "_max = " << mapNameTime[nodeListIterator->getName()] << " - difftime(t_fin, t_ini);" << std::endl;
					this->fs << "\t\t\t\t\tbreak;" << std::endl;
				} 
			}
			this->fs << "\t\t\t\tdefault:" << std::endl;
			this->fs << "\t\t\t\t\tbreak;" << std::endl;
			this->fs << "\t\t\t\tsub_" << id << " = (State_Sub_" << id << ")(sub_" << id << " + 1);" << std::endl;
			this->fs << "\t\t\t}" << std::endl;
			this->fs << "\t\t}" << std::endl;
		}
		this->fs << std::endl;

		this->fs << "\t\tgettimeofday(&b, NULL);" << std::endl;
		this->fs << "\t\ttotalb = b.tv_sec * 1000000 + b.tv_usec;" << std::endl;
		this->fs << "\t\tdiff = (totalb - totala) / 1000;" << std::endl;
		this->fs << "\t\tif (diff < 0 || diff > cycle)" << std::endl;
		this->fs << "\t\t\tdiff = cycle;" << std::endl;
		this->fs << "\t\telse" << std::endl;
		this->fs << "\t\t\tdiff = cycle - diff;" << std::endl;
		this->fs << std::endl;
		this->fs << "\t\tusleep(diff * 1000);" << std::endl;
		this->fs << "\t\tif (diff < 33 )" << std::endl;
		this->fs << "\t\t\tusleep (33 * 1000);" << std::endl;

		this->fs << "\t}" << std::endl;
		this->fs << "}" << std::endl;

		this->fs << std::endl;
		this->fs.flush();
	}
}

void Generate::generateMain () {
	this->fs << "int main (int argc, char* argv[]) {" << std::endl;
	this->fs << "\tint status;" << std::endl;
	this->fs << "\tIce::CommunicatorPtr ic;" << std::endl;
	this->fs << std::endl;
	this->fs << "\ttry {" << std::endl;
	this->fs << "\t\tic = Ice::initialize(argc, argv);" << std::endl;
	this->fs << std::endl;

	this->fs << "\t\t// Contact to camera" << std::endl;
	this->fs << "\t\tIce::ObjectPrx camera = ic->propertyToProxy(\"comp.Camera.Proxy\");" << std::endl;
	this->fs << "\t\tif (camera == 0)" << std::endl;
	this->fs << "\t\t\t throw \"Could not create proxy with camera\";" << std::endl;
	this->fs << "\t\tcameraprx = jderobot::CameraPrx::checkedCast(camera);" << std::endl;
	this->fs << "\t\tif (cameraprx == 0)" << std::endl;
	this->fs << "\t\t\tthrow \"Invalid proxy naooperator.Camera.Proxy\";" << std::endl;
	this->fs << "\t\tstd::cout << \"Camera connected\" << std::endl;" << std::endl;
	this->fs << std::endl;

	this->fs << "\t\t// Contact to motors (for walking)" << std::endl;
	this->fs << "\t\tIce::ObjectPrx motors = ic->propertyToProxy(\"comp.Motors.Proxy\");" << std::endl;
	this->fs << "\t\tif (motors == 0)" << std::endl;
	this->fs << "\t\t\t throw \"Could not create proxy with motors\";" << std::endl;
	this->fs << "\t\tmotorsprx = jderobot::MotorsPrx::checkedCast(motors);" << std::endl;
	this->fs << "\t\tif (motorsprx == 0)" << std::endl;
	this->fs << "\t\t\tthrow \"Invalid proxy naooperator.Motors.Proxy\";" << std::endl;
	this->fs << "\t\tstd::cout << \"Motors connected\" << std::endl;" << std::endl;
	this->fs << std::endl;

	this->fs.flush();

	this->fs << "\t\t// Contact to motors (for different actions)" << std::endl;
	this->fs << "\t\tIce::ObjectPrx motionsPrx = ic->propertyToProxy(\"comp.Motions.Proxy\");" << std::endl;
	this->fs << "\t\tif (motionsPrx == 0)" << std::endl;
	this->fs << "\t\t\t throw \"Could not create proxy with motions\";" << std::endl;
	this->fs << "\t\tmotions = jderobot::NaoMotionsPrx::checkedCast(motionsPrx);" << std::endl;
	this->fs << "\t\tif (motions == 0)" << std::endl;
	this->fs << "\t\t\tthrow \"Invalid proxy naooperator.Motions.Proxy\";" << std::endl;
	this->fs << "\t\tstd::cout << \"Motions connected\" << std::endl;" << std::endl;
	this->fs << std::endl;

	this->fs << "\t\t// Contact to head motors" << std::endl;
	this->fs << "\t\tIce::ObjectPrx headmotors = ic->propertyToProxy(\"comp.HeadMotors.Proxy\");" << std::endl;
	this->fs << "\t\tif (headmotors == 0)" << std::endl;
	this->fs << "\t\t\t throw \"Could not create proxy with head motors\";" << std::endl;
	this->fs << "\t\thead = jderobot::Pose3DMotorsPrx::checkedCast(headmotors);" << std::endl;
	this->fs << "\t\tif (head == 0)" << std::endl;
	this->fs << "\t\t\tthrow \"Invalid proxy naooperator.HeadMotors.Proxy\";" << std::endl;
	this->fs << "\t\tstd::cout << \"Head motors connected\" << std::endl;" << std::endl;
	this->fs << std::endl;

	this->fs << "\t\t// Contact to shoulders motors" << std::endl;
	this->fs << "\t\tIce::ObjectPrx leftshouldermotors = ic->propertyToProxy(\"comp.LeftShoulderMotors.Proxy\");" << std::endl;
	this->fs << "\t\tif (leftshouldermotors == 0)" << std::endl;
	this->fs << "\t\t\t throw \"Could not create proxy with left shoulder motors\";" << std::endl;
	this->fs << "\t\tleftshoulder = jderobot::Pose3DMotorsPrx::checkedCast(leftshouldermotors);" << std::endl;
	this->fs << "\t\tif (leftshoulder == 0)" << std::endl;
	this->fs << "\t\t\tthrow \"Invalid proxy naooperator.LeftShoulderMotors.Proxy\";" << std::endl;
	this->fs << "\t\tstd::cout << \"Left shoulder connected\" << std::endl;" << std::endl;
	this->fs << std::endl;

	this->fs.flush();

	this->fs << "\t\tIce::ObjectPrx rightshouldermotors = ic->propertyToProxy(\"comp.RightShoulderMotors.Proxy\");" << std::endl;
	this->fs << "\t\tif (rightshouldermotors == 0)" << std::endl;
	this->fs << "\t\t\t throw \"Could not create proxy with right shoulder motors\";" << std::endl;
	this->fs << "\t\trightshoulder = jderobot::Pose3DMotorsPrx::checkedCast(rightshouldermotors);" << std::endl;
	this->fs << "\t\tif (rightshoulder == 0)" << std::endl;
	this->fs << "\t\t\tthrow \"Invalid proxy naooperator.RightShoulderMotors.Proxy\";" << std::endl;
	this->fs << "\t\tstd::cout << \"Right shoulder connected\" << std::endl;" << std::endl;
	this->fs << std::endl;

	this->fs << "\t\t// Contact to elbows motors" << std::endl;
	this->fs << "\t\tIce::ObjectPrx leftelbowmotors = ic->propertyToProxy(\"comp.LeftElbowMotors.Proxy\");" << std::endl;
	this->fs << "\t\tif (leftelbowmotors == 0)" << std::endl;
	this->fs << "\t\t\t throw \"Could not create proxy with right elbow motors\";" << std::endl;
	this->fs << "\t\tleftelbow = jderobot::Pose3DMotorsPrx::checkedCast(leftelbowmotors);" << std::endl;
	this->fs << "\t\tif (leftelbow == 0)" << std::endl;
	this->fs << "\t\t\tthrow \"Invalid proxy naooperator.LeftElbowMotors.Proxy\";" << std::endl;
	this->fs << "\t\tstd::cout << \"Left elbow connected\" << std::endl;" << std::endl;
	this->fs << std::endl;

	this->fs << "\t\tIce::ObjectPrx rightelbowmotors = ic->propertyToProxy(\"comp.RightElbowMotors.Proxy\");" << std::endl;
	this->fs << "\t\tif (rightelbowmotors == 0)" << std::endl;
	this->fs << "\t\t\t throw \"Could not create proxy with right elbow motors\";" << std::endl;
	this->fs << "\t\trightelbow = jderobot::Pose3DMotorsPrx::checkedCast(rightelbowmotors);" << std::endl;
	this->fs << "\t\tif (rightelbow == 0)" << std::endl;
	this->fs << "\t\t\tthrow \"Invalid proxy naooperator.RightElbowMotors.Proxy\";" << std::endl;
	this->fs << "\t\tstd::cout << \"Right elbow connected\" << std::endl;" << std::endl;
	this->fs << std::endl;

	this->fs.flush();

	this->fs << "\t\t// Contact to hips motors" << std::endl;
	this->fs << "\t\tIce::ObjectPrx lefthipmotors = ic->propertyToProxy(\"comp.LeftHipMotors.Proxy\");" << std::endl;
	this->fs << "\t\tif (lefthipmotors == 0)" << std::endl;
	this->fs << "\t\t\t throw \"Could not create proxy with left hip motors\";" << std::endl;
	this->fs << "\t\tlefthip = jderobot::Pose3DMotorsPrx::checkedCast(lefthipmotors);" << std::endl;
	this->fs << "\t\tif (lefthip == 0)" << std::endl;
	this->fs << "\t\t\tthrow \"Invalid proxy naooperator.LeftHipMotors.Proxy\";" << std::endl;
	this->fs << "\t\tstd::cout << \"Left hip connected\" << std::endl;" << std::endl;
	this->fs << std::endl;

	this->fs << "\t\tIce::ObjectPrx righthipmotors = ic->propertyToProxy(\"comp.RightHipMotors.Proxy\");" << std::endl;
	this->fs << "\t\tif (righthipmotors == 0)" << std::endl;
	this->fs << "\t\t\t throw \"Could not create proxy with right hip motors\";" << std::endl;
	this->fs << "\t\trighthip = jderobot::Pose3DMotorsPrx::checkedCast(righthipmotors);" << std::endl;
	this->fs << "\t\tif (righthip == 0)" << std::endl;
	this->fs << "\t\t\tthrow \"Invalid proxy naooperator.RightHipMotors.Proxy\";" << std::endl;
	this->fs << "\t\tstd::cout << \"Right hip connected\" << std::endl;" << std::endl;
	this->fs << std::endl;

	this->fs << "\t\t// Contact to knees motors" << std::endl;
	this->fs << "\t\tIce::ObjectPrx leftkneemotors = ic->propertyToProxy(\"comp.LeftKneeMotors.Proxy\");" << std::endl;
	this->fs << "\t\tif (leftkneemotors == 0)" << std::endl;
	this->fs << "\t\t\t throw \"Could not create proxy with left knee motors\";" << std::endl;
	this->fs << "\t\tleftknee = jderobot::Pose3DMotorsPrx::checkedCast(leftkneemotors);" << std::endl;
	this->fs << "\t\tif (leftknee == 0)" << std::endl;
	this->fs << "\t\t\tthrow \"Invalid proxy naooperator.LeftKneeMotors.Proxy\";" << std::endl;
	this->fs << "\t\tstd::cout << \"Left knee connected\" << std::endl;" << std::endl;
	this->fs << std::endl;

	this->fs.flush();

	this->fs << "\t\tIce::ObjectPrx rightkneemotors = ic->propertyToProxy(\"comp.RightKneeMotors.Proxy\");" << std::endl;
	this->fs << "\t\tif (rightkneemotors == 0)" << std::endl;
	this->fs << "\t\t\t throw \"Could not create proxy with right knee motors\";" << std::endl;
	this->fs << "\t\trightknee = jderobot::Pose3DMotorsPrx::checkedCast(rightkneemotors);" << std::endl;
	this->fs << "\t\tif (rightknee == 0)" << std::endl;
	this->fs << "\t\t\tthrow \"Invalid proxy naooperator.RightKneeMotors.Proxy\";" << std::endl;
	this->fs << "\t\tstd::cout << \"Right knee motors connected\" << std::endl;" << std::endl;
	this->fs << std::endl;

	this->fs << "\t\t// Contact to ankles motors" << std::endl;
	this->fs << "\t\tIce::ObjectPrx leftanklemotors = ic->propertyToProxy(\"comp.LeftAnkleMotors.Proxy\");" << std::endl;
	this->fs << "\t\tif (leftanklemotors == 0)" << std::endl;
	this->fs << "\t\t\t throw \"Could not create proxy with left ankle motors\";" << std::endl;
	this->fs << "\t\tleftankle = jderobot::Pose3DMotorsPrx::checkedCast(leftanklemotors);" << std::endl;
	this->fs << "\t\tif (leftankle == 0)" << std::endl;
	this->fs << "\t\t\tthrow \"Invalid proxy naooperator.LeftAnkleMotors.Proxy\";" << std::endl;
	this->fs << "\t\tstd::cout << \"Left ankle motors connected\" << std::endl;" << std::endl;
	this->fs << std::endl;

	this->fs << "\t\tIce::ObjectPrx rightanklemotors = ic->propertyToProxy(\"comp.RightAnkleMotors.Proxy\");" << std::endl;
	this->fs << "\t\tif (rightanklemotors == 0)" << std::endl;
	this->fs << "\t\t\t throw \"Could not create proxy with right ankle motors\";" << std::endl;
	this->fs << "\t\trightankle = jderobot::Pose3DMotorsPrx::checkedCast(rightanklemotors);" << std::endl;
	this->fs << "\t\tif (rightankle == 0)" << std::endl;
	this->fs << "\t\t\tthrow \"Invalid proxy naooperator.RightAnkleMotors.Proxy\";" << std::endl;
	this->fs << "\t\tstd::cout << \"Right ankle motors connected\" << std::endl;" << std::endl;
	this->fs << std::endl;

	this->fs.flush();

	for ( std::list<SubAutomata>::iterator subListIterator = this->subautomataList.begin();
            subListIterator != this->subautomataList.end(); subListIterator++ ) {
		int id = subListIterator->getId();
		this->fs << "\t\tpthread_create(&thr_sub_" << id << ", NULL, &subautomata_" << id << ", NULL);" << std::endl;
	}
	this->fs << std::endl;

	for ( std::list<SubAutomata>::iterator subListIterator = this->subautomataList.begin();
            subListIterator != this->subautomataList.end(); subListIterator++ ) {
		int id = subListIterator->getId();
		this->fs << "\t\tpthread_join(thr_sub_" << id << ", NULL);" << std::endl;
	}
	this->fs << std::endl;

	this->fs.flush();

	this->fs << this->mapTab[T_ONE] << "} catch ( const Ice::Exception& ex ) {" << std::endl;
	this->fs << this->mapTab[T_TWO] << "std::cerr << ex << std::endl;" << std::endl;
	this->fs << this->mapTab[T_TWO] << "status = 1;" << std::endl;
	this->fs << this->mapTab[T_ONE] << "} catch ( const char* msg ) {" << std::endl;
	this->fs << this->mapTab[T_TWO] << "std::cerr << msg << std::endl;" << std::endl;
	this->fs << this->mapTab[T_TWO] << "status = 1;" << std::endl;
	this->fs << this->mapTab[T_ONE] << "}" << std::endl;
	this->fs << std::endl;
	this->fs << this->mapTab[T_ONE] << "if (ic)" << std::endl;
	this->fs << this->mapTab[T_TWO] << "ic->destroy();" << std::endl;
	this->fs << std::endl;
	this->fs << this->mapTab[T_ONE] << "return status;" << std::endl;
	this->fs << "}" << std::endl;

	this->fs.flush();
}

void Generate::generateCfg () {
	this->fs << "comp.HeadMotors.Proxy=NeckMotors:default -h 192.168.14.113 -p 10000" << std::endl;
	this->fs << "comp.HeadSpeed.Proxy=NeckSpeed:default -h 192.168.14.113 -p 10000" << std::endl;
	this->fs << "comp.LeftShoulderMotors.Proxy=LeftShoulderMotors:default -h 192.168.14.113 -p 10000" << std::endl;
	this->fs << "comp.RightShoulderMotors.Proxy=RightShoulderMotors:default -h 192.168.14.113 -p 10000" << std::endl;
	this->fs << "comp.LeftElbowMotors.Proxy=LeftElbowMotors:default -h 192.168.14.113 -p 10000" << std::endl;
	this->fs << "comp.RightElbowMotors.Proxy=RightElbowMotors:default -h 192.168.14.113 -p 10000" << std::endl;
	this->fs << "comp.LeftHipMotors.Proxy=LeftHipMotors:default -h 192.168.14.113 -p 10000" << std::endl;
	this->fs << "comp.RightHipMotors.Proxy=RightHipMotors:default -h 192.168.14.113 -p 10000" << std::endl;
	this->fs << "comp.LeftKneeMotors.Proxy=LeftKneeMotors:default -h 192.168.14.113 -p 10000" << std::endl;
	this->fs << "comp.RightKneeMotors.Proxy=RightKneeMotors:default -h 192.168.14.113 -p 10000" << std::endl;
	this->fs << "comp.LeftAnkleMotors.Proxy=LeftAnkleMotors:default -h 192.168.14.113 -p 10000" << std::endl;
	this->fs << "comp.RightAnkleMotors.Proxy=RightAnkleMotors:default -h 192.168.14.113 -p 10000" << std::endl;

	this->fs.flush();
}

void Generate::generateCmake () {
	this->fs << "project (AUTOMATA)" << std::endl;
	this->fs << std::endl;
	this->fs << "cmake_minimum_required(VERSION 2.8)" << std::endl;
	this->fs << "include(FindPkgConfig)" << std::endl;
	this->fs << std::endl;
	this->fs << "SET( SOURCE_FILES_AUTOMATA " << this->getCppName() << " )" << std::endl;
	this->fs << std::endl;
	this->fs << "SET( INTERFACES_CPP_DIR /usr/local/lib/jderobot )" << std::endl;
	this->fs << "SET( LIBS_DIR /usr/local/include/jderobot )" << std::endl;
	this->fs << std::endl;
	this->fs << "SET( CMAKE_CXX_FLAGS \"-lpthread -lIce\" ) # Opciones para el compilador" << std::endl;
	this->fs << std::endl;
	this->fs << "SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})" << std::endl;
	this->fs << std::endl;
	this->fs << "include_directories (" << std::endl;
	this->fs << "\t${INTERFACES_CPP_DIR}" << std::endl;
	this->fs << "\t${LIBS_DIR}" << std::endl;
	this->fs << "\t${CMAKE_CURRENT_SOURCE_DIR}" << std::endl;
	this->fs << ")" << std::endl;
	this->fs << std::endl;
	this->fs << "add_executable (automata ${SOURCE_FILES_AUTOMATA})" << std::endl;
	this->fs << std::endl;
	this->fs << "TARGET_LINK_LIBRARIES ( automata " << std::endl;
	this->fs << "\t${INTERFACES_CPP_DIR}/libJderobotInterfaces.so" << std::endl;
	this->fs << "\t${INTERFACES_CPP_DIR}/libjderobotutil.so" << std::endl;
	this->fs << "\t/usr/lib/libIceUtil.so" << std::endl;
	this->fs << ")" << std::endl;
	
	this->fs.flush();
}

std::string Generate::getCppName () {
	size_t last_pos = this->path.find_last_of(std::string("/"));
	if (last_pos == std::string::npos)
        return NULL;

    return this->path.substr(last_pos + 1, std::string::npos);
}