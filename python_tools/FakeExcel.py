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

class ExcelShapeFake(object):
    """A fake excel sheet"""

    _public_methods_ = ["Select"]
    _public_attrs_ = ['Name', 'Text']

    _reg_progid_ = "FakeExcel.Shape"
    _reg_clsid_ = "{324EB3A0-0362-485D-9929-17068EAF26B3}"
    
    def __init__(self):
        """Data will be the values we get from MemoryServes"""
        self._name = u""
        self._text = 0
        self._selected = False

    def Name(self):
        return self._name
    def SetName(self, name):
        self._name = name
    
    def Text(self):
        if not self._selected:
            raise Exception("Must be selected first!")
        return self._text
    def SetText(self, text):
        if not self._selected:
            raise Exception("Must be selected first!")
        self._text = text
    
    def Select(self):
        self._selected = True

class ExcelCellFake(object):
    """A fake excel sheet"""

    _public_methods_ = []
    _public_attrs_ = ['Row', 'Column', 'Text']

    _reg_progid_ = "FakeExcel.Cell"
    _reg_clsid_ = "{5987F955-04C9-4041-B7F4-D7A1F9027CBF}"
    
    def __init__(self):
        """Data will be the values we get from MemoryServes"""
        self._row = 0
        self._column = 0
        self._text = 0
    
    def Row(self):
        return self._row
    def SetRow(self, row):
        self._row = row
    
    def Column(self):
        return self._column
    def SetColumn(self, column):
        self._column = column
    
    def Text(self):
        return self._text
    def SetText(self, text):
        self._text = text


class ExcelSheetFake(object):
    """A fake excel sheet"""

    _public_methods_ = ["Cells", "Shapes"]
    _public_attrs_ = ['Name']

    _reg_progid_ = "FakeExcel.Sheet"
    _reg_clsid_ = "{932F78B5-D6B7-4D5C-9549-CA9FC52C9E8F}"
    
    def __init__(self):
        """Data will be the values we get from MemoryServes"""
        self._name = u""
        
        self._cells = {}
        self._shapes = {}
    
    def Cells(self, row, col):
        key = (row, col)
        if key not in self._cells:
            self._cells[key] = Dispatch(util.wrap(ExcelCellFake()))
        return self._cells[key]

    def Shapes(self, key):
        if key not in self._shapes:
            self._shapes[key] = ExcelShapeFake()
        shape = self._shapes[key]
        shape._selected = False
        return Dispatch(util.wrap(shape))

    def Name(self):
        return self._name
    def SetName(self, name):
        self._name = name

class ExcelWorkbookFake(object):
    """A fake excel workbook"""

    _public_methods_ = []
    _public_attrs_ = ['FullName', 'Sheets']

    _reg_progid_ = "FakeExcel.Workbook"
    _reg_clsid_ = "{A9CEBFB9-4673-4031-832A-A39C32518974}"
    
    def __init__(self, sheets=None):
        self._name = u""
        if sheets:
            self._sheets = sheets
        else:
            self._sheets = []
    
    def FullName(self):
        return self._name
    def SetFullName(self, name):
        self._name = name
    
    def Sheets(self):
        return util.NewCollection(self._sheets)
    def SetSheets(self, sheets):
        self._sheets = sheets

class ExcelAppFake(object):
    """A fake excel app"""

    _public_methods_ = []
    _public_attrs_ = ['ActiveWorkbook', 'ActiveSheet', 'Selection']

    _reg_progid_ = "FakeExcel.App"
    _reg_clsid_ = "{962ADC4A-C635-4829-A9DF-E3E841690D3E}"

    def __init__(self):
        """Data will be the values we get from MemoryServes"""
        self._workbook = None
        self._sheet = None
        self._selection = None

    def ActiveSheet(self):
        return self._sheet
    def SetActiveSheet(self, sheet):
        self._sheet = sheet

    def ActiveWorkbook(self):
        return self._workbook
    def SetActiveWorkbook(self, workbook):
        self._workbook = workbook
   
    def Selection(self):
        return self._selection
    def SetSelection(self, selection):
        self._selection = selection

def reg():    
    """Register COM servers"""
    
    print "Registering COM servers"
    win32com.server.register.UseCommandLine(ExcelSheetFake)
    win32com.server.register.UseCommandLine(ExcelWorkbookFake)
    win32com.server.register.UseCommandLine(ExcelAppFake)
    win32com.server.register.UseCommandLine(ExcelCellFake)
    win32com.server.register.UseCommandLine(ExcelShapeFake)

# Add code so that when this script is run by
# Python.exe, it self-registers.
if __name__ == '__main__':
    reg()
