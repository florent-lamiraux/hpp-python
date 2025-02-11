from test_python_cxx import *

class Empty:

    def __init__(self, obj = None):
        if obj:
            self.obj = obj
        else:
            self.obj = create_empty()

class Store:
    def __init__(self):
        self.obj = create_store()

    def setValue(self, empty):
        store_setValue(self.obj, empty.obj)

    def getValue(self):
        return Empty(store_getValue(self.obj))
