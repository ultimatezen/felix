#coding: UTF8
"""
Translation History COM Server
"""
from win32com.server import util
from win32com.client import Dispatch
import win32com.server.register
import pythoncom
from cPickle import load, dump
import sys
import os
import utils
from utils import debug, warn, error, log_err
from utils import TransUnit
from utils import serialized_source
from utils import FelixObject

__version__ = "0.2"
__progname__ = "Felix Word translation history"

WORD_STORY_SHAPE = 5

def unserialize_seg(selection, segdata):
    segment = SegmentObject(selection, segdata["unit"])
    start, end = segdata["pos"]
    segment.segment.SetRange(start, end)
    segment.guard.SetRange(end, end+1)
    return segment

def unserialize_segments(selection, segdata):
    return [unserialize_seg(selection, seg) for seg in segdata]

def unserialize_shape(document, shapedata):
    """
    Unserialize the shape with the given name.
    If the unserialize operation failes, return
    None.
    """
    try:
        name = u"Undefined"
        name = shapedata["name"]
        shape = document.Shapes(name)
        shape.TextFrame.TextRange.Select()
        shape_obj = ShapeObject(shape)
        shape_obj.segments = unserialize_segments(shape.Application.Selection,
                                                  shapedata["segments"])
    
        return shape_obj
    except:
        LOGGER.exception(u"Failed to unserialize shape [%s]" % name)
        return None

def unserialize_shapes(document, shapedata):
    """
    Unserialize the shapes from the pickled file.
    If the unserialize_shape() function returns None,
    don't add that shape to our dict.
    """

    shapes = {}
    for key, val in shapedata.iteritems():
        shape = unserialize_shape(document, val)
        if shape:
            shapes[key] = shape
    return shapes

def unserialize_story(document, story):
    comstory = document.StoryRanges(story["story_type"])
    comstory.Select()
    story_obj = StoryObject(comstory)
    story_obj.segments = unserialize_segments(comstory.Application.Selection,
                                              story["segments"])
    return story_obj

def load_data(document, datasource):
    """
    Load the data from the data source.
    Synchronize the sheets in the data with the worksheets in the workbook.
    """

    try:
        original_range = document.Application.Selection.Range
    
        data = load(datasource)
        
        stories = data["stories"]
        shapes = data["shapes"]
        
        doc_obj = DocumentObject(document)
        doc_obj.shapes = unserialize_shapes(document, shapes)
        for story in data["stories"]:
            story_type = story["story_type"]
            doc_obj.stories[story_type] = unserialize_story(document, story)
        original_range.Select()
        return doc_obj
    except:
        error("Failed to load serialized data")
        raise

def serialize_segment(segment):
    segment.expand()
    segrange = segment.segment
    return dict(pos=(segrange.Start, segrange.End), unit=segment.unit)

def valid_segment(segment):
    return segment.segment.Start < segment.segment.End

def serialize_segments(segments):
    return [serialize_segment(seg)
            for seg in segments
            if valid_segment(seg)]

def serialize_shape(shape):
    """Create a dictionary from the shape for serialization"""

    name = shape.Name
    return dict(name=name,
                segments=serialize_segments(shape.segments))

def serialize_story(story):
    """Create a dictionary from the story for serialization"""

    story_type = story.StoryType
    debug(u"Serializing story: %s" % story_type)
    return dict(story_type=story_type,
                segments=serialize_segments(story.segments))

def make_serialization_object(docobj):
    """Get the filename and data for serialization"""

    try:
        filename = serialized_source(docobj)
        stories = [serialize_story(story) for key, story in docobj.stories.items()]
        shapes = dict([(shape.Name, serialize_shape(shape))
            for key, shape in docobj.shapes.items()])
        return filename, dict(stories=stories, shapes=shapes)
    except:
        error("Failed to serialize document data")
        raise

class SegmentObject(object):
    def __init__(self, selection, unit):
        self.segment = None
        self.guard = None
        self.set_segrange(selection)
        self.unit = unit
    def set_segrange(self, selection):
        self.segment = selection.Range
        self.guard = selection.Range
        start = self.guard.End
        self.guard.SetRange(start, start+1)
    def expand(self):
        end = self.guard.End-1
        segend = self.segment.End
        if segend != end:
            debug(u"Expanding segment end from %s to %s" % (segend, end))
            self.segment.End = end
            self.guard.Start = end
    def __repr__(self):
        return u"<SegmentObject (%s, %s): %s>" % (self.segment.Start,
                                                  self.segment.End,
                                                  self.unit)

