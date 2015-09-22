#coding: UTF8
"""
Locations

"""
import sys
import os
import winpaths
import win32api
import win32con
from ctypes import wintypes
from cPickle import load, dump

MODULE_NAME = u"Felix"
DEFAULT_GUI_LANGUAGE = "English"
DEFAULT_PROPFILE = u"prefs.database"

LANG_JAPANESE = win32con.LANG_JAPANESE

def primary_lang_id(langid):
    return win32con.PRIMARYLANGID(langid)

def get_user_lang():
    return primary_lang_id(win32api.GetUserDefaultLangID())

try:
    langid = get_user_lang()
    if langid == LANG_JAPANESE:
        DEFAULT_GUI_LANGUAGE = "Japanese"
except Exception:
    pass

class FelixUtilityException(Exception):
    pass

def we_are_frozen():
    """Returns whether we are frozen via py2exe.
    This will affect how we find out where we are located."""

    return hasattr(sys, "frozen")

# module_path
def module_path_frozen(mylocation=None):
    """
    This will get us the program's directory,
    if we are frozen using py2exe
    """

    return os.path.dirname(unicode(sys.executable,
                                   sys.getfilesystemencoding()))

def module_path_thawed(mylocation=None):
    """
    This will get us the program's directory,
    if we are not frozen using py2exe
    """

    if not mylocation:
        mylocation = __file__
    return os.path.abspath(os.path.dirname(unicode(mylocation,
                                   sys.getfilesystemencoding())))

def get_local_appdata():
    return winpaths.get_local_appdata()

def get_local_app_data_folder(modname=MODULE_NAME):
    app_data = get_local_appdata()
    return os.path.join(app_data, modname)

def get_data_folder_frozen(mylocation=None):
        data_folder = get_local_app_data_folder()
        if not os.path.isdir(data_folder):
            os.makedirs(data_folder)

        return data_folder

def get_log_base_frozen(mylocation=None):
    return get_local_app_data_folder()

def get_log_base_thawed(mylocation=None):
    return module_path(mylocation)

def get_log_filename(filename):
    basepath = get_log_base()

    if not os.path.isdir(basepath):
        os.makedirs(basepath)
    return os.path.join(basepath, "logs", filename)

def get_default_prefs():
    return dict(check_updates=True,
                    ask_about_updates=True,
                    last_update_check=None,
                    language=DEFAULT_GUI_LANGUAGE)

def get_prefs(filename=DEFAULT_PROPFILE):
    defaults = get_default_prefs()
    try:
        propfile = os.path.join(get_data_folder(), "prefs", filename)
        defaults.update(load(open(propfile)))
    finally:
        return defaults

def set_prefs(prefs, filename=DEFAULT_PROPFILE):
    propfile = os.path.join(get_data_folder(), "prefs", filename)
    dump(prefs, open(propfile, "w"))

def get_string_resources(filename=u"stringres.database"):
    try:
        folder = os.path.join(get_data_folder(), "prefs", filename)
        return load(open(folder))
    except IOError, details:
        print "Failed to load string resources: %s" % str(details)
        stringres = {}
        dump(stringres, open(folder, "w"))
        return stringres


def get_logfile():
    return os.path.join(get_data_folder(), "logs",
                        u"align_assist.log")

class Language(object):
    """This class handles string localization in the application"""

    def __init__(self, dbname="stringres.database", prefsname="prefs.database"):
        """
        @param dbname: Name of string database
        """

        self.prefname = prefsname
        self.language = DEFAULT_GUI_LANGUAGE
        self.trans = {}

        self.set_preferred_language()
        self.load_translations(dbname)


    def set_preferred_language(self):

        user_language = get_user_lang()
        if user_language == LANG_JAPANESE:
            self.language = "Japanese"
        else:
            self.language = "English"

        try:
            prefs = get_prefs(self.prefname)
            if prefs:
                self.language = prefs["language"]

        except IOError:
            pass

    def load_translations(self, dbname="stringres.database"):
        """Load translations from database file"""

        try:
            self.trans = get_string_resources(dbname)

        except IOError, details:
            print "Failed to load translation database:", details

    def get_translation(self, msgid):
        """Retrieve translation for msgid from the database

        @param msgid: A string to be translated
        """

        key = unicode(msgid)
        term = self.trans.get(key)
        if not term:
            return key

        if self.language == 'Japanese':
            return term['ja']
        return term['en']

    def change_language(self, language):
        langdict = dict(ja="Japanese",
                        jp="Japanese",
                        en="English",
                        Japanese="Japanese",
                        English="English")
        language = langdict.get(str(language), "English")
        self.language = language
        prefs = get_prefs()
        prefs["language"] = language
        set_prefs(prefs)

# Detect whether we are frozen, and set functions appropriately.
if we_are_frozen():
    get_data_folder = get_data_folder_frozen
    module_path = module_path_frozen
    get_log_base = get_log_base_frozen
else:
    get_data_folder = module_path_thawed
    module_path = module_path_thawed
    get_log_base = get_log_base_thawed

language = Language()
_ = language.get_translation
