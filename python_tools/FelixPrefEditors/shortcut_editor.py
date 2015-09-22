#coding: UTF8
"""
Edit list of shortcutiations for Word/Ppt

"""
import wx
from wx.lib import sized_controls as sc
from lxml import etree
from FelixMemoryServes import utils, loc
from FelixMemoryServes.loc import _
import string
import os
import sys

known_keys = set(["LBUTTON",
	"RBUTTON",
	"CANCEL",
	"MBUTTON",
	"XBUTTON1",
	"XBUTTON2",
	"BACK",
	"TAB",
	"CLEAR",
	"RETURN",
	"SHIFT",
	"CONTROL",
	"MENU",
	"PAUSE",
	"CAPITAL",
	"KANA",
	"HANGEUL",
	"HANGUL",
	"JUNJA",
	"FINAL",
	"HANJA",
	"KANJI",
	"ESCAPE",
	"CONVERT",
	"NONCONVERT",
	"ACCEPT",
	"MODECHANGE",
	"SPACE",
	"PRIOR",
	"NEXT",
	"END",
	"HOME",
	"LEFT",
	"UP",
	"RIGHT",
	"DOWN",
	"SELECT",
	"PRINT",
	"EXECUTE",
	"SNAPSHOT",
	"INSERT",
	"DELETE",
	"HELP",
	"LWIN",
	"RWIN",
	"APPS",
	"SLEEP",
	"NUMPAD0",
	"NUMPAD1",
	"NUMPAD2",
	"NUMPAD3",
	"NUMPAD4",
	"NUMPAD5",
	"NUMPAD6",
	"NUMPAD7",
	"NUMPAD8",
	"NUMPAD9",
	"MULTIPLY",
	"ADD",
	"SEPARATOR",
	"SUBTRACT",
	"DECIMAL",
	"DIVIDE",
	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"F11",
	"F12",
	"F13",
	"F14",
	"F15",
	"F16",
	"F17",
	"F18",
	"F19",
	"F20",
	"F21",
	"F22",
	"F23",
	"F24",
	"NUMLOCK",
	"SCROLL",
	"OEM_NEC_EQUAL",
	"OEM_FJ_JISHO",
	"OEM_FJ_MASSHOU",
	"OEM_FJ_TOUROKU",
	"OEM_FJ_LOYA",
	"OEM_FJ_ROYA",
	"LSHIFT",
	"RSHIFT",
	"LCONTROL",
	"RCONTROL",
	"LMENU",
	"RMENU",
	"BROWSER_BACK",
	"BROWSER_FORWARD",
	"BROWSER_REFRESH",
	"BROWSER_STOP",
	"BROWSER_SEARCH",
	"BROWSER_FAVORITES",
	"BROWSER_HOME",
	"VOLUME_MUTE",
	"VOLUME_DOWN",
	"VOLUME_UP",
	"MEDIA_NEXT_TRACK",
	"MEDIA_PREV_TRACK",
	"MEDIA_STOP",
	"MEDIA_PLAY_PAUSE",
	"LAUNCH_MAIL",
	"LAUNCH_MEDIA_SELECT",
	"LAUNCH_APP1",
	"LAUNCH_APP2",
	"OEM_1",
	"OEM_PLUS",
	"OEM_COMMA",
	"OEM_MINUS",
	"OEM_PERIOD",
	"OEM_2",
	"OEM_3"])