def make_segment(felix, selection, text):
    record = felix.get_record()
    unit = TransUnit(record.Id, record.Source, text)
    return SegmentObject(selection, unit)

class ShapeObject(object):
    def __init__(self, shape):
        self.shape = shape
        self.segments = []
    def _get_name(self):
        return self.shape.Name
    Name = property(_get_name)
    
    def __repr__(self):
        return u"<ShapeObject (%s): %s>" % (self.Name, self.segments)

class StoryObject(object):
    """Wraps a Word story object"""
    
    def __init__(self, story):
        app = story.Application
        doc = app.ActiveDocument
        story_type = story.StoryType
        the_story = doc.StoryRanges(story_type)
        self.story = the_story
        self.segments = []
    def _get_storytype(self):
        return self.story.StoryType
    StoryType = property(_get_storytype)
    
    def __repr__(self):
        return u"<StoryObject (%s): %s segments>" % (self.StoryType,
                                                    len(self.segments))

class DocumentObject(object):
    """Wraps a document object from Word"""

    def __init__(self, document):
        self.document = document
        self.stories = {}
        self.shapes = {}
    def _get_fullname(self):
        return self.document.FullName
    FullName = property(_get_fullname)

def segments_equal(segment_range, selection_range):
    if segment_range.Start != selection_range.Start:
        return False
    return segment_range.End >= selection_range.End

def fit_segment(segments, segment):
    start = segment.segment.Start
    end = segment.segment.End
    
    for seg in segments:
        segrange = seg.segment
        segstart, segend = segrange.Start, segrange.End
        if segstart < end <= segend:
            if not (start, end) == (segstart, segend):
                segrange.Start = end
        elif segstart < start <= segend:
            segrange.End = start
            seg.guard.SetRange(start, start+1)
    segments.append(segment)

