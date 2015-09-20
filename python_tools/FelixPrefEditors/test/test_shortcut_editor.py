#!/usr/bin/env python

from FelixPrefEditors import shortcut_editor as se
from lxml import objectify
from lxml import etree
from cStringIO import StringIO

def test_get_shortcut_filename():
    actual = se.get_shortcut_filename("word")
    assert actual == r"C:\dev\python\FelixMemoryServes\prefs\shortcuts_word.xml", actual

def test_get_shortcuts():
    nodes = etree.Element("shortcuts")
    node1 = etree.SubElement(nodes, "shortcut")
    make_command(node1, u"CTRL", u"FooBar", u"A", u"")
    node2 = etree.SubElement(nodes, "shortcut")
    make_command(node2, u"CTRL+Alt", u"BooBaz", u"Z", u"X")
    text = etree.tostring(nodes,
                              pretty_print=True,
                              encoding="utf-8",
                              xml_declaration=True)

    out = StringIO()
    out.write(text)

    out.seek(0)

    shortcuts = se.get_shortcuts(out)

    commands = [dict(type=u"CTRL",
                   command=u"FooBar",
                   first=u"A",
                   second=u""),
                dict(type=u"CTRL+ALT",
                   command=u"Correct Translation",
                   first=u"UP",
                   second=u""),
                dict(type=u"CTRL+Alt",
                   command=u"BooBaz",
                   first=u"Z",
                   second=u"X"),
               ]

    shortcuts = se.root2commands(shortcuts)

    assert commands == shortcuts, (shortcuts, commands)

def test_write_shortcut_text():
    nodes = etree.Element("shortcuts")
    node1 = etree.SubElement(nodes, "shortcut")
    make_command(node1, u"CTRL", u"FooBar", u"A", u"")
    node2 = etree.SubElement(nodes, "shortcut")
    make_command(node2, u"CTRL+Alt", u"BooBaz", u"Z", u"X")
    expected = etree.tostring(nodes,
                              pretty_print=True,
                              encoding="utf-8",
                              xml_declaration=True)

    out = StringIO()
    se.write_shortcut_text(out, se.get_xml_text(nodes))
    actual = out.getvalue()
    assert expected == actual, (expected, actual)

def make_command(node, t, c, f, s):
    etree.SubElement(node, "type").text = t
    etree.SubElement(node, "command").text = c
    etree.SubElement(node, "first").text = f
    etree.SubElement(node, "second").text = s
    return node

def test_keymap():
    for val in se.keyMap.values():
        assert val in se.known_keys, val

def test_node2command():
    node = etree.Element("shortcut")
    make_command(node, u"CTRL", u"FooBar", u"A", u"")

    actual = se.node2command(node)
    expected = dict(type=u"CTRL",
                    command=u"FooBar",
                    first=u"A",
                    second=u"")
    assert actual == expected, (actual, expected)

def test_root2commands():
    nodes = etree.Element("shortcuts")
    node1 = etree.SubElement(nodes, "shortcut")
    make_command(node1, u"CTRL", u"FooBar", u"A", u"")
    node2 = etree.SubElement(nodes, "shortcut")
    make_command(node2, u"CTRL+Alt", u"BooBaz", u"Z", u"X")

    print etree.tostring(nodes, pretty_print=True)
    assert len(se.root2commands(nodes)) == 3, se.root2commands(nodes)

def test_command2node():
    node = etree.Element("shortcut")
    make_command(node, u"CTRL", u"FooBar", u"A", u"")
    expected = etree.tostring(node)

    command = dict(type=u"CTRL",
                   command=u"FooBar",
                   first=u"A",
                   second=u"")

    actual = etree.tostring(se.command2node(command))

    assert expected == actual, (expected, actual)

def test_get_xml_text():
    nodes = etree.Element("shortcuts")
    node1 = etree.SubElement(nodes, "shortcut")
    make_command(node1, u"CTRL", u"FooBar", u"A", u"")
    node2 = etree.SubElement(nodes, "shortcut")
    make_command(node2, u"CTRL+Alt", u"BooBaz", u"Z", u"X")
    expected = etree.tostring(nodes,
                              pretty_print=True,
                              encoding="utf-8",
                              xml_declaration=True)
    actual = se.get_xml_text(nodes)

    assert expected == actual, (actual, expected)

