#coding: UTF8
"""
Edit Felix rules
"""
import wx
from wx.lib import sized_controls as sc
from FelixMemoryServes.loc import _
from FelixMemoryServes import loc
import sys
import traceback
from RuleManager import managerules
from RuleManager import editor
from RuleManager.editor import get_lang
import os

VERSION = '1.0'
KEYS = managerules.KEYS

class RulesPane(object):
    def __init__(self, parent, title, rules, window, move_btn):

        self.rules = rules
        self.window = window
        self.move_btn = move_btn

        pane = sc.SizedPanel(parent)
        pane.SetSizerProps(expand=True, proportion=3)

        # label
        wx.StaticText(pane, label=title)

        # command list
        self.commands = None
        self.make_command_list(pane)

        # buttons
        self.make_button_row(pane)

        self.populate(self.rules)

    def get_move_btn(self):
        return getattr(self.window, self.move_btn)

    def make_command_list(self, pane):
        style = wx.LC_SINGLE_SEL | wx.LC_REPORT | wx.LC_NO_HEADER
        self.commands = wx.ListCtrl(pane, -1, style=style)
        self.commands.InsertColumn(0, _("Rule Name"))
        self.commands.SetSizerProps(expand=True, proportion=1)
        self.commands.SetMinSize((self.commands.Size[0], 200))

        # bind list events
        self.commands.Bind(wx.EVT_LIST_ITEM_SELECTED,
                  self.OnItemSelected,
                  self.commands)
        self.commands.Bind(wx.EVT_LIST_ITEM_DESELECTED,
                  self.OnItemDeselected,
                  self.commands)
        self.commands.Bind(wx.EVT_LIST_ITEM_ACTIVATED,
                  self.OnItemActivated,
                  self.commands)

    def OnItemSelected(self, evt):
        self.remove_btn.Enabled = True
        self.edit_btn.Enabled = True
        self.get_move_btn().Enabled = True

        index = self.get_selected_items()[0]
        rule = self.rules[index]
        self.window.set_form_values(self.window, rule)

    def OnItemDeselected(self, evt):
        activate = bool(self.get_selected_items())
        self.remove_btn.Enabled = activate
        self.edit_btn.Enabled = activate
        self.get_move_btn().Enabled = activate

        self.window.set_form_values(self.window, {})

    def OnItemActivated(self, evt):
        self.remove_btn.Enabled = True
        self.edit_btn.Enabled = True
        self.get_move_btn().Enabled = True

        index = self.get_selected_items()[0]
        rule = self.rules[index]
        self.window.set_form_values(self.window, rule)

    def get_selected_items(self):
        """
        Gets the selected items for the list control.
        Select returned as a list of indices,
        low to high.
        """
        selection = []
        current = -1
        while True:
            next = self.commands.GetNextSelected(current)
            if next == -1:
                return list(reversed(selection))

            selection.append(next)
            current = next

    def GetNextSelected(self, current):
        """
        Return subsequent selected items, or -1 when no more
        """
        return self.GetNextItem(current,
                                wx.LIST_NEXT_ALL,
                                wx.LIST_STATE_SELECTED)

    def make_button_row(self, pane):
        """
        Create the button row for the list:
            - Add button
            - Remove button
            - Edit button
        """

        button_row = sc.SizedPanel(pane)
        button_row.SetSizerType("horizontal")
        button_row.SetSizerProps(expand=True)

        self.add_btn = wx.Button(button_row, -1, _("&Add"))

        self.remove_btn = wx.Button(button_row, -1, _("&Remove"))
        self.remove_btn.Enabled = False

        self.edit_btn = wx.Button(button_row, -1, _("&Edit"))
        self.edit_btn.Enabled = False

        # bind button events
        pane.Bind(wx.EVT_BUTTON, self.OnAdd, self.add_btn)
        pane.Bind(wx.EVT_BUTTON, self.OnRemove, self.remove_btn)
        pane.Bind(wx.EVT_BUTTON, self.OnEdit, self.edit_btn)

    def populate(self, rules):
        """
        Populate the list control with the rule names.
        """

        self.commands.DeleteAllItems()
        for rule in rules:
            self.commands.InsertStringItem(sys.maxint, rule["name"])

        self.commands.SetColumnWidth(0, wx.LIST_AUTOSIZE)

    def remove_rule(self, index):
        try:
            rule = self.rules[index]
            self.rules[:] = [r for (i,r) in enumerate(self.rules) if i != index]
            self.populate(self.rules)
            return rule
        except IndexError, e:
            print "Rule index %i out of range: %s" % (index, e)
            return None

    def add_rules(self, rules):
        """
        Add a list of rules.
        `rules` is the list of rules.
        We make sure that we don't add dupes.
        """
        existing = set((r["name"], r["source"], r["target"]) for r in self.rules)
        self.rules.extend([r for r in rules if not (r["name"], r["source"], r["target"]) in existing])
        self.populate(self.rules)
        self.commands.SetItemState(len(self.rules) - 1, wx.LIST_STATE_SELECTED, wx.LIST_STATE_SELECTED)

    def add_rule(self, rule):
        """
        Add a rule to the end of our list
        """
        if not rule:
            return
        self.rules.append(rule)
        self.populate(self.rules)
        self.commands.SetItemState(len(self.rules) - 1, wx.LIST_STATE_SELECTED, wx.LIST_STATE_SELECTED)

    # enabled command events
    def OnAdd(self, event):
        """
        Add a rule.
        """
        with editor.get_rule_creator(self.window.window, {}) as dlg:
            retval = dlg.ShowModal()
            if retval == wx.ID_OK:
                rule = dlg.get_values()
                if not rule.get("name"):
                    rule["name"] = rule["source"]
                self.window.set_form_values(self.window, rule)
                self.add_rule(rule)

    def OnRemove(self, event):
        """
        Remove the selected rule
        """
        try:
            index = self.get_selected_items()[0]
            self.remove_rule(index)
        except IndexError:
            print "No selected items to remove; removing top item"
            self.remove_rule(0)

    def OnEdit(self, event):
        """
        Edit the selected rule
        """
        index = self.get_selected_items()[0]
        rule = self.rules[index]
        vals = self.window.get_form_values(self.window)
        rule.update(vals)
        with editor.get_rule_editor(self.window.window, rule) as dlg:
            retval = dlg.ShowModal()
            if retval == wx.ID_OK:
                rule = dlg.get_values()
                if not rule.get("name"):
                    rule["name"] = rule["source"]
                self.window.set_form_values(self.window, rule)
                self.rules[index] = rule
                self.populate(self.rules)
                self.commands.SetItemState(index, wx.LIST_STATE_SELECTED, wx.LIST_STATE_SELECTED)