class TransHistoryWord(object):
    """Translation history server for Word files"""
    
    _public_methods_ = ["RecordTrans",
                        "ShutDown",
                        "LookupTrans",
                        "ReflectChanges",
                        "CorrectTrans",
                        "CorrectTransFixed",
                        "OpenDoc"]
    _public_attrs_ = ['App', 'Felix', 'Parser']
    _reg_progid_ = "FelixTransHistory.Word"
    
    # import pythoncom
    # print pythoncom.CreateGuid()
    _reg_clsid_ = "{D967E6DC-3F23-4331-B481-AF803988E249}"

    def __init__(self):
        self._app = None
        self._felix = None
        self._parser = None
        
        self._history = []
        
        debug("FelixTransHistory.Word")
    
    def get_felix(self):
        """
        Get the Felix COM server.
        Create it if it doesn't exist.
        """
        if not self._felix:
            self._felix = FelixObject()
        return self._felix

    def get_document(self, active_doc):
        """
        Get the DocumentObject for the
        active document. Add it if it doesn't
        already exist.
        """
        name = active_doc.FullName
        for _doc in self._history:
            if _doc.FullName == name:
                return _doc

        if os.path.exists(serialized_source(active_doc)):
            datasource = open(serialized_source(active_doc))
            self._history.append(load_data(active_doc, datasource))
        else:
            self._history.append(DocumentObject(active_doc))
        
        return self._history[-1]

    def Parser(self):
        """Get the parser COM server."""
        return self._parser
    def SetParser(self, parser):
        """Set the parser COM server."""
        try:
            self._parser = Dispatch(parser)
        except:
            LOGGER.exception("Error setting parser")
            raise

    def App(self):
        """Get the Word application"""
        return self._app
    def SetApp(self, app):
        """Set the Word application"""
        debug("Set Word application")
        try:
            self._app = Dispatch(app)
        except:
            LOGGER.exception("Error setting Word application")
            raise

    def Felix(self):
        """Get the Felix application"""
        self._felix.ensure_felix()
        return self._felix._felix
    def SetFelix(self, felix):
        """
        Set the Felix application.
        Create if it hasn't been set.
        """
        try:
            if not felix:
                self._felix = FelixObject()
            else:
                self._felix = FelixObject(Dispatch(felix))
        except:
            LOGGER.exception("Error setting Felix application")
            raise

    def get_shape(self, doc, selection):
        """
        Get the shape of the current selection.
        Assumes that ShapeRange is non-empty.
        """
        shape = selection.ShapeRange[0]
        
        try:
            return doc.shapes[shape.Name]
        except KeyError:
            shapeobj = ShapeObject(shape)
            doc.shapes[shape.Name] = shapeobj
            return shapeobj

    def add_segment(self, holder, segment):
        """
        Add the segment to the holder (either story or shape).
        If the segment already exists, then update the translation unit.
        """

        for seg in holder.segments:
            if segments_equal(segment.segment, seg.segment):
                old_sel = self._app.Selection
                seg.set_segrange(self._app.Selection)
                seg.unit = segment.unit
                old_sel.Select()
                return
        fit_segment(holder.segments, segment)

    def get_story(self, doc, selection):
        """
        Get the story from the active document.
        Add it if it doesn't exist.
        """
        story_type = selection.StoryType
        if story_type not in doc.stories:
            doc.stories[story_type] = StoryObject(doc.document.StoryRanges(story_type))
        
        return doc.stories[story_type]

    def record_shape(self, doc, selection):
        """
        Record the translation for a segment in a shape.
        """

        shape = self.get_shape(doc, selection)
        self.add_segment(shape, make_segment(self.get_felix(),
                                             selection,
                                             self.parse_text(selection)))

    def record_range(self, doc, selection):

        story = self.get_story(doc, selection)
        self.add_segment(story, make_segment(self.get_felix(),
                                             selection,
                                             self.parse_text(selection)))

    def RecordTrans(self):
        """
        Record a translation in the history
        """
        try:
            doc = self.get_document(self._app.ActiveDocument)
            selection = Dispatch(self._app.Selection)
    
            if selection.StoryType == WORD_STORY_SHAPE:
                self.record_shape(doc, selection)
            else:
                self.record_range(doc, selection)
        except:
            LOGGER.exception("Error recording translation")
            raise

    def get_segments(self, doc, selection):
        if selection.StoryType == WORD_STORY_SHAPE:
            shape = self.get_shape(doc, selection)
            return shape.segments  
        else:
            story = self.get_story(doc, selection)
            return story.segments

    def LookupTrans(self):
        try:
            doc = self.get_document(self._app.ActiveDocument)
            selection = Dispatch(self._app.Selection)
    
            felix = self.get_felix()
    
            seg = self.find_segment(doc, selection)
            if seg:
                reflect = felix.ReviewTranslation
                self.reflect_segment(Dispatch(self._app.Selection),
                                     seg,
                                     reflect)
                return
            
            felix.LookupTrans(self.parse_text(selection))
        except:
            LOGGER.exception("Error looking up translation")
            raise

    def find_segment(self, doc, selection):
        """
        See if we have a segment in our history matching the
        segment being looked up.
        Allow for the possibility that our segment is actually longer
        than the selection (like maybe we had two translation sentences
        for one source segment)
        """

        selrange = selection.Range
        for seg in self.get_segments(doc, selection):
            if segments_equal(seg.segment, selrange):
                seg.expand()
                if seg.segment.End > selrange.End:
                    seg.segment.Select()
                else:
                    seg.set_segrange(selection)
                return seg
        return None

    def CorrectTrans(self):
        try:
            doc = self.get_document(self._app.ActiveDocument)
            selection = Dispatch(self._app.Selection)
    
            felix = self.get_felix()
    
            seg = self.find_segment(doc, selection)
            if seg:
                reflect = felix.ReflectChanges
                self.reflect_segment(selection, seg, reflect)
                return

            felix.CorrectTrans(self.parse_text(selection))
            self.RecordTrans()
        except:
            LOGGER.exception("Error correcting translation")
            raise

    def parse_text(self, selection):
        try:
            return self._parser.RangeToHtml(selection)
        except:
            LOGGER.exception(u"Failed to parse Word range to HTML")
            return selection.Text

    def CorrectTransFixed(self):
        try:
            doc = self.get_document(self._app.ActiveDocument)
            selection = Dispatch(self._app.Selection)
    
            felix = self.get_felix()
    
            selrange = selection.Range
            selpos = selrange.Start, selrange.End
            for seg in self.get_segments(doc, selection):
                segment = seg.segment
                if (segment.Start, segment.End) == selpos:
                    reflect = felix.ReflectChanges
                    self.reflect_segment(selection, seg, reflect)
                    return
    
            felix.CorrectTrans(self.parse_text(selection))
            self.RecordTrans()
        except:
            LOGGER.exception("Error correcting translation")
            raise
    
    def ReflectChanges(self):
        """
        Reflect changes to the document in the translation memory
        """

        try:
            docname = self._app.ActiveDocument.FullName
            debug(u"Reflecting changes in Word document %s" % docname)
    
            doc = self.get_document(self._app.ActiveDocument)
            selection = Dispatch(self._app.Selection)
    
            start, end = selection.Start, selection.End
            
            felix = self.get_felix()
            reflect = felix.ReflectChanges
            
            debug("Reflecting shape translations")
            for key, shape in doc.shapes.items():
                self.reflect_segments(selection, shape.segments, reflect)
            
            for key, story in doc.stories.items():
                debug("uReflecting translations for story %s" % story.StoryType)
                self.reflect_segments(selection, story.segments, reflect)
        except:
            LOGGER.exception("Error correcting translation")
            raise

    def reflect_segments(self, selection, segments, reflect):
        """
        Expand to the guard. This ensures that if we added text to the
        end, that we capture it.
        Next, we make sure that we didn't step on any other toes.
        """

        end = 0
        segments = sorted([(seg.segment.Start, seg) for seg in segments])
        for start, segment in segments:
            segment.expand()
            segrange = segment.segment
            if start < end:
                segrange.Start = end
            segrange.Select()
            this_end = segrange.End
            if end < this_end:
                self.reflect_segment(selection, segment, reflect)
            end = max(end, this_end)
        
    def reflect_segment(self, selection, segment, reflect):
        seg = segment.segment
        seg.Select()
        unit = segment.unit
        reflect(unit.recid,
                unit.source,
                self.parse_text(selection))

    def OpenDoc(self, docdisp):
        try:
            debug("Document opening")
            try:
                comdoc = Dispatch(docdisp)
                name = comdoc.FullName
            except:
                # no harm, no foul
                LOGGER.exception("Failed to retrieve document object")
                return
            
            # if it's already loaded, don't do anything
            for _doc in self._history:
                if _doc.FullName == name:
                    return
    
            debug(" ... Document history not loaded. Checking for file.")
            # it's not loaded, and there's a translation history
            if os.path.exists(serialized_source(comdoc)):
                debug(" ... Loading document history")
                datasource = open(serialized_source(comdoc))
                self._history.append(load_data(comdoc, datasource))
            
        except:
            LOGGER.exception("Error handling document open event")
            raise

    def ShutDown(self, docdisp):
        try:
            debug("Document closing")
            try:
                comdoc = Dispatch(docdisp)
                name = comdoc.FullName
            except:
                # no harm, no foul
                LOGGER.exception("Failed to retrieve document object")
                return

            for doc in self._history:
                if name == doc.FullName:
                    debug(u"Serializing history for %s" % doc.FullName)
                    filename, data = make_serialization_object(doc)
                    dump(data, file(filename, "w"))
                    # We've got to remove the document
                    self.remove_doc(name)
                    return
            
            # We've got to remove the document
            self.remove_doc(name)
    
            debug("No history for document")
        except:
            LOGGER.exception("Error shutting down")
            raise
    
    def remove_doc(self, name):
        self._history = [doc for doc in self._history
                         if name != doc.FullName]

def reg():    
    """Register COM servers"""
    
    debug("Registering COM servers")
    win32com.server.register.UseCommandLine(TransHistoryWord)

utils.determine_redirect("history_word.log")

# Add code so that when this script is run by
# Python.exe, it self-registers.
if __name__ == '__main__':
    reg()