#(c) JdeRobot-kids 2018

# -*- coding: utf-8 -*-
import abc
from abc import ABCMeta

class JdeRobotKids(object):
    __metaclass__ = ABCMeta

    # METODOS DE VISIONLIB    
    @abc.abstractmethod
    def damePosicionDeObjetoDeColor(image, color):
        print ("damePosicionDeObjetoDeColor abstracto")

    # METODOS COMUNES A LAS TRES PLATAFORMAS    
    @abc.abstractmethod
    def quienSoy(self):
        print ("Soy un tipo de robot abstracto")

    @abc.abstractmethod
    def avanzar(self, vel):
        print ("Avanzar abstracto")

    @abc.abstractmethod
    def retroceder(self, vel):
        print ("Retroceder abstracto")

    @abc.abstractmethod
    def parar(self):
        print ("Parar abstracto")

    @abc.abstractmethod
    def girarIzquierda(self, vel):
        print ("Girar izquierda abstracto")

    @abc.abstractmethod
    def girarDerecha(self, vel):
        print ("Girar derecha abstracto")

    @abc.abstractmethod
    def move(self, velV, velW):
        print ("Move abstracto")

    # METODOS COMUNES A PIBOT Y MBOTREAL
    @abc.abstractmethod
    def leerIntensidadLuz(self):
        print ("leerIntensidadLuz abstracto")

    @abc.abstractmethod
    def leerUltrasonido(self):
        print ("leerUltrasonido abstracto")

    @abc.abstractmethod
    def leerIRSigueLineas(self):
        print ("leerIRSigueLineas abstracto")

    @abc.abstractmethod
    def leerIntensidadSonido(self):
        print ("leerIntensidadSonido abstracto")

    @abc.abstractmethod
    def leerPotenciometro(self):
        print ("leerPotenciometro abstracto")

    @abc.abstractmethod
    def moverServo(self, *args):
        print ("moverServo abstracto")

    def get_tipo(self):
        pass
    
    def set_tipo(self, valor):
        pass

    tipo = abc.abstractproperty(get_tipo, set_tipo)

    # METODOS PROPIOS DEL MBOTREAL    
    @abc.abstractmethod
    def encenderLedPlaca(self, indice, r, g, b):
        print ("encenderLedPlaca abstracto")

    @abc.abstractmethod
    def apagarLedPlaca(self, indice):
        print ("apagarLedPlaca abstracto")

    @abc.abstractmethod
    def encenderLed(self, puerto, indice, r, g, b):
        print ("encenderLed abstracto")

    @abc.abstractmethod
    def apagarLed(self, puerto, indice):
        print ("apagarLed abstracto")

    @abc.abstractmethod
    def escribirTexto(self, puerto, texto, dx, dy, brillo):
        print ("escribirTexto abstracto")

    @abc.abstractmethod
    def escribirFrase(self, puerto, texto, brillo = 3):
        print ("escribirFrase abstracto")

    @abc.abstractmethod
    def dibujosPosibles(self):
        print ("dibujosPosibles abstracto")

    @abc.abstractmethod
    def pintarDibujo(self, puerto, nombreDibujo, brillo):
        print ("pintarDibujo abstracto")

    @abc.abstractmethod
    def dibujar (self, puerto, dibujo, posicionX, posicionY, brillo, ancho):
        print ("dibujar abstracto")

    @abc.abstractmethod
    def escribirReloj(self, puerto, hora, minuto, brillo):
        print ("escribirReloj abstracto")

    @abc.abstractmethod
    def borrarMatriz(self, puerto):
        print ("borrarMatriz abstracto")

    @abc.abstractmethod
    def leerBoton(self):
        print ("leerBoton abstracto")

    @abc.abstractmethod
    def playTono(self, tono, tiempo):
        print ("playTono abstracto")

    @abc.abstractmethod
    def valoresMandoPosibles(self):
        print ("valoresMandoPosibles abstracto")

    @abc.abstractmethod
    def leerMandoIR(self):
        print ("leerMandoIR abstracto")

    @abc.abstractmethod
    def dameImagen(self):
        print ("dameImagen abstracto")

    @abc.abstractmethod
    def mostrarImagen(self):
        print ("mostrarImagen abstracto")

    @abc.abstractmethod
    def dameSonarVisual (self, image, cameraModel):
        print ("dameSonarVisual abstracto")

    @abc.abstractmethod
    def damePosicionDeObjetoDeColor(self, image, color):
        print ("damePosicionDeObjetoDeColor abstracto")