keyMap = {
    wx.WXK_BACK : "BACK",
    wx.WXK_TAB : "TAB",
    wx.WXK_RETURN : "RETURN",
    wx.WXK_ESCAPE : "ESCAPE",
    wx.WXK_SPACE : "SPACE",
    wx.WXK_DELETE : "DELETE",
    wx.WXK_START : "HOME",
    wx.WXK_LBUTTON : "LBUTTON",
    wx.WXK_RBUTTON : "RBUTTON",
    wx.WXK_CANCEL : "CANCEL",
    wx.WXK_MBUTTON : "MBUTTON",
    wx.WXK_CLEAR : "CLEAR",
    wx.WXK_SHIFT : "SHIFT",
    wx.WXK_MENU : "MENU",
    wx.WXK_PAUSE : "PAUSE",
    wx.WXK_CAPITAL : "CAPITAL",
    #wx.WXK_PRIOR : "PRIOR",
    #wx.WXK_NEXT : "NEXT",
    wx.WXK_END : "END",
    wx.WXK_HOME : "HOME",
    wx.WXK_LEFT : "LEFT",
    wx.WXK_UP : "UP",
    wx.WXK_RIGHT : "RIGHT",
    wx.WXK_DOWN : "DOWN",
    wx.WXK_SELECT : "SELECT",
    wx.WXK_PRINT : "PRINT",
    wx.WXK_EXECUTE : "EXECUTE",
    wx.WXK_SNAPSHOT : "SNAPSHOT",
    wx.WXK_INSERT : "INSERT",
    wx.WXK_HELP : "HELP",
    wx.WXK_NUMPAD0 : "NUMPAD0",
    wx.WXK_NUMPAD1 : "NUMPAD1",
    wx.WXK_NUMPAD2 : "NUMPAD2",
    wx.WXK_NUMPAD3 : "NUMPAD3",
    wx.WXK_NUMPAD4 : "NUMPAD4",
    wx.WXK_NUMPAD5 : "NUMPAD5",
    wx.WXK_NUMPAD6 : "NUMPAD6",
    wx.WXK_NUMPAD7 : "NUMPAD7",
    wx.WXK_NUMPAD8 : "NUMPAD8",
    wx.WXK_NUMPAD9 : "NUMPAD9",
    wx.WXK_MULTIPLY : "MULTIPLY",
    wx.WXK_ADD : "ADD",
    wx.WXK_SEPARATOR : "SEPARATOR",
    wx.WXK_SUBTRACT : "SUBTRACT",
    wx.WXK_DECIMAL : "DECIMAL",
    wx.WXK_DIVIDE : "DIVIDE",
    wx.WXK_F1 : "F1",
    wx.WXK_F2 : "F2",
    wx.WXK_F3 : "F3",
    wx.WXK_F4 : "F4",
    wx.WXK_F5 : "F5",
    wx.WXK_F6 : "F6",
    wx.WXK_F7 : "F7",
    wx.WXK_F8 : "F8",
    wx.WXK_F9 : "F9",
    wx.WXK_F10 : "F10",
    wx.WXK_F11 : "F11",
    wx.WXK_F12 : "F12",
    wx.WXK_F13 : "F13",
    wx.WXK_F14 : "F14",
    wx.WXK_F15 : "F15",
    wx.WXK_F16 : "F16",
    wx.WXK_F17 : "F17",
    wx.WXK_F18 : "F18",
    wx.WXK_F19 : "F19",
    wx.WXK_F20 : "F20",
    wx.WXK_F21 : "F21",
    wx.WXK_F22 : "F22",
    wx.WXK_F23 : "F23",
    wx.WXK_F24 : "F24",
    wx.WXK_NUMLOCK : "NUMLOCK",
    wx.WXK_SCROLL : "SCROLL",
    wx.WXK_PAGEUP : "PRIOR",
    wx.WXK_PAGEDOWN : "NEXT",
    wx.WXK_NUMPAD_SPACE : "SPACE",
    wx.WXK_NUMPAD_TAB : "TAB",
    wx.WXK_NUMPAD_ENTER : "RETURN",
    wx.WXK_NUMPAD_F1 : "F1",
    wx.WXK_NUMPAD_F2 : "F2",
    wx.WXK_NUMPAD_F3 : "F3",
    wx.WXK_NUMPAD_F4 : "F4",
    wx.WXK_NUMPAD_HOME : "HOME",
    wx.WXK_NUMPAD_LEFT : "LEFT",
    wx.WXK_NUMPAD_UP : "UP",
    wx.WXK_NUMPAD_RIGHT : "RIGHT",
    wx.WXK_NUMPAD_DOWN : "DOWN",
    #wx.WXK_NUMPAD_PRIOR : "PRIOR",
    wx.WXK_NUMPAD_PAGEUP : "PRIOR",
    #wx.WXK_NUMPAD_NEXT : "NEXT",
    wx.WXK_NUMPAD_PAGEDOWN : "NEXT",
    wx.WXK_NUMPAD_END : "END",
    wx.WXK_NUMPAD_INSERT : "INSERT",
    wx.WXK_NUMPAD_DELETE : "DELETE",
    # wx.WXK_NUMPAD_EQUAL : "EQUAL",
    wx.WXK_NUMPAD_MULTIPLY : "MULTIPLY",
    wx.WXK_NUMPAD_ADD : "ADD",
    wx.WXK_NUMPAD_SEPARATOR : "SEPARATOR",
    wx.WXK_NUMPAD_SUBTRACT : "SUBTRACT",
    wx.WXK_NUMPAD_DECIMAL : "DECIMAL",
    wx.WXK_NUMPAD_DIVIDE : "DIVIDE",

    #wx.WXK_WINDOWS_LEFT : "WINDOWS_LEFT",
    #wx.WXK_WINDOWS_RIGHT : "WINDOWS_RIGHT",
    #wx.WXK_WINDOWS_MENU : "WINDOWS_MENU",

    # wx.WXK_COMMAND : "COMMAND",

}


