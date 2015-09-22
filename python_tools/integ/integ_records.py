"""
Adding, modifying, and editing records via Memory Serves API
"""
import datetime
import socket
from win32com.client import Dispatch

from FelixMemoryServes import MemoryEngine as engine

HOST_NAME = socket.gethostname()
HOST = socket.gethostbyname(HOST_NAME)
CONNECTION_STRING = 'http://%s:8765/api/mems/1/'


class IntegRecord(object):
    def setup(self):
        self.memory = Dispatch('Felix.RemoteMemory')
        self.record = Dispatch('Felix.RemoteRecord')

    def teardown(self):
        if self.record.Id:
            self.memory.DeleteRecord(self.record)

    def integ_add_record(self):
        value = unicode(datetime.datetime.now())

        self.record.Source = value
        self.record.Trans = value

        self.memory.AddRecord(self.record)


