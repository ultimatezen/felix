# coding: UTF8
"""
Create executable MemoryEngine COM server

You will need to supply the MS CRT DLLs, and modify DLL_PATH to point to them
"""

import sys
from distutils.core import setup
import py2exe
import MemoryEngine
import FelixUtilities
import WordHistory
import ExcelHistory
import os
import modulefinder
import win32com
import wx
from wx import Locale
from FelixPrefEditors import FelixPrefs
from RuleManager import main as rules_main
import traceback

from win32com.shell import shell, shellcon

DESCRIPTION = "Felix Memory Engine COM server"
BASE_DIR = os.path.dirname(os.path.abspath(__file__))
# Locaton of MS CRT files
DLL_PATH = r'D:\dev\cpp\Common DLLs'


def CleanDir(DirName):
    """Remove all prior fields from the dist (DirName) directory"""

    print "-" * 20
    print "Cleaning directory", DirName
    import os
    from os.path import join, getsize

    for root, dirs, files in os.walk(DirName, topdown=False):
        print root, "consumes",
        print sum([getsize(join(root, name)) for name in files]),
        print "bytes in", len(files), "files"
        for name in files:
            os.remove(join(root, name))
        for name in dirs:
            os.rmdir(join(root, name))
    print "-" * 20


class Target(object):
    '''
    Information common to all executables.
    '''

    def __init__(self, **kw):
        self.__dict__.update(kw)
        # for the version info resources (Properties -- Version)
        self.company_name = "Ginstrom IT Solutions (GITS)"
        self.copyright = "(C) Ginstrom IT Solutions"


class ComTarget(Target):
    '''
    Common to all COM servers
    '''

    def __init__(self, modname, **kw):
        Target.__init__(self, **kw)
        # specify which type of com server you want (exe and/or dll)
        self.create_exe = True,
        self.create_dll = False,
        # specify which type of com server you want (exe and/or dll)
        self.version = modname.__version__
        self.name = modname.__progname__


def main():
    excludes = ["pywin",
                "pywin.debugger",
                "pywin.debugger.dbgcon",
                "pywin.dialogs",
                "pywin.dialogs.list",
                "win32ui",
                "Tkconstants", "Tkinter", "tcl"]
    options = dict(optimize=2,
                   dist_dir="MemoryEngine",
                   excludes=excludes,
                   packages="winpaths wx.lib.activex email lxml gzip win32api wx".split(),
                   dll_excludes="POWRPROF.dll MSVCP90.dll".split(),)

    CleanDir("MemoryEngine")

    memory_serves_engine = ComTarget(
        MemoryEngine,
        description="Memory Serves COM interface",
        # use module name for win32com exe/dll server
        modules=["FelixMemoryServes.MemoryEngine"],
    )

    felix_prefs = ComTarget(
        MemoryEngine,
        description="Preference Editors for Felix",
        # use module name for win32com exe/dll server
        modules=["FelixPrefEditors.FelixPrefs"],
    )
    trans_history_excel = ComTarget(
        ExcelHistory,
        description="Translation history COM interface for Excel",
        # use module name for win32com exe/dll server
        modules=["FelixMemoryServes.ExcelHistory"],
    )
    trans_history_word = ComTarget(
        WordHistory,
        description="Translation history COM interface for Word",
        # use module name for win32com exe/dll server
        modules=["FelixMemoryServes.WordHistory"],
    )
    felix_utils = ComTarget(
        FelixUtilities,
        description="Felix Utilities",
        # use module name for win32com exe/dll server
        modules=["FelixMemoryServes.FelixUtilities"],
    )

    clean_up_menus = Target(
        script=r"CleanupOfficeMenus\main.py",
        description="CleanUpMenus application",
        name="CleanUpMenus Application v 1.0",
        dest_base="CleanupOffice"
    )

    show_logs = Target(
        script=r"ShowLogs.py",
        description="ShowLogs application",
        name="ShowLogs Application v 1.0",
        dest_base="ShowLogs"
    )

    check_updates = Target(
        script=r"update_checker_felix.py",
        description="CheckUpdates application",
        name="CheckUpdates Application v 1.0",
        dest_base="CheckUpdates"
    )

    abbrev = Target(
        script=r"FelixPrefEditors\abbrev_editor.py",
        description="EditAbbreviations application",
        name="Edit Abbreviations v 1.0",
        dest_base="EditAbbreviations"
    )

    shortcut = Target(
        script=r"FelixPrefEditors\shortcut_editor.py",
        description="EditShortcuts application",
        name="Edit Shortcuts v 1.0",
        dest_base="EditShortcuts"
    )

    rule_manager = Target(
        script=r"RuleManager\main.py",
        description="Manage Felix replacement rules",
        name="Rule Manager version %s" % rules_main.VERSION,
        dest_base="RuleManager"
    )

    reg_dll = Target(
        script=r"RegDll.py",
        description="RegDll application",
        name="RegDll Application v 1.0",
        dest_base="RegDll"
    )
    unreg_dll = Target(
        script=r"UnRegDll.py",
        description="UnRegDll application",
        name="UnRegDll Application v 1.0",
        dest_base="UnRegDll"
    )


    # need these two DLLs to be on the system...
    dll_files = (
        '.',
         [
             os.path.join(DLL_PATH, 'msvcp90.dll'),
             os.path.join(DLL_PATH, 'msvcr90.dll'),
         ])

    try:
        setup(
            name="MemoryEngine",
            # the following two parameters embed support files within exe/dll file
            options={"py2exe": options},
            version=MemoryEngine.__version__,
            description=DESCRIPTION,
            # author, maintainer, contact go here:
            author="Ryan Ginstrom",
            author_email="info@felix-cat.com",
            packages=[],
            windows=[clean_up_menus,
                     reg_dll,
                     unreg_dll,
                     show_logs,
                     check_updates,
                     abbrev,
                     shortcut,
                     rule_manager],
            com_server=[memory_serves_engine,
                        felix_utils,
                        trans_history_word,
                        trans_history_excel,
                        felix_prefs],
            data_files=[("res", ["res/Felix.ico"]), dll_files]
        )
    except Exception, e:
        print traceback.format_exc()
        print e
        raise


if __name__ == '__main__':
    mypath = globals().get("__file__")
    if not mypath:
        mypath = "setup.py"

    os.chdir(os.path.abspath(os.path.dirname(mypath)))

    # If run without args, build executables, in quiet mode.
    if len(sys.argv) == 1:
        sys.argv.append("py2exe")
        sys.argv.append("-q")

    main()

