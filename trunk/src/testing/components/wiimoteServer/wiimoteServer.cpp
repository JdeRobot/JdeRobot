// <editor-fold defaultstate="collapsed" desc="comment">
/*
 *  Copyright (C) 1997-2010 JDE Developers Team
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundationfile:///home/mikel/Escritorio/PFC/repository_JDErobot/Workspace/trunk/src/components/wiimoteClient/wiimoteClient.cpp
, either version 3 of the License, or
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
 *  Authors : Jose María Cañas <jmplaza@gsyc.es>
 *            Maikel González Baile <m.gonzalezbai@gmail.com>
 *
 */

#include <jderobot/wiimote.h>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <gbxsickacfr/gbxiceutilacfr/safethread.h>
#include <colorspaces/colorspacesmm.h>
#include <jderobotice/component.h>
#include <jderobotice/application.h>
#include <tr1/memory>
#include <list>
#include <sstream>
#include <jderobotice/exceptions.h>
#include <float.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <bluetooth/bluetooth.h>
#include <cwiid.h>

#define toggle_bit(bf,b)	\
	(bf) = ((bf) & b)		\
	       ? ((bf) & ~(b))	\
	       : ((bf) | (b))
using namespace std;

    cwiid_err_t err;
    cwiid_wiimote_t *wiimote = NULL;
    cwiid_mesg_callback_t cwiid_callback;
    bdaddr_t bdaddr;
    struct acc_cal nc_cal;

namespace wiimoteServer {

    //Global Functions
    //set_rpt_mode: Toggle sensors data reporting
    void set_rpt_mode(cwiid_wiimote_t *wiimote, unsigned char rpt_mode);
    //err: Manages wiimote errors
    void err(cwiid_wiimote_t *wiimote, const char *s, va_list ap);
    //cwiid_callback: manages data received by the wiimote's sensors
    void cwiid_callback(cwiid_wiimote_t *wiimote, int mesg_count,
            union cwiid_mesg mesg[], struct timespec *timestamp);
    //Activates leds on wiimote
    void set_led_state(cwiid_wiimote_t *wiimote, unsigned char led_state);

    //ApiMote: Middleware to communicate the WiiMoteI class with Component class.

    class ApiMote {
    public:

        // starts the connection with the wiimote and runs the "callback" method which manages the wiimote data obtained by.

        
        
        void runWiimote() {

            //cwiid_set_err(err);
            
            unsigned char mesg;
            
            
            mesg = 0;
            this->led_state = 0;
            
            this->rumbleM = 0;
            this->exit = 0;

            //Connect to address given on command-line, if present
            bdaddr = *BDADDR_ANY;
            usleep(100);
            cout << endl;
            //Connect to the wiimote
            cout << "*******************************************************" << endl;
            printf("* Put Wiimote in discoverable mode now (press 1+2)... *\n");
            cout << "*******************************************************" << endl;
            if (!(wiimote = cwiid_open(&bdaddr, CWIID_FLAG_MESG_IFC))) {
                fprintf(stderr, "Unable to connect to wiimote\n");
                //return -1;
            }
            
            
            
            if (cwiid_set_mesg_callback(wiimote, &cwiid_callback)) {
                fprintf(stderr, "Unable to set message callback\n");
            }else{
                cwiid_get_acc_cal(wiimote, CWIID_EXT_NONE, &nc_cal);
            }
            
            uint8_t rpt_mode;
            
            rpt_mode = CWIID_RPT_STATUS | CWIID_RPT_BTN;
            
            cwiid_set_rpt_mode(wiimote, rpt_mode);

            cout << endl;
            cout << "Info: wiimote ready to use" << endl;
        }


        int buttonApi;
        int accApi[3];
        int irApi1[2];
        int irApi2[2];
        int irApi3[2];
        int irApi4[2];
        int nunchukAccApi[3];
        int nunchukStickApi[2];
        int nunchukButtonApi;
        int sourceDetected;
        int rumbleM;
        unsigned char led_state;

    private:
        int exit;

    };

    //Global variables and functions
    ApiMote *api = new ApiMote();

    
    void cwiid_callback(cwiid_wiimote_t *wiimote, int mesg_count,
            union cwiid_mesg mesg[], struct timespec *timestamp) {
        int i, j;
        int valid_source;
        double a_x, a_y, a_z;

