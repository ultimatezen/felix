#!/usr/bin/env python

import sys
from FelixMemoryServes import WordHistory as Word
import FakeWord
import FakeFelix
from win32com.client import Dispatch as d
from win32com.server import util
from cStringIO import StringIO
import os
from nose.tools import raises

def make_shape(name, text):
    shape = d("FakeWord.Shape")
    shape.Name = name
    shape.TextFrame.TextRange.Text = text
    return shape

class TestSegmentObject(object):
    def setup(self):
        selection = d("FakeWord.Range")
        selection.SetRange(1, 10)
        unit = Word.TransUnit(1, u"source", u"trans")
        self.seg = Word.SegmentObject(selection, unit)

    def test_create(self):

        seg = self.seg
        assert seg.segment.Start == 1, seg.segment.Start
        assert seg.segment.End == 10, seg.segment.End
        assert seg.guard.Start == 10, seg.guard.Start
        assert seg.guard.End == 11, seg.guard.End

    def test_no_expansion(self):
        seg = self.seg
        seg.expand()
        assert seg.segment.Start == 1, seg.segment.Start
        assert seg.segment.End == 10, seg.segment.End
        assert seg.guard.Start == 10, seg.guard.Start
        assert seg.guard.End == 11, seg.guard.End

    def test_expansion(self):
        seg = self.seg
        seg.guard.End = 15
        seg.expand()
        assert seg.segment.Start == 1, seg.segment.Start
        assert seg.segment.End == 14, seg.segment.End
        assert seg.guard.Start == 14, seg.guard.Start
        assert seg.guard.End == 15, seg.guard.End

    def test_set_segrange(self):
        selection = d("FakeWord.Range")
        selection.SetRange(10, 11)
        seg = self.seg
        seg.set_segrange(selection)
        assert seg.segment.Start == 10, seg.segment.Start
        assert seg.segment.End == 11, seg.segment.End
        assert seg.guard.Start == 11, seg.guard.Start
        assert seg.guard.End == 12, seg.guard.End

class TestSegmentsEqual(object):
    def setup(self):
        self.s1 = d("FakeWord.Range")
        self.s2 = d("FakeWord.Range")

    def test_init(self):
        assert Word.segments_equal(self.s1, self.s2)

    def test_identical(self):
        s1, s2 = self.s1, self.s2
        s1.Start = 1
        s1.End = 10
        s2.Start = 1
        s2.End = 10
        assert Word.segments_equal(s1, s2)

    def test_starts_diff(self):
        s1, s2 = self.s1, self.s2
        s1.Start = 2
        s1.End = 10
        s2.Start = 1
        s2.End = 10
        assert not Word.segments_equal(s1, s2)

    def test_first_end_greater(self):
        s1, s2 = self.s1, self.s2
        s1.Start = 1
        s1.End = 15
        s2.Start = 1
        s2.End = 10
        assert Word.segments_equal(s1, s2)

    def test_second_end_greater(self):
        s1, s2 = self.s1, self.s2
        s1.Start = 1
        s1.End = 15
        s2.Start = 1
        s2.End = 16
        assert not Word.segments_equal(s1, s2)

def default_seg(start, end):
    segrange = d("FakeWord.Range")
    segrange.Start = start
    segrange.End = end
    unit = Word.TransUnit(10, u"source", u"trans")
    return Word.SegmentObject(segrange, unit)

class TestFitSegment(object):
    def setup(self):
        self.u = Word.TransUnit(10, u"source", u"trans")
    def test_empty(self):
        seg1 = default_seg(1, 10)
        segments = []
        Word.fit_segment(segments, seg1)
        assert segments == [seg1], segments
    def test_no_overlap(self):
        seg1 = default_seg(1, 10)
        seg2 = default_seg(10, 20)
        segments = [seg1]
        Word.fit_segment(segments, seg2)
        assert segments == [seg1, seg2], segments
        assert (1, 10) == (seg1.segment.Start, seg1.segment.End), seg1
        assert (10, 20) == (seg2.segment.Start, seg2.segment.End), seg2
    def test_overlap_start(self):
        seg1 = default_seg(1, 15)
        seg2 = default_seg(10, 20)
        segments = [seg1]
        Word.fit_segment(segments, seg2)
        assert segments == [seg1, seg2], segments
        assert (1, 10) == (seg1.segment.Start, seg1.segment.End), seg1
        assert (10, 20) == (seg2.segment.Start, seg2.segment.End), seg2
    def test_overlap_end(self):
        seg1 = default_seg(10, 20)
        seg2 = default_seg(1, 15)
        segments = [seg1]
        Word.fit_segment(segments, seg2)
        assert segments == [seg1, seg2], segments
        assert (15, 20) == (seg1.segment.Start, seg1.segment.End), seg1
        assert (1, 15) == (seg2.segment.Start, seg2.segment.End), seg2

