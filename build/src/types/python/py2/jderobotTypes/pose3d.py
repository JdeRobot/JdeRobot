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

class Pose3d ():

	def __init__(self):

		self.x = 0 # X coord [meters]
		self.y = 0 # Y coord [meters]
		self.z = 0 # Z coord [meters]
		self.h = 1 # H param
		self.yaw = 0 #Yaw angle[rads]
		self.pitch = 0 # Pitch angle[rads]
		self.roll = 0 # Roll angle[rads]
		self.q = [0,0,0,0] # Quaternion
		self.timeStamp = 0 # Time stamp [s]


	def __str__(self):
		s = "Pose3D: {\n   x: " + str(self.x) + "\n   Y: " + str(self.y)
		s = s + "\n   Z: " + str(self.z) + "\n   H: " + str(self.h) 
		s = s + "\n   Yaw: " + str(self.yaw) + "\n   Pitch: " + str(self.pitch) + "\n   Roll: " + str(self.roll)
		s = s + "\n   quaternion: " + str(self.q) + "\n   timeStamp: " + str(self.timeStamp)  + "\n}"
		return s 