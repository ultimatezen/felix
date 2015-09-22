import fakewx
import fakewx.lib

from cStringIO import StringIO
import sys
sys.modules['wx'] = fakewx
sys.modules['wx.lib'] = fakewx.lib

from RuleManager import main, managerules
import unittest

class TestRulesPane(unittest.TestCase):
    def test_add_rule(self):
        pane = main.RulesPane(None, "", [], None, "foo")
        assert not pane.rules, pane.rules
        
        pane.add_rule(dict(name="foo"))
        assert pane.rules == [dict(name="foo")], pane.rules

    def test_remove_rule(self):
        rules = [dict(name=x) for x in list("abcdef")]
        pane = main.RulesPane(None, "", rules, None, "foo")
        
        pane.remove_rule(3)
        expected = [dict(name=x) for x in list("abcef")]
        assert pane.rules == expected, pane.rules

    def test_add_rules(self):
        rules = [dict(name=x, source=x, target=x) for x in list("abcdef")]
        pane = main.RulesPane(None, "", [], None, "foo")
        pane.add_rules(rules)
        assert pane.rules == rules, pane.rules

    def test_add_rules_dupes(self):
        rules = [dict(name=x, source=x, target=x) for x in list("abcdef")]
        pane = main.RulesPane(None, "", rules, None, "foo")
        rules2 = [dict(name=x, source=x, target=x) for x in list("abcdef")]
        pane.add_rules(rules2)
        assert len(pane.rules) == len("abcdef"), pane.rules
        assert pane.rules == rules, pane.rules

    def test_add_rules_one_dupe(self):
        rules = [dict(name=x, source=x, target=x) for x in list("abc")]
        pane = main.RulesPane(None, "", rules, None, "foo")
        rules2 = [dict(name=x, source=x, target=x) for x in list("cde")]
        pane.add_rules(rules2)
        assert len(pane.rules) == len("abcde"), pane.rules
        assert pane.rules == rules, pane.rules


class TestMain(unittest.TestCase):
    def test_init(self):
        dlg = main.get_controller()
        assert dlg

    def test_widgets_created_top(self):
        dlg = main.get_controller()

        assert getattr(dlg, "disabled_commands")

        assert getattr(dlg, "enable_btn")
        assert getattr(dlg, "diable_btn")

        assert getattr(dlg, "enabled_commands")

    def test_widgets_created_middle(self):
        dlg = main.get_controller()

        assert getattr(dlg, "add_btn")

        assert getattr(dlg, "remove_btn")
        assert getattr(dlg, "edit_btn")

    def test_widgets_created_bottom(self):
        dlg = main.get_controller()

        assert getattr(dlg, "rule_text")
        assert getattr(dlg, "source_text")
        assert getattr(dlg, "target_text")

    def test_get_form_values(self):
        dlg = main.get_controller()

        dlg.rule_text.Value = "aaa"
        dlg.source_text.Value = "bbb"
        dlg.target_text.Value = "ccc"

        expected = dict(name="aaa",
                        source="bbb",
                        target="ccc")

        actual = dlg.get_form_values(dlg)

        assert actual == expected, actual

    def test_set_form_values(self):
        dlg = main.get_controller()

        values = dict(name=u"aaa-x",
                      source=u"bbb-x",
                      target=u"ccc-x")

        dlg.set_form_values(dlg, values)

        assert dlg.rule_text.Value == "aaa-x", rule.rule_text.Value
        assert dlg.source_text.Value == "bbb-x", rule.source_text.Value
        assert dlg.target_text.Value == "ccc-x", rule.target_text.Value

class TestLoadRules(unittest.TestCase):
    def setUp(self):
        self.fileobj = None
        self.old_open = managerules.open_file
        managerules.open_file = self.fake_open_file
    
    def tearDown(self):
        managerules.open_file = self.old_open

    def fake_open_file(self, filename):
        print "Fake open file:", filename
        return self.fileobj

    def test_1_enabled(self):
        self.fileobj = StringIO('''<rules>
<rule>
  <name>my rule</name>
  <source>X</source>
  <target>Y</target>
  <sample>X marks the spot</sample>
  <enabled>1</enabled>
</rule>
</rules>
''')
        dlg = main.get_controller()
        dlg = main.get_controller()
        rules = dlg.get_rules()
        print rules
        act, inact = managerules.partition_rules(rules)
        assert len(inact) == 0, inact
        assert len(act) == 1, act

    def test_1_disabled(self):
        self.fileobj = StringIO('''<rules>
<rule>
  <name>my rule</name>
  <source>X</source>
  <target>Y</target>
  <sample>X marks the spot</sample>
  <enabled>0</enabled>
</rule>
</rules>
''')
        dlg = main.get_controller()
        rules = dlg.get_rules()
        print rules
        act, inact = managerules.partition_rules(rules)
        assert len(inact) == 1, inact
        assert len(act) == 0, act

    def test_1_each(self):
        self.fileobj = StringIO('''<rules>
<rule>
  <name>my rule</name>
  <source>X</source>
  <target>Y</target>
  <sample>X marks the spot</sample>
  <enabled>1</enabled>
</rule>
<rule>
  <name>my disabled rule</name>
  <source>X</source>
  <target>Y</target>
  <sample>X marks the spot</sample>
  <enabled>0</enabled>
</rule>
</rules>
''')
        dlg = main.get_controller()
        rules = dlg.get_rules()
        print "rules from get_rules:", rules
        act, inact = managerules.partition_rules(rules)
        assert len(inact) == 1, inact
        assert len(act) == 1, act
