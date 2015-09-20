#!/usr/bin/env python

import unittest
import os

import winpaths
from nose.tools import raises

import fakewidgets
from FelixMemoryServes import update_checker_ta as uct


BASE_DIR = os.path.dirname(os.path.abspath(__file__))
APP_DATA = winpaths.get_local_appdata()


def one_up(dirname):
    """
    Gets one directory up.
    """

    return os.path.split(dirname)[0]


class TestFoldersAndLanguage(unittest.TestCase):

    def test_get_html_folder_english(self):
        folder = uct.get_html_folder("en")
        expected = os.path.join(APP_DATA, "TagAssist", "html", "en")
        assert folder == expected, folder

    def test_get_html_folder_japanese(self):
        folder = uct.get_html_folder("jp")
        expected = os.path.join(APP_DATA, "TagAssist", "html", "jp")
        assert folder == expected, folder

    def test_get_about_page_english(self):
        page = uct.get_about_page("en")
        expected = os.path.join(APP_DATA, "TagAssist", "html", "en", "about.html")
        assert page == expected, page

    def test_get_about_page_japanese(self):
        page = uct.get_about_page("jp")
        expected = os.path.join(APP_DATA, "TagAssist", "html", "jp", "about.html")
        assert page == expected, page


class TestAskToCheckDialog(unittest.TestCase):

    def setUp(self):
        self.dialog = uct.AskToCheckDialog()

    def test_init(self):
        assert self.dialog


class TestGetLatestVersionInfo(unittest.TestCase):

    def test_linestodata(self):
        lines = "foo=2\nbar=baz".splitlines()
        data = uct.linestodata(lines)
        expected = dict(foo='2', bar="baz")
        assert data == expected, (data, expected)


class TestLatestIsNewer(unittest.TestCase):
    @raises(ValueError)
    def test_empty(self):
        this = dict(version="")
        latest = dict(version="")
        assert not uct.latest_is_newer(this, latest)

    def test_true(self):
        this = dict(version="2.5")
        latest = dict(version="2.6")
        assert uct.latest_is_newer(this, latest)

    def test_len(self):
        this = dict(version="2.5")
        latest = dict(version="2.5.1")
        assert uct.latest_is_newer(this, latest)

    def test_false_equal(self):
        this = dict(version="2.5")
        latest = dict(version="2.5")
        assert not uct.latest_is_newer(this, latest)

    def test_false_lt(self):
        this = dict(version="2.6")
        latest = dict(version="2.5")
        assert not uct.latest_is_newer(this, latest)
