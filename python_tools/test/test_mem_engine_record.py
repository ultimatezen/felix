#coding: UTF8
"""
Unit tests of Python translation record COM server

"""

import unittest
import socket

import mock
from win32com.client import Dispatch as d

from FelixMemoryServes import MemoryEngine

HOST = socket.gethostbyname(socket.gethostname())
PORT = 8765

BASE_URL = "http://%s:%s" % (HOST, PORT)

class TestMemoryEngineRecord(unittest.TestCase):
    def test_massage_dict(self):
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

class TestFelixRemoteRecord(unittest.TestCase):

    def setUp(self):
        self.rec = d("Felix.RemoteRecord")
        self.rec.Source = u"aaa"
        self.rec.Trans = u"bbb"
        self.rec.Context = u"Context"
        self.rec.ModifiedBy = u"Your mom"
        self.rec.CreatedBy = u"Your granny"

    def test_source(self):
        assert self.rec.Source == u"aaa", self.rec.Source

    def test_set_source(self):
        self.rec.Source = u"test_set_source"
        assert self.rec.Source == u"test_set_source", self.rec.Source

    def test_trans(self):
        assert self.rec.Trans == u"bbb", self.rec.Trans

    def test_set_trans(self):
        self.rec.Trans = u"test_set_trans"
        assert self.rec.Trans == u"test_set_trans", self.rec.Trans

    def test_context(self):
        assert self.rec.Context == u"Context", self.rec.Context

    def test_set_context(self):
        self.rec.Context = u"test_set_context"
        assert self.rec.Context == u"test_set_context", self.rec.Context

    def test_modified_by(self):
        assert self.rec.ModifiedBy == u"Your mom", self.rec.ModifiedBy

    def test_set_modified_by(self):
        self.rec.ModifiedBy = u"test_set_modified_by"
        assert self.rec.ModifiedBy == u"test_set_modified_by", self.rec.ModifiedBy

    def test_created_by(self):
        assert self.rec.CreatedBy == u"Your granny", self.rec.CreatedBy

    def test_set_created_by(self):
        self.rec.CreatedBy = u"test_set_created_by"
        assert self.rec.CreatedBy == u"test_set_created_by", self.rec.CreatedBy

    def test_created_by_maintained(self):
        self.rec.CreatedBy = u"test_created_by_maintained"
        self.rec.ModifiedBy = u"test_set_modified_by"
        assert self.rec.CreatedBy == u"test_created_by_maintained", self.rec.CreatedBy

    def test_modified(self):
        assert self.rec.Modified.startswith("20"), self.rec.Modified

    def test_set_modified(self):
        self.rec.Modified = u"1918/11/22 11:22:33"
        assert self.rec.Modified == u"1918/11/22 11:22:33", self.rec.Modified

    def test_created(self):
        assert self.rec.Created.startswith("20"), self.rec.Created

    def test_set_created(self):
        self.rec.Created = u"1918/11/22 11:22:33"
        assert self.rec.Created == u"1918/11/22 11:22:33", self.rec.Created
