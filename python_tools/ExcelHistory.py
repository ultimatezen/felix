#coding: UTF8
"""
Translation History COM Server
"""
from win32com.server import util
from pywintypes import com_error
from win32com.client import Dispatch
import win32com.server.register
import pythoncom
from loc import we_are_frozen
from cPickle import load, dump
import sys
import os
import utils
from utils import TransUnit
from utils import serialized_source
from utils import debug, warn, error, log_err
from utils import FelixObject

__version__ = "0.2"
__progname__ = "Felix Excel translation history"

def load_data(workbook, datasource):
    """
    Load the data from the data source.
    Synchronize the sheets in the data with the worksheets in the workbook.
    """

    data = load(datasource)
    sheets = []
    for sheet in workbook.Sheets:
        if sheet.Name in data:
            sheet_obj = SheetObject(sheet)
            sheet_obj.cells = data[sheet.Name]["cells"]
            sheet_obj.shapes = data[sheet.Name]["shapes"]
            sheets.append(sheet_obj)
    return sheets

def serialize_sheet(sheet):
    """Create a dictionary from the worksheet for serialization"""

    return dict(cells=sheet.cells,
                shapes=sheet.shapes)

def serialized_source(workbook):
    """Get the filename"""

    return workbook.FullName + u".fhist"

def make_serialization_object(workbook):
    """Get the filename and data for serialization"""

    filename = serialized_source(workbook)
    serialized = {}
    for sheet in workbook.sheets:
        serialized[sheet.Name] = serialize_sheet(sheet)
    return filename, serialized

def get_key_and_collection(selection, worksheet):
    """Get the dict key and the dictionary to look up in"""

    try:
        # try for cell
        key = (selection.Row, selection.Column)
        coll = worksheet.cells
    except AttributeError:
        # it's a shape
        key = selection.Name
        coll = worksheet.shapes

    return key, coll


class SheetObject(object):
    """Wraps a worksheet object from Excel"""

    def __init__(self, sheet):
        self.sheet = sheet
        self.cells = {}
        self.shapes = {}
    def _get_name(self):
        return self.sheet.Name
    Name = property(_get_name)

class WorkbookObject(object):
    """Wraps a workbook object from Excel"""

    def __init__(self, workbook):
        self.workbook = workbook
        self.sheets = []
    def _get_fullname(self):
        try:
            return self.workbook.FullName
        except com_error, e:
            LOGGER.exception("Failed to get property FullName from workbook")
            error(str(e))
            excel = Dispatch("Excel.Application")
            self.workbook = excel.ActiveWorkbook

    FullName = property(_get_fullname)


