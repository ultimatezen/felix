"""
Unit tests for TransHistory module
"""
from win32com.server import util
import ExcelHistory as Excel
import FakeExcel, FakeFelix
from win32com.client import Dispatch as d
import os
from cStringIO import StringIO
import utils as history_utils

def test_serialize_sheet_empty():
    sheet = Excel.SheetObject(None)
    serialized = Excel.serialize_sheet(sheet)
    assert serialized == dict(cells={}, shapes={}), serialized

def test_serialize_sheet():
    sheet = Excel.SheetObject(None)
    sheet.cells = {(0,1) : u"spam", (2,2) : u"egg"}
    sheet.shapes = {u"rectangle 1" : u"reccy"}
    serialized = Excel.serialize_sheet(sheet)
    assert serialized == dict(cells=sheet.cells, shapes=sheet.shapes), serialized

def test_make_serialization_object():
    workbook = make_wb_obj(u"test.xls")
    sheet1 = make_sheet_obj(u"sheet 1")
    sheet1.cells = {(0,1) : u"spam", (2,2) : u"egg"}
    sheet1.shapes = {u"rectangle 1" : u"reccy"}
    sheet2 = make_sheet_obj(u"sheet 2")
    sheet2.cells = {(5,5) : u"5,5", (1,1) : u"1,1"}
    sheet2.shapes = {u"rectangle 2" : u"reccy"}
    workbook.sheets = [sheet1, sheet2]

    filename, serialized = Excel.make_serialization_object(workbook)
    assert filename == u"test.xls.fhist", filename
    assert len(serialized) == 2, serialized

    sheet1s, sheet2s = serialized["sheet 1"], serialized["sheet 2"]
    print sheet1s
    assert sheet1s["cells"] == {(0,1) : u"spam", (2,2) : u"egg"}, sheet1s
    assert sheet1s["shapes"] == {u"rectangle 1" : u"reccy"}, sheet1s

    assert sheet2s["cells"] == {(5,5) : u"5,5", (1,1) : u"1,1"}, sheet2s
    assert sheet2s["shapes"] == {u"rectangle 2" : u"reccy"}, sheet2s


class TestTransHistoryExcel(object):
    def setup(self):
        self.history = Excel.TransHistoryExcel()

    def test_create(self):
        assert self.history

    def test_app(self):
        app = d("FakeExcel.App")
        self.history.SetApp(app)
        assert self.history.App() == app, self.history.App()

    def test_felix(self):
        server = d("Felix.RemoteRecordFake")
        self.history.SetFelix(server)
        assert self.history.Felix() == server, self.history.App()

def make_wb_obj(name):
    workbook = d("FakeExcel.Workbook")
    workbook.FullName = name
    wb_obj = Excel.WorkbookObject(workbook)
    return wb_obj

def make_sheet(name):
    sheet = d("FakeExcel.Sheet")
    sheet.Name = name
    return sheet

def make_sheet_obj(name):
    return Excel.SheetObject(make_sheet(name))


def make_cell(row, col, text):
    cell = d("FakeExcel.Cell")
    cell.Row = row
    cell.Column = col
    cell.Text = text
    return cell

