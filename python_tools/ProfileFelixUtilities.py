#!/usr/bin/env python
import utils
utils.TESTING = True

from FelixMemoryServes import FelixUtilities
import os
from win32com.client import Dispatch as d
import shutil

def test_ensure_html():
    if os.path.isdir(r"C:\Users\Ryan\AppData\Local\Felix\html"):
        shutil.rmtree(r"C:\Users\Ryan\AppData\Local\Felix\html")

    utils = FelixUtilities.FelixUtilities()
    utils.EnsureHtml()

    assert os.path.isdir(r"C:\Users\Ryan\AppData\Local\Felix\html")

def test_ensure_html_com():
    if os.path.isdir(r"C:\Users\Ryan\AppData\Local\Felix\html"):
        shutil.rmtree(r"C:\Users\Ryan\AppData\Local\Felix\html")

    utils = d("Felix.Utilities")
    utils.EnsureHtml()

    assert os.path.isdir(r"C:\Users\Ryan\AppData\Local\Felix\html")

def test_instantiate():
    utils = d(u"Felix.Utilities")
    assert utils, utils