        for (i = 0; i < mesg_count; i++) {
            switch (mesg[i].type) {
                case CWIID_MESG_BTN:                    
                        api->buttonApi = mesg[i].btn_mesg.buttons;
                    break;
                case CWIID_MESG_ACC:
                    api->accApi[0] = mesg->acc_mesg.acc[CWIID_X];
                    api->accApi[1] = mesg->acc_mesg.acc[CWIID_Y];
                    api->accApi[2] = mesg->acc_mesg.acc[CWIID_Z];
//                    cout << "x" << mesg->acc_mesg.acc[CWIID_X] << endl;
//                    cout << "y" << mesg->acc_mesg.acc[CWIID_Y] << endl;
//                    cout << "z" << mesg->acc_mesg.acc[CWIID_Y] << endl;
                    break;  
                case CWIID_MESG_IR:
                    valid_source = 0;
                    for (j = 0; j < CWIID_IR_SRC_COUNT; j++) {
                        if (mesg[i].ir_mesg.src[j].valid) {
                            valid_source = 1;
                            if (j == 0) {
                                api->irApi1[0] = mesg[i].ir_mesg.src[j].pos[CWIID_X];
                                api->irApi1[1] = mesg[i].ir_mesg.src[j].pos[CWIID_Y];
                                api->sourceDetected = 1;
                            }
                            if (j == 1) {
                                api->irApi2[0] = mesg[i].ir_mesg.src[j].pos[CWIID_X];
                                api->irApi2[1] = mesg[i].ir_mesg.src[j].pos[CWIID_Y];
                                api->sourceDetected = 1;
                            }
                            if (j == 2) {
                                api->irApi3[0] = mesg[i].ir_mesg.src[j].pos[CWIID_X];
                                api->irApi3[1] = mesg[i].ir_mesg.src[j].pos[CWIID_Y];
                                api->sourceDetected = 1;
                            }
                            if (j == 3){
                                api->irApi4[0] = mesg[i].ir_mesg.src[j].pos[CWIID_X];
                                api->irApi4[1] = mesg[i].ir_mesg.src[j].pos[CWIID_Y];
                                api->sourceDetected = 1;

                            }
                        }
                    }
                    if (!valid_source) {
                        api->sourceDetected = 0;
                    }
                    break;                    
  		case CWIID_MESG_NUNCHUK:
			       
                   api->nunchukButtonApi = mesg[i].nunchuk_mesg.buttons;
			       api->nunchukStickApi[0] = mesg[i].nunchuk_mesg.stick[CWIID_X];
			       api->nunchukStickApi[1] = mesg[i].nunchuk_mesg.stick[CWIID_Y];
			       api->nunchukAccApi[0] = mesg[i].nunchuk_mesg.acc[CWIID_X];
			       api->nunchukAccApi[1] =  mesg[i].nunchuk_mesg.acc[CWIID_Y];
			       api->nunchukAccApi[2] =  mesg[i].nunchuk_mesg.acc[CWIID_Z];
			       
			       //cout << api->nunchukStickApi[0] << endl;
			       //			       cout << api->nunchukStickApi[1] << endl;

			break;                  



                case CWIID_MESG_MOTIONPLUS:
                    printf("MotionPlus Report: angle_rate=(%d,%d,%d) low_speed=(%d,%d,%d)\n",
                            mesg[i].motionplus_mesg.angle_rate[0],
                            mesg[i].motionplus_mesg.angle_rate[1],
                            mesg[i].motionplus_mesg.angle_rate[2]);
                    break;
                case CWIID_MESG_ERROR:
                    if (cwiid_close(wiimote)) {
                        fprintf(stderr, "Error on wiimote disconnect\n");
                        exit(-1);
                    }
                    exit(0);
                    break;
                default:
                    printf("Unknown Report");
                    break;
            }
        }

    }
    
    void set_led_state(cwiid_wiimote_t *wiimote, unsigned char led_state)
{
	if (cwiid_set_led(wiimote, led_state)) {
		fprintf(stderr, "Error setting LEDs \n");
	}
}

    //WiiMoteI: Defines the methods declared in wiimote.ice to communicate it with other clients.

    class WiiMoteI : virtual public jderobot::wiiMote {
    public:

        WiiMoteI(std::string& propertyPrefix, const jderobotice::Context& context) : chuckData(new jderobot::NunchukData), irData(new jderobot::InfraredData), accData(new jderobot::AccelerometerData) {
            Ice::PropertiesPtr prop = context.properties();
            accData->accelerometer.resize(sizeof (int) *3);
            irData->infrared1.resize(sizeof (int) *2);
            irData->infrared2.resize(sizeof (int) *2);
            irData->infrared3.resize(sizeof (int) *2);
            irData->infrared4.resize(sizeof (int) *2);
            chuckData->stick.resize(sizeof (int) *2);
            chuckData->acc.resize(sizeof (int) *3);
        }

        virtual Ice::Int getButtonData(const Ice::Current&) {
            return api->buttonApi;
        }

        virtual jderobot::AccelerometerDataPtr getAccData(const Ice::Current&) {
            accData->accelerometer[0] = api->accApi[0];
            accData->accelerometer[1] = api->accApi[1];
            accData->accelerometer[2] = api->accApi[2];

            return accData;
        }

        virtual jderobot::InfraredDataPtr getIrData(const Ice::Current&) {
            irData->infrared1[0] = api->irApi1[0];
            irData->infrared1[1] = api->irApi1[1];
            irData->infrared2[0] = api->irApi2[0];
            irData->infrared2[1] = api->irApi2[1];
            irData->infrared3[0] = api->irApi3[0];
            irData->infrared3[1] = api->irApi3[1];
            irData->infrared4[0] = api->irApi4[0];
            irData->infrared4[1] = api->irApi4[1];

            irData->sourceDetected = api->sourceDetected;
            return irData;
        }

