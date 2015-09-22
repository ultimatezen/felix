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
import datetime

# system stuff
from win32api import GetUserName
import sys
import os

import datetime
import time


from loc import (get_local_app_data_folder,
                 we_are_frozen,
                 get_log_filename)


try:
    DEFAULT_USER = unicode(GetUserName(), sys.getfilesystemencoding())
except:
    DEFAULT_USER = u"Default"

__version__ = "1.0"
__author__ = "Ryan Ginstrom"
__license__ = "MIT"
__progname__ = "Felix Memory Engine"


def get_now():
    return datetime.datetime(*time.localtime()[:6]).strftime("%Y/%m/%d %H:%M:%S")

def make_date(value):
    if isinstance(value, basestring):
        return value.replace("-", "/")
    try:
        return value.strftime("%Y/%m/%d %H:%M:%S")
    except:
        return get_now()

def default_data():
    return dict(source=u"",
                trans=u"",
                context=u"",
                reliability=0,
                validated=False,
                ref_count=0,
                date_created=get_now(),
                last_modified=get_now(),
                created_by=DEFAULT_USER,
                modified_by=DEFAULT_USER)

def record2dict(record):
    return dict(source=record.Source.encode("utf-8"),
                trans=record.Trans.encode("utf-8"),
                context=record.Context.encode("utf-8"),
                reliability=record.Reliability,
                validated=record.Validated,
                ref_count=record.RefCount,
                date_created=record.Created,
                last_modified=record.Modified,
                created_by=record.CreatedBy.encode("utf-8"),
                modified_by=record.ModifiedBy.encode("utf-8"),
                id=record.Id)

def massage_dict(values):
    data = {}
    data.update(values)
    for key in "source trans context created_by modified_by".split():
        if isinstance(data[key], unicode):
            data[key] = data[key].encode("utf-8")

    for key in "source_cmp trans_cmp".split():
        if key in data:
            del data[key]

    return data

class FelixRemoteRecordFake(object):
    """A Felix record"""

    _public_methods_ = []
    _public_attrs_ = ['Source',
                      'Trans',
                      'Context',
                      'Validated',
                      'Reliability',
                      'Created',
                      'Modified',
                      'RefCount',
                      'CreatedBy',
                      'ModifiedBy',
                      'Id']

    _reg_progid_ = "Felix.RemoteRecordFake"
    _reg_clsid_ = "{EDD78DE5-4773-4F5C-897B-32205EBE951A}"

    def __init__(self, data=None, commands=None):
        """Data will be the values we get from MemoryServes"""
        self.data = default_data()
        if data:
            self.data.update(data)

    #id
    def Id(self):
        try:
            return self.data["id"]
        except KeyError:
            return 0
    def SetId(self, newid):
        self.data["id"] = newid

    # source
    def Source(self):
        return self.data["source"]
    def SetSource(self, source):
        self.data["modified_by"] = DEFAULT_USER
        self.data["source"] = source

    # trans
    def Trans(self):
        return self.data["trans"]
    def SetTrans(self, trans):
        self.data["modified_by"] = DEFAULT_USER
        self.data["trans"] = trans

    # context
    def Context(self):
        return self.data["context"]
    def SetContext(self, context):
        self.data["modified_by"] = DEFAULT_USER
        self.data["context"] = context

    # created/modified
    def Created(self):
        return make_date(self.data["date_created"])
    def SetCreated(self, created):
        self.data["date_created"] = created

    def Modified(self):
        return make_date(self.data["last_modified"])
    def SetModified(self, modified):
        self.data["modified_by"] = DEFAULT_USER
        self.data["last_modified"] = modified

    def CreatedBy(self):
        return self.data["created_by"]
    def SetCreatedBy(self, created_by):
        self.data["created_by"] = created_by
    def ModifiedBy(self):
        return self.data["modified_by"]
    def SetModifiedBy(self, modified_by):
        self.data["modified_by"] = modified_by

    # reliability
    def Reliability(self):
        return self.data["reliability"]
    def SetReliability(self, reliability):
        self.data["modified_by"] = DEFAULT_USER
        self.data["reliability"] = max(0, min(9, int(reliability)))

    # validated
    def Validated(self):
        return self.data["validated"]
    def SetValidated(self, validated):
        self.data["modified_by"] = DEFAULT_USER
        self.data["validated"] = bool(validated)

    # ref count
    def RefCount(self):
        return self.data["ref_count"]
    def SetRefCount(self, ref_count):
        self.data["modified_by"] = DEFAULT_USER
        self.data["ref_count"] = ref_count

