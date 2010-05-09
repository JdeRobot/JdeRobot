import ctypes
import exceptions
import threading
import jde
import schema
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

#jde_myexport = ljde.myexport
#jde_myexport.argtypes = [ctypes.c_char_p,ctypes.c_char_p,ctypes.c_void_p]
#jde_myimport = ljde.myimport
#jde_myimport.argtypes = [ctypes.c_char_p,ctypes.c_char_p]
#jde_myimport.restype = ctypes.c_void_p


#def myexport(name,varname,data_p):
#	vdata_p = ctypes.cast(data_p,ctypes.c_void_p)
#	return jde_myexport(name,varname,vdata_p)
	
#def myimport(name,varname):
#	return jde_myimport(name,varname)
	

class PyJDESchema(schema.JDESchema):
	def __init__(self,name):
		#WARNING!! Keep references to callbacks to avoid 
		#garbage collector destroys them
		self.init_cb = init_CBT(self.init)
		self.terminate_cb = terminate_CBT(self.terminate)
		self.stop_cb = stop_CBT(self.stop)
		self.run_cb = run_CBT(self.run)
		self.show_cb = show_CBT(self.show)
		self.hide_cb = hide_CBT(self.hide)
		rc = ctypes.c_void_p(jde_new_JDESchema(name,
						       self.init_cb,
						       self.terminate_cb,
						       self.stop_cb,
						       self.run_cb,
						       self.show_cb,
						       self.hide_cb))
		if rc.value == None:
			raise NewJDESchemaException(name),"Can't load schema %s" % name
		#swig object initialization
		this = jde.find_schema(name)
		try: self.this.append(this)
		except: self.this = this

		self.__cond = threading.Condition()
		self.__thread = threading.Thread(target=self.thread)
		self.__thread.start()
		print "hebra creada"

	def thread(self):
		state = schema.slept
		while state >= 0:
			if state == schema.slept:
				print "wait"
				self.wait_statechange()
				state = self.get_state()
				print "nuevo estado:",state

			if state == schema.notready:
				self.set_state(schema.ready)
			elif state == schema.ready:
				self.set_state(schema.winner)

			if state == schema.winner:
				self.speedcounter()
				self.iteration()
			state = self.get_state()
			time.sleep(1)

	def init(self,configfile=''):
		print "init(",self.name,"):",configfile

	def terminate(self):
		print "terminate(",self.name,")"
		self.hide()
		self.stop()
		self.set_state(-1);
		self.__thread.join()

	def run(self,father,brothers,arbitration):
		print "run(",self.name,"):",father,",",brothers
		self.set_state(schema.notready)
		self.run_children()

	def stop(self):
		print "stop(",self.name,")"
		self.set_state(schema.slept)
		self.stop_children()

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

	def get_state(self):
		self.__cond.acquire()
		state = super(PyJDESchema,self).get_state()
		self.__cond.release()
		return state

	def set_state(self,newstate):
		self.__cond.acquire()
		state = super(PyJDESchema,self).get_state()
		if state != newstate:
			super(PyJDESchema,self).set_state(newstate)
			self.__cond.notifyAll()
		self.__cond.release()

	def wait_statechange(self):
		self.__cond.acquire()
		self.__cond.wait()
		self.__cond.release()