class TransHistoryExcel(object):
    """Translation history server for Excel files"""

    _public_methods_ = ["RecordTrans",
                        "ShutDown",
                        "LookupTrans",
                        "ReflectChanges",
                        "CorrectTrans"]
    _public_attrs_ = ['App', 'Felix']
    _reg_progid_ = "FelixTransHistory.Excel"

    # import pythoncom
    # print pythoncom.CreateGuid()
    _reg_clsid_ = "{64598561-C7C5-4232-87D4-6D1CC66BAF09}"

    def __init__(self):
        self._app = None
        self._felix = None
        self._curr_sheet_name = None

        self._history = []

        debug("FelixTransHistory.Excel")

    def get_workbook(self, workbook):
        """
        If we've already got it, we're done.
        Otherwise, check for a file. Load if it exists.
        """

        for _workbook in self._history:
            if _workbook.FullName == workbook.FullName:
                return _workbook

        debug("Adding workbook object")
        _workbook = WorkbookObject(workbook)
        self._history.append(_workbook)
        filename = serialized_source(workbook)
        if os.path.exists(filename):
            _workbook.sheets = load_data(workbook, open(filename))
        return self._history[-1]

    def get_sheet(self, sheet, sheets):
        for sheet_obj in sheets:
            if sheet_obj.Name == sheet.Name:
                return sheet_obj
        sheets.append(SheetObject(sheet))
        return sheets[-1]

    def App(self):
        return self._app
    def SetApp(self, app):
        debug("Setting app")
        self._app = Dispatch(app)

    def Felix(self):
        return self._felix
    def SetFelix(self, felix):
        """Create if False"""
        if not felix:
            self._felix = Dispatch("Felix.App")
        else:
            self._felix = Dispatch(felix)

    def get_felix(self):
        """
        Get the Felix COM server.
        Create it if it doesn't exist.
        """
        if not self._felix:
            self._felix = FelixObject()
        return self._felix

    def RecordTrans(self):
        try:
            selection = Dispatch(self._app.Selection)
            workbook = self.get_workbook(self._app.ActiveWorkbook)
            worksheet = self.get_sheet(self._app.ActiveSheet, workbook.sheets)

            key, collection = get_key_and_collection(selection, worksheet)

            felix = self.get_felix()
            record = felix.get_record()
            collection[key] = TransUnit(record.Id, record.Source, selection.Text)
        except:
            LOGGER.exception("Error recording translation")
            raise

    def LookupTrans(self):
        try:
            selection = Dispatch(self._app.Selection)
            workbook = self.get_workbook(self._app.ActiveWorkbook)
            worksheet = self.get_sheet(self._app.ActiveSheet, workbook.sheets)
            key, collection = get_key_and_collection(selection, worksheet)

            felix = self.get_felix()

            try:
                unit = collection[key]
                felix.App2.ReviewTranslation(unit.recid,
                                             unit.source,
                                             selection.Text)
            except KeyError:
                felix.LookupTrans(selection.Text)
        except:
            LOGGER.exception("Error looking up translation")
            raise

    def CorrectTrans(self):
        try:
            selection = Dispatch(self._app.Selection)
            workbook = self.get_workbook(self._app.ActiveWorkbook)
            worksheet = self.get_sheet(self._app.ActiveSheet, workbook.sheets)

            try:
                # try for cell
                key = (selection.Row, selection.Column)
                item = worksheet.cells[key]
                reflect = self.reflect_cell
            except AttributeError:
                # it's a shape
                key = selection.Name
                item = worksheet.shapes[key]
                reflect = self.reflect_shape

            reflect(worksheet.sheet, key, item)
        except:
            LOGGER.exception("Error reflecting translation")
            raise

    def ReflectChanges(self):
        try:
            selection = Dispatch(self._app.Selection)
            workbook = self.get_workbook(self._app.ActiveWorkbook)
            worksheet = self.get_sheet(self._app.ActiveSheet, workbook.sheets)

            sheet = worksheet.sheet

            for key, cell in worksheet.cells.iteritems():
                self.reflect_cell(sheet, key, cell)
            for key, shape in worksheet.shapes.iteritems():
                self.reflect_shape(sheet, key, shape)
        except:
            LOGGER.exception("Error reflecting changes to workbook")
            raise

    def reflect_cell(self, sheet, key, unit):
        row, col = key
        newtext = sheet.Cells(row, col).Text
        self.get_felix().App2.ReflectChanges(unit.recid, unit.source, newtext)

    def reflect_shape(self, sheet, key, unit):
        shape = sheet.Shapes(key)
        shape.Select()
        newtext = self._app.Selection.Text
        self.get_felix().App2.ReflectChanges(unit.recid, unit.source, newtext)

    def ShutDown(self, workbook):
        workbook = Dispatch(workbook)
        try:
            name = workbook.FullName
            debug(u"Workbook closing (%s)" % name)
            for _workbook in self._history:
                if _workbook.FullName == name:
                    filename, data = make_serialization_object(_workbook)
                    debug(u"Serializing to %s" % filename)
                    dump(data, file(filename, "w"))
                    return
            debug("No translation history")
        except:
            LOGGER.exception("ShutDown")
            raise
def reg():
    """Register COM servers"""

    debug("Registering COM servers")
    win32com.server.register.UseCommandLine(TransHistoryExcel)

utils.determine_redirect("history_excel.log")

# Add code so that when this script is run by
# Python.exe, it self-registers.
if __name__ == '__main__':
    reg()
