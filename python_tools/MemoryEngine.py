#coding: UTF8
"""
COM sever for interfacing with Memory Serves memory

"""
# COM stuff
from win32com.server import util
from win32com.client import Dispatch
import win32com.server.register
from urlparse import urlparse
# system stuff
from win32api import GetUserName
import sys
import logging

# HTTP communication stuff
import urllib2
import urllib
import cPickle
import datetime
import time
import socket
import utils
from utils import debug, warn, error

__version__ = "1.1"
__author__ = "Ryan Ginstrom"
__license__ = "MIT"
__progname__ = "Felix Memory Engine"

HOST = socket.gethostbyname(socket.gethostname())
PORT = 8765

INFO_REFRESH_INTERVAL = 10  # 10 seconds

BASE_URL = "http://%s:%s" % (HOST, PORT)

logger = logging.getLogger(__name__)


class Globals(object):
    """
    Global info for Felix COM interface
    """
    USER_NAME = u"Default"

# set the user name to the computer's user name by default
try:
    Globals.USER_NAME = unicode(GetUserName(), sys.getfilesystemencoding())
except:
    logger.exception("Failed to retrieve user name")


def get_now():
    """
    For time-stamping purposes
    """

    format = "%Y/%m/%d %H:%M:%S"
    return datetime.datetime(*time.localtime()[:6]).strftime(format)


def make_date(value):
    """
    Change from Felix date format to Python date format
    """

    if isinstance(value, basestring):
        return value.replace("-", "/")
    try:
        return value.strftime("%Y/%m/%d %H:%M:%S")
    except:
        return get_now()


def default_data():
    """
    The default data for a record
    """
    return dict(source=u"",
                trans=u"",
                context=u"",
                reliability=0,
                validated=False,
                ref_count=0,
                date_created=get_now(),
                last_modified=get_now(),
                created_by=Globals.USER_NAME,
                modified_by=Globals.USER_NAME)


def record2dict(record):
    """
    Make the record into a dict for passing to server
    """
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
    """
    Change unicode values to ascii so the server won't complain.
    Also delete the source_cmp and trans_cmp fields.
    """

    data = {}
    data.update(values)
    for key in "source trans context created_by modified_by".split():
        if isinstance(values[key], unicode):
            data[key] = data[key].encode("utf-8")

    for key in "source_cmp trans_cmp".split():
        if key in data:
            del data[key]

    return data


def get_url_data(req):
    """
    Retrieve the data from a request object as python
    """
    return cPickle.loads(urllib2.urlopen(req).read())


def url_retrieve(command, values=None):
    """
    Retrieve python data from a URL.
    """
    values = values or {}
    req = urllib2.Request(command, urllib.urlencode(values))
    return get_url_data(req)


def within_refresh_interval(now, last_info_pull):
    """
    This is so we don't flood the server with overly frequent HTTP traffic
    """

    return now - last_info_pull < INFO_REFRESH_INTERVAL


