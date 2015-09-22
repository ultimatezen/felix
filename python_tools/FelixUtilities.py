# coding: UTF8
"""
COM sever for utilities for Felix

"""

import os
import shutil
from datetime import date
import subprocess
# COM stuff
import win32com.server.register

import loc
from loc import get_local_app_data_folder, module_path
from loc import get_prefs, set_prefs
import utils
from utils import debug, log_err
from FelixMemoryServes import update_checker_felix

__version__ = "1.0"
__author__ = "Ryan Ginstrom"
__license__ = "MIT"
__progname__ = "Felix Utilities"

CHECK_INTERVAL = update_checker_felix.CHECK_INTERVAL


class FelixUtilities(object):
    """Utilities for Felix"""

    _public_methods_ = """EnsureHtml CopyHtml LoadProp SaveProp
                         CheckUpdates CheckUpdatesRequested
                         HasUpdate CurrentVersion NewVersion""".split()
    _public_attrs_ = []

    _reg_progid_ = "Felix.Utilities"
    # import pythoncom
    # print pythoncom.CreateGuid()
    _reg_clsid_ = "{3D368688-B9E8-4BFE-ADF6-22F1CBA8845B}"

    def __init__(self):
        debug("Felix.Utilities")

    def HasUpdate(self):
        return False

    def CurrentVersion(self):
        return "1"

    def NewVersion(self):
        return "1"

    def LoadProp(self, key):
        """
        Loads a key-value pair from the properties.

        :param key: The key to retrieve
        :return: The value corresponding to the key
        """
        val = get_prefs().get(key, None)
        debug("Loaded prop: %s=%s" % (key, val))
        return val

    def SaveProp(self, key, val):
        """
        Saves a property as a key-value pair

        :param key: The key
        :param val: The value
        """
        prefs = get_prefs()
        prefs[key] = val
        set_prefs(prefs)
        debug("Saved prop: %s=%s" % (key, val))

    def CopyHtml(self, source, dest):
        """
        Forces copy of HTML files to local app folder.
        :param source: The location of the backup HTML files
        :param dest: Copy the HTML files here (local app data)
        """
        debug(u"Forcing copy of HTML files to local app folder")
        debug(u"source: %s" % source)
        debug(u"destination: %s" % dest)
        shutil.copyfile(source, dest)

    def EnsureHtml(self):
        """
        Ensure that HTML files are in local app data. If they are not,
        copies them over.
        """
        debug("Checking for HTML files in local app folder")
        data_folder = get_local_app_data_folder()
        html_folder = os.path.join(data_folder, u"html")
        if not os.path.isdir(html_folder):
            debug(" ... Files not found. Copying.")
            head, tail = os.path.split(module_path())
            src = os.path.join(head, u"html")
            dst = html_folder
            shutil.copytree(src, dst)
            return True
        return False

    def CheckUpdates(self, language):
        """
        Check Felix server for updates.
        :param language: The UI language for showing the update dialog.
        """
        try:
            debug(u"Checking for updates. Language is %s" % language)
            loc.language.change_language(language)
            prefs = get_prefs()
            check_permission(prefs, date.today())
            set_prefs(prefs)
        except:
            LOGGER.exception("Failed to check updates")

    def CheckUpdatesRequested(self, language):
        """
        If the user checks for updates, we don't have to see whether the user
        wants to check automatically.
        :param language: The UI language for showing dialogs.
        """
        try:
            debug(u"Checking for updates at user request. Language is %s" % language)
            loc.language.change_language(language)
            check_updates()
            prefs = get_prefs()
            prefs["last_update_check"] = date.today()
            set_prefs(prefs)
        except:
            LOGGER.exception("Failed to check updates")


def get_command():
    modpath = loc.module_path()
    if " " in modpath:
        format = '"%s"'
    else:
        format = '%s'
    if loc.we_are_frozen():
        return [format % os.path.join(modpath, "CheckUpdates.exe")]
    else:
        return ["pythonw.exe",
                format % os.path.join(modpath, "update_checker_felix.py")]


def check_updates():
    cmd = get_command()
    subprocess.Popen(cmd,
                     shell=False)


def ask_updates():
    cmd = get_command()
    cmd.append("ask")
    subprocess.Popen(cmd,
                     shell=False)


def check_permission(prefs, today):
    if not prefs["ask_about_updates"] and not prefs["check_updates"]:
        return
    last_check = prefs["last_update_check"]
    if not last_check:
        if prefs["ask_about_updates"]:
            ask_updates()
        else:
            check_updates()
        prefs["last_update_check"] = today
        return
    else:
        diff = today - last_check
        if diff.days >= CHECK_INTERVAL:
            if prefs["ask_about_updates"]:
                ask_updates()
            else:
                check_updates()
            prefs["last_update_check"] = today


def reg():
    """Register COM servers"""

    debug("Registering COM servers")
    win32com.server.register.UseCommandLine(FelixUtilities)


utils.determine_redirect("felix_utilities.log")

# Add code so that when this script is run by
# Python.exe, it self-registers.
if __name__ == '__main__':
    reg()
