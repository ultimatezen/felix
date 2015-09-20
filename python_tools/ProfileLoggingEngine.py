#!/usr/bin/env python
import utils
utils.TESTING = True

from win32com.client import Dispatch

def run_gauntlet(component):
    logger = Dispatch(component)

    logger.LogFileName = u"test.log"

    assert logger.LogFileName.endswith(u"test.log"), logger.LogFileName

    filename = logger.LogFileName

    logger.Debug(component)
    logger.Warn(component)
    logger.Error(component)

    logger.LogFileName = u"foo"

    lines = open(filename).read().splitlines()

    dline, wline, eline = lines

    assert dline.startswith("DEBUG"), dline
    assert dline.endswith(component), dline

    assert wline.startswith("WARN"), wline
    assert wline.endswith(component), wline

    assert eline.startswith("ERROR"), eline
    assert eline.endswith(component), eline

def main():
    run_gauntlet("FelixHelpers.FelixLogger")
    run_gauntlet("FelixHelpers.PowerPointLogger")
    run_gauntlet("FelixHelpers.ExcelLogger")
    run_gauntlet("FelixHelpers.WordLogger")
    
    logger = Dispatch("FelixHelpers.FelixLogger")
    logger.Debug("Debug message")
    logger.Warn("Warning message")
    logger.Error("Error message")
    logger.Report("Japanese", r"C:\Users\RyanVista\AppData\Local\Temp\Felix\v1.4.1-20081218-194851-3300-376.dmp")
    logger.Report("English", r"C:\Users\RyanVista\AppData\Local\Temp\Felix\v1.4.1-20081218-194851-3300-376.dmp")

if __name__ == "__main__":
    main()