import jderobot
import threading
import os
from threadImage import ThreadImage #camera isn a threadImage
import numpy as np
from picamera.array import PiRGBArray
from picamera import PiCamera

class WorkQueue(threading.Thread):
		def __init__(self):
				self.callbacks = []
				threading.Thread.__init__(self)
				

		def run(self):
				if not len(self.callbacks) == 0:
						self.callbacks[0].execute()
						del self.callbacks[0]
			
		def add(self, job):
				self.callbacks.append(job)
				self.run()
				

class Job(object):
	
	def __init__(self,cb,format, camera, uri):
		self.cb = cb
		self.format = format
		self.camera = camera
		self.imageData = jderobot.ImageData()
		self.uri = uri
	def execute(self):
		if not self.getData():
			print "No data"
			return
		self.cb.ice_response(self.imageData)

	def getData(self):
		img = self.camera.get_image()
		self.imageData = jderobot.ImageData()
		self.imageData.description = jderobot.ImageDescription()

		if self.uri == 2:
			self.imageData.description.width = 640
			self.imageData.description.height = 480
		else:
			self.imageData.description.width = img.shape[1]
			self.imageData.description.height = img.shape[0]

		self.pixelData = np.getbuffer(img)
		self.imageData.description.format = 'RGB8'
		self.imageData.pixelData = self.pixelData

		return True

class ImageProviderI(jderobot.Camera):

	def __init__(self,workQueue, camera, uri):
		self.workQueue = workQueue
		self.camera = camera
		self.uri = uri

	def getCameraDescription(self, current=None):
		return jderobot.CameraDescription()

	def setCameraDescription(self, description):
		return 0

	def startCameraStreaming(self, current=None):
		return ''
	
	def stopCameraStreaming(self,current=None):
		print('')

	def reset(self):
		print('---')

	def getImageDescription(self,current=None):

		self.imageData = jderobot.ImageDescription() 
		img = self.camera.get_image()
		if self.uri == 2:
			self.imageData.width = 640
			self.imageData.height = 480
		else:
			self.imageData.width = img.shape[1]
			self.imageData.height = img.shape[0]
			
		self.format = 'RGB'    
		return self.imageData

	def getImageData_async(self,cb,formato,curren=None):
		job = Job(cb,formato, self.camera,self.uri)
		return self.workQueue.add(job)
