#coding: UTF8
"""
Edit list of abbreviations for Word/Ppt

"""
import wx
from wx.lib import sized_controls as sc

from FelixMemoryServes import utils, loc
from FelixMemoryServes.loc import _

import os
import sys

def get_abbrev_filename():
    folder = loc.get_data_folder()
    return os.path.join(folder, "prefs", "abbreviations.txt")

def get_abbrev_text(filename):
    if not os.path.exists(filename):
        return u"""Mr.
    Dr.
    Mrs.
    Ms.
    e.g.
    i.e."""

    return unicode(open(filename).read(), "utf-8")

def write_abbrev_text(filename, text):
    open(filename, "w").write(text.encode("utf-8"))

class AbbrevEditorDialog(sc.SizedDialog):
    """Main frame for the application"""
    def __init__(self, parent=None):
        title = _("Edit Abbreviations for Felix in Word/PowerPoint")
        pos = (10, 10)
        sc.SizedDialog.__init__(self, None, wx.NewId(), title,
                                style=wx.DEFAULT_DIALOG_STYLE | wx.RESIZE_BORDER | wx.STAY_ON_TOP)
        pane = self.GetContentsPane()

        self.textbox = wx.TextCtrl(pane, -1, style=wx.TE_MULTILINE | wx.TE_RICH2)
        self.textbox.SetSizerProps(expand=True, proportion=2)

        text = get_abbrev_text(get_abbrev_filename())
        text = u"\n".join(text.split())
        self.textbox.Value = text

        # add dialog buttons
        sizer = self.CreateStdDialogButtonSizer(wx.OK | wx.CANCEL)
        sizer.CancelButton.Label = _("Cancel")
        sizer.AffirmativeButton.Label = _("OK")
        self.SetButtonSizer(sizer)

        self.Bind(wx.EVT_INIT_DIALOG, self.OnInit, self)

    def OnInit(self, event):
        if self.IsIconized():
            self.Iconize(False)
        if not self.IsShown():
            self.Show(True)
        self.Raise()
        self.textbox.SetFocus()
        text = self.textbox.Value
        self.textbox.SetSelection(len(text), len(text))

def main(language=None):
    if not language:
        language = sys.argv[-1]
    if language == "English" or language == "Japanese":
        loc.language.change_language(language)
    app = wx.PySimpleApp()
    with AbbrevEditorDialog() as dlg:
        if dlg.ShowModal() == wx.ID_OK:
            text = dlg.textbox.Value.encode("utf-8")
            text = u"\n".join(text.split())
            write_abbrev_text(get_abbrev_filename(), text)

            return True
        else:
            return False


if __name__ == "__main__":
    main()