class FelixRemoteMemoryFake(object):
    """Interface for Memory Serves"""

    _public_methods_ = """AddRecord DeleteRecord
                    AddHit
                    Create Delete
                    Connect
                    SetInfo GetInfo
                    Search ReverseSearch
                    Concordance ReverseConcordance
                    Gloss RecordById
                    UpdateRecord""".split()
    _reg_progid_ = "Felix.RemoteMemoryFake"

    # import pythoncom
    # print pythoncom.CreateGuid()
    _reg_clsid_ = "{6CF6A90D-0705-4117-8481-EA6F6F487977}"

    def __init__(self):
        self.records = []
        self.hits = []
        self.info = dict(modified_by=u"Taro Mod",
                         name=u"Fake Memory",
                         creator=u"Taro Creator",
                         notes=u"This is a fake memory",
                         source_language=u"English",
                         normalize_case=True,
                         normalize_hira=True,
                         created_on=datetime.datetime(1999,
                                                      11,
                                                      12,
                                                      11,
                                                      22,
                                                      33),
                         client=u"Felix Taro",
                         normalize_width=False,
                         modified_on=datetime.datetime(2005,
                                                       11,
                                                       22,
                                                       11,
                                                       22,
                                                       33),
                         memtype=u"m",
                         size=0,
                         id=u"Felix Taro",
                         target_language=u"Japanese",)
        self.next_id = 1

    # Add and remove records
    def AddRecord(self, record):

        rec = Dispatch(record)
        rec.Id = self.next_id
        self.next_id += 1
        self.records.append(rec)
        self.info["size"] = len(self.records)
        return True

    def UpdateRecord(self, record):

        updated_rec = Dispatch(record)
        for i, rec in enumerate(self.records):
            if rec.Id == updated_rec.Id:
                self.records[i] = updated_rec
                return True
        return False

    def RecordById(self, id):
        for record in self.records:
            if record.Id == id:
                return record
        return None

    def AddHit(self, record):

        rec = Dispatch(record)
        self.hits.append(rec)
        return True

    def DeleteRecord(self, record):

        rec = Dispatch(record)
        self.records = [x for x in self.records if x.Id != rec.Id]
        self.info["size"] = len(self.records)
        return True

    def Connect(self, connection):
        if u"http://" not in connection:
            return False
        return True

    def SetInfo(self, key, val):
        self.info[key] = val
    def GetInfo(self, key):
        return self.info[key]

    # search
    def Search(self, query, minscore):

        return util.NewCollection(self.hits)

    def ReverseSearch(self, query, minscore):

        return util.NewCollection(self.hits)

    # concordance
    def Concordance(self, query):

        return util.NewCollection(self.hits)

    def ReverseConcordance(self, query):

        return util.NewCollection(self.hits)

    # glossary searches
    def Gloss(self, query, minscore):

        return util.NewCollection(self.hits)

    # These are for debugging
    def Create(self, name, type):
        pass

    def Delete(self):
        pass

def reg():
    """
    Register COM servers
    """

    print "Registering COM servers"
    win32com.server.register.UseCommandLine(FelixRemoteMemoryFake)
    win32com.server.register.UseCommandLine(FelixRemoteRecordFake)

# Add code so that when this script is run by
# Python.exe, it self-registers.
if __name__ == '__main__':
    reg()
