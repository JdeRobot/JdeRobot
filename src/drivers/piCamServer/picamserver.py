import Ice
import sys
import config
import cv2
from ImageProviderI import ImageProviderI,WorkQueue
from threadImage import ThreadImage
import imutils

if __name__== "__main__":

	try:
		cfg = config.load(sys.argv[1])
		id = Ice.InitializationData()
		ic = Ice.initialize(None, id)

		endpoint = cfg.getProperty("piCamServer.Proxy")
		fps = cfg.getProperty("piCamServer.FrameRate")
		uri = cfg.getProperty("piCamServer.Uri") # uri = 2 ==> PiCamera
		name = cfg.getProperty("piCamServer.Name")

		if uri == 2:
			#from imutils.video.pivideostream import PiVideoStream
			from picamera import PiCamera

			camera = PiCamera() # Por defecto se conectara al banco 0, el unico que tenemos para la PiCam
			camera.resolution = (640, 480)
			camera.framerate = 32
		else:
			camera = cv2.VideoCapture(uri)
		
		workQueue = WorkQueue()
		workQueue.setDaemon(True)

		imageThread = ThreadImage(camera, fps, uri)

		imageThread.start()
		workQueue.start()

		adapter = ic.createObjectAdapterWithEndpoints("youtubeServer",endpoint) #Properties
		object = ImageProviderI(workQueue, imageThread, uri)
		adapter.add(object, Ice.stringToIdentity(name))
		adapter.activate()
		workQueue.join()
		print (name, ":", endpoint)
		ic.waitForShutdown()

	except KeyboardInterrupt:
		imageThread.stop()
		del(camera)
		del(ic)
		sys.exit() 