class TestUnserialize(object):

    def test_seg(self):
        story = d("FakeWord.Range")
        unit = Word.TransUnit(5, u"source", u"shape trans")
        segdata = dict(pos=(1, 5), unit=unit)
        segment = Word.unserialize_seg(story, segdata)

        assert segment.segment.Start == 1, segment.segment.Start
        assert segment.segment.End == 5, segment.segment.End
        assert segment.unit == unit, segment.unit

    def test_segments(self):
        story = d("FakeWord.Range")
        unit1 = Word.TransUnit(5, u"source 1", u"story trans 1")
        segdata1 = dict(pos=(1, 5), unit=unit1)

        story = d("FakeWord.Range")
        unit2 = Word.TransUnit(15, u"source 2", u"story trans 2")
        segdata2 = dict(pos=(11, 15), unit=unit2)

        segs = [segdata1, segdata2]

        seg1, seg2 = Word.unserialize_segments(story, segs)

        assert seg1.unit == unit1, seg1.unit
        assert seg2.unit == unit2, seg2.unit


    def test_shape_error_none(self):
        assert None == Word.unserialize_shape(None, None)
    def test_shape_error_doc_none(self):
        assert None == Word.unserialize_shape(None, dict(name="foo"))
    def test_shape_error_bad(self):
        doc = FakeWord.WordDocumentFake()
        shape = make_shape(u"foo", u"text")

        doc._shapes = {shape.Name : shape}
        assert None == Word.unserialize_shape(doc, dict(name="foo"))

    def test_shape(self):
        doc = FakeWord.WordDocumentFake()
        shape = make_shape(u"rectangle 1", u"text")

        doc._shapes = {shape.Name : shape}

        shapedata = dict(name=u"rectangle 1", segments=[])

        shapeobj = Word.unserialize_shape(d(util.wrap(doc)), shapedata)

        text = shapeobj.shape.TextFrame.TextRange.Text
        assert text == u"text", text
        assert not shapeobj.segments, shapeobj.segments

    def test_shapes(self):
        doc = FakeWord.WordDocumentFake()
        shape1 = make_shape(u"rectangle 1", u"text1")
        shape2 = make_shape(u"rectangle 2", u"text2")

        doc._shapes = {shape1.Name : shape1, shape2.Name : shape2}

        unit1 = Word.TransUnit(5, u"source 1", u"story trans 1")
        segdata1 = dict(pos=(1, 5), unit=unit1)
        data1 = dict(name=u"rectangle 1", segments=[segdata1])

        unit2 = Word.TransUnit(15, u"source 2", u"story trans 2")
        segdata2 = dict(pos=(11, 15), unit=unit2)
        data2 = dict(name=u"rectangle 2", segments=[segdata2])

        shapedata = {shape1.Name : data1, shape2.Name : data2}

        shapes = Word.unserialize_shapes(d(util.wrap(doc)), shapedata)

        s1, s2 = shapes[u"rectangle 1"], shapes[u"rectangle 2"]

        text = s1.shape.TextFrame.TextRange.Text
        assert text == u"text1", text

        assert len(s1.segments) == 1, s1.segments

        text = s2.shape.TextFrame.TextRange.Text
        assert text == u"text2", text

        assert s2.segments[0].unit.recid == 15, s2.segments[0]

    def test_story(self):
        doc = FakeWord.WordDocumentFake()
        story = d("FakeWord.Range")
        story.StoryType = 1
        doc._stories = {1 : story}

        storydata = dict(story_type=1, segments=[])

        storyobj = Word.unserialize_story(d(util.wrap(doc)), storydata)

        story_type = storyobj.story.StoryType
        assert story_type == 1, story_type
        assert not storyobj.segments, storyobj.segments

