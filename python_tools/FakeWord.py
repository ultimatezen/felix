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

selection = None

class WordTextFrameFake(object):
    """A fake Word sheet"""

    _public_methods_ = []
    _public_attrs_ = ['TextRange']

    _reg_progid_ = "FakeWord.TextFrame"
    _reg_clsid_ = "{F5A1A470-81D7-4AE1-A739-656B64DCAD05}"
    
    def __init__(self):
        """Data will be the values we get from MemoryServes"""
        self._text = Dispatch("FakeWord.Range")
        self._text.StoryType = 5
    
    def TextRange(self):
        return self._text
    def SetTextRange(self, text):
        self._text = text


class WordShapeFake(object):
    """A fake Word sheet"""

    _public_methods_ = ["Select"]
    _public_attrs_ = ['Name', 'TextFrame', 'Application']

    _reg_progid_ = "FakeWord.Shape"
    _reg_clsid_ = "{6D701340-6998-4CB9-816B-45469F41C219}"
    
    def __init__(self):
        """Data will be the values we get from MemoryServes"""
        self._name = u""
        self._text = Dispatch("FakeWord.TextFrame")
        self._selected = False
        self._app = None
    
    def Application(self):
        if not self._app:
            self._app = Dispatch("FakeWord.App")
        return self._app
    def SetApplication(self, app):
        self._app = app

    def Name(self):
        return self._name
    def SetName(self, name):
        self._name = name
    
    def TextFrame(self):
        return self._text
    def SetTextFrame(self, text):
        self._text = text
    
    def Select(self):
        self._selected = True

class WordRangeFake(object):
    """A fake Word range"""

    _public_methods_ = ["Select", "AddShape", "SetRange", "SetTheRange"]
    _public_attrs_ = ['Start',
                      'End',
                      'Text',
                      'StoryType',
                      "ShapeRange",
                      "Range",
                      "Application"]

    _reg_progid_ = "FakeWord.Range"
    _reg_clsid_ = "{040DB49F-9051-4B12-A2B2-8221FFA0E803}"
    
    def __init__(self):
        """Data will be the values we get from MemoryServes"""
        self._text = [u""]
        self._start = 0
        self._end = 0
        self._selected = False
        self._story_type = 1
        self._shapes = []
        self._textrange = None
        self._app = None
    
    def Application(self):
        if not self._app:
            self._app = Dispatch("FakeWord.App")
        return self._app
    def SetApplication(self, app):
        self._app = Dispatch(app)
   
    def ShapeRange(self):
        return util.NewCollection(self._shapes)
    def AddShape(self, shape):
        self._shapes.append(shape)

    def SetRange(self, start, end):
        self._start = start
        self._end = end

    def Range(self):
        textrange = Dispatch("FakeWord.Range")
        if self._text:
            textrange.Text = self._text[-1]
        textrange.Start = self._start
        textrange.End = self._end
        textrange.StoryType = self._story_type
        for shape in self._shapes:
            textrange.AddShape(shape)
        return textrange

    def SetTheRange(self, textrange):
        tr = Dispatch(textrange)
        self._text.append(tr.Text)
        self._start = tr.Start
        self._end = tr.End

    def StoryType(self):
        return self._story_type
    def SetStoryType(self, story_type):
        self._story_type = story_type
  
    def Start(self):
        return self._start
    def SetStart(self, start):
        self._start = start

    def End(self):
        return self._end
    def SetEnd(self, end):
        self._end = end

    def Text(self):
        text = self._text[-1]
        if len(self._text) > 1:
            self._text = self._text[:-1]
        return text
    def SetText(self, text):
        self._text.append(text)
    
    def Select(self):
        self._selected = True
        global selection
        selection = self.Range()

class WordDocumentFake(object):
    """A fake Word document"""

    _public_methods_ = ['Shapes', 'StoryRanges']
    _public_attrs_ = ['FullName', 'Application']

    _reg_progid_ = "FakeWord.Document"
    _reg_clsid_ = "{9804C8D8-9295-4C7D-8F98-BA0912E9DAFA}"
    
    def __init__(self):
        """Data will be the values we get from MemoryServes"""
        self._name = u""
        self._shapes = {}
        self._stories = {}
        self._app = None
    
    def Application(self):
        if not self._app:
            self._app = Dispatch("FakeWord.App")
        return self._app
    def SetApplication(self, app):
        self._app = Dispatch(app)
    
    def Shapes(self, key):
        return self._shapes[key]

    def StoryRanges(self, key):
        try:
            return self._stories[key]
        except KeyError:
            self._stories[key] = Dispatch("FakeWord.Range")
            self._stories[key].StoryType = key
            return self._stories[key]

    def FullName(self):
        return self._name
    def SetFullName(self, name):
        self._name = name

class WordAppFake(object):
    """A fake Word app"""

    _public_methods_ = []
    _public_attrs_ = ['Selection', 'ActiveDocument']

    _reg_progid_ = "FakeWord.App"
    _reg_clsid_ = "{F5E3773A-5B78-4C1D-8D97-933C1E0AEF95}"

    def __init__(self):
        """Data will be the values we get from MemoryServes"""

        self._selection = None
        self._active_doc = Dispatch("FakeWord.Document")

    def ActiveDocument(self):
        return self._active_doc
    def SetActiveDocument(self, doc):
        self._active_doc = doc

    def Selection(self):
        global selection
        if not selection:
            selection = Dispatch("FakeWord.Range")
        return selection
    def SetSelection(self, selection):
        self._selection = selection

def reg():    
    """Register COM servers"""
    
    print "Registering COM servers"
    win32com.server.register.UseCommandLine(WordAppFake)
    win32com.server.register.UseCommandLine(WordDocumentFake)
    win32com.server.register.UseCommandLine(WordRangeFake)
    win32com.server.register.UseCommandLine(WordShapeFake)
    win32com.server.register.UseCommandLine(WordTextFrameFake)

# Add code so that when this script is run by
# Python.exe, it self-registers.
if __name__ == '__main__':
    reg()
