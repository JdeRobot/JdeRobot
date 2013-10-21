/*
 *  Copyright (C) 1997-2010 JDE Developers Team
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
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

#ifndef WIIMOTE_ICE
#define WIIMOTE_ICE

#include <jderobot/common.ice>

module jderobot{

	/* Wiimote information */
  class AccelerometerData
  {
        IntSeq accelerometer;
	
  };

  class InfraredData{
	
	IntSeq infrared1;
	IntSeq infrared2;
	IntSeq infrared3;
	IntSeq infrared4;
	int sourceDetected;

  };

  class NunchukData{

	int button;
	IntSeq stick;
	IntSeq acc;

  };
  

   /* 
   * Interface to the Wiimote interaction.
   */
    interface wiiMote {
	//SET DIFFERENT MODES
        int setValue(int Value);
        int changeRumbleMode();
        int changeIrMode();
        int changeAccMode();
        int changeButtonMode();
	    int changeNunchukMode();
	    int activateLed(int led);

	//GET DATA
        int getButtonData();

	idempotent NunchukData getNunchukData();	
	idempotent AccelerometerData getAccData();
	idempotent InfraredData getIrData();
	int getBatteryStatus();
    };

}; //module

#endif //WIIMOTE_ICE
