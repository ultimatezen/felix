"""
Unit tests for remote memories
"""
import unittest

import mock

from FelixMemoryServes import MemoryEngine as engine


class TestRemoteMemory(unittest.TestCase):

    def test_init(self):
        """
        On construction, a FelixRemoteMemory instance has the expected values
        """
        mem = engine.FelixRemoteMemory()

        assert mem.connection == u"", mem.connection
        assert mem.data == {}, mem.data
        assert mem.token is None, mem.token

    def test_username(self):
        """
        The user name starts with the default global, and can be customized.
        """

        mem = engine.FelixRemoteMemory()

        assert mem.UserName() == engine.Globals.USER_NAME, (mem.UserName(), engine.Globals.USER_NAME)

        mem.SetUserName(u"test_username")
        assert mem.UserName() == u"test_username", mem.UserName()

    def test_record_by_id(self):
        """
        We can retrieve a record by its record ID
        """

        mem = engine.FelixRemoteMemory()
        mem.commands = {"rec_by_id": "foo"}

        with mock.patch("FelixMemoryServes.MemoryEngine.url_retrieve") as retrieve:
            with mock.patch("FelixMemoryServes.MemoryEngine.util") as util:
                retrieve.return_value = {"source": "record source", "trans": "record trans"}
                util.wrap.return_value = {"source": "record source", "trans": "record trans"}

                record = mem.RecordById(3)
                assert record == {"source": "record source", "trans": "record trans"}, record

    def test_record_by_id_no_command(self):
        """
        When there is no command data, returns None
        """

        mem = engine.FelixRemoteMemory()
        mem.commands = {}

        record = mem.RecordById(3)
        assert record is None, record

    def test_record_by_id_no_raw(self):
        """
        When there is no command data, returns None
        """

        mem = engine.FelixRemoteMemory()
        mem.commands = {"rec_by_id": "foo"}

        with mock.patch("FelixMemoryServes.MemoryEngine.url_retrieve") as retrieve:
            retrieve.return_value = {}

            record = mem.RecordById(3)
            assert record is None, record

    def test_update_record(self):
        """
        We can update a record
        """

        mem = engine.FelixRemoteMemory()
        mem.commands = {"update": "foo"}

        with mock.patch("FelixMemoryServes.MemoryEngine.url_retrieve") as retrieve:
            with mock.patch("FelixMemoryServes.MemoryEngine.Dispatch") as dispatch:
                with mock.patch("FelixMemoryServes.MemoryEngine.util") as util:
                    data = {"source": "record source", "trans": "record trans", "id": 3}
                    retrieve.return_value = data
                    dispatch.return_value = mock.Mock()
                    util.wrap.return_value = data

                    record = mem.UpdateRecord(mock.Mock())
                    assert record == data, record

    def test_update_record_no_command(self):
        """
        returns False if we don't have an update command
        """

        mem = engine.FelixRemoteMemory()
        mem.commands = {}

        with mock.patch("FelixMemoryServes.MemoryEngine.Dispatch") as dispatch:
            dispatch.return_value = mock.Mock()
            assert mem.UpdateRecord(mock.Mock()) is False, mem.UpdateRecord(mock.Mock())

    def test_update_record_no_updated_data(self):
        """
        return False if there is no updated data
        """

        mem = engine.FelixRemoteMemory()
        mem.commands = {"update": "foo"}

        with mock.patch("FelixMemoryServes.MemoryEngine.url_retrieve") as retrieve:
            with mock.patch("FelixMemoryServes.MemoryEngine.Dispatch") as dispatch:
                retrieve.return_value = None
                dispatch.return_value = mock.Mock()

                assert mem.UpdateRecord(mock.Mock()) is False, mem.UpdateRecord(mock.Mock())

    def test_add_record(self):
        """
        We can add a record
        """

        mem = engine.FelixRemoteMemory()
        mem.commands = {"add": "foo"}

        with mock.patch("FelixMemoryServes.MemoryEngine.url_retrieve") as retrieve:
            with mock.patch("FelixMemoryServes.MemoryEngine.Dispatch") as dispatch:
                with mock.patch("FelixMemoryServes.MemoryEngine.util") as util:
                    data = {"source": "record source", "trans": "record trans", "id": 3}
                    retrieve.return_value = data
                    dispatch.return_value = mock.Mock()
                    util.wrap.return_value = data

                    mem.AddRecord(mock.Mock()) is True, mem.AddRecord(mock.Mock())

    def test_add_record_no_command(self):
        """
        returns False if we don't have an add command
        """

        mem = engine.FelixRemoteMemory()
        mem.commands = {}

        with mock.patch("FelixMemoryServes.MemoryEngine.Dispatch") as dispatch:
            dispatch.return_value = mock.Mock()
            assert mem.AddRecord(mock.Mock()) is False, mem.AddRecord(mock.Mock())

    def test_add_record_no_updated_data(self):
        """
        return False if there is no added data
        """

        mem = engine.FelixRemoteMemory()
        mem.commands = {"add": "foo"}

        with mock.patch("FelixMemoryServes.MemoryEngine.url_retrieve") as retrieve:
            with mock.patch("FelixMemoryServes.MemoryEngine.Dispatch") as dispatch:
                retrieve.return_value = None
                dispatch.return_value = mock.Mock()

                assert mem.AddRecord(mock.Mock()) is False, mem.AddRecord(mock.Mock())

    def test_delete_record(self):
        """
        We can delete a record
        """

        mem = engine.FelixRemoteMemory()
        mem.commands = {"delete": "foo"}

        with mock.patch("FelixMemoryServes.MemoryEngine.url_retrieve") as retrieve:
            with mock.patch("FelixMemoryServes.MemoryEngine.Dispatch") as dispatch:
                with mock.patch("FelixMemoryServes.MemoryEngine.util") as util:
                    data = {"source": "record source", "trans": "record trans", "id": 3}
                    retrieve.return_value = data
                    dispatch.return_value = mock.Mock()
                    util.wrap.return_value = data

                    mem.DeleteRecord(mock.Mock()) is True, mem.DeleteRecord(mock.Mock())

    def test_delete_record_no_command(self):
        """
        returns False if we don't have a delete command
        """

        mem = engine.FelixRemoteMemory()
        mem.commands = {}

        with mock.patch("FelixMemoryServes.MemoryEngine.Dispatch") as dispatch:
            dispatch.return_value = mock.Mock()
            assert mem.DeleteRecord(mock.Mock()) is False, mem.DeleteRecord(mock.Mock())

    def test_connect(self):
        """
        We can delete a record
        """

        mem = engine.FelixRemoteMemory()
        mem.commands = {"delete": "foo"}

        with mock.patch("FelixMemoryServes.MemoryEngine.url_retrieve") as retrieve:
            data = {"add": "add command"}
            retrieve.return_value = data

            mem.Connect("http://example.com/") is True, mem.Connect("http://example.com/")

    def test_connect_error(self):
        """
        We can delete a record
        """

        mem = engine.FelixRemoteMemory()
        mem.commands = {"delete": "foo"}

        with mock.patch("FelixMemoryServes.MemoryEngine.url_retrieve") as retrieve:
            retrieve.side_effect = ValueError('boom')

            mem.Connect("http://example.com/") is False, mem.Connect("http://example.com/")

    def test_get_login_url(self):
        """
        We can get the login url
        """

        mem = engine.FelixRemoteMemory()
        mem.commands = {"login": "from command"}

        login = mem.get_login_url()
        assert login == "from command", login

    def test_get_login_url_commands_missing(self):
        """
        We can get the login url when we don't have commands
        """

        mem = engine.FelixRemoteMemory()
        mem.commands = {}
        mem.base = "base"

        login = mem.get_login_url()
        assert login == "base/api/login/", login

    def test_login(self):
        """
        We can delete a record
        """

        mem = engine.FelixRemoteMemory()
        mem.commands = {"login": "foo"}

        with mock.patch("FelixMemoryServes.MemoryEngine.url_retrieve") as retrieve:
            retrieve.return_value = "token"

            mem.Login(u"username", u"password") is True, mem.Login(u"username", u"password")
            assert mem.token == "token", mem.token

    def test_login_error(self):
        """
        We can delete a record
        """

        mem = engine.FelixRemoteMemory()
        mem.commands = {"login": "foo"}

        with mock.patch("FelixMemoryServes.MemoryEngine.url_retrieve") as retrieve:
            retrieve.side_effect = Exception('boom')

            mem.Login(u"username", u"password") is False, mem.Login(u"username", u"password")

    def test_is_logged_in(self):
        """
        We can delete a record
        """

        mem = engine.FelixRemoteMemory()
        assert not mem.IsLoggedIn()
        mem.token = "token"
        assert mem.IsLoggedIn()