        virtual jderobot::NunchukDataPtr getNunchukData(const Ice::Current&) {

            chuckData->stick[0] = api->nunchukStickApi[0];
            chuckData->stick[1] = api->nunchukStickApi[1];
            chuckData->acc[0] = api->nunchukAccApi[0];
            chuckData->acc[1] = api->nunchukAccApi[1];
            chuckData->acc[2] = api->nunchukAccApi[2];
            chuckData->button = api->nunchukButtonApi;

            return chuckData;
        }

        virtual Ice::Int setValue(Ice::Int value, const Ice::Current&) {
            cout << value << endl;
            return 0;
        }

        virtual Ice::Int changeRumbleMode(const Ice::Current&) {
            if (cwiid_set_rumble(wiimote, 1)) {
                fprintf(stderr, "Error setting rumble\n");
            }

            return 1;
        }

        virtual Ice::Int changeIrMode(const Ice::Current&) {
            uint8_t rpt_mode;
            //rpt_mode = CWIID_RPT_STATUS | CWIID_RPT_BTN;
            rpt_mode |= CWIID_RPT_IR | CWIID_RPT_ACC | CWIID_RPT_NUNCHUK | CWIID_RPT_BTN;
            if(cwiid_set_rpt_mode(wiimote, rpt_mode)){
                std::cout << "error setting report mode" << std::endl;
            }
              

            return 1;
        }

        virtual Ice::Int changeAccMode(const Ice::Current&) {
            uint8_t rpt_mode;
            
            rpt_mode |= CWIID_RPT_ACC | CWIID_RPT_NUNCHUK;
            if(cwiid_set_rpt_mode(wiimote, rpt_mode)){
                std::cout << "error setting report mode" << std::endl;
            }
            return 1;
        }

        virtual Ice::Int changeButtonMode(const Ice::Current&) {
            uint8_t rpt_mode;
            //rpt_mode = CWIID_RPT_STATUS | CWIID_RPT_BTN;
            rpt_mode |= CWIID_RPT_BTN | CWIID_RPT_ACC | CWIID_RPT_NUNCHUK;
            if(cwiid_set_rpt_mode(wiimote, rpt_mode)){
                std::cout << "error setting report mode" << std::endl;
            }
            return 1;

        }

        virtual Ice::Int changeNunchukMode(const Ice::Current&) {
            uint8_t rpt_mode;
            rpt_mode = CWIID_RPT_STATUS | CWIID_RPT_BTN;
            rpt_mode |= CWIID_RPT_EXT;
            if(cwiid_set_rpt_mode(wiimote, rpt_mode)){
                std::cout << "error setting report mode" << std::endl;
            }
            return 1;

        }

        virtual Ice::Int getBatteryStatus(const Ice::Current&) {
            struct cwiid_state state; // wiimote state
            if (cwiid_get_state(wiimote, &state)) {
                fprintf(stderr, "Error getting state\n");
            }

            return (int) (100.0 * state.battery / CWIID_BATTERY_MAX);
        }

        virtual Ice::Int activateLed(Ice::Int led, const Ice::Current&) {
            switch(led){
                case 1:
                    toggle_bit(api->led_state, CWIID_LED1_ON);
                    set_led_state(wiimote, api->led_state);                    
                    break;
                case 2:
                    toggle_bit(api->led_state, CWIID_LED2_ON);
                    set_led_state(wiimote, api->led_state);                    
                    break;
                case 3:
                    toggle_bit(api->led_state, CWIID_LED3_ON);
                    set_led_state(wiimote, api->led_state);                    
                    break;
                case 4:
                    toggle_bit(api->led_state, CWIID_LED4_ON);
                    set_led_state(wiimote, api->led_state);                    
                    break;
                default:
                    break;
                    
            }

            return 1;
        }


    private:
        jderobot::AccelerometerDataPtr accData;
        jderobot::InfraredDataPtr irData;
        jderobot::NunchukDataPtr chuckData;
    };

    //Component: Here is established the connection between the server and clients through ICE

    class Component : public jderobotice::Component {
    public:

        Component()
        : jderobotice::Component("WiimoteServer"), wiiMote1(0) {
        }

        virtual void start() {
            Ice::PropertiesPtr prop = context().properties();
            std::string objPrefix2 = "wiiMote1";
            std::string gazeboactName = "wiiMote1";
            context().tracer().info("Creating wiiMote1 " + gazeboactName);
            wiiMote1 = new WiiMoteI(objPrefix2, context());
            context().createInterfaceWithString(wiiMote1, gazeboactName);
            api->runWiimote();
        }

        virtual ~Component() {
        }

    private:
        Ice::ObjectPtr wiiMote1;


    };

}//end namespace

int main(int argc, char** argv) {
    wiimoteServer::Component component;
    jderobotice::Application app(component);
    return app.jderobotMain(argc, argv);
}