def test_segments_equal_false():
    seg1 = d("FakeWord.Range")
    seg1.Start = 5
    seg1.End = 10

    seg2 = d("FakeWord.Range")
    seg2.Start = 5
    seg2.End = 11

    assert not Word.segments_equal(seg1, seg2)

def test_segments_equal_false():
    seg1 = d("FakeWord.Range")
    seg1.Start = 15
    seg1.End = 20

    seg2 = d("FakeWord.Range")
    seg2.Start = 15
    seg2.End = 20

    assert Word.segments_equal(seg1, seg2)

class TestSerialization(object):

    def test_segment(self):
        segment = d("FakeWord.Range")
        segment.Start = 5
        segment.End = 10
        unit = Word.TransUnit(5, u"source", u"trans")
        data = Word.serialize_segment(Word.SegmentObject(segment, unit))
        assert data == dict(pos=(5,10), unit=unit), data

    def test_shape(self):
        shape = make_shape(u"rectangle 1", u"shape text")
        shapeobj = Word.ShapeObject(shape)

        data = Word.serialize_shape(shapeobj)
        assert data == dict(name=u"rectangle 1", segments=[]), data

    def test_story_empty(self):
        story = d("FakeWord.Range")
        story.StoryType = 1
        data = Word.serialize_story(Word.StoryObject(story))
        assert data == dict(story_type=1, segments=[]), data

    def test_story_two_segs(self):
        story = d("FakeWord.Range")
        story.StoryType = 2

        story_obj = Word.StoryObject(story)

        seg1 = d("FakeWord.Range")
        seg1.Start = 5
        seg1.End = 10
        unit1 = Word.TransUnit(5, u"source1", u"trans1")

        seg2 = d("FakeWord.Range")
        seg2.Start = 15
        seg2.End = 100
        unit2 = Word.TransUnit(3, u"source2", u"trans2")

        wobj1 = Word.SegmentObject(seg1, unit1)
        wobj2 = Word.SegmentObject(seg2, unit2)

        story_obj.segments = [wobj1, wobj2]

        data = Word.serialize_story(story_obj)
        assert data == dict(story_type=2,
                            segments=[dict(pos=(5,10), unit=unit1),
                                      dict(pos=(15,100), unit=unit2)]), data

class TestTransHistoryWord(object):
    def setup(self):
        self.word = Word.TransHistoryWord()

    def test_create(self):
        pass

    def test_get_document_empty(self):
        doc = d("FakeWord.Document")
        doc.FullName = u"foo"
        document = self.word.get_document(doc)
        assert document.document == doc, document.document
        assert document.stories == {}, document.stories
        assert document.shapes == {}, document.shapes

    def test_get_document_existing(self):
        doc1 = d("FakeWord.Document")
        doc1.FullName = u"foo"
        doc2 = d("FakeWord.Document")
        doc2.FullName = u"bar"

        self.word._history = [Word.DocumentObject(doc1), Word.DocumentObject(doc2)]

        document = self.word.get_document(doc1)
        assert document.document == doc1, document.document
        assert document.stories == {}, document.stories
        assert document.shapes == {}, document.shapes
        assert document.FullName == u"foo"

    def test_set_app(self):
        app = d("FakeWord.App")

        doc = d("FakeWord.Document")
        doc.FullName = u"doc1"

        app.ActiveDocument = doc

        self.word.SetApp(app)

        assert self.word.App().ActiveDocument.FullName == u"doc1"

    def test_set_parser(self):
        parser = d("FakeFelix.RangeToHtml")
        self.word.SetParser(parser)
        assert self.word.Parser() == parser

    def test_set_felix(self):
        felix = d("FakeFelix.App")
        felix.App2.CurrentMatch.Record.Source = u"felix"
        self.word.SetFelix(felix)
        record = self.word.Felix().App2.CurrentMatch.Record
        assert record.Source == u"felix", record.Source

    def test_get_shape_empty(self):
        doc = d("FakeWord.Document")
        docobj = Word.DocumentObject(doc)

        shape1 = make_shape(u"rectangle 1", u"text1")

        selection = d("FakeWord.Range")
        selection.AddShape(shape1)

        shape = self.word.get_shape(docobj, selection)

        assert shape.Name == u"rectangle 1", shape.Name
        assert docobj.shapes[shape.Name].Name == u"rectangle 1", shape.Name

    def test_get_shape(self):
        doc = d("FakeWord.Document")
        docobj = Word.DocumentObject(doc)

        shape1 = make_shape(u"rectangle 1", u"text1")
        shape2 = make_shape(u"rectangle 2", u"text2")

        docobj.shapes = {shape1.Name : shape1, shape2.Name : shape2}

        selection = d("FakeWord.Range")
        selection.AddShape(shape1)

        shape = self.word.get_shape(docobj, selection)

        assert shape.Name == u"rectangle 1", shape.Name

    def test_bad_doc_on_shutdown(self):
        self.word.ShutDown(None)

