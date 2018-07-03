# -*- coding: utf-8 -*-
from JdeRobotKids import JdeRobotKids
import Ice
import numpy
import threading
import sys
import comm
import config
import progeo

class PiBot:

	'''
	Controlador para el Robot PiBot de JdeRobot-Kids
	Actualmente incluye dos funciones de procesamiento visual:
	- damePosicionDeObjetoDeColor
	- dameSonarVisual
	'''
	def __init__(self, camara):
		# Libreria RPi.GPIO
		print("real")
		import RPi.GPIO as GPIO
		import pigpio # Libreria para manejar los servos
		JdeRobotKids.__init__(self)
		self._GPIO = GPIO
		self._tipo = "PiBot"
		self._dit = pigpio.pi()
		self._frame = None
		if camara == "PiCam"
			self._videostream = VideoStream(usePiCamera=True).start()
		else
			self._videostream = VideoStream(usePiCamera=False).start()
		'''
		props = Ice.createProperties()
		props.setProperty("JdeRobotKids.Motors.Proxy", "Motors:default -h localhost -p 9999")
		props.setProperty("JdeRobotKids.Motors.maxV", "1")
		props.setProperty("JdeRobotKids.Motors.maxW", "0.5")
		id = Ice.InitializationData()
		id.properties = props
		ic = Ice.initialize(id)
		self.motors = Motors (ic, "JdeRobotKids.Motors")
		'''

	def moverServo(self, *args):
		'''
		Función que hace girar al servo motor a un angulo dado como parámetro.
		@type args: lista
		@param args: lista de argumentos:
		args[0]: puerto al que esta conectado el controlador del servo
		args[1]: banco al que esta conectado el servo en el controlador
		args[2]: angulo de giro del servo. 0-180 grados. ¡PROBAR GIRO ANTES DE MONTAR EL SERVO!
		'''
		puerto = args[0]
		banco = args[1]
		angulo = args[2]/10

		self._GPIO.setmode(GPIO.BOARD)   #Ponemos la Raspberry en modo BOARD
		self._GPIO.setup(puerto,GPIO.OUT)	#Ponemos el pin args[0] como salida
		p = self._GPIO.PWM(puerto,50)		#Ponemos el pin args[0] en modo PWM y enviamos 50 pulsos por segundo
		p.start(7.5)			   #Enviamos un pulso del 7.5% para centrar el servo
		p.ChangeDutyCycle(angulo)
		time.sleep(0.5)  

	def avanzar(self, vel):
		'''
		Función que hace avanzar al robot en línea recta a una velocidad dada como parámetro.
		@type vel: entero
		@param vel: velocidad de avance del robot (máximo 255)
		'''
	# Puertos de datos para servos izquierdo y derecho
		puertoL = 4 
		puertoR = 18
		#servos = pigpio.pi()
		self._dit.set_servo_pulsewidth(puertoL, 1720)
		self._dit.set_servo_pulsewidth(puertoR, 1280)

	def retroceder(self, vel):
		'''
		Función que hace retroceder al robot en línea recta a una velocidad dada como parámetro.
		@type vel: entero
		@param vel: velocidad de retroceso del robot (máximo 255)
		'''
	# Puertos de datos para servos izquierdo y derecho
		puertoL = 4 
		puertoR = 18
		#servos = pigpio.pi()
		self._dit.set_servo_pulsewidth(puertoL, 1280)
		self._dit.set_servo_pulsewidth(puertoR, 1720)

	def parar(self):
		'''
		Función que hace detenerse al robot.
		'''
	# Puertos de datos para servos izquierdo y derecho
		puertoL = 4 
		puertoR = 18
		#servos = pigpio.pi()
		self._dit.set_servo_pulsewidth(puertoL, 1500)
		self._dit.set_servo_pulsewidth(puertoR, 1480)

	def girarIzquierda(self, vel):
		'''
		Función que hace rotar al robot sobre sí mismo hacia la izquierda a una velocidad dada como parámetro.
		@type vel: entero
		@param vel: velocidad de giro del robot (máximo 255)
		'''
	# Puertos de datos para servos izquierdo y derecho
		puertoL = 4 
		puertoR = 18
		#servos = pigpio.pi()
		self._dit.set_servo_pulsewidth(puertoL, 1530)
		self._dit.set_servo_pulsewidth(puertoR, 1380)

	def girarDerecha(self, vel):
		'''
		Función que hace rotar al robot sobre sí mismo hacia la derecha a una velocidad dada como parámetro.
		@type vel: entero
		@param vel: velocidad de giro del robot (máximo 255)
		'''
	# Puertos de datos para servos izquierdo y derecho
		puertoL = 4 
		puertoR = 18
		#servos = pigpio.pi()
		self._dit.set_servo_pulsewidth(puertoL, 1620)
		self._dit.set_servo_pulsewidth(puertoR, 1460)

        def move(self, velV, velW):
		'''
		Función que hace avanzar y girar al robot al mismo tiempo, según las velocidades V,W dadas como parámetro.

		@type velV, velW: enteros de [1 a 5, a mas, se corta en 5]
		@param velV, velW: velocidades de avance de motores lineal y angular, en m/s y rad/s respectivamente
		'''
		# Puertos de datos para servos izquierdo y derecho
		puertoL = 4 
		puertoR = 18
		#servos = pigpio.pi()
		if ((velV == 0) and (velW != 0)): # pivotar
			if (velW > 0): # hacia derecha
				if (velW < 3): # despacio
					self._dit.set_servo_pulsewidth(puertoL, 1550)
					self._dit.set_servo_pulsewidth(puertoR, 1520)

				else: # a tope
					self._dit.set_servo_pulsewidth(puertoL, 1700)
					self._dit.set_servo_pulsewidth(puertoR, 1700)

			else: # hacia izquierda
				if (velW > -3): # despacio
					self._dit.set_servo_pulsewidth(puertoL, 1520)
					self._dit.set_servo_pulsewidth(puertoR, 1490)

				else: # a tope
					self._dit.set_servo_pulsewidth(puertoL, 1300)
					self._dit.set_servo_pulsewidth(puertoR, 1300)

		elif ((velV != 0) and (velW == 0)): # linea recta
			if (velV > 0): # adelante
				if (velV < 2): # minimo
					self._dit.set_servo_pulsewidth(puertoL, 1530)
					self._dit.set_servo_pulsewidth(puertoR, 1509)
				elif (velV <= 4): # despacio
					self._dit.set_servo_pulsewidth(puertoL, 1540)
					self._dit.set_servo_pulsewidth(puertoR, 1500)
				else: # a tope
					self._dit.set_servo_pulsewidth(puertoL, 1700)
					self._dit.set_servo_pulsewidth(puertoR, 1300)

			else: # atras
				if (velV > -3): # despacio
					self._dit.set_servo_pulsewidth(puertoL, 1520)
					self._dit.set_servo_pulsewidth(puertoR, 1520)

				else: # a tope
					self._dit.set_servo_pulsewidth(puertoL, 1300)
					self._dit.set_servo_pulsewidth(puertoR, 1700)

		elif ((velV == 0) and (velW == 0)): # PARADO
			self._dit.set_servo_pulsewidth(puertoL, 1525)
			self._dit.set_servo_pulsewidth(puertoR, 1510)
		
		elif ((velV > 0) and (velV <= 3)): # COMBINACION DE VELOCIDADES, AVANZANDO DESPACIO...
			if ((velW > 0) and (velW <= 3)): # ...mientras gira despacio a derecha
				self._dit.set_servo_pulsewidth(puertoL, 1540)
				self._dit.set_servo_pulsewidth(puertoR, 1509)
			elif (velW > 3): # ...mientras gira rapido a derecha
				self._dit.set_servo_pulsewidth(puertoL, 1700)
				self._dit.set_servo_pulsewidth(puertoR, 1509)
			elif ((velW < 0) and (velW >= -3)): # ...mientras gira despacio a izquierda
				self._dit.set_servo_pulsewidth(puertoL, 1530)
				self._dit.set_servo_pulsewidth(puertoR, 1500)
			elif (velW < -3): # ...mientras gira rapido a izquierda
				self._dit.set_servo_pulsewidth(puertoL, 1530)
				self._dit.set_servo_pulsewidth(puertoR, 1300)

		elif (velV > 3): # COMBINACION DE VELOCIDADES, AVANZANDO RAPIDO...
			if ((velW > 0) and (velW <= 3)): # ...mientras gira despacio a derecha
				self._dit.set_servo_pulsewidth(puertoL, 1700)
				self._dit.set_servo_pulsewidth(puertoR, 1500)
			elif (velW > 3): # ...mientras gira rapido a derecha
				self._dit.set_servo_pulsewidth(puertoL, 1700)
				self._dit.set_servo_pulsewidth(puertoR, 1509)
			elif ((velW < 0) and (velW >= -3)): # ...mientras gira despacio a izquierda
				self._dit.set_servo_pulsewidth(puertoL, 1540)
				self._dit.set_servo_pulsewidth(puertoR, 1300)
			elif (velW < -3): # ...mientras gira rapido a izquierda
				self._dit.set_servo_pulsewidth(puertoL, 1530)
				self._dit.set_servo_pulsewidth(puertoR, 1300)
	
	def leerIRSigueLineas(self):
		self._GPIO.setmode(self._GPIO.BOARD)
		self._GPIO.setup(12, self._GPIO.IN)
		self._GPIO.setup(14, self._GPIO.IN)
		right_value = self._GPIO.input(12)
		left_value = self._GPIO.input(14)
		#0: ambos sobre la linea
		#1: izquierdo sobre la linea
		#2: derecho sobre la linea
		#3: ambos fuera de la linea
		
		if(right_value == 1 and left_value == 1):
			state = 0
		elif(right_value == 0 and left_value == 1):
			state = 1
		elif(right_value == 1 and left_value == 0):
			state = 2
		else:
			state = 3

		return state

    def dameImagen (self):
        self._frame = self._videostream.read()
        self._frame = imutils.resize(self._frame, width=400)
        return self._frame

    def mostrarImagen (self):
        cv2.imshow("Imagen", self._frame)

	def damePosicionDeObjetoDeColor():
		'''
		Función que devuelve el centro del objeto que tiene un color verde en el rango [GREEN_MIN, GREEN_MAX] para ser detectado
		'''
		# resize the image
		image = dameImagen()

		# convert to the HSV color space
		hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)

		# construct a mask for the color specified
		# then perform a series of dilations and erosions
		# to remove any small blobs left in the mask
		mask = cv2.inRange(hsv, GREEN_MIN, GREEN_MAX)
		mask = cv2.erode(mask, None, iterations=2)
		mask = cv2.dilate(mask, None, iterations=2)

		# find contours in the mask and
		# initialize the current center
		cnts = cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)[-2]
		center = None

		# only proceed if at least one contour was found
		if len(cnts) > 0:
			# find the largest contour in the mask, then use
			# it to compute the minimum enclosing circle and
			# centroid
			c = max(cnts, key=cv2.contourArea)
			((x, y), radius) = cv2.minEnclosingCircle(c)
			M = cv2.moments(c)
			center = (int(M["m10"] / M["m00"]), int(M["m01"] / M["m00"]))

			# only proceed if the radius meets a minimum size
			if radius > 10:
				# draw the circle border
				cv2.circle(image, (int(x), int(y)), int(radius), (0, 255, 0), 2)

				# and the centroid
				cv2.circle(image, center, 5, (0, 255, 255), -1)

		return center

	def dameSonarVisual ():
		'''
		Función que devuelve el array de puntos [X,Y] (Z=0) correspondiente al obstáculo detectado
		'''
		cameraModel = loadPiCamCameraModel ()
		puntosFrontera = 0
		fronteraArray = numpy.zeros((ANCHO_IMAGEN,2), dtype = "float64")

		image = dameImagen ()
		hsvImg = cv2.cvtColor(image,cv2.COLOR_BGR2HSV)
		bnImg = cv2.inRange(hsvImg, GREEN_MIN, GREEN_MAX)

		pixel = Punto2D()
		pixelOnGround3D = Punto3D()
		tmp2d = Punto2D()
		puntosFrontera = 0
		j = 0

		# Ground image: recorremos la imagen de abajo a arriba (i=filas) y de izquierda a derecha (j=columnas)
		while (j < ANCHO_IMAGEN): # recorrido en columnas
			i = LARGO_IMAGEN-1
			esFrontera = None
			while ((i>=0) and (esFrontera == None)): # recorrido en filas
				pos = i*ANCHO_IMAGEN+j # posicion actual

				pix = bnImg[i, j] # value 0 or 255 (frontera)

				if (pix != 0): # si no soy negro
					esFrontera = True # lo damos por frontera en un principio, luego veremos
					# Calculo los demás vecinos, para ver de qué color son...
					c = j - 1
					row = i
					v1 = row*ANCHO_IMAGEN+c

					if (not((c >= 0) and (c < ANCHO_IMAGEN) and 
					(row >= 0) and (row < LARGO_IMAGEN))): # si no es válido ponemos sus valores a 0
						pix = 0
					else:
						pix = bnImg[row, c]

					if (esFrontera == True): # si NO SOY COLOR CAMPO y alguno de los vecinitos ES color campo...
						pixel.x = j
						pixel.y = i
						pixel.h = 1

						# obtenemos su backproject e intersección con plano Z en 3D
						pixelOnGround3D = getIntersectionZ (pixel, cameraModel)

						# vamos guardando estos puntos frontera 3D para luego dibujarlos con PyGame
						fronteraArray[puntosFrontera][0] = pixelOnGround3D.x
						fronteraArray[puntosFrontera][1] = pixelOnGround3D.y
						puntosFrontera = puntosFrontera + 1
						#print "Hay frontera en pixel [",i,",",j,"] que intersecta al suelo en [",pixelOnGround3D.x,",",pixelOnGround3D.y,",",pixelOnGround3D.z,"]"

				i = i - 1
			j = j + 5

		return fronteraArray

	def quienSoy(self):
		print ("Yo soy un robot real PiBot")

	@property
	def tipo(self):
		return self._tipo

	@tipo.setter
	def tipo(self, valor):
		self._tipo = valor
