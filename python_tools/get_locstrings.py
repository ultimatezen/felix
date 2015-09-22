#coding: UTF8
"""
Pulls in localizable strings from python files, translates them,
and places them in stringres.database file.

Strings that have no translations are placed in the to_translate.txt file.

"""

import os
from win32com import client
from pyparsing import quotedString, removeQuotes, SkipTo
import loc
import glob
from cPickle import dump, load
from MemoryServes import mem_parser
import normalizer

MEMORY_FILENAME = r"C:\Users\Ryan\Documents\Translation\Felix\Assistant Suite E2J.ftm"

def get_searchpaths():
    """Retrieve the path that we will search on"""

    return [os.path.dirname(os.path.abspath(__file__)),
           r"C:\dev\python\FelixPrefEditors"]

def get_translatables(searchpath):
    """Scan all source files for localizable strings"""

    loc = "_(" + quotedString.setParseAction(removeQuotes).setResultsName("body") + SkipTo(")") + ")"

    root = mem_parser.get_root(open(MEMORY_FILENAME))
    raw_records = mem_parser.get_records(root)
    records = {}
    for record in raw_records:
        records[normalizer.strip(record["source"])] = normalizer.strip(record["trans"])

    matches = 0
    non_matches = 0

    translatables = {}
    not_translated = []

    for root, folders, filenames in os.walk(searchpath):
        for filename in filenames:
            if filename.lower().endswith(".py") and os.path.basename(filename) != os.path.basename(__file__):
                fullpath = os.path.join(root, filename)
                text = open(fullpath,"r").read()
                for tokens, start, end in loc.scanString(text):
                    word = unicode(tokens.body)
                    if word:
                        if tokens.body in records:
                            matches += 1

                            english = word.replace("\\t", "\t").replace("\\n", "\n").rstrip("\r")
                            japanese = records[word].replace("\\t", "\t").replace("\\n", "\n").rstrip("\r")
                            translatables[english] = dict(en=english,
                                                       ja=japanese)

                        else:
                            print word.encode("utf8")
                            not_translated.append(word.encode("utf8"))
                            non_matches += 1

    print "=" * 30
    print matches, "matches, and", non_matches, "non-matches"

    return translatables, not_translated

def get_all_translatables(spaths):
    translatables = {}
    not_translated = []

    for searchpath in get_searchpaths():

        t, n = get_translatables(searchpath)
        translatables.update(t)
        not_translated.extend(n)

    return translatables, not_translated

def write_files(trans, nottrans, basename):
    basedir = os.path.dirname(os.path.abspath(basename))
    dump(trans, open(os.path.join(basedir,
                                          "prefs",
                                          "stringres.database"), "w"))

    with open(os.path.join(basedir, "to_translate.txt"), "w") as outfile:
        print >> outfile, "\n".join(nottrans)

def main():
    """Run this if called as the main script"""

    translatables, not_translated = get_all_translatables(get_searchpaths())
    write_files(translatables, not_translated, __file__)

if __name__ == '__main__':
    main()