class TestAddSegment(object):
    def setup(self):
        self.word = Word.TransHistoryWord()

    def test_empty(self):
        holder = Word.StoryObject(d("FakeWord.Range"))
        selection = d("FakeWord.Range")
        selection.Start = selection.Range.Start = 1
        selection.End = selection.Range.End = 10
        unit = Word.TransUnit(1, u"a", u"b")
        segment = Word.SegmentObject(selection, unit)

        self.word.add_segment(holder, segment)
        assert len(holder.segments) == 1, holder.segments

    def test_story_exists(self):
        holder = Word.StoryObject(d("FakeWord.Range"))
        selection = d("FakeWord.Range")
        selection.Text = u"text"
        selection.Start = 1
        selection.End = 5
        unit = Word.TransUnit(1, u"a", u"b")
        segment = Word.SegmentObject(selection, unit)
        holder.segments = [segment]
        self.word.SetApp(d("FakeWord.App"))
        self.word.add_segment(holder, segment)
        assert len(holder.segments) == 1, holder.segments

    def test_shape_doesnt(self):
        holder = Word.ShapeObject(d("FakeWord.Shape"))
        sel1 = d("FakeWord.Range")
        sel1.Start = 1
        sel1.End = 10
        unit1 = Word.TransUnit(1, u"a", u"b")
        seg1 = Word.SegmentObject(sel1, unit1)
        holder.segments = [seg1]

        sel2 = d("FakeWord.Range")
        sel2.Start = 11
        sel2.End = 20
        unit2 = Word.TransUnit(2, u"a", u"b")
        seg2 = Word.SegmentObject(sel2, unit2)

        self.word.add_segment(holder, seg2)
        assert len(holder.segments) == 2, holder.segments

class TestGetStory(object):
    def setup(self):
        self.word = Word.TransHistoryWord()
        self.doc = FakeWord.WordDocumentFake()
        self.doc._stories[2] = d("FakeWord.Range")
        self.doc._stories[2].StoryType = 2
        self.docobj = Word.DocumentObject(d(util.wrap(self.doc)))
        self.sel = d("FakeWord.Range")
        self.sel.Start = 5
        self.sel.End = 10

    def test_empty(self):
        assert len(self.docobj.stories) == 0, self.docobj.stories
        self.sel.StoryType = 2
        story = self.word.get_story(self.docobj, self.sel)
        assert len(self.docobj.stories) == 1, self.docobj.stories
        assert story.StoryType == 2, story.StoryType

    def test_exists(self):
        story = d("FakeWord.Range")
        story.StoryType = 2
        self.docobj.stories = {2 : story}
        self.sel.StoryType = 2
        story = self.word.get_story(self.docobj, self.sel)
        assert len(self.docobj.stories) == 1, self.docobj.stories
        assert story.StoryType == 2, story.StoryType

    def test_doesnt(self):
        story = d("FakeWord.Range")
        story.StoryType = 1
        self.docobj.stories = {1 : story}
        self.sel.StoryType = 2
        story = self.word.get_story(self.docobj, self.sel)
        assert len(self.docobj.stories) == 2, self.docobj.stories
        assert story.StoryType == 2, story.StoryType
        assert self.sel.StoryType == 2, self.sel.StoryType


