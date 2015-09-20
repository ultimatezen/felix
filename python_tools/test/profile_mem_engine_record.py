#coding: UTF8
"""
Unit tests of Python translation record COM server

"""

import unittest
import socket

from FelixMemoryServes import MemoryEngine
from win32com.client import Dispatch as d

HOST = socket.gethostbyname(socket.gethostname())
PORT = 8765

BASE_URL = "http://%s:%s" % (HOST, PORT)


def profile_massage_dict():
    values = dict(source=u"日本語",
                  trans=u"egg",
                  context=u"context",
                  created_by=u"ryan",
                  modified_by=u"parrot",
                  reliability=3)
    out = MemoryEngine.massage_dict(values)

    assert out["source"] == "日本語", out
    assert out["source"].decode("utf-8") == u"日本語", out
    assert out["trans"] == "egg", out
    assert out["reliability"] == 3, out

class ProfileFelixRemoteRecord(unittest.TestCase):
    def setup(self):
        self.mem = MemoryEngine.FelixRemoteMemory()
        self.mem.Create("foo", "m")
        self.rec = d("Felix.RemoteRecord")
        self.rec.Source = u"aaa"
        self.rec.Trans = u"bbb"
        self.rec.Context = u"Context"
        self.rec.ModifiedBy = u"Your mom"
        self.rec.CreatedBy = u"Your granny"
        self.mem.AddRecord(self.rec)

    def teardown(self):
        self.mem.DeleteRecord(self.rec)
        self.mem.Delete()

    def profile_source(self):
        assert self.rec.Source == u"aaa", self.rec.Source

    def profile_set_source(self):
        old = d(self.mem.RecordById(self.rec.Id))
        old.Source = u"TestFelixRemoteRecord.profile_set_source"
        modified = d(self.mem.RecordById(self.rec.Id))
        assert modified.Source == u"TestFelixRemoteRecord.profile_set_source", modified.Source

    def profile_trans(self):
        assert self.rec.Trans == u"bbb", self.rec.Trans

    def profile_set_trans(self):
        old = d(self.mem.RecordById(self.rec.Id))
        old.Trans = u"TestFelixRemoteRecord.profile_set_trans"
        modified = d(self.mem.RecordById(self.rec.Id))
        assert modified.Trans == u"TestFelixRemoteRecord.profile_set_trans", modified.Trans

    def profile_context(self):
        assert self.rec.Context == u"Context", self.rec.Context

    def profile_set_context(self):
        old = d(self.mem.RecordById(self.rec.Id))
        old.Context = u"TestFelixRemoteRecord.profile_set_context"
        modified = d(self.mem.RecordById(self.rec.Id))
        assert modified.Context == u"TestFelixRemoteRecord.profile_set_context", modified.Context

    def profile_modified_by(self):
        assert self.rec.ModifiedBy == u"Your mom", self.rec.ModifiedBy

    def profile_set_modified_by(self):
        old = d(self.mem.RecordById(self.rec.Id))
        old.ModifiedBy = u"TestFelixRemoteRecord.profile_set_modified_by"
        modified = d(self.mem.RecordById(self.rec.Id))
        assert modified.ModifiedBy == u"TestFelixRemoteRecord.profile_set_modified_by", modified.ModifiedBy

    def profile_created_by(self):
        assert self.rec.ModifiedBy == u"Your mom", self.rec.ModifiedBy

    def profile_set_created_by(self):
        old = d(self.mem.RecordById(self.rec.Id))
        old.CreatedBy = u"TestFelixRemoteRecord.profile_set_created_by"
        modified = d(self.mem.RecordById(self.rec.Id))
        assert modified.CreatedBy == u"TestFelixRemoteRecord.profile_set_created_by", modified.CreatedBy

    def profile_created_by_maintained(self):
        old = d(self.mem.RecordById(self.rec.Id))
        old.CreatedBy = u"TestFelixRemoteRecord.profile_created_by_maintained"
        old.Source = u"Some new source that we don't care about"
        modified = d(self.mem.RecordById(self.rec.Id))
        assert modified.CreatedBy == u"TestFelixRemoteRecord.profile_created_by_maintained", modified.CreatedBy

    def profile_modified(self):
        assert self.rec.Modified.startswith("20"), self.rec.Modified

    def profile_set_modified(self):
        old = d(self.mem.RecordById(self.rec.Id))
        old.Modified = u"1918/11/22 11:22:33"
        modified = d(self.mem.RecordById(self.rec.Id))
        assert modified.Modified == u"1918/11/22 11:22:33", modified.Modified

    def profile_created(self):
        assert self.rec.Created.startswith("20"), self.rec.Created

    def profile_set_created(self):
        old = d(self.mem.RecordById(self.rec.Id))
        old.Created = u"1999/11/22 11:22:33"
        modified = d(self.mem.RecordById(self.rec.Id))
        assert modified.Created == u"1999/11/22 11:22:33", modified.Created
