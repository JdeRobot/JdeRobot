import ctypes
import exceptions
import threading
import jde
import time

class NewJDESchemaException(exceptions.Exception):
	def __init__(self,name):
		self.name = name
	def __str__(self):
		return repr(self.name)
	
init_CBT = ctypes.CFUNCTYPE(None, ctypes.c_char_p)
voidvoid_CBT = ctypes.CFUNCTYPE(None)
terminate_CBT = voidvoid_CBT
stop_CBT = voidvoid_CBT
run_CBT = ctypes.CFUNCTYPE(None, ctypes.c_int,
			   ctypes.POINTER(ctypes.c_int),voidvoid_CBT)
show_CBT = voidvoid_CBT
hide_CBT = voidvoid_CBT

ljde = ctypes.CDLL('libjde.so.0')
new_JDESchema = ljde.new_JDESchema
new_JDESchema.argtypes = [ctypes.c_char_p,init_CBT,
			  terminate_CBT,stop_CBT,run_CBT,show_CBT,hide_CBT]
new_JDESchema.restype = ctypes.c_void_p	

#def myimport(interface_name,name,data_p):
	

class JDESchema(object):
	def __init__(self,name):
		self.name = name
		rc = ctypes.c_void_p(new_JDESchema(name,init_CBT(self.init),
										   terminate_CBT(self.terminate),
										   stop_CBT(self.stop),run_CBT(self.run),
										   show_CBT(self.show),hide_CBT(self.hide)))
		if rc.value == None:
			raise NewJDESchemaException(name),"Can't load schema %s" % name
		self.__schema = jde.find_schema(self.name)
		self.__mutex = threading.Lock()
		self.__cond = threading.Condition(self.__mutex)
		self.__thread = threading.Thread(target=self.thread)

	def thread(self):
		while 1:
			self.__cond.acquire()
			if jde.get_state(self.__schema) == jde.slept:
				self.__cond.wait()
			elif jde.get_state(self.__schema) == jde.notready:
				jde.set_state(self.__schema,jde.ready)
			elif jde.get_state(self.__schema) == jde.ready:
				jde.set_state(self.__schema,jde.winner)
			    #run hijos

			if jde.get_state(self.__schema) == jde.winner:    
				self.__cond.release()
				jde.speedcounter2(self.__schema)
				self.iteration()
			else:
				self.__cond.release()
			time.sleep(1)

	def init(self,configfile=''):
		self.__mutex.acquire()
		print "init(",self.name,"):",configfile
		jde.set_state(self.__schema,jde.slept)
		self.__thread.start()
		self.__mutex.release()

	def terminate(self):
		print "terminate(",self.name,")"
		self.hide()
		self.stop()

	def run(self,father,brothers,arbitration):
		self.__cond.acquire()
		print "run(",self.name,"):",father,",",brothers
		jde.set_state(self.__schema,jde.notready)
	    #simbolos de hijos
		self.__cond.notify()
		self.__cond.release()

	def stop(self):
		self.__mutex.acquire()
		print "stop(",self.name,")"
		jde.set_state(self.__schema,jde.slept)
                #stop hijos
		self.__mutex.release()

	def show(self):
		print "show(",self.name,")"

	def hide(self):
		print "hide(",self.name,")"

	def iteration(self):
		print "iteration(",self.name,")"
