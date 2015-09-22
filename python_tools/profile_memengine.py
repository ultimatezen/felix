#coding: UTF-8

import cProfile
import pstats
from win32com.client import Dispatch
import MemoryEngine

STAT_FILENAME = 'profile_memengine.stats'

def prof_tm_straight():
    engine = MemoryEngine.FelixRemoteMemory()

    # SAP Japan.ftm
    engine.Connect("http://192.168.56.1:8765/api/mems/1/")
    engine.Login("ryan", "ryan")

    queries = [u"（HTML→XHTML→HTMLというページ遷移にも対応可能とすること）",
               u"それ以下の場合はユーザ確認を行わず取得すること。",
               ]
    for query in queries:
        engine.Search(query, .5)


def prof_tm():
    engine = Dispatch("Felix.RemoteMemory")

    # SAP Japan.ftm
    engine.Connect("http://192.168.56.1:8765/api/mems/1/")
    engine.Login("ryan", "ryan")

    queries = [u"（HTML→XHTML→HTMLというページ遷移にも対応可能とすること）",
               u"それ以下の場合はユーザ確認を行わず取得すること。",
               ]
    for query in queries:
        results = engine.Search(query, .5)
        assert len(results) > 0, results
        print results[0].Source.encode("utf-8")

def prof_gloss_fuzzy():
    engine = Dispatch("Felix.RemoteMemory")

    # SAP Japan.fgloss
    engine.Connect("http://192.168.56.1:8765/api/mems/1/")
    engine.Login("ryan", "ryan")
    queries = [u"（HTML→XHTML→HTMLというページ遷移にも対応可能とすること）",
               u"それ以下の場合はユーザ確認を行わず取得すること。",
               ]
    for query in queries:
        results = engine.Gloss(query, .8)
        assert len(results) > 0, results
        print results[0].Source.encode("utf-8")

def prof_gloss_100():
    engine = Dispatch("Felix.RemoteMemory")

    # SAP Japan.fgloss
    engine.Connect("http://192.168.56.1:8765/api/mems/1/")
    engine.Login("ryan", "ryan")
    queries = [u"（HTML→XHTML→HTMLというページ遷移にも対応可能とすること）",
               u"それ以下の場合はユーザ確認を行わず取得すること。",
               ]
    for query in queries:
        results = engine.Gloss(query, 1)
        assert len(results) > 0, results
        print len(results), "::", results[0].Source.encode("utf-8")

def prof_gloss_100_straight():
    engine = MemoryEngine.FelixRemoteMemory()

    # SAP Japan.fgloss
    engine.Connect("http://192.168.56.1:8765/api/mems/1/")
    engine.Login("ryan", "ryan")
    queries = [u"（HTML→XHTML→HTMLというページ遷移にも対応可能とすること）",
               u"それ以下の場合はユーザ確認を行わず取得すること。",
               ]
    for query in queries:
        engine.Gloss(query, 1)

def run_tests():
    prof_tm()
    prof_gloss_fuzzy()
    prof_gloss_100()
    prof_tm_straight()
    prof_gloss_100_straight()

def main():
    """Run test functions and profile them"""
    cProfile.run('run_tests()', STAT_FILENAME)
    statfile = pstats.Stats(STAT_FILENAME)
    statfile.sort_stats('cumulative').print_stats(30)

if __name__ == "__main__":
    main()
