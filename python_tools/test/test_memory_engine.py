"""
Tests for MemoryEngine::RemoteMemory
"""
import unittest
import cPickle
from cStringIO import StringIO

import mock

from FelixMemoryServes import MemoryEngine


class WebMocker(unittest.TestCase):

    def set_request_val(self, val):
        self.req_val = cPickle.dumps(val)

    def get_request(self, x):
        return StringIO(self.req_val)

    def setUp(self):
        # mock COM stuff
        self.wrap = MemoryEngine.util.wrap
        self.unwrap = MemoryEngine.util.unwrap
        self.coll = MemoryEngine.util.NewCollection
        MemoryEngine.util.wrap = lambda x : x
        MemoryEngine.util.unwrap = lambda x : x
        MemoryEngine.util.NewCollection = lambda x : x

        # mock urllib stuff
        self.urlencode = MemoryEngine.urllib.urlencode
        self.Request = MemoryEngine.urllib2.Request
        self.urlopen = MemoryEngine.urllib2.urlopen

        MemoryEngine.urllib2.Request = lambda x, y : (x, y)
        self.req_val = cPickle.dumps(None)
        MemoryEngine.urllib2.urlopen = self.get_request
        MemoryEngine.urllib.urlencode = lambda x : x

        self.engine = MemoryEngine.FelixRemoteMemory()

    def tearDown(self):
        MemoryEngine.util.unwrap = self.unwrap
        MemoryEngine.util.wrap = self.wrap
        MemoryEngine.util.NewCollection = self.coll

        MemoryEngine.urllib2.Request = self.Request
        MemoryEngine.urllib2.urlopen = self.urlopen
        MemoryEngine.urllib.urlencode = self.urlencode


class TestRecordById(WebMocker):
    def test_no_url(self):
        self.engine.commands = {}
        result = self.engine.RecordById(3)
        assert result is None, result

    def test_None_result(self):
        self.engine.commands = dict(rec_by_id="foo")
        result = self.engine.RecordById(3)
        assert result is None, result

    def test_a_b(self):
        self.engine.commands = dict(rec_by_id="foo")
        self.set_request_val(dict(source="a", trans="b"))
        result = self.engine.RecordById(3)
        assert result.Source() == "a", result.Source()
        assert result.Trans() == "b", result.Trans()


class TestRecToRaw(WebMocker):

    def test_a_b(self):

        record = mock.Mock()
        record.data = dict(source="raw a",
                           trans="raw b",
                           context="context",
                           created_by="Ryan",
                           modified_by="Sam")
        record.commands = {}
        data = self.engine.rec_to_raw(record)
        assert data == record.data, data

    def test_a_b_record2dict(self):
        class Foo: pass
        record = Foo()
        record.data = dict(source="raw a",
                           trans="raw b",
                           context="context")
        record.Source = u"record2d source"
        record.Trans = u"record2d trans"
        record.Context = u"record2d context"
        record.Reliability = 5
        record.Validated = 4
        record.RefCount= 3
        record.Created = 2
        record.Modified = 1
        record.CreatedBy = u"Created by"
        record.ModifiedBy = u"Modified by"
        record.Id = 3
        record.commands = {}
        data = self.engine.rec_to_raw(record)
        assert data["source"] == "record2d source", data
        assert data["trans"] == "record2d trans", data
        assert data["context"] == "record2d context", data


class TestPrepareHits(WebMocker):

    def test_empty(self):
        hits = self.engine.prepare_hits([])
        assert hits == [], hits

    def test_two(self):
        rec1 = dict(source="sa", trans="ta")
        rec2 = dict(source="sb", trans="tb")

        h1, h2 = self.engine.prepare_hits([rec1, rec2])
        assert h1.Source() == "sa", h1.Source()
        assert h2.Trans() == "tb", h2.Trans()


class TestSetBase(unittest.TestCase):
    def setUp(self):
        self.engine = MemoryEngine.FelixRemoteMemory()

    def test_connection(self):
        connection = "http://ginstrom.com:8000/api/mems/1"
        expected = "http://ginstrom.com:8000"

        base = self.engine.set_base(connection)
        assert base == expected, base


class TestGetLoginUrl(unittest.TestCase):
    def setUp(self):
        self.engine = MemoryEngine.FelixRemoteMemory()

    def test_commands(self):
        expected = "http://ginstrom.com:8000/api/login"
        self.engine.commands = dict(login=expected)

        login = self.engine.get_login_url()
        assert login == expected, login

    def test_no_command(self):
        self.engine.base = "http://felix-cat.com:8000"
        expected = "http://felix-cat.com:8000/api/login/"

        login = self.engine.get_login_url()
        assert login == expected, login
