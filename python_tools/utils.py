#!/usr/bin/env python
from loc import (we_are_frozen,
                 module_path)
import os
import datetime
import time
import loc
import traceback
import sys
from win32com.client import Dispatch

logfile = sys.stdout


def determine_redirect(filename):
    """
    Determine where to redirect stdout
    """
    global logfile
    try:
        logfile = open(get_log_filename(filename), "w")
    except:
        class DevNull(object):
            def __getattr__(self, attr):
                return lambda *kwds, **kwargs : None
        logfile = DevNull()


def get_now():
    return datetime.datetime(*time.localtime()[:6]).strftime("%Y-%m-%d %H:%M:%S")


def log(severity, msg):
    if isinstance(msg, unicode):
        msg = msg.encode("utf-8")
    logfile.write("%s\t%s\t%s\n" % (severity, get_now(), msg))
    logfile.flush()


def debug(msg):
    log("INFO", msg)


def warn(msg):
    log("WARN", msg)


def error(msg):
    log("ERROR", msg)


def log_err(msg = None):
    """
    Log an error from a traceback.
    We have to do a repr, because we don't know what
    the encoding of the traceback will be.
    (Is that true?)
    """
    if msg:
        error(msg)
    lines = []
    for line in traceback.format_exc().splitlines():
        try:
            lines.append(line.decode(sys.getfilesystemencoding()).encode("utf-8"))
        except:
            lines.append(repr(line))
    error("\n".join(lines))


def serialized_source(workbook):
    """
    Get the filename for a history file
    """

    return workbook.FullName + u".fhist"


def get_local_app_data_folder():
    """
    This is the folder where we will save the log
    """

    appdata_folder = loc.get_local_appdata()
    return os.path.join(appdata_folder, u"Felix")


def get_log_filename(filename):
    """
    Get the filename for logging.
    """
    if we_are_frozen():
        basepath = get_local_app_data_folder()
    else:
        basepath = module_path()

    basepath = os.path.join(basepath, u'logs')

    if not os.path.isdir(basepath):
        os.makedirs(basepath)
    return os.path.join(basepath, filename)


class FelixObject(object):
    def __init__(self, felix=None):
        self._felix = None
        self.App2 = None

        if felix:
            self._felix = felix
        else:
            self.init_felix()

    def init_felix(self):
        self._felix = Dispatch("Felix.App")
        self.App2 = self._felix.App2

    def ensure_felix(self):
        try:
            self._felix.Visible = True
        except:
            self.init_felix()
            self._felix.Visible = True

    def get_record(self):
        self.ensure_felix()
        return self._felix.App2.CurrentMatch.Record

    def ReviewTranslation(self, recid, source, trans):
        self.ensure_felix()
        return self._felix.App2.ReviewTranslation(recid, source, trans)

    def ReflectChanges(self, recid, source, trans):
        self.ensure_felix()
        return self._felix.App2.ReflectChanges(recid, source, trans)

    def LookupTrans(self, trans):
        self.ensure_felix()
        return self._felix.LookupTrans(trans)

    def CorrectTrans(self, trans):
        self.ensure_felix()
        return self._felix.CorrectTrans(trans)


class TransUnit(object):
    """
    A translation unit
    """

    def __init__(self, recid, source, trans):
        self.recid = recid
        self.source = source
        self.trans = trans

    def __repr__(self):
        return u"<TransUnit (%d): %s - %s>" % (self.recid,
                                               self.source,
                                               self.trans)
