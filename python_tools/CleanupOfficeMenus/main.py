#coding: UTF8
"""
Module description.
"""

from win32com import client

def get_word():
    return client.Dispatch("Word.Application")
def get_excel():
    return client.Dispatch("Excel.Application")
def get_ppt():
    return client.Dispatch("PowerPoint.Application")

def massage_name(name):
    return name.replace("&", "").lower()

def is_ta(name):
    return u"transassist" in massage_name(name) or u"felix" in massage_name(name)

def get_ta_bars(component):
    """Retrieve TransAssist toolbars from component"""

    return [bar for bar in component.CommandBars if is_ta(bar.Name)]

def get_word_ta_menus(word):
    """Retrieve any and all menus from menu bar with "TransAssist" in them"""

    menus = []
    for bar in word.CommandBars:
        if u"Menu Bar" == bar.Name:
            for control in bar.Controls:
                if is_ta(control.Caption):
                    menus.append(control)
                    
    return menus

def get_excel_ta_menus(excel):
    menus = []
    for bar in excel.CommandBars:
        if u"Worksheet Menu Bar" == bar.Name:
            for control in bar.Controls:
                if is_ta(control.Caption):
                    menus.append(control)

    return menus

def delete_ta_bars(component):
    """Delete the TransAssist toolbars from the component"""
    
    toolbars = get_ta_bars(component)
    for bar in toolbars:
        bar.Delete()
    

def clean_word():
    """Remove the TransAssist toolbar and menu from Word, if any"""

    word = None

    try:
        word = get_word()
    
        delete_ta_bars(word)

        menus = get_word_ta_menus(word)
        for menu in menus:
            menu.Delete()
    finally:
        if word:
            word.Quit()
    
def clean_excel():
    """Remove the TransAssist toolbar and menu from Excel, if any"""

    excel = None

    try:
        excel = get_excel()
    
        delete_ta_bars(excel)

        menus = get_excel_ta_menus(excel)
        for menu in menus:
            menu.Delete()
    finally:
        if excel:
            excel.Quit()

def clean_ppt():
    """Remove the TransAssist toolbar and menu from PowerPoint, if any"""

    ppt = None

    try:
        ppt = get_ppt()

        delete_ta_bars(ppt)

        menus = get_word_ta_menus(ppt)
        for menu in menus:
            menu.Delete()
    finally:
        if ppt:
            ppt.Quit()

def main():
    try:
        clean_word()
    except:
        print "Failed to clean word"
    try:
        clean_excel()
    except:
        print "Failed to clean excel"
    try:
        clean_ppt()
    except:
        print "Failed to clean powerpoint"

if __name__ == "__main__":
    main()