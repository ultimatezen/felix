"""
tests for RemoteRecord class.
"""

import unittest
import datetime
import urllib2

import mock

from FelixMemoryServes import MemoryEngine as engine


class TestFelixRemoteRecord(unittest.TestCase):

    def test_id(self):
        with mock.patch("FelixMemoryServes.MemoryEngine.url_retrieve") as retrieve:
            retrieve.return_value = {"id": 1}

            record = engine.FelixRemoteRecord(data={"id": 3}, commands={"update": None})

            assert record.Id() == 3, record.Id()
            record.SetId(5)
            assert record.Id() == 1, record.Id()

    def test_id_keyerr(self):
        with mock.patch("FelixMemoryServes.MemoryEngine.url_retrieve"):
            record = engine.FelixRemoteRecord(data={}, commands={"update": None})

            assert record.Id() == 0, record.Id()

    def test_source(self):
        with mock.patch("FelixMemoryServes.MemoryEngine.Globals") as FakeGlobals:
            FakeGlobals.USER_NAME = "test_source"
            with mock.patch("FelixMemoryServes.MemoryEngine.url_retrieve") as retrieve:
                retrieve.return_value = {"source": "modified", "modified_by": "me"}

                record = engine.FelixRemoteRecord(data={"source": "source"}, commands={"update": None})

                assert record.Source() == "source", record.Source()
                record.SetSource(None)
                assert record.Source() == "modified", record.Source()
                assert record.data["modified_by"] == "me", record.data
                retrieve.assert_called_once()

    def test_update_no_commands(self):
        """
        If there are no commands, update_self fails.
        """
        record = engine.FelixRemoteRecord({}, {})
        assert record.update_self() is False, record.update_self()

    def test_update_error(self):
        with mock.patch("FelixMemoryServes.MemoryEngine.Globals") as FakeGlobals:
            FakeGlobals.USER_NAME = "test_update_error"
            with mock.patch("FelixMemoryServes.MemoryEngine.url_retrieve") as retreive:

                retreive.side_effect = urllib2.HTTPError(url="http://example.com",
                                                                 code="404",
                                                                 msg="message",
                                                                 hdrs={},
                                                                 fp=mock.Mock())

                record = engine.FelixRemoteRecord(data={"source": "source"}, commands={"update": None})

                assert record.Source() == "source", record.Source()
                record.SetSource("test_update_error")
                assert record.Source() == "test_update_error", record.Source()
                assert record.data["modified_by"] == "test_update_error", record.data
                retreive.Request.assert_called_once()

    def test_trans(self):
        with mock.patch("FelixMemoryServes.MemoryEngine.Globals") as FakeGlobals:
            FakeGlobals.USER_NAME = "test_trans"
            with mock.patch("FelixMemoryServes.MemoryEngine.url_retrieve") as retrieve:
                retrieve.return_value = {"trans": "modified", "modified_by": "me"}

                record = engine.FelixRemoteRecord(data={"trans": "trans"}, commands={"update": None})

                assert record.Trans() == "trans", record.Trans()
                record.SetTrans(None)
                assert record.Trans() == "modified", record.Trans()
                assert record.data["modified_by"] == "me", record.data
                retrieve.assert_called_once()

    def test_context(self):
        with mock.patch("FelixMemoryServes.MemoryEngine.Globals") as FakeGlobals:
            FakeGlobals.USER_NAME = "test_context"
            with mock.patch("FelixMemoryServes.MemoryEngine.url_retrieve") as retrieve:
                retrieve.return_value = {"context": "modified", "modified_by": "me"}

                record = engine.FelixRemoteRecord(data={"context": "context"}, commands={"update": None})

                assert record.Context() == "context", record.Context()
                record.SetContext(None)
                assert record.Context() == "modified", record.Context()
                assert record.data["modified_by"] == "me", record.data
                retrieve.assert_called_once()

    def test_created(self):
        with mock.patch("FelixMemoryServes.MemoryEngine.Globals") as FakeGlobals:
            FakeGlobals.USER_NAME = "test_context"
            with mock.patch("FelixMemoryServes.MemoryEngine.url_retrieve") as retrieve:
                ret_date = datetime.datetime(1999, 1, 1, 10, 11, 12)
                retrieve.return_value = {"date_created": ret_date, "modified_by": "me"}

                data = {"date_created": "2010/1/1 10:11:12"}
                record = engine.FelixRemoteRecord(data=data, commands={"update": None})

                assert record.Created() == u"2010/1/1 10:11:12", record.Created()
                record.SetCreated(None)
                assert record.Created() == u"1999/01/01 10:11:12", record.Created()
                assert record.data["modified_by"] == "me", record.data
                retrieve.assert_called_once()

    def test_modified(self):
        with mock.patch("FelixMemoryServes.MemoryEngine.Globals") as FakeGlobals:
            FakeGlobals.USER_NAME = "test_context"
            with mock.patch("FelixMemoryServes.MemoryEngine.url_retrieve") as retrieve:
                ret_date = datetime.datetime(1999, 1, 1, 10, 11, 12)
                retrieve.return_value = {"last_modified": ret_date, "modified_by": "me"}

                data = {"last_modified": "2010/1/1 10:11:12"}
                record = engine.FelixRemoteRecord(data=data, commands={"update": None})

                assert record.Modified() == u"2010/1/1 10:11:12", record.Modified()
                record.SetModified(None)
                assert record.Modified() == u"1999/01/01 10:11:12", record.Modified()
                assert record.data["modified_by"] == "me", record.data
                retrieve.assert_called_once()

    def test_created_by(self):
        with mock.patch("FelixMemoryServes.MemoryEngine.Globals") as FakeGlobals:
            FakeGlobals.USER_NAME = "test_trans"
            with mock.patch("FelixMemoryServes.MemoryEngine.url_retrieve") as retrieve:
                retrieve.return_value = {"created_by": "modified", "modified_by": "me"}

                record = engine.FelixRemoteRecord(data={"created_by": "xxx"}, commands={"update": None})

                assert record.CreatedBy() == "xxx", record.CreatedBy()
                record.SetCreatedBy(u"username")
                assert record.CreatedBy() == "modified", record.CreatedBy()
                assert record.data["modified_by"] == "me", record.data
                assert record.user_name == u"username", record.user_name
                retrieve.assert_called_once()

    def test_modified_by(self):
        with mock.patch("FelixMemoryServes.MemoryEngine.Globals") as FakeGlobals:
            FakeGlobals.USER_NAME = "test_trans"
            with mock.patch("FelixMemoryServes.MemoryEngine.url_retrieve") as retrieve:
                retrieve.return_value = {"modified_by": "me"}

                record = engine.FelixRemoteRecord(data={"modified_by": "xxx"}, commands={"update": None})

                assert record.ModifiedBy() == "xxx", record.ModifiedBy()
                record.SetModifiedBy(u"username")
                assert record.ModifiedBy() == "me", record.ModifiedBy()
                assert record.data["modified_by"] == "me", record.data
                assert record.user_name == u"username", record.user_name
                retrieve.assert_called_once()

    def test_reliability(self):
        with mock.patch("FelixMemoryServes.MemoryEngine.Globals") as FakeGlobals:
            FakeGlobals.USER_NAME = "test_trans"
            with mock.patch("FelixMemoryServes.MemoryEngine.url_retrieve") as retrieve:
                retrieve.return_value = {"modified_by": "me"}

                data = {"reliability": 2}
                record = engine.FelixRemoteRecord(data=data, commands={"update": None})

                assert record.Reliability() == 2, record.Reliability()
                record.SetReliability("12")
                assert record.Reliability() == 9, record.Reliability()
                assert record.data["modified_by"] == "me", record.data
                retrieve.assert_called_once()

    def testvalidated(self):
        with mock.patch("FelixMemoryServes.MemoryEngine.Globals") as FakeGlobals:
            FakeGlobals.USER_NAME = "test_trans"
            with mock.patch("FelixMemoryServes.MemoryEngine.url_retrieve") as retrieve:
                retrieve.return_value = {"modified_by": "me"}

                data = {"validated": True}
                record = engine.FelixRemoteRecord(data=data, commands={"update": None})

                assert record.Validated() is True, record.Validated()
                record.SetValidated(False)
                assert record.Validated() is False, record.Validated()
                assert record.data["modified_by"] == "me", record.data
                retrieve.assert_called_once()

    def test_ref_count(self):
        with mock.patch("FelixMemoryServes.MemoryEngine.Globals") as FakeGlobals:
            FakeGlobals.USER_NAME = "ref_count"
            with mock.patch("FelixMemoryServes.MemoryEngine.url_retrieve") as retrieve:
                retrieve.return_value = {"modified_by": "me"}

                data = {"ref_count": 2}
                record = engine.FelixRemoteRecord(data=data, commands={"update": None})

                assert record.RefCount() == 2, record.RefCount()
                record.SetRefCount("12")
                assert record.RefCount() == 12, record.RefCount()
                assert record.data["modified_by"] == "me", record.data
                retrieve.assert_called_once()
