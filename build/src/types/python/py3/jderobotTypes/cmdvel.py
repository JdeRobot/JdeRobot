#
#  Copyright (C) 1997-2017 JDE Developers Team
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
#  Authors :
#       Aitor Martinez Fernandez <aitor.martinez.fernandez@gmail.com>
#

class CMDVel ():

	def __init__(self):

		self.vx = 0 # vel in x[m/s] (use this for V in wheeled robots)
		self.vy = 0 # vel in y[m/s]
		self.vz = 0 # vel in z[m/s]
		self.ax = 0 # angular vel in X axis [rad/s]
		self.ay = 0 # angular vel in X axis [rad/s]
		self.az = 0 # angular vel in Z axis [rad/s] (use this for W in wheeled robots)
		self.timeStamp = 0 # Time stamp [s]


	def __str__(self):
		s = "CMDVel: {\n   vx: " + str(self.vx) + "\n   vy: " + str(self.vy)
		s = s + "\n   vz: " + str(self.vz) + "\n   ax: " + str(self.ax) 
		s = s + "\n   ay: " + str(self.ay) + "\n   az: " + str(self.az)
		s = s + "\n   timeStamp: " + str(self.timeStamp)  + "\n}"
		return s 