class FelixRemoteRecord(object):
    """
    A Felix record
    COM server as Felix.RemoteRecord
    """

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

    _reg_progid_ = "Felix.RemoteRecord"
    _reg_clsid_ = "{C2F3EF94-8680-470F-B10D-1C81B4515BE4}"

    def __init__(self, token=None, data=None, commands=None):
        """
        Data will be the values we get from MemoryServes
        """
        self.token = token

        self.data = default_data()
        if data:
            self.data.update(data)

        self.user_name = Globals.USER_NAME

        self.commands = commands or {}

    #id
    def Id(self):
        """
        get id
        """
        try:
            return self.data["id"]
        except KeyError:
            return 0

    def SetId(self, newid):
        """
        set id
        """
        self.data["id"] = newid
        self.update_self()

    # source
    def Source(self):
        """
        get source (string)
        """
        return self.data["source"]

    def SetSource(self, source):
        """
        set source (string)
        """
        self.data["modified_by"] = self.user_name
        self.data["source"] = source
        self.update_self()

    # trans
    def Trans(self):
        """
        get translation (string)
        """
        return self.data["trans"]

    def SetTrans(self, trans):
        """
        set translation (string)
        """
        self.data["modified_by"] = self.user_name
        self.data["trans"] = trans
        self.update_self()

    # context
    def Context(self):
        """
        get context (string)
        """
        return self.data["context"]

    def SetContext(self, context):
        """
        set context (string)
        """
        self.data["modified_by"] = self.user_name
        self.data["context"] = context
        self.update_self()

    # created/modified
    def Created(self):
        """
        get created date
        """
        return make_date(self.data["date_created"])

    def SetCreated(self, created):
        """
        set created date
        """
        self.data["date_created"] = created
        self.update_self()

    def Modified(self):
        """
        get modified (date)
        """
        return make_date(self.data["last_modified"])

    def SetModified(self, modified):
        """
        set modified (date)
        """
        self.data["modified_by"] = self.user_name
        self.data["last_modified"] = modified
        self.update_self()

    def CreatedBy(self):
        """
        get created by
        """
        return self.data["created_by"]

    def SetCreatedBy(self, created_by):
        """
        set created by
        """
        self.data["created_by"] = created_by
        self.user_name = created_by
        self.update_self()

    def ModifiedBy(self):
        """
        get modified by
        """
        return self.data["modified_by"]

    def SetModifiedBy(self, modified_by):
        """
        set modified by
        """
        self.data["modified_by"] = modified_by
        self.user_name = modified_by
        self.update_self()

    # reliability
    def Reliability(self):
        """
        get reliability
        """
        return self.data["reliability"]

    def SetReliability(self, reliability):
        """
        set reliability
        """
        self.data["modified_by"] = self.user_name
        self.data["reliability"] = max(0, min(9, int(reliability)))
        self.update_self()

    # validated
    def Validated(self):
        """
        get validated (bool)
        """

        return self.data["validated"]

    def SetValidated(self, validated):
        """
        set validated (bool)
        """

        self.data["modified_by"] = self.user_name
        self.data["validated"] = bool(validated)
        self.update_self()

    # ref count
    def RefCount(self):
        """
        get ref count
        """
        return self.data["ref_count"]

    def SetRefCount(self, ref_count):
        """
        set ref count
        """
        self.data["modified_by"] = self.user_name
        self.data["ref_count"] = int(ref_count)
        self.update_self()

    def update_self(self):
        """
        Update the record on the server
        """

        if not self.commands:
            logger.warn("no commands configured in remote record")
            return False

        values = massage_dict(self.data)
        logger.debug("Updating record data")
        try:
            self.data.update(url_retrieve(self.commands["update"], values))
        except urllib2.HTTPError:
            logging.exception("Failed to update record on server")
            return False
        return True


