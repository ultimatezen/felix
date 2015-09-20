import fakewx
import fakewx.lib

import sys
sys.modules['wx'] = fakewx
sys.modules['wx.lib'] = fakewx.lib
from mock import Mock

from RuleManager import editor
import unittest

class TestFormFields(unittest.TestCase):
    def test_widgets_created(self):
        pane = Mock()
        form = editor.FormFields(pane)

        assert getattr(form, "rule_text")
        assert getattr(form, "source_text")
        assert getattr(form, "target_text")

class TestMain(unittest.TestCase):
    def test_init(self):
        dlg = editor.RuleCreatorDialog(None)
        controller = editor.RuleEditorController(dlg)
        assert controller
    
    def test_get_rule_editor(self):
        dlg = editor.get_rule_editor(None)
        assert dlg, dlg

    def test_get_rule_creator(self):
        dlg = editor.get_rule_creator(None)
        assert dlg, dlg


    def test_window_widgets_created(self):
        rule = editor.get_rule_editor(None)
        assert getattr(rule, "rule_text")
        assert getattr(rule, "source_text")
        assert getattr(rule, "target_text")

        assert getattr(rule, "sample_text")
        assert getattr(rule, "test_button")
        assert getattr(rule, "output_text")

    def test_get_form_values(self):
        rule = editor.get_rule_editor(None)

        rule.rule_text.Value = "aaa"
        rule.source_text.Value = "bbb"
        rule.target_text.Value = "ccc"
        rule.sample_text.Value = "ddd"

        expected = dict(name="aaa",
                        source="bbb",
                        target="ccc",
                        sample="ddd")

        actual = rule.get_form_values(rule)

        assert actual == expected, actual

    def test_set_form_values(self):
        rule = editor.get_rule_editor(None)

        values = dict(name=u"aaa-x",
                      source=u"bbb-x",
                      target=u"ccc-x",
                      sample=u"ddd-x")

        rule.set_form_values(rule, values)

        assert rule.rule_text.Value == "aaa-x", rule.rule_text.Value
        assert rule.source_text.Value == "bbb-x", rule.source_text.Value
        assert rule.target_text.Value == "ccc-x", rule.target_text.Value
        assert rule.sample_text.Value == "ddd-x", rule.sample_text.Value

    def test_set_form_values_missing_keys(self):
        rule = editor.get_rule_editor(None)

        values = {}

        rule.set_form_values(rule, values)

        assert rule.rule_text.Value == "", rule.rule_text.Value
        assert rule.source_text.Value == "", rule.source_text.Value
        assert rule.target_text.Value == "", rule.target_text.Value
        assert rule.sample_text.Value == "", rule.sample_text.Value

    def test_set_form_values_clears_output(self):
        rule = editor.get_rule_editor(None)

        rule.output_text.Value = u"some text"

        values = dict(name=u"aaa-x",
                      source=u"bbb-x",
                      target=u"ccc-x",
                      sample=u"ddd-x")

        rule.set_form_values(rule, values)

        assert rule.output_text.Value == "", rule.output_text.Value

    def test_on_test(self):
        rule = editor.get_rule_editor(None)

        values = dict(name=u"",
                      source=u"foo",
                      target=u"bar",
                      sample=u"I foo you!")

        rule.set_form_values(rule, values)
        rule.output_text.Value = u""
        
        rule.OnTest(None)
        
        assert rule.output_text.Value == u"I bar you!", rule.output_text.Value
        