def get_shortcut_filename(prog):
    folder = loc.get_data_folder()
    return os.path.join(folder, "prefs", "shortcuts_%s.xml" % prog)

def get_shortcuts(fileobj):

    return etree.parse(fileobj)

def get_xml_text(root):

    return etree.tostring(root,
                          pretty_print=True,
                          encoding="utf-8",
                          xml_declaration=True)

def write_shortcut_text(out, text):
    out.write(text)

def command2node(command):
    node = etree.Element("shortcut")
    for key in u"type command first second".split():
        etree.SubElement(node, key).text = command[key]
    return node

def commands2nodes(commands):
    nodes = etree.Element("shortcuts")
    for command in commands:
        nodes.append(command2node(command))
    return nodes

def node2command(node):
    return dict((key, node.find(key).text or u"")
        for key in u"type first second command".split())

def root2commands(root):
    nodes = root.findall("shortcut")
    commands = {}
    commands[u'Correct Translation'] = dict(type=u'CTRL+ALT',
                                            first=u'UP',
                                            second=u'',
                                            command=u'Correct Translation')
    for node in nodes:
        command = node2command(node)
        commands[command['command']] = command
    return commands.values()

def get_shortcuts_for_command(commands, name):
    return [x for x in commands if x["command"] == name]

def get_shortcut_text(command):
    text = command["type"] + u"+" + command["first"]
    if command["second"]:
        text += u", " + command["second"]
    return text

def get_command(commands, name, shortcut):
    for c in commands:
        if c["command"] == name and shortcut == get_shortcut_text(c):
            return c

def remove_command(commands, name, shortcut):
    command = get_command(commands, name, shortcut)
    for key in u"type first second".split():
        command[key] = u""
    out = []
    for c in commands:
        if not c in out:
            out.append(c)
    return out

def is_same_command(c1, c2):
    for key in u"first second type".split():
        if c1[key] != c2[key]:
            return False
    return True

def add_command(commands, command):
    for c in commands:
        if is_same_command(c, command):
            for key in u"first second type".split():
                c[key] = u""
    commands.append(command)
    out = []
    for c in commands:
        if not c in out:
            out.append(c)
    return out

