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
# Modified by:
#       Francisco Perez Salgado <f.perez475@gmail.com>
#

class SonarData ():

	def __init__(self):

		self.range = 0 # meters
		self.minAngle = 0 # Angle of first value (rads)
		self.maxAngle = 0 # Angle of last value (rads)
		self.minRange = 0 # Max Range posible (meters)
		self.maxRange = 0 #Min Range posible (meters)
		self.timeStamp = 0 # seconds


	def __str__(self):
		s = "SonarData: {\n   Range: " + str(self.range) + "\n minAngle: " + str(self.minAngle) + "\n   maxAngle: " + str(self.maxAngle)
		s = s + "\n   minRange: " + str(self.minRange) + "\n   maxRange: " + str(self.maxRange) 
		s = s + "\n   timeStamp: " + str(self.timeStamp)  + "\n}"
		return s 