#!/bin/bash

#  Copyright (C) JDE DEVELOPERS
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see http://www.gnu.org/licenses/.
#
#  Authors : Roberto Calvo <rocapal at gmail dot com>

idVendor="1d27"
idProduct="0601"

XTION_USB_DEVICE=""
XTION_BUS=""

function check_result {

	if [ "$?" != "0" ]; then
		echo "Error"
		exit
	else
		echo "OK"
	fi
}


IFS=$(echo -en "\n\b")
for X in /sys/bus/usb/devices/*; do      

	vendor=`cat "$X/idVendor" 2>/dev/null`
	product=`cat "$X/idProduct" 2>/dev/null`

	if [ "${vendor}" = "${idVendor}" ] && [ "${product}" = "${idProduct}" ]; then
		XTION_USB_DEVICE=$X
		break
	fi
done

echo "Found Xtion in $XTION_USB_DEVICE"

XTION_BUS=$(basename `echo $XTION_USB_DEVICE | cut -d"." -f-2`)
echo "Found Xtion Bus in $XTION_BUS"

echo -n "Unbind Xtion ... "
echo $XTION_BUS | sudo tee /sys/bus/usb/drivers/usb/unbind &>/dev/null
check_result

sleep 1
echo -n "Bind Xtion ... "
echo $XTION_BUS | sudo tee /sys/bus/usb/drivers/usb/bind &>/dev/null
check_result