class TestRecordTrans(object):
    def setup(self):
        self.word = Word.TransHistoryWord()
        felix = d("FakeFelix.App")
        record = felix.App2.CurrentMatch.Record
        record.Id = 10
        record.Source = u"source"
        record.Trans = u"trans"
        self.word.SetFelix(felix)

        self.doc = FakeWord.WordDocumentFake()
        self.doc._name = u"fake document"
        self.doc._stories[1] = d("FakeWord.Range")
        self.doc._shapes[u"Text Box 1"] = d("FakeWord.Shape")
        self.word._history = [Word.DocumentObject(d(util.wrap(self.doc)))]

        self.word._history = [Word.DocumentObject(d(util.wrap(self.doc)))]

        self.word.SetApp(d("FakeWord.App"))

    def test_shape(self):
        docobj = Word.DocumentObject(d(util.wrap(self.doc)))
        selection = d("FakeWord.Range")
        shape = d("FakeWord.Shape")
        shape.Name = u"Text Box 1"
        shape.TextFrame.TextRange.Text = u"spam"
        shape.TextFrame.TextRange.Start = 3
        shape.TextFrame.TextRange.End = 13
        selection.AddShape(shape)

        selection.Text = u"spam"
        selection.SetTheRange(shape.TextFrame.TextRange)
        selection.Start = selection.Range.Start = 1
        selection.End = selection.Range.End = 10
        self.word.SetParser(d("FakeFelix.RangeToHtml"))
        self.word.record_shape(docobj, selection)
        docshape = docobj.shapes[u"Text Box 1"]
        assert len(docshape.segments) == 1, docshape.segments
        docseg = docshape.segments[0]
        assert docseg.segment.Start == 1, docseg
        assert docseg.segment.End == 10, docseg
        assert docseg.unit.recid == 10, docseg.unit.recid
        assert docseg.unit.source == u"source", docseg.unit.source
        assert docseg.unit.trans == u"spam", docseg.unit.trans

    def test_range(self):
        docobj = Word.DocumentObject(d(util.wrap(self.doc)))
        selection = d("FakeWord.Range")
        selection.Start = 5
        selection.End = 105
        selection.Text = u"betty"

        selection.Text = u"betty"
        self.word.SetParser(d("FakeFelix.RangeToHtml"))
        self.word.record_range(docobj, selection)
        story = docobj.stories[1]
        assert len(story.segments) == 1, story.segments
        docseg = story.segments[0]
        assert docseg.segment.Start == 5, docseg
        assert docseg.segment.End == 105, docseg
        assert docseg.unit.recid == 10, docseg.unit.recid
        assert docseg.unit.source == u"source", docseg.unit.source
        assert docseg.unit.trans == u"betty", docseg.unit.trans

    def test_RecordTrans_story(self):
        self.word._app.ActiveDocument = d(util.wrap(self.doc))

        selection = d("FakeWord.Range")
        selection.Start = 5
        selection.End = 105
        selection.Text = u"betty"

        selection.Text = u"betty"
        selection.Select()

        self.word.SetParser(d("FakeFelix.RangeToHtml"))
        self.word.RecordTrans()
        story = self.word._history[0].stories[1]
        assert len(story.segments) == 1, story.segments
        docseg = story.segments[0]
        assert docseg.segment.Start == 5, docseg
        assert docseg.segment.End == 105, docseg
        assert docseg.unit.recid == 10, docseg.unit.recid
        assert docseg.unit.source == u"source", docseg.unit.source
        assert docseg.unit.trans == u"betty", docseg.unit.trans

    def test_RecordTrans_shape(self):
        self.word._app.ActiveDocument = d(util.wrap(self.doc))

        selection = d("FakeWord.Range")
        shape = d("FakeWord.Shape")
        shape.Name = u"Text Box 1"
        shape.TextFrame.TextRange.Text = u"spam"
        shape.TextFrame.TextRange.Start = 3
        shape.TextFrame.TextRange.End = 13
        shape.TextFrame.TextRange.AddShape(shape)

        shape.TextFrame.TextRange.Select()

        self.word.SetParser(d("FakeFelix.RangeToHtml"))
        self.word.RecordTrans()
        shapes = self.word._history[0].shapes

        assert len(shapes) == 1, shapes
        docshape = shapes[u"Text Box 1"]
        assert len(docshape.segments) == 1, docshape.segments
        docseg = docshape.segments[0]
        assert docseg.segment.Start == 3, docseg
        assert docseg.segment.End == 13, docseg
        assert docseg.unit.recid == 10, docseg.unit.recid
        assert docseg.unit.source == u"source", docseg.unit.source
        assert docseg.unit.trans == u"spam", docseg.unit.trans

