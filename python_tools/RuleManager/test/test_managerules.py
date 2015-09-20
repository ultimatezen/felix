from RuleManager import managerules
import unittest
from lxml import etree, objectify
from cStringIO import StringIO
from FelixMemoryServes import loc
import os

class TestGetFilenames(unittest.TestCase):
    def test_data_folder_frozen(self):
        expected = r"C:\Users\Ryan\AppData\Local\Felix"
        actual = loc.get_data_folder_frozen(__file__)
        assert actual == expected, actual

    def test_data_folder_thawed(self):
        expected = r"C:\dev\python\RuleManager"
        actual = loc.module_path_thawed(os.path.split(__file__)[0])
        assert actual == expected, actual
        
    def test_get_rules_filename(self):
        expected = r"C:\dev\python\RuleManager\prefs\felix_rules.frules"
        actual = managerules.get_rules_filename()
        assert actual == expected, actual

class TestGetRules(unittest.TestCase):
    def test_simple(self):
        expected = [dict(name='my rule',
                    source='X',
                    target='Y',
                    sample='X marks the spot',
                    enabled='1'),
                    dict(name='my rule',
                    source='X',
                    target='Y',
                    sample='',
                    enabled='0')]
        nodes = '''<rules>
<rule>
  <name>my rule</name>
  <source>X</source>
  <target>Y</target>
  <sample>X marks the spot</sample>
  <enabled>1</enabled>
</rule>
<rule>
  <name>my rule</name>
  <source>X</source>
  <target>Y</target>
  <sample></sample>
  <enabled>0</enabled>
</rule>
</rules>
'''

        xmlnodes = managerules.get_rules(StringIO(nodes))
        actual = managerules.nodes2rules(xmlnodes)
        assert actual == expected, actual

class TestRule2Node(unittest.TestCase):
    def test_empty(self):
        rule = {}
        node = managerules.rule2node(rule)
        expected = '''<rule>
  <name></name>
  <source></source>
  <target></target>
  <sample></sample>
  <enabled></enabled>
</rule>
'''
        actual = etree.tostring(node, pretty_print=True)
        assert actual == expected, actual

    def test_simple(self):
        rule = dict(name='my rule',
                    source='X',
                    target='Y',
                    sample='X marks the spot',
                    enabled='1')
        node = managerules.rule2node(rule)
        expected = '''<rule>
  <name>my rule</name>
  <source>X</source>
  <target>Y</target>
  <sample>X marks the spot</sample>
  <enabled>1</enabled>
</rule>
'''
        actual = etree.tostring(node, pretty_print=True)
        assert actual == expected, actual


    def test_no_sample(self):
        rule = dict(name='my rule',
                    source='X',
                    target='Y',
                    enabled='0')
        node = managerules.rule2node(rule)
        expected = '''<rule>
  <name>my rule</name>
  <source>X</source>
  <target>Y</target>
  <sample></sample>
  <enabled>0</enabled>
</rule>
'''
        actual = etree.tostring(node, pretty_print=True)
        assert actual == expected, actual

class TestRules2Nodes(unittest.TestCase):

    def test_simple(self):
        rules = [dict(name='my rule',
                    source='X',
                    target='Y',
                    sample='X marks the spot',
                    enabled='1'),
                dict(name='my rule',
                    source='X',
                    target='Y',
                    enabled='0')]
        nodes = managerules.rules2nodes(rules)
        expected = '''<rules>
  <rule>
    <name>my rule</name>
    <source>X</source>
    <target>Y</target>
    <sample>X marks the spot</sample>
    <enabled>1</enabled>
  </rule>
  <rule>
    <name>my rule</name>
    <source>X</source>
    <target>Y</target>
    <sample></sample>
    <enabled>0</enabled>
  </rule>
</rules>
'''
        actual = etree.tostring(nodes, pretty_print=True)
        assert actual == expected, actual


class TestNode2Rule(unittest.TestCase):
    def test_empty(self):
        expected = dict(name='',
                    source='',
                    target='',
                    sample='',
                    enabled='')
        node = etree.fromstring('''<rule>
  <name></name>
  <source></source>
  <target></target>
  <sample></sample>
  <enabled></enabled>
</rule>
''')
        actual = managerules.node2rule(node)
        assert actual == expected, actual

    def test_simple(self):
        expected = dict(name='my rule',
                    source='X',
                    target='Y',
                    sample='X marks the spot',
                    enabled='1')
        node = etree.fromstring('''<rule>
  <name>my rule</name>
  <source>X</source>
  <target>Y</target>
  <sample>X marks the spot</sample>
  <enabled>1</enabled>
</rule>
''')
        actual = managerules.node2rule(node)
        assert actual == expected, actual


    def test_no_sample(self):
        expected = dict(name='my rule',
                    source='X',
                    target='Y',
                    sample='',
                    enabled='0')
        node = etree.fromstring('''<rule>
  <name>my rule</name>
  <source>X</source>
  <target>Y</target>
  <sample></sample>
  <enabled>0</enabled>
</rule>
''')
        actual = managerules.node2rule(node)
        assert actual == expected, actual

