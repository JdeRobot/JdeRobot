import loader
import schema
import random_iface

class sA(schema.Schema):
    def __init__(self,sid,father_sid):
        super(sA,self).__init__(sid,father_sid,self.init,self.get_interface)
        self.seed = 46486422
        self.child = None
        self.child_sr = None
        self.child_interface = None

    def init(self):
        print 'executing init on schema sA(%d)...' % self.s.sid

        if self.child == None:
            self.child_sr = loader.search_schema_reg(random_iface.IFACE_NAME)
            if self.child_sr == None:
                print 'can\'t find child on schema %d' % self.s.sid
                return 1

            self.child = self.child_sr.instance(self.s.sid*10,
                                                self.s.sid);
            self.child_interface = self.child.get_interface(random_iface.IFACE_NAME);

    def get_interface(self,interface_name):
        return None


sr = loader.new_pySchema_reg('sA','null',sA)
loader.add_schema_reg(sr)