class TestLookupTrans(object):
    def setup(self):
        self.word = Word.TransHistoryWord()
        self.felix = FakeFelix.FelixAppFake()
        self.word.SetFelix(d(util.wrap(self.felix)))

        self.doc = FakeWord.WordDocumentFake()
        self.doc._name = u"fake document"
        self.doc._stories[1] = d("FakeWord.Range")
        self.doc._shapes[u"Text Box 1"] = d("FakeWord.Shape")

        self.word._history = [Word.DocumentObject(d(util.wrap(self.doc)))]

        self.word.SetApp(d("FakeWord.App"))
        self.word._app.ActiveDocument = d(util.wrap(self.doc))

    def add_story_seg(self, start, end, text, unit=None):
        sel = d("FakeWord.Range")
        sel.Start = start
        sel.End = end
        sel.Text = text

        doc = self.word._history[0]

        if not unit:
            unit = Word.TransUnit(1, u"source", u"trans")

        if not 1 in doc.stories:
            doc.stories[1] = Word.StoryObject(sel)
        doc.stories[1].segments.append(Word.SegmentObject(sel, unit))

    def test_lookup(self):
        unit = Word.TransUnit(10, u"lookup source", u"lookup trans")
        self.add_story_seg(1, 10, u"selection text", unit)
        sel = d("FakeWord.Range")
        sel.Start = 1
        sel.End = 10
        sel.Text = u"new sel text"
        sel.StoryType = 1
        sel.Select()

        app2 = FakeFelix.FelixApp2Fake()
        self.felix._app2 = d(util.wrap(app2))

        parser = d("FakeFelix.RangeToHtml")
        self.word.SetParser(parser)
        self.word.LookupTrans()
        actual = app2._review
        assert actual == [(10, u"lookup source", u"new sel text")], actual

    def test_lookup_empty(self):
        sel = d("FakeWord.Range")
        sel.Start = 1
        sel.End = 10
        sel.Text = u"new sel text"
        sel.Select()

        parser = d("FakeFelix.RangeToHtml")
        self.word.SetParser(parser)
        self.word.LookupTrans()
        actual = self.felix._lookup_trans
        assert actual == [u"new sel text"], actual

class TestCorrectTrans(object):

    def setup(self):
        self.word = Word.TransHistoryWord()
        self.felix = FakeFelix.FelixAppFake()
        self.word.SetFelix(d(util.wrap(self.felix)))

        self.doc = FakeWord.WordDocumentFake()
        self.doc._name = u"fake document"
        self.doc._stories[1] = d("FakeWord.Range")
        self.doc._shapes[u"Text Box 1"] = d("FakeWord.Shape")

        self.word._history = [Word.DocumentObject(d(util.wrap(self.doc)))]

        self.word.SetApp(d("FakeWord.App"))
        self.word._app.ActiveDocument = d(util.wrap(self.doc))

    def add_story_seg(self, start, end, text, unit=None):
        sel = d("FakeWord.Range")
        sel.Range.Start = start
        sel.Range.End = end
        sel.Range.Text = text
        self.Text = text

        doc = self.word._history[0]

        if not unit:
            unit = Word.TransUnit(1, u"source", u"trans")

        if not 1 in doc.stories:
            doc.stories[1] = Word.StoryObject(sel)
        doc.stories[1].segments.append(Word.SegmentObject(sel, unit))

    def test_correct(self):
        unit = Word.TransUnit(10, u"lookup source", u"lookup trans")
        self.add_story_seg(1, 10, u"selection text", unit)
        sel = d("FakeWord.Range")
        sel.Range.Start = 1
        sel.Range.End = 10
        sel.Range.Text = u"new sel text"
        sel.Text = u"new sel text"
        sel.StoryType = 1
        sel.Select()

        app2 = FakeFelix.FelixApp2Fake()
        self.felix._app2 = d(util.wrap(app2))

        parser = d("FakeFelix.RangeToHtml")
        self.word.SetParser(parser)
        self.word.CorrectTrans()
        actual = app2._changes
        assert actual == [(10, u"lookup source", u"new sel text")], actual

    def test_correct_empty(self):
        sel = d("FakeWord.Range")
        sel.Start = 1
        sel.End = 10
        sel.Text = u"new sel text"
        self.word._app.Selection = sel

        self.word.SetParser(d("FakeFelix.RangeToHtml"))
        self.word.CorrectTrans()
        actual = self.felix._correct_trans
        assert actual == [u"new sel text"], actual

