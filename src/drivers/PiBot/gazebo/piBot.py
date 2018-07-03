# -*- coding: utf-8 -*-
from JdeRobotKids import JdeRobotKids
import numpy
import threading
import sys
import comm
import config
import cv2

class PiBot:

    '''
    Controlador para el Robot PiBot de JdeRobot-Kids
    '''
    def __init__(self, cfg):
	 
        print("En constructor")
        cfg = config.load(cfg)
        
        #starting comm
        jdrc= comm.init(cfg, 'JdeRobotKids.Sim')
        self.camera = jdrc.getCameraClient("JdeRobotKids.Sim.Camera")
        self.motors = jdrc.getMotorsClient("JdeRobotKids.Sim.Motors")    
        self.irLeft = jdrc.getIRClient("JdeRobotKids.Sim.IRLeft")    
        self.irRight = jdrc.getIRClient("JdeRobotKids.Sim.IRRight") 
        self.us = jdrc.getSonarClient("JdeRobotKids.Sim.Sonar")    
        

    def moverServo(self, *args):
        '''
        Función que hace girar al servo motor a un angulo dado como parámetro.
        @type args: lista
        @param args: lista de argumentos:
        args[0]: puerto al que esta conectado el controlador del servo
        args[1]: banco al que esta conectado el servo en el controlador
        args[2]: angulo de giro del servo. 0-180 grados. ¡PROBAR GIRO ANTES DE MONTAR EL SERVO!
        '''
        None

    def avanzar(self, vel):
        '''
        Función que hace avanzar al robot en línea recta a una velocidad dada como parámetro.
        @type vel: entero
        @param vel: velocidad de avance del robot (máximo 255)
        '''
        self.motors.sendW(0)
        self.motors.sendV(vel)

    def retroceder(self, vel):
        '''
        Función que hace retroceder al robot en línea recta a una velocidad dada como parámetro.
        @type vel: entero
        @param vel: velocidad de retroceso del robot (máximo 255)
        '''
        self.motors.sendW(0)
        self.motors.sendV(-vel)

    def parar(self):
        '''
        Función que hace detenerse al robot.
        '''
        self.motors.sendV(0)
        self.motors.sendW(0)

    def girarIzquierda(self, vel):
        '''
        Función que hace rotar al robot sobre sí mismo hacia la izquierda a una velocidad dada como parámetro.
        @type vel: entero
        @param vel: velocidad de giro del robot (máximo 255)
        '''
        self.motors.sendV(0)
        self.motors.sendW(vel)
        

    def girarDerecha(self, vel):
        '''
        Función que hace rotar al robot sobre sí mismo hacia la derecha a una velocidad dada como parámetro.
        @type vel: entero
        @param vel: velocidad de giro del robot (máximo 255)
        '''
        self.motors.sendV(0)
        self.motors.sendW(-vel)

    def move(self, velV, velW):
        '''
        Función que hace avanzar y girar al robot al mismo tiempo, según las velocidades V,W dadas como parámetro.
        @type velV, velW: entero
        @param velV, velW: velocidades de avance de motores izquierdo y derecho
        '''
        self.motors.sendV(velV)
        self.motors.sendW(velW)

    def dameImagen(self):
        '''
        Función que muestra la imagen percibida por la camara
        '''
        img = self.camera.getImage().data
        cv2.imshow("img", img)
        cv2.waitKey(0)

    def leerIRSigueLineas(self):
        '''
        Función que retorna las lecturas del sensor siguelineas de la siguiente forma:
            0: ambos sensores sobre la linea
            1: solo sensor izquierdo sobre la linea
            2: solo sensor derecho sobre la linea 
            3: ambos sensores fuera de la linea
        '''
        lft = self.irLeft.getIRData().received
        rgt = self.irRight.getIRData().received
        value = -1
        if lft == 1 and rgt == 1:
            value = 0
        elif lft == 1 and rgt == 0:
            value = 1
        elif lft == 0 and rgt == 1:
            value = 2
        elif lft == 0 and rgt == 0:
            value = 3

        return value

    def leerUltrasonido(self):
        '''
        Función que retorna las lecturas del sensor ultrasonidos
        '''
        value = self.us.getSonarData().range
        return value

    def quienSoy(self):
        print ("Yo soy un robot simulado PiBot")

    @property
    def tipo(self):
        return self._tipo

    @tipo.setter
    def tipo(self, valor):
        self._tipo = valor