class FelixRemoteMemory(object):
    """
    Interface for Memory Serves TM/glossary
    COM server as Felix.RemoteMemory
    """

    _public_methods_ = """AddRecord DeleteRecord
                    Create Delete
                    Connect GetInfo
                    Search ReverseSearch
                    Concordance ReverseConcordance
                    Gloss RecordById
                    UpdateRecord
                    Login IsLoggedIn
                    Page NumPages
                    UserName""".split()
    _reg_progid_ = "Felix.RemoteMemory"

    # import pythoncom
    # print pythoncom.CreateGuid()
    _reg_clsid_ = "{6AEA2DA2-409A-4B68-A290-AC5313A3829D}"

    def __init__(self):
        """
        Don't expect to be initialized other than from COM
        """
        self.connection = u""
        self.commands = {}
        self.data = {}
        self.last_info_pull = None
        self.info = {}
        self.token = None
        self.base = BASE_URL

    def UserName(self):
        """
        The user name for modified/created info
        """
        return Globals.USER_NAME

    def SetUserName(self, user_name):
        """
        Sets the computer's user name
        """
        Globals.USER_NAME = user_name

    def RecordById(self, id):
        """
        Find a record by its id
        """
        values = dict(rec_id=id, token=self.token)
        command = self.commands.get("rec_by_id")
        logger.debug("rec_by_id command: %s" % command)
        if not command:
            logger.warn("Failed to get record by ID [%s]" % id)
            return None
        raw = url_retrieve(command, values)
        if not raw:
            logger.warn("Failed to retrieve raw data")
            return None
        return self.make_rec(raw)

    # Add and remove records
    def UpdateRecord(self, record):
        """
        Update a record entry.
        record is a COM object -- assume it's from Python
        """

        self.last_info_pull = None
        rec = Dispatch(record)
        logger.debug("Updating record %s" % rec.Id)

        data = self.rec_to_raw(record)
        data["token"] = self.token

        command = self.commands.get("update")
        logger.debug("update command: %s" % command)
        if not command:
            logger.warn("Failed to get update command")
            return False

        updated_data = url_retrieve(command, data)
        if not updated_data:
            logger.warn("No data updated -- check that record exists (id:%s)" % rec.Id)
            return False

        logger.debug("Updated record id: %(id)s" % updated_data)
        return self.make_rec(updated_data)

    def AddRecord(self, record):
        """
        Add a record to the server memory
        record is a COM object -- assume it's from Python
        """
        self.last_info_pull = None
        rec = Dispatch(record)
        logger.debug("Adding record %s" % rec.Id)

        data = self.rec_to_raw(record)
        data["token"] = self.token

        if "id" in data:
            del data["id"]

        command = self.commands.get("add")
        logger.debug("add command: %s" % command)
        if not command:
            logger.warn("Failed to get add command")
            return False

        added_data = url_retrieve(command, data)
        if not added_data:
            logger.warn("Record not added")
            return False

        rec.Id = added_data["id"]

        logger.debug("added record: %s" % data)
        return True

    def DeleteRecord(self, record):
        """
        Delete the specified record from the server
        """

        self.last_info_pull = None
        rec = Dispatch(record)
        logger.debug("Adding record %s" % rec.Id)

        data = self.rec_to_raw(record)
        data["token"] = self.token

        command = self.commands.get("delete")
        logger.debug("add command: %s" % command)
        if not command:
            logger.warn("Failed to get delete command")
            return False

        url_retrieve(command, data)
        logger.debug("deleted record")
        return True

    def set_base(self, connection):
        """
        Set the base URL from the connection
        """
        url = urlparse(connection)
        self.base = "%s://%s" % (url.scheme, url.netloc)
        return self.base

    def Connect(self, connection):
        """
        Connect to the URL string connection

        >>> o = urlparse("http://ginstrom.com/foo/")
        >>> o.scheme
        'http'
        >>> o.netloc
        'ginstrom.com'
        """

        self.connection = connection
        try:
            self.commands = url_retrieve(self.connection)
            self.set_base(connection)
        except ValueError:
            logger.exception("Failed to connect to %ss" % connection)
            return False
        return bool(self.commands)

    def get_login_url(self):
        """
        Get the login URL
        Later version of MemoryServes will include it in the commands;
        otherwise, we need to calculate it ourselves.
        """
        login = self.commands.get("login")
        if login:
            return login
        return "%s/api/login/" % self.base

    def Login(self, username, password):
        """
        Log into Memory Serves server
        """

        login = self.get_login_url()

        values = dict(username=username.encode("utf-8"),
                      password=password.encode("utf-8"))

        try:
            self.token = url_retrieve(login, values)
        except Exception, details:
            warn("Failed to log in : %s" % details)
            return False

        return True

    def IsLoggedIn(self):
        """
        Is the user logged in?
        """
        return self.token is not None

    def GetInfo(self, key):
        """
        Get info about the memory
        """

        if not self.last_info_pull:
            self.last_info_pull = time.time()
        elif within_refresh_interval(time.time(), self.last_info_pull):
            return self.info.get(key, u"")

        values = dict(token=self.token)
        info_command = self.commands["info"]

        self.info = url_retrieve(info_command, values)
        self.last_info_pull = time.time()
        return self.info.get(key, u"")

    # search
    def Search(self, query, minscore):
        """
        Fuzzy matching on source
        """

        values = dict(query=query.encode("utf-8"),
                      minscore=float(minscore),
                      token=self.token)
        raw_hits = url_retrieve(self.commands["search"], values)
        return self.prepare_hits(raw_hits)

    def ReverseSearch(self, query, minscore):
        """
        Search on trans rather than source
        """

        values = dict(query=query.encode("utf-8"),
                      minscore=minscore,
                      token=self.token)
        data = urllib.urlencode(values)
        req = urllib2.Request(self.commands["rsearch"], data)
        return self.prepare_hits(get_url_data(req))

    # pages
    def Page(self, page):
        """
        Fuzzy matching on source
        """

        values = dict(page=page,
                      token=self.token)
        data = urllib.urlencode(values)
        req = urllib2.Request(self.commands["get_page"], data)
        return self.prepare_hits(get_url_data(req))

    def NumPages(self, page):
        """
        Fuzzy matching on source
        """

        values = dict(page=page,
                      token=self.token)
        data = urllib.urlencode(values)
        req = urllib2.Request(self.commands["get_page"], data)
        return get_url_data(req)

    # concordance
    def Concordance(self, query):
        """
        Search for query in source field
        """

        values = dict(query=query.encode("utf-8"),
                      token=self.token)
        data = urllib.urlencode(values)
        req = urllib2.Request(self.commands["concordance"], data)
        return self.prepare_hits(get_url_data(req))

    def ReverseConcordance(self, query):
        """
        Concordance with the translation as the query field
        """

        values = dict(query=query.encode("utf-8"),
                      token=self.token)
        data = urllib.urlencode(values)
        req = urllib2.Request(self.commands["rconcordance"], data)
        return self.prepare_hits(get_url_data(req))

    # glossary searches
    def Gloss(self, query, minscore):
        """
        Do a glossary search
        """

        values = dict(query=query.encode("utf-8"),
                      minscore=minscore,
                      token=self.token)
        data = urllib.urlencode(values)
        req = urllib2.Request(self.commands["gloss"], data)
        return self.prepare_hits(get_url_data(req))

    # These are for debugging
    def Create(self, name, memtype):
        """
        Create a new memory on the server
        """
        values = dict(name=name,
                      memtype=memtype,
                      token=self.token)
        url = "%s/api/addmem/" % self.base
        self.data = url_retrieve(url, values)
        self.info = self.data
        itemid = self.data["id"]

        url = "%s/api/mems/%s/" % (self.base, itemid)
        self.Connect(url)

    def Delete(self):
        """
        Delete the current memory from the sever
        """
        itemid = self.data["id"]
        url = "%s/api/delmem/%s/" % (self.base, itemid)
        urllib2.urlopen(url)

    def rec_to_raw(self, record):
        """
        Take a COM record and turn it into a Python object
        """
        try:
            raw = util.unwrap(record)
            # Lets the record update itself
            if raw:
                raw.commands = self.commands
            return massage_dict(raw.data)
        except Exception, details:
            error("Failed to unwrap record: %s" % str(details))
            return record2dict(record)

    def prepare_hits(self, hits):
        """
        Prepare hits for return via COM interface
        """

        records = [self.make_rec(hit) for hit in hits]
        return util.NewCollection(records)

    def make_rec(self, data):
        return util.wrap(FelixRemoteRecord(self.token,
                                           data,
                                           self.commands))


def reg():
    """
    Register COM servers
    Call this when run as script.

    To register: MemoryEngine.py
    To unregister: MemoryEngine.py --unregister
    """

    debug("Registering COM servers")
    win32com.server.register.UseCommandLine(FelixRemoteMemory)
    win32com.server.register.UseCommandLine(FelixRemoteRecord)

utils.determine_redirect("memory_engine.log")

# Add code so that when this script is run by
# Python.exe, it self-registers.
if __name__ == '__main__':
    reg()
