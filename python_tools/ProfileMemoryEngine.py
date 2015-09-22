#coding: UTF8
"""
Unit tests of COM servers

"""
import utils
utils.TESTING = True

from win32com.client import Dispatch as d
from win32com.server.exception import COMException
from nose import tools
import urllib2
import urllib
import cPickle
import datetime
import socket
import time
from pywintypes import com_error
from FelixMemoryServes import MemoryEngine

HOST = socket.gethostbyname(socket.gethostname())
PORT = 8765

BASE_URL = "http://%s:%s" % (HOST, PORT)

def test_within_refresh_interval():
    assert MemoryEngine.within_refresh_interval(10, 5)
    assert not MemoryEngine.within_refresh_interval(25, 5)

def test_get_now():
    now = MemoryEngine.get_now()
    assert "2010" in str(now), now

def test_create_remote_record():
    o = d("Felix.RemoteRecord")
    assert o, o

def test_create_remote_memory():
    o = d("Felix.RemoteMemory")
    assert o, o

def test_record2dict():
    rec = d("Felix.RemoteRecord")
    rec.Source = u"spam"
    rec.Trans = u"egg"

    recd = MemoryEngine.record2dict(rec)

    assert recd["source"] == u"spam", recd
    assert recd["trans"] == u"egg", recd
    assert recd["context"] == u"", recd
    assert not recd["validated"], recd
    assert recd["reliability"] == 0, recd

class TestRecordClass(object):
    def setup(self):
        self.rec = MemoryEngine.FelixRemoteRecord()

    def test_source(self):
        self.rec.SetSource(u"Spam")
        assert self.rec.Source() == u"Spam", self.rec.Source()

class TestRemoteRecord:
    def setup(self):
        self.rec = d("Felix.RemoteRecord")

    def test_source(self):
        self.rec.Source = u"Spam"
        assert self.rec.Source == u"Spam", self.rec.Source

    def test_trans(self):
        self.rec.Trans = u"Spam"
        assert self.rec.Trans == u"Spam", self.rec.Trans

    def test_context(self):
        self.rec.Context = u"Spam"
        assert self.rec.Context == u"Spam", self.rec.Context

    def test_ref_count(self):
        assert self.rec.RefCount == 0, self.rec.RefCount
        self.rec.RefCount = 5
        assert self.rec.RefCount == 5, self.rec.RefCount

    def test_reliability(self):
        assert self.rec.Reliability == 0, self.rec.Reliability
        self.rec.Reliability = 5
        assert self.rec.Reliability == 5, self.rec.Reliability

    def test_reliability_max(self):
        assert self.rec.Reliability == 0, self.rec.Reliability
        self.rec.Reliability = 10
        assert self.rec.Reliability == 9, self.rec.Reliability

    def test_created(self):
        this_year = unicode(time.localtime()[0])
        assert self.rec.Created.startswith(this_year), self.rec.Created

    def test_set_created(self):
        self.rec.Created = u"2008/01/01 12:34:56"
        assert self.rec.Created == u"2008/01/01 12:34:56", self.rec.Created

    def test_set_created_hyphens(self):
        self.rec.Created = u"2008-01-01 12:34:56"
        assert self.rec.Created == u"2008/01/01 12:34:56", self.rec.Created

    def test_modified(self):
        this_year = unicode(time.localtime()[0])
        assert self.rec.Modified.startswith(this_year), self.rec.Modified

    def test_set_modified(self):
        self.rec.Modified = u"2008/01/01 12:34:56"
        assert self.rec.Modified == u"2008/01/01 12:34:56", self.rec.Modified

    def test_set_modified_hyphens(self):
        self.rec.Modified = u"2008-01-01 12:34:56"
        assert self.rec.Modified == u"2008/01/01 12:34:56", self.rec.Modified

    def test_created_by(self):
        self.rec.CreatedBy = u"Ryan"
        assert self.rec.CreatedBy == u"Ryan", self.rec.CreatedBy

    def test_modified_by(self):
        self.rec.ModifiedBy = u"Knights that say 'ni'"
        assert self.rec.ModifiedBy == u"Knights that say 'ni'", self.rec.ModifiedBy

    def test_id(self):
        assert self.rec.Id == 0, self.rec.Id
        self.rec.Id = 5
        assert self.rec.Id == 5, self.rec.Id

class TestRemoteMemoryUnconnected:
    def setup(self):
        self.mem = d("Felix.RemoteMemory")

    def test_add(self):
        rec = d("Felix.RemoteRecord")
        rec.Source = u"spam"
        rec.Trans = u"egg"
        assert not self.mem.AddRecord(rec)
