import loader
import schema
import random_iface

class sA(schema.Schema):
    def __init__(self,sid,father_sid):
        schema.Schema.__init__(self,sid,father_sid,self.init,self.cast)
        self.seed = 46486422
        self.child = None
        self.child_sr = None
        self.child_interface = None

    def init(self):
        print 'executing init on schema sA(%d)...' % self.sid

        if self.child == None:
            self.child_sr = loader.search_schema_reg(random_iface.IFACE_NAME)
            if self.child_sr == None:
                print 'can\'t find child on schema %d' % self.sid
                return 1

            self.child = self.child_sr.instance(self.sid*10,
                                                self.sid);
            self.child_interface = random_iface.cast_Random(self.child)
            self.child_interface.set_seed(self.seed)

        print '\tget_random() from %s: %f' % (random_iface.IFACE_NAME,
                                              self.child_interface.get_random())
        return 0

    def cast(self,interface_name):
        return None


sr = loader.new_pySchema_reg('sA','null',sA)
loader.add_schema_reg(sr)