class TestTransHistoryExcelSetTrans(object):
    def setup(self):
        self.history = Excel.TransHistoryExcel()
        app = d("FakeExcel.App")
        wb = d("FakeExcel.Workbook")
        wb.FullName = u"book 1"
        sheet = d("FakeExcel.Sheet")
        sheet.Name = u"sheet 1"
        app.ActiveWorkbook = wb
        app.ActiveSheet = sheet
        self.history.SetApp(app)

        self.felix = d("FakeFelix.App")
        self.felix.App2 = d("FakeFelix.App2")
        self.felix.App2.CurrentMatch = d("FakeFelix.Match")
        self.felix.App2.CurrentMatch.Record = d("Felix.RemoteRecordFake")

        self.felix_obj = history_utils.FelixObject(self.felix)

        self.history._felix = self.felix_obj

    def test_create(self):
        pass

    def set_rec(self, recid, source, trans):
        rec = self.felix.App2.CurrentMatch.Record
        rec.Id = recid
        rec.Source = source
        rec.Trans = trans

    def set_cell(self, row, col, text):
        cell = make_cell(row, col, text)
        self.history.App().Selection = cell

    def test_set_first(self):
        self.set_rec(10, u"rec source", u"rec trans")
        self.set_cell(5, 15, u"cell text")
        self.history.RecordTrans()

        assert len(self.history._history) == 1, self.history._history

        rec = self.history._history[0].sheets[0].cells[(5,15)]
        assert 10 == rec.recid, rec.recid
        assert u"rec source" == rec.source, rec.source
        assert u"cell text" == rec.trans, rec.trans


    def test_set_shape(self):
        self.set_rec(10, u"rec source", u"rec trans")
        shape = d("FakeExcel.Shape")
        shape.Name = u"Rectangle 1"
        shape.Select()
        shape.Text = u"hello, world"
        self.history.App().Selection = shape
        self.history.RecordTrans()

        assert len(self.history._history) == 1, self.history._history

        rec = self.history._history[0].sheets[0].shapes[u"Rectangle 1"]
        assert 10 == rec.recid, rec.recid
        assert u"rec source" == rec.source, rec.source
        assert u"hello, world" == rec.trans, rec.trans


    def test_set_two(self):
        self.set_rec(1, u"rec source 1", u"rec trans 1")
        self.set_cell(1, 1, u"cell text 1")
        self.history.RecordTrans()
        assert len(self.history._history) == 1, self.history._history

        self.set_rec(2, u"rec source 2", u"rec trans 2")
        self.set_cell(2, 2, u"cell text 2")
        self.history.RecordTrans()

        # basic info about history
        assert len(self.history._history) == 1, self.history._history
        sheets = self.history._history[0].sheets
        assert len(sheets) == 1, sheets
        cells = sheets[0].cells
        assert len(cells) == 2, cells

        # first record
        rec = self.history._history[0].sheets[0].cells[(1, 1)]

        assert 1 == rec.recid, rec.recid
        assert u"rec source 1" == rec.source, rec.source
        assert u"cell text 1" == rec.trans, rec.trans

        # second record
        rec = self.history._history[0].sheets[0].cells[(2, 2)]

        assert 2 == rec.recid, rec.recid
        assert u"rec source 2" == rec.source, rec.source
        assert u"cell text 2" == rec.trans, rec.trans

    def test_lookup_trans_no_history(self):
        self.set_cell(1, 1, u"cell text 1")
        felix = FakeFelix.FelixAppFake()
        self.history._felix = d(util.wrap(felix))

        self.history.LookupTrans()

        assert felix._lookup_trans == [u"cell text 1"], felix._lookup_trans

    def set_fake_app2(self):
        felix = FakeFelix.FelixAppFake()
        app2 = FakeFelix.FelixApp2Fake()
        felix._app2 = d(util.wrap(app2))
        felix._app2.CurrentMatch = d("FakeFelix.Match")
        felix._app2.CurrentMatch.Record = d("Felix.RemoteRecordFake")
        self.history._felix = d(util.wrap(felix))

        return app2

    def test_lookup_trans_history(self):

        self.set_rec(1, u"rec source 1", u"rec trans 1")
        self.set_cell(1, 1, u"cell text 1")
        self.history.RecordTrans()

        app2 = self.set_fake_app2()
        self.history.LookupTrans()

        assert app2._review == [(1, u"rec source 1", u"cell text 1")], app2._review

    def test_reflect_sheet_cell(self):

        self.set_rec(1, u"rec source 1", u"rec trans 1")
        self.set_cell(1, 1, u"cell text 1")
        self.history.RecordTrans()

        app2 = self.set_fake_app2()

        self.history.App().ActiveSheet.Cells(1, 1).Text = u"new cell text"

        self.history.ReflectChanges()
        assert app2._changes == [(1, u"rec source 1", u"new cell text")], app2._changes

    def test_correct_trans(self):

        self.set_rec(1, u"rec source 1", u"rec trans 1")
        self.set_cell(1, 1, u"cell text 1")
        self.history.RecordTrans()

        app2 = self.set_fake_app2()

        self.history.App().ActiveSheet.Cells(1, 1).Text = u"new cell text"

        self.history.CorrectTrans()
        assert app2._changes == [(1, u"rec source 1", u"new cell text")], app2._changes

    def test_reflect_sheet_shape(self):

        self.set_rec(10, u"rec source", u"rec trans")
        shape = d("FakeExcel.Shape")
        shape.Name = u"Rectangle 1"
        shape.Select()
        shape.Text = u"hello, world"
        self.history.App().Selection = shape
        self.history.RecordTrans()

        app2 = self.set_fake_app2()

        shape = self.history.App().ActiveSheet.Shapes(u"Rectangle 1")
        shape.Select()
        shape.Text = u"new shape text"
        self.history.App().Selection = shape

        self.history.ReflectChanges()
        assert app2._changes == [(10, u"rec source", u"new shape text")], app2._changes


class TestSerialization(object):
    def setup(self):
        self.history = Excel.TransHistoryExcel()
        app = d("FakeExcel.App")
        self.history.SetApp(app)
        sheets = [make_sheet(u"sheet 1"), make_sheet(u"sheet 2")]
        workbook = FakeExcel.ExcelWorkbookFake(sheets)
        self.history.App().ActiveWorkbook = util.wrap(workbook)

    def test_create(self):
        pass