class RuleManagerDialog(sc.SizedDialog):
    """
    The actual wxPython subclass.
    Put as little code as possible here.
    """
    def __init__(self):

        title = _("Felix Rule Manager")
        style = wx.DEFAULT_DIALOG_STYLE | wx.RESIZE_BORDER | wx.STAY_ON_TOP \
                | wx.MAXIMIZE_BOX | wx.MINIMIZE_BOX
        sc.SizedDialog.__init__(self, None, wx.NewId(), title,
                                style=style)

class RuleManagerController(object):
    """
    Main frame for the application.
    Wraps a wxPython dialog for mocking.
    """
    def __init__(self, window):

        self.window = window

        # load rules
        rules = managerules.load()
        enabled_rules, disabled_rules = managerules.partition_rules(rules)

        # create widgets
        pane = self.GetContentsPane()
        self.make_imp_exp_buttons(pane)
        self.make_top(pane, enabled_rules, disabled_rules)
        self.make_bottom(pane)

        # ok/cancel buttons
        sizer = self.CreateStdDialogButtonSizer(wx.OK | wx.CANCEL)
        sizer.AffirmativeButton.Label = _("OK")
        sizer.CancelButton.Label = _("Cancel")
        self.SetButtonSizer(sizer)

        # set icon
        self.set_icon()

        # size
        self.Fit()
        self.MinSize = self.Size
        self.Refresh()

    def set_icon(self):
        ib = wx.IconBundle()
        folder = loc.get_data_folder()
        filename = os.path.join(folder, "res", "Felix.ico")
        ib.AddIconFromFile(filename, wx.BITMAP_TYPE_ANY)
        self.SetIcons(ib)

    def add_rules(self, enabled, disabled):
        """
        Populate the rules in the lists
        """

        self.enabled_commands.add_rules(enabled)
        self.disabled_commands.add_rules(disabled)

    def get_rules(self):
        """
        Gets the rules for writing to file.
        """

        rules = []

        for rule in self.enabled_commands.rules:
            rule["enabled"] = "1"
            rules.append(rule)
        for rule in self.disabled_commands.rules:
            rule["enabled"] = "0"
            rules.append(rule)

        return rules

    def make_imp_exp_buttons(self, pane):
        button_row = sc.SizedPanel(pane)
        button_row.SetSizerType("horizontal")

        self.import_btn = wx.Button(button_row, -1, _("&Import..."))
        self.export_btn = wx.Button(button_row, -1, _("&Export..."))

        self.window.Bind(wx.EVT_BUTTON, self.OnImport, self.import_btn)
        self.window.Bind(wx.EVT_BUTTON, self.OnExport, self.export_btn)

    def make_top(self, pane, arules, irules):
        """
        Create the top part of the form:
            - Disabled rules
            - Enable/disable buttons
            - Enabled rules
        """

        top = sc.SizedPanel(pane)

        top.SetSizerType("horizontal")
        top.SetSizerProps(expand=True, proportion=3)

        # disabled
        self.disabled_commands = RulesPane(top,
                                             _("Disabled Rules"),
                                             irules,
                                             self,
                                             "enable_btn")

        # move
        self.make_move_buttons(top)

        # enabled
        self.enabled_commands = RulesPane(top,
                                           _("Enabled Rules"),
                                           arules,
                                           self,
                                           "disable_btn")


    def make_move_buttons(self, parent):
        move_pane = sc.SizedPanel(parent)
        border = (["left", "right"], 6)
        move_pane.SetSizerProps(proportion=1, halign="center", border=border)
        move_static = wx.StaticText(move_pane, label=_("Move"))
        move_static.SetSizerProps(halign="center")

        self.enable_btn = wx.Button(move_pane, -1, _("&Enable >>"))
        self.enable_btn.Enabled = False
        self.enable_btn.SetSizerProps(halign="center")

        self.disable_btn = wx.Button(move_pane, -1, _("<< &Disable"))
        self.disable_btn.SetSizerProps(halign="center")
        self.disable_btn.Enabled = False

        self.window.Bind(wx.EVT_BUTTON, self.OnEnable, self.enable_btn)
        self.window.Bind(wx.EVT_BUTTON, self.OnDisable, self.disable_btn)


    def get_wildcard(self):
        return '|'.join([_("Felix Rules (*.frules)|*.frules"),
                         _("All Files (*.*)|*.*"),
                          ])

    def get_file_dlg(self):
        return wx.FileDialog(
            self.window,
            message=_("Select Rule File"),
            defaultFile="",
            wildcard=self.get_wildcard(),
            style=wx.OPEN|wx.CHANGE_DIR
           )

    def OnImport(self, event):
        """
        Import rules from file
        """
        dlg = self.get_file_dlg()

        if dlg.ShowModal() == wx.ID_OK:
            path = dlg.GetPaths()[0]
            rules = managerules.load(path)
            act, inact = managerules.partition_rules(rules)
            self.enabled_commands.add_rules(act)
            self.disabled_commands.add_rules(inact)
            msg = _("Imported {0} rules from {1}").format(len(rules), path)
            wx.MessageBox(msg, _("Import Complete"), wx.OK)

        dlg.Destroy()

    def OnExport(self, event):
        """
        Export rules to file.
        """
        dlg = self.get_file_dlg()

        if dlg.ShowModal() == wx.ID_OK:
            path = dlg.GetPaths()[0]
            if not path.endswith(u".frules"):
                path += u".frules"

            rules = self.get_rules()
            managerules.write_rules(rules, path)
            msg = _("Exported {0} rules to {1}").format(len(rules), path)
            wx.MessageBox(msg, _("Export Complete"), wx.OK)


    def OnEnable(self, event):
        """
        Move the selected rule from disabled to enabled.
        """
        try:
            index = self.disabled_commands.get_selected_items()[0]
        except IndexError, e:
            print "No selected item to disable:", e
            index = 0
        rule = self.disabled_commands.remove_rule(index)
        self.enable_btn.Enabled = False
        self.enabled_commands.add_rule(rule)
        self.set_form_values(self, rule)

    def OnDisable(self, event):
        """
        Move the selected rule from enabled to disabled.
        """
        try:
            index = self.enabled_commands.get_selected_items()[0]
        except IndexError, e:
            print "No selected item to disable:", e
            index = 0
        rule = self.enabled_commands.remove_rule(index)
        self.disable_btn.Enabled = False
        self.disabled_commands.add_rule(rule)
        self.set_form_values(self, rule)

    def make_bottom(self, pane):
        """
        Create the bottom part of the form*
            - rule name
            - source text
            - target text
        """

        bottom = sc.SizedPanel(pane)
        bottom.SetSizerProps(expand=True)

        fields = editor.FormFields(bottom)

        self.rule_text = fields.rule_text
        self.source_text = fields.source_text
        self.target_text = fields.target_text


    def get_form_values(self, form):
        """
        Get the values from the form as a dict.
        """
        return dict(name=form.rule_text.Value,
                    source=form.source_text.Value,
                    target=form.target_text.Value)

    def set_form_values(self, form, values):
        """
        Set the text in the form fields from a dict object
        """
        form.rule_text.Value = values.get("name", u"")
        form.source_text.Value = values.get("source", u"")
        form.target_text.Value = values.get("target", u"")

    def __getattr__(self, attr):
        """
        Pass on window calls to wrapped window
        """
        return getattr(self.window, attr)

    # `with` support
    def __enter__ (self):
        return self
    def __exit__ (self, type, value, tb):
        pass

def get_controller():
    """
    Get the controller object, wrapping a `RuleManagerDialog`.
    """
    return RuleManagerController(RuleManagerDialog())

def main(language): # pragma: nocover
    """
    Call when run as main.
    """
    try:
        loc.language.change_language(language)
        app = wx.PySimpleApp()
        with get_controller() as dlg:
            if dlg.ShowModal() == wx.ID_OK:
                rules = dlg.get_rules()
                print "OK, saving rules:", rules
                managerules.write_rules(rules)
                return True
            else:
                return False
    except Exception, e:
        print "Exception:", e
        print traceback.print_exc()

if __name__ == "__main__": # pragma: nocover
    main(get_lang(sys.argv))
