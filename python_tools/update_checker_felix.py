# coding: UTF8
"""
Dialog to check for updates for Felix
"""

import wx
import wx.lib.sized_controls as sc
import loc
from loc import _
import sys
import os
import urllib2
import webbrowser

WINDOW_WIDTH = 300
WINDOW_HEIGHT = 175
CHECK_INTERVAL = 14  # days


def get_html_folder(language):
    """This is the folder where we will save the log"""

    appdata_folder = loc.get_local_appdata()
    return os.path.join(appdata_folder, u"Felix", u"html", language)


def get_about_page(language):
    base = get_html_folder(language)
    return os.path.join(base, "about.html")


def get_this_version_info():
    appdata_folder = loc.get_local_appdata()
    filename = os.path.join(appdata_folder, u"Felix", u"version.txt")
    return linestodata(open(filename))


def linestodata(lines):
    valpairs = (line.split("=") for line in lines)
    return dict((key.strip(), val.strip()) for key, val in valpairs)


def get_latest_version_info():
    url = "http://felix-cat.com/media/version.txt"
    return linestodata(urllib2.urlopen(url))


def latest_is_newer(this, latest):
    this_version = this["version"]
    latest_version = latest["version"]
    tbits, lbits = this_version.split("."), latest_version.split(".")
    for a, b in zip(tbits, lbits):
        if int(a) < int(b):
            return True
        elif int(a) > int(b):
            return False
    return len(tbits) < len(lbits)


class AskToCheckDialog(sc.SizedDialog):
    def __init__(self, ask_about_updates=False):
        sc.SizedDialog.__init__(self,
                                None,
                                size=(WINDOW_WIDTH, WINDOW_HEIGHT),
                                style=wx.DEFAULT_DIALOG_STYLE | wx.RESIZE_BORDER | wx.STAY_ON_TOP)

        self.Title = _("Check for Updates")

        pane = self.GetContentsPane()
        message_static = wx.StaticText(pane, -1, _("Check online for updates?"))
        wx.StaticText(pane, -1, u"")
        self.dontask = checkbox = wx.CheckBox(pane, -1, _("Don't ask me again"))
        self.dontask.Value = ask_about_updates
        sizer = self.CreateStdDialogButtonSizer(wx.YES | wx.CANCEL)
        sizer.AffirmativeButton.Label = _("Yes")
        sizer.CancelButton.Label = _("No")
        self.SetButtonSizer(sizer)
        pane.Fit()
        self.Bind(wx.EVT_INIT_DIALOG, self.OnInit, self)

    def OnInit(self, event):
        self.SetFocus()
        self.Raise()


def check_updates():
    this = get_this_version_info()
    latest = get_latest_version_info()
    print "this:", this
    print "latest:", latest
    if not latest_is_newer(this, latest):
        dlg = wx.MessageDialog(None, _("Version %s is the latest version") % this["version"],
                               _("Felix Update Check"),
                               wx.OK | wx.ICON_INFORMATION | wx.STAY_ON_TOP
                               #wx.YES_NO | wx.NO_DEFAULT | wx.CANCEL | wx.ICON_INFORMATION
        )
        dlg.ShowModal()
        dlg.Destroy()
        return

    dlg = wx.MessageDialog(None, "\n".join([_("There is a new version of Felix: %s") % latest["version"],
                                            _("Visit download page?")]),
                           _("Felix Update Check"),
                           wx.YES | wx.NO | wx.ICON_INFORMATION | wx.STAY_ON_TOP
                           #wx.YES_NO | wx.NO_DEFAULT | wx.CANCEL | wx.ICON_INFORMATION
    )
    if dlg.ShowModal() == wx.ID_YES:
        webbrowser.open(_("http://felix-cat.com/download/"))
    dlg.Destroy()


def show_permissions_dlg(props):
    dlg = AskToCheckDialog(not props["ask_about_updates"])
    if dlg.ShowModal() == wx.ID_YES:
        print "Yes, check for updates."
        props["check_updates"] = True
        check_updates()
    else:
        print "No, don't check for updates."
        props["check_updates"] = False
    props["ask_about_updates"] = not dlg.dontask.Value
    dlg.Destroy()


def ask_permission():
    show_permissions_dlg(loc.get_prefs())


def main(last):
    app = wx.App()
    if last == "ask":
        ask_permission()
    else:
        check_updates()


if __name__ == "__main__":
    last = sys.argv[-1]
    main(last)


