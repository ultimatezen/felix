#coding: UTF8
"""
COM sever for interfacing with Memory Serves memory

"""
# COM stuff
from win32com.server import util
from win32com.client import Dispatch
import win32com.server.register
from win32com.server.exception import COMException
from winerror import ERROR_INVALID_INDEX, E_NOTIMPL

# system stuff
from win32api import GetUserName
import sys
import os

class FelixRange2HtmlFake(object):
    """A fake Word document"""

    _public_methods_ = ["RangeToHtml"]
    _public_attrs_ = []

    _reg_progid_ = "FakeFelix.RangeToHtml"
    _reg_clsid_ = "{325604AC-6993-46D0-B22E-928B61ABAB02}"
    
    def __init__(self):
        """Data will be the values we get from MemoryServes"""
        self._text = []
    
    def RangeToHtml(self, selection):
        self._text.append(Dispatch(selection).Text)
        return self._text[-1]

class FelixMatchFake(object):
    """A fake Felix sheet"""

    _public_methods_ = []
    _public_attrs_ = ['Record', 'Score']

    _reg_progid_ = "FakeFelix.Match"
    _reg_clsid_ = "{7CBA6180-98B0-4329-BAFA-A133A6210C0A}"
    
    def __init__(self):
        """Data will be the values we get from MemoryServes"""
        self._record = Dispatch("Felix.RemoteRecordFake")
        self._score = 0.0
    
    def Record(self):
        return self._record
    def SetRecord(self, record):
        self._record = record
    
    def Score(self):
        return self._score
    def SetScore(self, score):
        self._score = score

class FelixApp2Fake(object):
    """A fake Felix workbook"""

    _public_methods_ = ["ReviewTranslation", 'ReflectChanges']
    _public_attrs_ = ['CurrentMatch']

    _reg_progid_ = "FakeFelix.App2"
    _reg_clsid_ = "{385EBE61-C139-409D-B3C1-B5B6EF42DC50}"
    
    def __init__(self):
        self._match = Dispatch("FakeFelix.Match")
        self._review = []
        self._changes = []
    
    def CurrentMatch(self):
        return self._match
    def SetCurrentMatch(self, match):
        self._match = match

    def ReviewTranslation(self, recid, source, trans):
        print "Review translation:", recid, source, trans
        self._review.append((recid, source, trans))
    def ReflectChanges(self, recid, source, trans):
        self._changes.append((recid, source, trans))


class FelixAppFake(object):
    """A fake Felix app"""

    _public_methods_ = ['LookupTrans', 'CorrectTrans']
    _public_attrs_ = ['App2', 'Visible']

    _reg_progid_ = "FakeFelix.App"
    _reg_clsid_ = "{CCA233F5-6B6E-43DB-8D16-F9AFD5FCF617}"
    
    def __init__(self):
        self._app2 = Dispatch("FakeFelix.App2")
    
        self._lookup_trans = []
        self._correct_trans = []
        
        self._visible = False
    
    def Visible(self):
        return self._visible
    def SetVisible(self, visible):
        self._visible = visible
    
    def App2(self):
        return self._app2
    def SetApp2(self, app2):
        self._app2 = app2
        
    def LookupTrans(self, trans):
        self._lookup_trans.append(trans)
        
    def CorrectTrans(self, trans):
        self._correct_trans.append(trans)

def reg():    
    """Register COM servers"""
    
    print "Registering COM servers"
    win32com.server.register.UseCommandLine(FelixAppFake)
    win32com.server.register.UseCommandLine(FelixApp2Fake)
    win32com.server.register.UseCommandLine(FelixMatchFake)
    win32com.server.register.UseCommandLine(FelixRange2HtmlFake)

# Add code so that when this script is run by
# Python.exe, it self-registers.
if __name__ == '__main__':
    reg()
