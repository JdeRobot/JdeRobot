import loader
import schema
import random
import random_iface

class sD(schema.Schema,random_iface.Random):
    def __init__(self,sid,father_sid):
        self.this = []
        schema.Schema.__init__(self,sid,father_sid,self.init,self.cast)
        random_iface.Random.__init__(self,self.get_random,self.set_seed)

    def init(self):
        print 'executing init on schema sD(%d)...' % self.s.sid

    def cast(self,interface_name):
        return self

    def get_random(self):
	print 'call get_random() inside sD(%d)' % self.sid
        return random.random()

    def set_seed(self,seed):
	print 'call set_seed(%d) inside sD(%d)' % (seed,self.sid)
	random.seed(seed)

sr = loader.new_pySchema_reg('sD',random_iface.IFACE_NAME,sD)
loader.add_schema_reg(sr)