class ShortcutEditorDialog(sc.SizedDialog):
    """Main frame for the application"""
    def __init__(self, prog, parent=None):
        prog_names = dict(word=_("Word"),
                          excel=_("Excel"),
                          ppt=_("PowerPoint"))
        title = _("Edit shortcuts for Felix in %s") % prog_names[prog.lower()]
        pos = (10, 10)
        sc.SizedDialog.__init__(self, None, wx.NewId(), title,
                                size=(700,500),
                                style=wx.DEFAULT_DIALOG_STYLE | wx.RESIZE_BORDER | wx.STAY_ON_TOP)
        pane = self.GetContentsPane()

        top = sc.SizedPanel(pane)
        top.SetSizerType("horizontal")
        top.SetSizerProps(expand=True, proportion=2)

        # commands
        cmd_pane = sc.SizedPanel(top)
        cmd_pane.SetSizerProps(expand=True, proportion=1)
        wx.StaticText(cmd_pane, label=_("Commands"))

        style = wx.LC_SINGLE_SEL | wx.LC_REPORT | wx.LC_NO_HEADER
        self.command_list = wx.ListCtrl(cmd_pane, -1, style=style)
        self.command_list.InsertColumn(0, _("Command Name"))
        self.command_list.SetSizerProps(expand=True, proportion=2)

        # shortcuts
        shortcut_pane = sc.SizedPanel(top)
        shortcut_pane.SetSizerProps(expand=True, proportion=1)
        wx.StaticText(shortcut_pane, label=_("Shortcuts"))

        self.shortcut_list = wx.ListCtrl(shortcut_pane, -1, style=style)
        self.shortcut_list.InsertColumn(0, _("Shortcuts"))
        self.shortcut_list.SetSizerProps(expand=True, proportion=2)

        self.remove_btn = wx.Button(shortcut_pane, -1, _("&Remove"))
        self.remove_btn.Enabled = False

        # bottom pane
        row = sc.SizedPanel(pane)
        row.SetSizerType("grid", {"cols":4})
        row.SetSizerProps(expand=True, proportion=1, border=(["top"], 20))

        # row1
        wx.StaticText(row, label=_("Combo"))
        wx.StaticText(row, label=_("First Key"))
        wx.StaticText(row, label=_("Second Key"))
        wx.StaticText(row, label=u"")

        # row2
        self.key_combo = wx.ComboBox(row, -1, u"", (90,50), (100,-1), u"CTRL ALT CTRL+ALT".split())
        self.key_combo.SetSizerProps(expand=False, proportion=2)

        self.first_key = wx.TextCtrl(row)
        self.first_key.SetSizerProps(expand=False, proportion=3)
        self.first_key.MinSize = (120, 25)

        self.second_key = wx.TextCtrl(row)
        self.second_key.SetSizerProps(expand=False, proportion=3)
        self.second_key.MinSize = (120, 25)

        box = sc.SizedPanel(row)
        box.SetSizerProps(expand=True, proportion=1)
        self.assign_btn = wx.Button(box, -1, _("&Assign"))
        self.assign_btn.Enabled = False
        self.clear_btn = wx.Button(box, -1, _("&Clear"))

        # load commands
        root = get_shortcuts(open(get_shortcut_filename(prog)))
        self.commands = root2commands(root)

        command_names = set(x['command'] for x in self.commands)
        command_names = sorted(command_names, key=lambda k : k.lower())

        for command in command_names:
            self.command_list.InsertStringItem(sys.maxint, command)

        self.command_list.SetColumnWidth(0, wx.LIST_AUTOSIZE_USEHEADER)

        # add dialog buttons
        sizer = self.CreateStdDialogButtonSizer(wx.OK | wx.CANCEL)
        sizer.CancelButton.Label = _("Cancel")
        sizer.AffirmativeButton.Label = _("OK")
        self.SetButtonSizer(sizer)

        self.MinSize = (500, 400)
        self.Fit()
        self.bind_commands()


    def check_enable_assign(self):
        if not self.command_list.GetSelectedItemCount():
            self.assign_btn.Enabled = False
            return
        if not self.first_key.Value:
            self.assign_btn.Enabled = False
            return
        if self.key_combo.Value not in u"CTRL ALT CTRL+ALT".split():
            self.assign_btn.Enabled = False
            return
        self.assign_btn.Enabled = True

    def get_key(self, evt):
        keycode = evt.KeyCode
        if keycode in keyMap:
            return keyMap[keycode]
        return unichr(evt.UnicodeKey).upper()

    def bind_commands(self):
        # keys
        self.Bind(wx.EVT_COMBOBOX, self.OnComboBox, self.key_combo)
        self.first_key.Bind(wx.EVT_CHAR, self.OnFirstKey)
        self.second_key.Bind(wx.EVT_CHAR, self.OnSecondKey)

        # assign/clear
        self.Bind(wx.EVT_BUTTON, self.OnAssign, self.assign_btn)
        self.Bind(wx.EVT_BUTTON, self.OnClear, self.clear_btn)

        # remove button
        self.Bind(wx.EVT_BUTTON, self.OnRemoveShortcut, self.remove_btn)

        # command events
        self.Bind(wx.EVT_LIST_ITEM_DESELECTED,
                  self.OnCommandDeselected,
                  self.command_list)
        self.Bind(wx.EVT_LIST_ITEM_SELECTED,
                  self.OnCommandSelected,
                  self.command_list)
        #shortcut events
        self.Bind(wx.EVT_LIST_ITEM_SELECTED,
                  self.OnShortcutSelected,
                  self.shortcut_list)
        self.Bind(wx.EVT_LIST_ITEM_DESELECTED,
                  self.OnShortcutDeselected,
                  self.shortcut_list)
        self.Bind(wx.EVT_LIST_ITEM_ACTIVATED,
                  self.OnShortcutActivated,
                  self.shortcut_list)

        self.Bind(wx.EVT_INIT_DIALOG, self.OnInit, self)

    def OnInit(self, event):
        if self.IsIconized():
            self.Iconize(False)
        if not self.IsShown():
            self.Show(True)
        self.Raise()

    def OnComboBox(self, evt):
        self.check_enable_assign()

    def OnFirstKey(self, evt):
        self.first_key.Value = self.get_key(evt)
        self.check_enable_assign()

    def OnSecondKey(self, evt):
        self.second_key.Value = self.get_key(evt)

    def OnAssign(self, evt):
        cmd_index = self.command_list.GetFirstSelected()
        item = self.command_list.GetItem(cmd_index)

        command = dict(command=item.Text,
                       type=self.key_combo.Value,
                       first=self.first_key.Value,
                       second=self.second_key.Value)

        self.commands = add_command(self.commands, command)
        commands = get_shortcuts_for_command(self.commands, item.Text)
        self.fill_shortcut_list(commands)
        self.clear_key()

    def clear_key(self):
        self.first_key.Value = u""
        self.second_key.Value = u""
        self.key_combo.Value = u""
        self.assign_btn.Enabled = False

    def OnClear(self, evt):
        self.clear_key()

    def OnRemoveShortcut(self, evt):
        shortcut_index = self.shortcut_list.GetFirstSelected()
        shortcut = self.shortcut_list.GetItem(shortcut_index)
        cmd_index = self.command_list.GetFirstSelected()
        command = self.command_list.GetItem(cmd_index)

        self.commands = remove_command(self.commands,
                                       command.Text,
                                       shortcut.Text)

        self.shortcut_list.DeleteItem(shortcut_index)

    def fill_shortcut_list(self, commands):
        self.shortcut_list.DeleteAllItems()
        for command in commands:
            if command["first"]:
                text = get_shortcut_text(command)
                self.shortcut_list.InsertStringItem(sys.maxint, text)
        self.shortcut_list.SetColumnWidth(0, wx.LIST_AUTOSIZE_USEHEADER)
        self.remove_btn.Enabled = False

    # commands
    def OnCommandSelected(self, evt):
        item = evt.Item
        commands = get_shortcuts_for_command(self.commands, item.Text)
        self.fill_shortcut_list(commands)
        self.check_enable_assign()
    def OnCommandDeselected(self, evt):
        self.shortcut_list.DeleteAllItems()
        self.remove_btn.Enabled = False

    # shortcuts
    def OnShortcutSelected(self, evt):
        self.remove_btn.Enabled = True
    def OnShortcutDeselected(self, evt):
        self.remove_btn.Enabled = False
    def OnShortcutActivated(self, evt):
        self.remove_btn.Enabled = True

def main(prog=None, language=None):
    if not prog:
        prog = "word"
    if not language:
        language = "English"

    loc.language.change_language(language)
    app = wx.PySimpleApp()
    with ShortcutEditorDialog(prog) as dlg:
        if dlg.ShowModal() == wx.ID_OK:
            text = get_xml_text(commands2nodes(dlg.commands))
            write_shortcut_text(open(get_shortcut_filename(prog), "w"), text)
            return True
        else:
            return False

def get_prog_and_lang(args):
    language = "English"
    if args[-1] in "English Japanese".split():
            language = args[-1]
            prog = args[-2]
    else:
        prog = args[-1]

    if not prog in "word excel ppt".split():
        prog = "word"

    return prog, language

if __name__ == "__main__":
    prog, language = get_prog_and_lang(sys.argv)
    main(prog, language)
