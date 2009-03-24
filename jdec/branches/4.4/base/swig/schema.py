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
jde_new_JDESchema = ljde.new_JDESchema
jde_new_JDESchema.argtypes = [ctypes.c_char_p,init_CBT,
			      terminate_CBT,stop_CBT,run_CBT,show_CBT,hide_CBT]
jde_new_JDESchema.restype = ctypes.c_void_p	

jde_myexport = ljde.myexport
jde_myexport.argtypes = [ctypes.c_char_p,ctypes.c_char_p,ctypes.c_void_p]
jde_myimport = ljde.myimport
jde_myimport.argtypes = [ctypes.c_char_p,ctypes.c_char_p]
jde_myimport.restype = ctypes.c_void_p


def myexport(name,varname,data_p):
	vdata_p = ctypes.cast(data_p,ctypes.c_void_p)
	return jde_myexport(name,varname,vdata_p)
	
def myimport(name,varname):
	return jde_myimport(name,varname)
	

class JDESchema(object):
	def __init__(self,name):
		self.name = name
		rc = ctypes.c_void_p(jde_new_JDESchema(name,init_CBT(self.init),
						       terminate_CBT(self.terminate),
						       stop_CBT(self.stop),run_CBT(self.run),
						       show_CBT(self.show),hide_CBT(self.hide)))
		if rc.value == None:
			raise NewJDESchemaException(name),"Can't load schema %s" % name
		self.__schema = jde.find_schema(self.name)
		self.mutex = threading.Lock()
		self.__cond = threading.Condition(self.mutex)
		self.__thread = threading.Thread(target=self.thread)
		self.__finish = False

	def thread(self):
		while not self.__finish:
			self.__cond.acquire()
			if jde.get_state(self.__schema) == jde.slept:
				self.__cond.wait()
			elif jde.get_state(self.__schema) == jde.notready:
				jde.set_state(self.__schema,jde.ready)
			elif jde.get_state(self.__schema) == jde.ready:
				jde.set_state(self.__schema,jde.winner)
			mystate = jde.get_state(self.__schema)
			self.__cond.release()

			if mystate == jde.winner:    
				jde.speedcounter2(self.__schema)
				self.iteration()
			time.sleep(1)

	def init(self,configfile=''):
		self.mutex.acquire()
		print "init(",self.name,"):",configfile
		jde.set_state(self.__schema,jde.slept)
		self.__thread.start()
		self.mutex.release()

	def terminate(self):
		print "terminate(",self.name,")"
		self.hide()
		self.__finish = True
		self.stop()

	def run(self,father,brothers,arbitration):
		self.__cond.acquire()
		print "run(",self.name,"):",father,",",brothers
		jde.set_state(self.__schema,jde.notready)
		self.run_children()
		self.__cond.notify()
		self.__cond.release()

	def stop(self):
		self.mutex.acquire()
		print "stop(",self.name,")"
		jde.set_state(self.__schema,jde.slept)
		self.stop_children()
		self.mutex.release()

	def show(self):
		print "show(",self.name,")"

	def hide(self):
		print "hide(",self.name,")"

	def iteration(self):
		print "iteration(",self.name,")"

	def run_children(self):
		print "running children..."

	def stop_children(self):
		print "stoping children..."
