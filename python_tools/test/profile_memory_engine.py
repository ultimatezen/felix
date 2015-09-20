#coding: UTF8
"""
Unit tests of Python memory engine COM server

"""
import unittest
import socket

from FelixMemoryServes import MemoryEngine
from win32com.client import Dispatch as d

HOST = socket.gethostbyname(socket.gethostname())
PORT = 8765

BASE_URL = "http://%s:%s" % (HOST, PORT)


class ProfiletFelixRemoteMemory(unittest.TestCase):
    def setup(self):

        self.mem = MemoryEngine.FelixRemoteMemory()
        self.mem.Create("TestFelixRemoteMemory", "m")

    def teardown(self):
        self.mem.Delete()

    # info
    def profile_get_info_creator(self):
        creator = self.mem.GetInfo("creator")
        assert creator == u"Ryan", creator
    def profile_get_info_source_language(self):
        source_language = self.mem.GetInfo("source_language")
        assert source_language == u"Default", source_language
    def profile_get_info_target_language(self):
        target_language = self.mem.GetInfo("target_language")
        assert target_language == u"Default", target_language
    def profile_get_info_size(self):
        size = self.mem.GetInfo("size")
        assert size == 0, size

    # add / delete

    def profile_add(self):
        assert self.mem.GetInfo("size") == 0

        rec = d("Felix.RemoteRecord")
        rec.Source = u"aaa"
        rec.Trans = u"bbb"
        self.mem.AddRecord(rec)

        assert self.mem.GetInfo("size") == 1

        self.mem.DeleteRecord(rec)

        assert self.mem.GetInfo("size") == 0

    def profile_update(self):
        assert self.mem.GetInfo("size") == 0

        rec = d("Felix.RemoteRecord")
        rec.Source = u"aaa"
        rec.Trans = u"bbb"
        self.mem.AddRecord(rec)

        updated = d("Felix.RemoteRecord")
        updated.Source = u"日本語"
        updated.Trans = u"日本語"
        updated.Id = rec.Id

        result = d(self.mem.UpdateRecord(updated))

        assert self.mem.GetInfo("size") == 1, self.mem.GetInfo("size")
        assert result.Source == updated.Source, result.Source
        assert result.Trans == u"日本語", result.Trans

        self.mem.DeleteRecord(result)

        assert self.mem.GetInfo("size") == 0, self.mem.GetInfo("size")

    # record by id
    def profile_rec_by_id(self):
        size = self.mem.GetInfo("size")
        assert size == 0, size

        rec1 = d("Felix.RemoteRecord")
        rec1.Source = u"aaa"
        rec1.Trans = u"bbb"
        self.mem.AddRecord(rec1)

        print "rec1:", MemoryEngine.record2dict(rec1)

        rec2 = d("Felix.RemoteRecord")
        rec2.Source = u"ccc"
        rec2.Trans = u"ddd"
        self.mem.AddRecord(rec2)

        print "rec2:", MemoryEngine.record2dict(rec2)


        assert self.mem.GetInfo("size") == 2

        try:
            rec = d(self.mem.RecordById(rec1.Id))

            assert rec.Source == rec1.Source, (rec.Source, rec1.Source)
            assert rec.Trans == rec1.Trans, (rec.Trans, rec1.Trans)
            assert rec.Id == rec1.Id, (rec.Id, rec1.Id)
        finally:
            self.mem.DeleteRecord(rec1)
            self.mem.DeleteRecord(rec2)

        assert self.mem.GetInfo("size") == 0
