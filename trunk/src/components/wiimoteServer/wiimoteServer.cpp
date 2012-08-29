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

cwiid_err_t err;

float value_button;

cwiid_mesg_callback_t cwiid_callback;

using namespace std;

//using namespace Demo;

void set_rpt_mode(cwiid_wiimote_t *wiimote, unsigned char rpt_mode) {
    if (cwiid_set_rpt_mode(wiimote, rpt_mode)) {
        fprintf(stderr, "Error setting report mode\n");
    }
}

void cwiid_callback(cwiid_wiimote_t *wiimote, int mesg_count,
        union cwiid_mesg mesg[], struct timespec *timestamp) {
    int i, j;
    int valid_source;

    for (i = 0; i < mesg_count; i++) {
        switch (mesg[i].type) {
            case CWIID_MESG_STATUS:
                printf("Status Report: battery=%d extension=",
                        mesg[i].status_mesg.battery);
                switch (mesg[i].status_mesg.ext_type) {
                    case CWIID_EXT_NONE:
                        printf("none");
                        break;
                    case CWIID_EXT_NUNCHUK:
                        printf("Nunchuk");
                        break;
                    case CWIID_EXT_CLASSIC:
                        printf("Classic Controller");
                        break;
                    case CWIID_EXT_BALANCE:
                        printf("Balance Board");
                        break;
                    case CWIID_EXT_MOTIONPLUS:
                        printf("MotionPlus");
                        break;
                    default:
                        printf("Unknown Extension");
                        break;
                }
                printf("\n");
                break;
            case CWIID_MESG_BTN:
                //printf("Button Report: %.4X\n", mesg[i].btn_mesg.buttons);
                if (mesg[i].btn_mesg.buttons != 0) {
                    value_button = mesg[i].btn_mesg.buttons;
                    cout << value_button << endl;
                }
                break;
            case CWIID_MESG_ACC:
                printf("Acc Report: x=%d, y=%d, z=%d\n",
                        mesg[i].acc_mesg.acc[CWIID_X],
                        mesg[i].acc_mesg.acc[CWIID_Y],
                        mesg[i].acc_mesg.acc[CWIID_Z]);
                break;
            case CWIID_MESG_IR:
                printf("IR Report: ");
                valid_source = 0;
                for (j = 0; j < CWIID_IR_SRC_COUNT; j++) {
                    if (mesg[i].ir_mesg.src[j].valid) {
                        valid_source = 1;
                        printf("(%d,%d) ", mesg[i].ir_mesg.src[j].pos[CWIID_X],
                                mesg[i].ir_mesg.src[j].pos[CWIID_Y]);
                    }
                }
                if (!valid_source) {
                    printf("no sources detected");
                }
                printf("\n");
                break;
            case CWIID_MESG_NUNCHUK:
                printf("Nunchuk Report: btns=%.2X stick=(%d,%d) acc.x=%d acc.y=%d "
                        "acc.z=%d\n", mesg[i].nunchuk_mesg.buttons,
                        mesg[i].nunchuk_mesg.stick[CWIID_X],
                        mesg[i].nunchuk_mesg.stick[CWIID_Y],
                        mesg[i].nunchuk_mesg.acc[CWIID_X],
                        mesg[i].nunchuk_mesg.acc[CWIID_Y],
                        mesg[i].nunchuk_mesg.acc[CWIID_Z]);
                break;
            case CWIID_MESG_CLASSIC:
                printf("Classic Report: btns=%.4X l_stick=(%d,%d) r_stick=(%d,%d) "
                        "l=%d r=%d\n", mesg[i].classic_mesg.buttons,
                        mesg[i].classic_mesg.l_stick[CWIID_X],
                        mesg[i].classic_mesg.l_stick[CWIID_Y],
                        mesg[i].classic_mesg.r_stick[CWIID_X],
                        mesg[i].classic_mesg.r_stick[CWIID_Y],
                        mesg[i].classic_mesg.l, mesg[i].classic_mesg.r);
                break;
            case CWIID_MESG_BALANCE:
                printf("Balance Report: right_top=%d right_bottom=%d "
                        "left_top=%d left_bottom=%d\n",
                        mesg[i].balance_mesg.right_top,
                        mesg[i].balance_mesg.right_bottom,
                        mesg[i].balance_mesg.left_top,
                        mesg[i].balance_mesg.left_bottom);
                break;
            case CWIID_MESG_MOTIONPLUS:
                printf("MotionPlus Report: angle_rate=(%d,%d,%d) low_speed=(%d,%d,%d)\n",
                        mesg[i].motionplus_mesg.angle_rate[0],
                        mesg[i].motionplus_mesg.angle_rate[1],
                        mesg[i].motionplus_mesg.angle_rate[2]);
                //			       mesg[i].motionplus_mesg.low_speed[0],
                //			       mesg[i].motionplus_mesg.low_speed[1],
                //			       mesg[i].motionplus_mesg.low_speed[2]);
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

void err(cwiid_wiimote_t *wiimote, const char *s, va_list ap) {
    if (wiimote) printf("%d:", cwiid_get_id(wiimote));
    else printf("-1:");
    vprintf(s, ap);
    printf("\n");
}

namespace wiimoteServer {
    
    class ApiMote {
    public:
        void runWiimote (){

            cwiid_set_err(err);

            /* Connect to address given on command-line, if present */
            
            this->mesg = 0;
            this->led_state = 0;
            this->rumble = 0;
            this->exit = 0;

            

            bdaddr = *BDADDR_ANY;


            /* Connect to the wiimote */
            printf("Put Wiimote in discoverable mode now (press 1+2)...\n");
            if (!(wiimote = cwiid_open(&bdaddr, 0))) {
                fprintf(stderr, "Unable to connect to wiimote\n");
                //return -1;
            }
            if (cwiid_set_mesg_callback(wiimote, cwiid_callback)) {
                fprintf(stderr, "Unable to set message callback\n");
            }

            printf("Note: To demonstrate the new API interfaces, wmdemo no longer "
                    "enables messages by default.\n"
                    "Output can be gathered through the new state-based interface (s), "
                    "or by enabling the messages interface (m).\n");


            //SET BUTTONS IN ON
            unsigned char rpt_mode = 0;
            toggle_bit(rpt_mode, CWIID_RPT_BTN);
            set_rpt_mode(wiimote, rpt_mode);

            //SET IN MODE PRINT
            if (cwiid_enable(wiimote, CWIID_FLAG_MESG_IFC)) {
                fprintf(stderr, "Error enabling messages\n");
            }

        }
        int button;
        
        
    private:
            cwiid_wiimote_t *wiimote; /* wiimote handle */
            struct cwiid_state state; /* wiimote state */
            bdaddr_t bdaddr; /* bluetooth device address */
            unsigned char mesg;
            unsigned char led_state;

            unsigned char rumble;
            int exit;

    };
    
    ApiMote *api = new ApiMote();
    
    class WiiMoteI : virtual public jderobot::wiiMote {
    public:

        WiiMoteI(std::string& propertyPrefix, const jderobotice::Context& context) {
            Ice::PropertiesPtr prop = context.properties();
        }

        virtual Ice::Int saludar(const Ice::Current&) {
            cout << "¡Hola Mundo!" << endl;
            cout << api->button << endl;
            //printf("Hola Mundo");
            return 0;
        }

        virtual Ice::Int despedir(const Ice::Current&) {
            cout << "!Adios Mundo!" << endl;
            //printf("Hola Mundo");
            return 0;
        }

        virtual Ice::Int getValue(const Ice::Current&) {
            return value_button;
        }

        virtual Ice::Int setValue(Ice::Int value, const Ice::Current&) {
            cout << value << endl;
            //printf("%d", value);
            return 0;
        }

    };

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