def test_commands2nodes():
    nodes = etree.Element("shortcuts")
    node1 = etree.SubElement(nodes, "shortcut")
    make_command(node1, u"CTRL", u"FooBar", u"A", u"")
    node2 = etree.SubElement(nodes, "shortcut")
    make_command(node2, u"CTRL+Alt", u"BooBaz", u"Z", u"X")
    expected = etree.tostring(nodes,
                              pretty_print=True,
                              encoding="utf-8",
                              xml_declaration=True)

    commands = [dict(type=u"CTRL",
                   command=u"FooBar",
                   first=u"A",
                   second=u""),
                dict(type=u"CTRL+Alt",
                   command=u"BooBaz",
                   first=u"Z",
                   second=u"X")]

    actual = etree.tostring(se.commands2nodes(commands),
                              pretty_print=True,
                              encoding="utf-8",
                              xml_declaration=True)

    assert expected == actual, (expected, actual)

def test_get_shortcuts_for_command():
    commands = [dict(type=u"CTRL",
                   command=u"FooBar",
                   first=u"A",
                   second=u""),
                dict(type=u"CTRL+Alt",
                   command=u"BooBaz",
                   first=u"Z",
                   second=u"X"),
                dict(type=u"CTRL",
                   command=u"BooBaz",
                   first=u"P",
                   second=u"X")]

    expected = commands[1:]
    actual = se.get_shortcuts_for_command(commands, "BooBaz")
    assert expected == actual, (actual, expected)

class TestAddCommand:
    def setup(self):
        self.commands = [dict(type=u"CTRL",
                              command=u"FooBar",
                              first=u"A",
                              second=u""),
                         dict(type=u"CTRL+Alt",
                              command=u"BooBaz",
                              first=u"Z",
                              second=u"X"),
                         dict(type=u"CTRL",
                              command=u"BooBaz",
                              first=u"P",
                              second=u"X")]
    def test_simple(self):
        expected = [{}, {}, {}, {}]
        for e, a in zip(expected, self.commands):
            e.update(a)
        command = dict(type=u"CTRL",
                              command=u"FooBar",
                              first=u"A",
                              second=u"T")
        expected[-1].update(command)
        actual = se.add_command(self.commands, command)
        assert actual == expected, (actual, expected)

    def test_dupe(self):
        expected = [{}, {}, {}, {}]
        for e, a in zip(expected, self.commands):
            e.update(a)
        command = {}
        command.update(self.commands[-1])
        actual = se.add_command(self.commands, command)
        command = {}
        command.update(self.commands[-1])
        actual = se.add_command(actual, command)
        expected[-1].update(command)
        penult = expected[-2]
        for key in u"type first second".split():
            penult[key] = u""

        print actual
        print expected
        assert actual == expected

    def test_flatten(self):
        expected = [{}, {}, {}, {}]
        for e, a in zip(expected, self.commands):
            e.update(a)
        command = dict(type=u"CTRL",
                              command=u"BooBaz",
                              first=u"P",
                              second=u"X")
        expected[-1].update(command)
        third = expected[-2]
        for key in u"type first second".split():
            third[key] = u""
        actual = se.add_command(self.commands, command)
        assert actual == expected, (actual, expected)

def test_get_shortcut_text():
    command = dict(type=u"CTRL",
                   command=u"FooBar",
                   first=u"A",
                   second=u"")
    text = se.get_shortcut_text(command)
    assert text == "CTRL+A", text

def test_get_shortcut_text_second():
    command = dict(type=u"CTRL",
                   command=u"FooBar",
                   first=u"A",
                   second=u"X")
    text = se.get_shortcut_text(command)
    assert text == "CTRL+A, X", text


def test_remove_command():
    commands = [dict(type=u"CTRL",
                   command=u"FooBar",
                   first=u"A",
                   second=u""),
                dict(type=u"CTRL+Alt",
                   command=u"BooBaz",
                   first=u"Z",
                   second=u"X"),
                dict(type=u"CTRL",
                   command=u"BooBaz",
                   first=u"P",
                   second=u"X")]

    expected = [dict(type=u"CTRL",
                   command=u"FooBar",
                   first=u"A",
                   second=u""),
                dict(type=u"CTRL+Alt",
                   command=u"BooBaz",
                   first=u"Z",
                   second=u"X"),
                dict(type=u"",
                   command=u"BooBaz",
                   first=u"",
                   second=u"")]
    actual = se.remove_command(commands, "BooBaz", u"CTRL+P, X")
    assert expected == actual, (actual, expected)
