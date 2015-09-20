#coding: UTF8
'''
Manage rules.

Convert between XML and python representations, filter rules, etc.
'''

from FelixMemoryServes import loc
from lxml import etree
from itertools import tee
import StringIO
import os

KEYS = "name source target sample enabled".split()

def open_file(filename, privs="r"):
    '''
    Opens a file (function is for easy mocking)
    '''
    if not os.path.exists(filename) and "r" in privs:
        return StringIO.StringIO("<rules />")
    return open(filename, privs)

def load(filename=None):
    '''
    Loads the rules
    '''
    if not filename:
        filename = get_rules_filename()
    rulesfile = open_file(filename)
    return nodes2rules(get_rules(rulesfile))

def get_rules_filename():
    '''
    Where is the rules filename located?
    '''
    folder = loc.get_data_folder(__file__)
    return os.path.join(folder, "prefs", "felix_rules.frules")

def get_rules(fileobj):
    '''
    Parse out rules from file-like object
    '''
    return etree.parse(fileobj)

def get_xml_text(root):
    '''
    Convert the XML tree into a string.
    '''

    return etree.tostring(root,
                          pretty_print=True,
                          encoding="utf-8",
                          xml_declaration=True)

def write_rules(rules, filename=None):
    '''
    Write out the rules to a file.
    '''
    nodes = rules2nodes(rules)
    if not filename:
        filename = get_rules_filename()
    rulesfile = open_file(filename, "w")
    write_rule_text(rulesfile, get_xml_text(nodes))


def write_rule_text(out, text):
    '''
    Writes the rule text to `out` (for easy mocking)
    '''
    out.write(text)

def rule2node(rule):
    '''
    Converts a rule dict into an XML node
    '''
    node = etree.Element("rule")
    for key in KEYS:
        etree.SubElement(node, key).text = rule.get(key, u"")
    return node

def rules2nodes(rules):
    '''
    Converts a collection of rules into an XML doc
    '''
    nodes = etree.Element("rules")
    for rule in rules:
        nodes.append(rule2node(rule))
    return nodes

def node2rule(node):
    '''
    Converts an XML node into a rule dict
    '''

    rule = {}
    
    return dict((key, node.find(key).text or u"")
        for key in KEYS)

def nodes2rules(root):
    '''
    Converts an XML doc into a list of rule dicts
    '''
    return [node2rule(node) for node in root.xpath('//rule')] 

def partition_rules(rules):
    '''
    Divides the list of rule dicts into enabled and disabled
    '''
    enabled, disabled = [], []
    for rule in rules:
        if rule.get("enabled", "0") == "1":
            enabled.append(rule)
        else:
            disabled.append(rule)
    
    return enabled, disabled
