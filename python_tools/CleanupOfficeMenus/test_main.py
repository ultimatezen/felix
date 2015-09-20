#coding: UTF8
"""
Module description.
"""

from CleanupOfficeMenus import main

class FakeControl:
    def __init__(self, caption):
        self.Caption = caption
        
    def Delete(self):
        pass

class FakeCommandBar:
    def __init__(self, name, controls=None):
        self.Name=name
        if not controls:
            self.Controls = []
        else:
            self.Controls = controls
    
    def Delete(self):
        pass
        
command_bars = []

class FakeWord:
    
    def __init__(self):
        self.CommandBars = command_bars
    
    def Quit(self):
        pass

def fake_get_word():
    return FakeWord()

def fake_get_excel():
    return FakeWord()

def fake_get_ppt():
    return FakeWord()

def setup():
    main.get_word = fake_get_word
    main.get_excel = fake_get_excel
    main.get_ppt = fake_get_ppt

def teardown():
    pass

def test_massage_name():
    
    assert u"transassist" == main.massage_name(u"T&ransAssist"), main.massage_name(u"T&ransAssist")
    assert u"transassist" == main.massage_name(u"TransAssist"), main.massage_name(u"TransAssist")

def test_is_ta_transassist():
    
    assert main.is_ta(u"T&ransAssist")
    assert main.is_ta(u"TransAssist Interface")

def test_is_ta_felix():
    
    assert main.is_ta(u"Feli&x")
    assert main.is_ta(u"Felix Interface")

def test_is_ta_non():
    
    assert not main.is_ta(u"Spam")
    assert not main.is_ta(u"Menu Bar")

def test_get_ta_bars():
    global command_bars
    command_bars = [FakeCommandBar(u"Spam")]
    word = FakeWord()
    
    assert [] == main.get_ta_bars(word)
    
    command_bars = [FakeCommandBar(u"TransAssist Interface")]
    word = FakeWord()
    
    bars = main.get_ta_bars(word)
    assert len(bars) == 1
    assert bars[0].Name == u"TransAssist Interface"
    
def test_get_word_menus():
    global command_bars
    command_bars = [FakeCommandBar(u"Menu Bar")]
    word = FakeWord()
    
    assert [] == main.get_word_ta_menus(word)
    
    controls = [FakeControl(u"&View"),
                FakeControl(u"T&ransAssist"),
                FakeControl(u"&File")]

    command_bars = [FakeCommandBar(u"Spam"),
                    FakeCommandBar(u"TransAssist Interface"),
                    FakeCommandBar(u"Lumberjack"),
                    FakeCommandBar(u"Menu Bar", controls),
                    FakeCommandBar(u"Eggs")]
    word = FakeWord()
    
    menus = main.get_word_ta_menus(word)
    assert len(menus) == 1
    assert menus[0].Caption == u"T&ransAssist"

class TestCleanWord(object):
    
    def test_empty(self):
        global command_bars
        command_bars = [FakeCommandBar("Spam")]
        main.clean_word()
        
    def test_has_toolbar(self):
        global command_bars
        command_bars = [FakeCommandBar(u"TransAssist Interface")]
        main.clean_word()
        
    def test_has_menu(self):
        global command_bars
        controls = [FakeControl(u"T&ransAssist")]
        command_bars = [FakeCommandBar(u"Menu Bar", controls)]
        main.clean_word()

    def test_has_both(self):
        global command_bars
        controls = [FakeControl(u"&View"),
                    FakeControl(u"T&ransAssist"),
                    FakeControl(u"&File")]

        command_bars = [FakeCommandBar(u"Spam"),
                        FakeCommandBar(u"TransAssist Interface"),
                        FakeCommandBar(u"Lumberjack"),
                        FakeCommandBar(u"Menu Bar", controls),
                        FakeCommandBar(u"Eggs")]

        main.clean_word()

    
def test_clean_excel():
    main.clean_excel()
    
def test_clean_ppt():
    main.clean_ppt()
    
def test_main():
    main.main()
    