class TestWriteRules(unittest.TestCase):
    def setUp(self):
        self.fileobj = StringIO()
        self.old_open = managerules.open_file
        managerules.open_file = lambda x, y : self.fileobj
    
    def tearDown(self):
        managerules.open_file = self.old_open

    def test_one(self):
        rules = [dict(name='my rule',
                    source='X',
                    target='Y',
                    sample='X marks the spot',
                    enabled='1')]
        expected = '''<?xml version='1.0' encoding='utf-8'?>
<rules>
  <rule>
    <name>my rule</name>
    <source>X</source>
    <target>Y</target>
    <sample>X marks the spot</sample>
    <enabled>1</enabled>
  </rule>
</rules>
'''
        managerules.write_rules(rules)
        actual = self.fileobj.getvalue()
        assert actual == expected, actual

    def test_one_missing_fields(self):
        rules = [dict()]
        expected = '''<?xml version='1.0' encoding='utf-8'?>
<rules>
  <rule>
    <name></name>
    <source></source>
    <target></target>
    <sample></sample>
    <enabled></enabled>
  </rule>
</rules>
'''
        managerules.write_rules(rules)
        actual = self.fileobj.getvalue()
        assert actual == expected, actual


    def test_two(self):
        rules = [dict(name='rule 1',
                    source='A',
                    target='B',
                    sample='From A to ?',
                    enabled='1'),
                 dict(name='rule 2',
                    source='X',
                    target='Y',
                    sample='From X to ?',
                    enabled='0')]
        expected = '''<?xml version='1.0' encoding='utf-8'?>
<rules>
  <rule>
    <name>rule 1</name>
    <source>A</source>
    <target>B</target>
    <sample>From A to ?</sample>
    <enabled>1</enabled>
  </rule>
  <rule>
    <name>rule 2</name>
    <source>X</source>
    <target>Y</target>
    <sample>From X to ?</sample>
    <enabled>0</enabled>
  </rule>
</rules>
'''
        managerules.write_rules(rules)
        actual = self.fileobj.getvalue()
        assert actual == expected, actual

class TestNodes2Rules(unittest.TestCase):

    def test_empty(self):
        expected = [dict(name='',
                    source='',
                    target='',
                    sample='',
                    enabled=''),
                    dict(name='',
                    source='',
                    target='',
                    sample='',
                    enabled='')]
        nodes = etree.fromstring('''<rules>
<rule>
  <name></name>
  <source></source>
  <target></target>
  <sample></sample>
  <enabled></enabled>
</rule>
<rule>
  <name></name>
  <source></source>
  <target></target>
  <sample></sample>
  <enabled></enabled>
</rule>
</rules>
''')
        actual = managerules.nodes2rules(nodes)
        assert actual == expected, actual

    def test_simple(self):
        expected = [dict(name='my rule',
                    source='X',
                    target='Y',
                    sample='X marks the spot',
                    enabled='1'),
                    dict(name='my rule',
                    source='X',
                    target='Y',
                    sample='',
                    enabled='0')]
        node = etree.fromstring('''<rules>
<rule>
  <name>my rule</name>
  <source>X</source>
  <target>Y</target>
  <sample>X marks the spot</sample>
  <enabled>1</enabled>
</rule>
<rule>
  <name>my rule</name>
  <source>X</source>
  <target>Y</target>
  <sample></sample>
  <enabled>0</enabled>
</rule>
</rules>
''')
        actual = managerules.nodes2rules(node)
        assert actual == expected, actual

class TestPartitionRules(unittest.TestCase):
    def test_one_each(self):
        rules =[dict(name='my rule',
                     source='X',
                     target='Y',
                     sample='X marks the spot',
                     enabled='1'),
                dict(name='my rule',
                     source='X',
                     target='Y',
                     sample='',
                     enabled='0')]
        
        enabled, disabled = managerules.partition_rules(rules)
        
        assert enabled == [rules[0]], enabled
        assert disabled == [rules[1]], disabled