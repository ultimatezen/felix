#!/usr/bin/env python

import unittest
import os

import winpaths

from FelixMemoryServes import loc

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
APP_DATA = winpaths.get_local_appdata().lower()


def one_up(dirname):
    """
    Gets one directory up.
    """

    return os.path.split(dirname)[0]


class TestLoc(unittest.TestCase):

    def test_get_local_appdata(self):
        expected = APP_DATA
        actual = loc.get_local_appdata().lower()
        print actual
        print expected
        assert expected == actual, (expected, actual)

    def test_get_local_app_data_folder(self):
        expected = os.path.join(APP_DATA, "felix")
        actual = loc.get_local_app_data_folder().lower()
        print actual
        print expected
        assert actual == expected, (expected, actual)

    def test_get_log_filename(self):
        expected = os.path.join(one_up(BASE_DIR), "logs", "memoryserves.log").lower()
        actual = loc.get_log_filename(u"MemoryServes.log").lower()
        print actual
        print expected
        assert expected == actual, (expected, actual)