class TestReflectChanges(object):

    def setup(self):
        self.word = Word.TransHistoryWord()
        self.felix = FakeFelix.FelixAppFake()
        self.word.SetFelix(d(util.wrap(self.felix)))

        self.doc = FakeWord.WordDocumentFake()
        self.doc._name = u"fake document"
        self.doc._stories[1] = d("FakeWord.Range")
        self.doc._shapes[u"Text Box 1"] = d("FakeWord.Shape")

        comdoc = d(util.wrap(self.doc))
        docobj = Word.DocumentObject(comdoc)
        storyrange = d("FakeWord.Range")
        storyrange.StoryType = 1
        story = Word.StoryObject(storyrange)

        unit1 = Word.TransUnit(1, u"source 1", u"trans 1")
        sel1 = d("FakeWord.Range")
        sel1.Text = u"corrected 1"
        sel1.Start = 1
        sel1.End = 10
        story.segments.append(Word.SegmentObject(sel1, unit1))

        unit2 = Word.TransUnit(2, u"source 2", u"trans 2")
        sel2 = d("FakeWord.Range")
        sel2.Text = u"corrected 2"
        sel2.Start = 11
        sel2.End = 20
        story.segments.append(Word.SegmentObject(sel2, unit2))

        docobj.stories[1] = story

        shape = d("FakeWord.Shape")
        shape.Name = u"Text Box 1"
        shapeobj = Word.ShapeObject(shape)

        unit3 = Word.TransUnit(3, u"source 3", u"trans 3")
        sel3 = d("FakeWord.Range")
        sel3.Text = u"corrected 3"
        sel3.Start = 1
        sel3.End = 20
        shapeobj.segments.append(Word.SegmentObject(sel3, unit3))

        docobj.shapes[u"Text Box 1"] = shapeobj

        self.word._history = [docobj]

        self.word.SetApp(d("FakeWord.App"))
        self.word._app.ActiveDocument = comdoc
        self.word._app.Selection = d("FakeWord.Range")
        self.word._app.Selection.Text = sel1.Text
        self.word._app.Selection.Text = sel2.Text
        self.word._app.Selection.Text = sel3.Text
        self.word.SetParser(d("FakeFelix.RangeToHtml"))

    def test_reflect_segment(self):
        selection = d("FakeWord.Range")
        selection.Text = u"selected text"
        unit = Word.TransUnit(10, u"foo", u"bar")
        seg = Word.SegmentObject(selection, unit)
        foo = []
        reflect = lambda x, y, z : foo.append((x, y, z))
        self.word.reflect_segment(selection, seg, reflect)
        assert foo == [(10, u"foo", u"selected text")], foo

    def test_reflect_segments(self):
        app2 = FakeFelix.FelixApp2Fake()
        self.felix._app2 = d(util.wrap(app2))

        parser = d("FakeFelix.RangeToHtml")
        self.word.SetParser(parser)
        self.word.ReflectChanges()
        print app2._changes
        a1, a2, a3 = sorted(app2._changes)
        assert a1 == (1, u"source 1", u"corrected 2"), a1
        assert a2 == (2, u"source 2", u"corrected 1"), a2
        assert a3 == (3, u"source 3", u"corrected 3"), a3

    @raises(KeyError)
    def test_reflect_error(self):
        selection = d("FakeWord.Range")
        selection.Text = u"selected text"
        unit = Word.TransUnit(10, u"foo", u"bar")
        seg = Word.SegmentObject(selection, unit)
        self.word.reflect_segment(selection,
                                  seg,
                                  lambda a, b, c : {}["foo"])

    def test_shutdown(self):
        if os.path.exists(r"c:\test\test.doc.fhist"):
            os.remove(r"c:\test\test.doc.fhist")
        doc = self.word._history[0]
        doc.document.FullName = r"c:\test\test.doc"
        self.word.ShutDown(doc.document)
        assert not self.word._history, self.word._history
        assert os.path.exists(r"c:\test\test.doc.fhist")

#def test_range2html():
#    word = d("Word.Application")
#    word.Visible = True
#    doc = word.ActiveDocument
#
#    parser = d("WordAssist.Word2HtmlParser")
#    text = parser.RangeToHtml(word.Selection)
#    print text
#    assert text == u"<FONT FACE=\"Calibri\">foo</FONT>", text
