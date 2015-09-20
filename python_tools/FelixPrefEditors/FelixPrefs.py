#coding: UTF8
"""
COM sever for utilities for Felix

"""
# COM stuff
import win32com.server.register
from FelixMemoryServes import utils
from FelixMemoryServes import loc
import subprocess
import os

__version__ = "1.0"
__author__ = "Ryan Ginstrom"
__license__ = "MIT"
__progname__ = "Felix Preferences"

class FelixPrefs(object):
    """Felix Preferences dialog launcher"""

    _public_methods_ = """EditAbbreviations
        EditShortcuts
        RuleManager""".split()
    _public_attrs_ = []

    _reg_progid_ = "Felix.Preferences"
    # import pythoncom
    # print pythoncom.CreateGuid()
    _reg_clsid_ = "{2B2BD8BF-427B-4583-B96B-8E00AE977ED2}"

    def __init__(self):
        pass

    def EditAbbreviations(self, language):
        try:
            cmd = get_command("abbrev", language)
            subprocess.call(cmd)
            return True
        except:
            return False

    def EditShortcuts(self, app, language):
        try:
            cmd = get_command("shortcuts", language, app)
            subprocess.call(cmd)
            return True
        except:
            return False

    def RuleManager(self, app, language):
        try:
            cmd = get_command("rules", language, app)
            subprocess.call(cmd)
            return True
        except:
            return False

def get_command_path():
    modpath = None
    if loc.we_are_frozen():
        modpath = loc.module_path()
    else:
        modpath = os.path.dirname(os.path.abspath(__file__))

    if " " in modpath:
        format = '"%s"'
    else:
        format = '%s'
    return format, modpath

def get_command(name, language, app=None):
    format, modpath = get_command_path()

    if loc.we_are_frozen():
        if name == "shortcuts":
            return [format % os.path.join(modpath, "EditShortcuts.exe"), app, language]
        elif name == "rules":
            return [format % os.path.join(modpath, "RuleManager.exe"), app, language]
        else:
            return [format % os.path.join(modpath, "EditAbbreviations.exe"), language]
    else:
        if name == "shortcuts":
            return ["pythonw.exe",
                    format % os.path.join(modpath, "shortcut_editor.py"),
                    app,
                    language]
        elif name == "rules":
            return ["pythonw.exe",
                    r"C:\dev\python\RuleManager\main.py",
                    language]
        else:
            return ["pythonw.exe",
                    format % os.path.join(modpath, "abbrev_editor.py"),
                    language]


def reg():
    """Register COM servers"""

    win32com.server.register.UseCommandLine(FelixPrefs)

utils.determine_redirect("felix_preferences.log")

# Add code so that when this script is run by
# Python.exe, it self-registers.
if __name__ == '__main__':
    reg()
