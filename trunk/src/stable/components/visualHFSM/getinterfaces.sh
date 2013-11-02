#!/bin/bash

<<LICENSE
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
LICENSE

allInterfaces=`egrep -ri interface $1 | awk '{ if ($2 == "interface") print $1 $3 }' | sort`
for interface in $allInterfaces
do
    class=`echo $interface | cut -f1 -d: | cut -f7 -d/ | cut -f1 -d.`
    theInterface=`echo $interface | cut -f2 -d:`
    i="$((${#theInterface}-1))"
    last=${theInterface:$i:1}
    if [ $last == "{" ]; then
        echo "${theInterface%?} $class"
    else
        echo "$theInterface $class"
    fi
done

exit 0