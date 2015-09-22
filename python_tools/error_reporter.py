import wx
import wx.lib.sized_controls as sc
from loc import _
from loc import language

class ShowLogDlg(sc.SizedDialog):
    def __init__(self, parent, text):
        sc.SizedDialog.__init__(self, parent, -1, _("Log Text"),
                                style=wx.DEFAULT_DIALOG_STYLE | wx.RESIZE_BORDER)
        
        pane = self.GetContentsPane()
        textbox = wx.TextCtrl(pane, -1, text, style=wx.TE_MULTILINE | wx.TE_RICH2)
        textbox.SetSizerProps(expand=True, proportion=2)
        
        sizer = self.CreateStdDialogButtonSizer(wx.OK)
        self.SetButtonSizer(sizer)

class ReportErrorDlg(sc.SizedDialog):

    def __init__(self, data):
        self.data = data
        appname = data["appname"]
        language.change_language(data["language"])
        title = _("Report Error: %s") % appname
        sc.SizedDialog.__init__(self, None, wx.NewId(), title,
                                style=wx.DEFAULT_DIALOG_STYLE | wx.RESIZE_BORDER)
        pane = self.GetContentsPane()

        self.BackgroundColour = 'White'

        # row 0
        text = _("ERROR REPORT")
        sorry_static = wx.StaticText(pane, -1, text)
        sorry_static.Font = wx.Font(18, wx.SWISS, wx.NORMAL, wx.FONTWEIGHT_BOLD)
        sorry_static.SetSizerProps(expand=True)
        sorry_static.ForegroundColour = 'Red'

        # row 1
        text = _("An error has ocurred in %s.\nPlease send an error report.") % appname
        msg_static = wx.StaticText(pane, -1, text)
        msg_static.Font = wx.Font(12, wx.SWISS, wx.NORMAL, wx.FONTWEIGHT_BOLD)
        msg_static.SetSizerProps(expand=True)
        
        # row 3
        wx.StaticText(pane, -1, _("Please describe what action you were performing when the error occurred."))
        
        # row 4
        self.details_textbox = wx.TextCtrl(pane, -1, style=wx.TE_MULTILINE | wx.TE_RICH2)
        self.details_textbox.SetSizerProps(expand=True, proportion=2)
        
        # row 5
        text = _("A binary dump and a log file will be emailed to Felix support.\nClick to view the log file.")
        wx.StaticText(pane, -1, text)
        button = wx.Button(pane, -1, _("View Logfile"))
        self.Bind(wx.EVT_BUTTON, self.OnViewLog, button)
        
        # row 5
        # add dialog buttons
        sizer = self.CreateStdDialogButtonSizer(wx.OK | wx.CANCEL)
        sizer.CancelButton.Label = _("Cancel")
        sizer.AffirmativeButton.Label = _("Send")
        self.SetButtonSizer(sizer)

        # layout
        self.Fit()
        self.SetMinSize(self.GetSize())

        self.Bind(wx.EVT_INIT_DIALOG, self.OnInit, self)

    def OnInit(self, event):
        self.SetFocus()
        self.details_textbox.SetFocus()
        self.Raise()

    def OnViewLog(self, event):
        dlg = ShowLogDlg(self, self.data["log_text"])
        dlg.ShowModal()
        dlg.Destroy()

if __name__ == "__main__":
    app = wx.PySimpleApp()
    text = """foo
bar
baz"""
    data = dict(appname=u"Felix Word Assist", language=u"Japanese", log_text=text)
    dlg = ReportErrorDlg(data)
    if dlg.ShowModal() == wx.ID_OK:
        data["details"] = dlg.details_textbox.Value
        print data
    else:
        print "Cancel"
    dlg.Destroy()

