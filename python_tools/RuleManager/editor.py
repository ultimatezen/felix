#coding: UTF8
"""
Edit Felix rules.

Edit dialog box with rule tester.
"""
import wx
from wx.lib import sized_controls as sc
from FelixMemoryServes.loc import _
from FelixMemoryServes import loc
import sys
import traceback
import re

class FormFields(object):
    '''
    Sets up the fields for the rule form.
    '''

    def __init__(self, pane):
        form = sc.SizedPanel(pane)
        form.SetSizerProps(expand=True)

        wx.StaticText(form, label=_("Rule"))
        self.rule_text = wx.TextCtrl(form)
        self.rule_text.SetSizerProps(expand=True)

        wx.StaticText(form, label=_("Source"))
        self.source_text = wx.TextCtrl(form)
        self.source_text.SetSizerProps(expand=True)

        wx.StaticText(form, label=_("Target"))
        self.target_text = wx.TextCtrl(form)
        self.target_text.SetSizerProps(expand=True)

class RuleDialog(sc.SizedDialog):
    '''
    The actual dialog class for the rule editor.
    '''

    def __init__(self, parent, title):
        title = _("Edit Felix Rule")
        sc.SizedDialog.__init__(self, parent, wx.NewId(), title,
                                size=(500,400),
                                style=wx.DEFAULT_DIALOG_STYLE | wx.RESIZE_BORDER | wx.STAY_ON_TOP)

class RuleEditorDialog(RuleDialog):
    '''
    For editing
    '''

    def __init__(self, parent):
        RuleDialog.__init__(self, parent, title = _("Edit Felix Rule"))

class RuleCreatorDialog(RuleDialog):
    '''
    For creating
    '''

    def __init__(self, parent):
        RuleDialog.__init__(self, parent, title = _("Create Felix Rule"))


class RuleEditorController(object):
    """Main frame for the application"""
    def __init__(self, window, values=None):

        self.window = window
        pane = self.window.GetContentsPane()

        self.make_form(pane)
        if values:
            self.set_form_values(self, values)

        sizer = self.window.CreateStdDialogButtonSizer(wx.OK | wx.CANCEL)
        sizer.CancelButton.Label = _("Cancel")
        sizer.AffirmativeButton.Label = _("OK")
        self.SetButtonSizer(sizer)
        self.window.Fit()
        self.window.Size = 500, self.window.Size[1]
        self.window.MinSize = self.window.Size
        self.window.Refresh()

        self.rule_text.SetFocus()
        self.rule_text.SetSelection(-1, -1)


    def get_values(self):
        return self.get_form_values(self)

    def get_form_values(self, form):
        return dict(name=form.rule_text.Value,
                    source=form.source_text.Value,
                    target=form.target_text.Value,
                    sample=form.sample_text.Value)

    def set_form_values(self, form, values):
        form.rule_text.Value = values.get("name", u"")
        form.source_text.Value = values.get("source", u"")
        form.target_text.Value = values.get("target", u"")
        form.sample_text.Value = values.get("sample", u"")

        # Setting a new value clears the test output
        form.output_text.Value = u""
        self.clear_bgc()

    def make_form(self, pane):
        fields = FormFields(pane)

        self.rule_text = fields.rule_text

        self.source_text = fields.source_text
        self.target_text = fields.target_text

        wx.StaticText(pane, label=_("Sample Text"))
        self.sample_text = wx.TextCtrl(pane, style=wx.TE_MULTILINE|wx.TE_PROCESS_ENTER)
        self.sample_text.SetSizerProps(expand=True, proportion=1)

        self.test_button = wx.Button(pane, -1, _("&Test"))
        self.test_button.SetSizerProps(halign="right")

        self.Bind(wx.EVT_BUTTON, self.OnTest, self.test_button)

        wx.StaticText(pane, label=_("Output"))
        self.output_text = wx.TextCtrl(pane, style=wx.TE_MULTILINE|wx.TE_PROCESS_ENTER)
        self.output_text.SetSizerProps(expand=True, proportion=1)

        wx.StaticText(pane, label=_("Matches"))
        style = wx.LC_SINGLE_SEL | wx.LC_REPORT | wx.LC_NO_HEADER
        self.matchlist = wx.ListCtrl(pane, -1, style=style)
        self.matchlist.InsertColumn(0, _("Source"))
        self.matchlist.InsertColumn(1, _("Target"))
        self.matchlist.SetSizerProps(expand=True, proportion=2)

    def clear_bgc(self):
        '''
        Clears the background color of the output text
        '''
        self.output_text.BackgroundColour = "white"
        self.window.Refresh()

    def OnTest(self, event):
        self.matchlist.DeleteAllItems()
        pattern = self.source_text.Value
        repl = self.target_text.Value
        text = self.sample_text.Value

        try:
            result, hits = re.subn(pattern, repl, text, re.UNICODE)
        except Exception, e:
            print "Exception testing rule:", e
            self.output_text.BackgroundColour = "red"
            wx.CallLater(1500, self.clear_bgc)
            return

        self.output_text.Value = result
        if hits:

            for match in re.finditer(pattern, text, re.U):
                replacement = re.sub(pattern, repl, match.group(0), re.UNICODE)
                index = self.matchlist.InsertStringItem(sys.maxint, match.group(0))
                self.matchlist.SetStringItem(index, 1, replacement)

            self.output_text.BackgroundColour = "green"
            self.window.Refresh()

            for i in (0, 1):
                self.matchlist.SetColumnWidth(i, wx.LIST_AUTOSIZE_USEHEADER)

            wx.CallLater(1500, self.clear_bgc)

        else:
            self.clear_bgc()

    def __getattr__(self, attr):
        '''
        Pass on window calls to wrapped window
        '''
        return getattr(self.window, attr)

    # `with` support
    def __enter__ (self):
        return self
    def __exit__ (self, type, value, tb):
        pass

def get_rule_editor(parent, values=None):
    return RuleEditorController(RuleEditorDialog(parent), values)

def get_rule_creator(parent, values=None):
    return RuleEditorController(RuleCreatorDialog(parent), values)

def main(language):
    '''
    Create sample dialog for visual debugging when run as main.
    '''

    try:
        loc.language.change_language(language)
        app = wx.PySimpleApp()
        values = dict(name="some rule",
                      source=r"(\d+(\.\d+)?)",
                      target=r"\1",
                      sample="I have 1.234 apples and give 54.34 fucks.")
        with get_rule_editor(None, values) as dlg:
            if dlg.ShowModal() == wx.ID_OK:
                return True
            else:
                return False
    except Exception, e:
        print "Exception:", e
        print traceback.print_exc()

def get_lang(args):

    if args[-1] in "English Japanese".split():
            return args[-1]

    return "English"

if __name__ == "__main__":
    main(get_lang(sys.argv))
