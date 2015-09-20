import wx
import wx.grid
import wx.aui
import wx.lib.sized_controls as sc
import loc
from loc import _
import simplemapi

import os
import glob
import sys

WINDOW_WIDTH = 700
WINDOW_HEIGHT = 400

class LogViewer(sc.SizedDialog):
    def __init__(self, logfiles=None):
        sc.SizedDialog.__init__(self,
                                None,
                                size=(WINDOW_WIDTH, WINDOW_HEIGHT),
                                style=wx.DEFAULT_DIALOG_STYLE | wx.RESIZE_BORDER)

        self.Title = _("Felix Log Viewer")
        self.SetMinSize(self.GetSize())

        pane = self.GetContentsPane()
        self.nb = wx.aui.AuiNotebook(pane)
        self.nb.SetSizerProps(expand=True, proportion=3)

        if logfiles is None:
            datapath = os.path.join(loc.get_data_folder(), u"logs")
            self.logfiles = sorted(glob.glob(os.path.join(datapath, u"*.log")))
        else:
            self.logfiles = logfiles
        for logfile in self.logfiles:
            grid = wx.grid.Grid(self.nb)
            basename = os.path.basename(logfile)
            basename = os.path.splitext(basename)[0]
            self.nb.AddPage(grid, u" ".join([x.title() for x in basename.split(u"_")]))

            lines = open(logfile).readlines()
            grid.CreateGrid(len(lines), 3)
            for col, colname in enumerate((_("Severity"),
                                           _("Time"),
                                           _("Message"))):
                grid.SetColLabelValue(col, colname)

            err_color = "#FFCCCC"
            colors = dict(INFO="#CCFFCC",
                          DEBUG="#CCFFCC",
                          WARN="yellow",
                          ERROR=err_color)

            last_color = colors["ERROR"]

            for row, line in enumerate(lines):
                try:
                    severity, timestamp, msg = line.split("\t")
                except ValueError:
                    severity, timestamp = u"", u""
                    msg = line
                if not isinstance(msg, unicode):
                    try:
                        msg = unicode(msg, "utf-8")
                    except UnicodeDecodeError:
                        msg = unicode(msg, sys.getfilesystemencoding())
                grid.SetCellValue(row, 0, severity)
                grid.SetCellValue(row, 1, timestamp)
                grid.SetCellValue(row, 2, msg)

                attr = wx.grid.GridCellAttr()
                if severity:
                    color = colors.get(severity, last_color)
                else:
                    color = colors.get("INFO", last_color)
                attr.SetBackgroundColour(color)

                if color == err_color:
                    attr.SetFont(wx.Font(10, wx.SWISS, wx.NORMAL, wx.BOLD))

                last_color = color

                grid.SetRowAttr(row, attr)

                first_col_widths = grid.GetColSize(0) + grid.GetColSize(1) + 150
                grid.SetColSize(2, WINDOW_WIDTH - first_col_widths)

        # layout
        sizer = self.CreateStdDialogButtonSizer(wx.OK | wx.CANCEL)
        sizer.CancelButton.Label = _("Cancel")
        sizer.AffirmativeButton.Label = _("Send")
        self.SetButtonSizer(sizer)

        self.Fit()

        self.Bind(wx.EVT_INIT_DIALOG, self.OnInit, self)

    def OnInit(self, event):
        self.SetFocus()

def main(logfiles=None):
    app = wx.App()
    dlg = LogViewer(logfiles)
    if dlg.ShowModal() == wx.ID_OK:
        subject = "Felix Logs"
        try:
            version = os.path.join(loc.get_local_app_data_folder(),
                                           "version.txt")
            lines = open(version).readlines()
            keyval_pairs = (line.split("=") for line in lines)
            data = dict([(key.strip(),val.strip()) for key, val in keyval_pairs])
            subject = subject + " (ver. %s)" % data["version"]
        except IOError:
            print "No version info"

        body = "Sending Felix logs to support"
        attach = ";".join(dlg.logfiles)
        try:
            simplemapi.SendMail('support@felix-cat.com',
                                subject,
                                body,
                                attach)
        except Exception, details:
            print "Failed to send error report: %s" % details
            print "subject:", subject
    else:
        print "Cancel"
    dlg.Destroy()

if __name__ == "__main__":
    last = sys.argv[-1]
    logfiles = None
    langs = dict(en="en", English="en",
                 jp="jp", Japanese="jp")
    if last in langs:
        loc.language.change_language(langs[last])
    elif os.path.exists(last) and "ShowLogs" not in last:
        logfiles = [last]
    main(logfiles)
