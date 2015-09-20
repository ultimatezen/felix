#coding: UTF8
"""
Create executable MemoryEngine COM server

"""

from distutils.core import setup
import py2exe
import sys
import MemoryEngine
import FelixUtilities
import WordHistory
import ExcelHistory
import os
import modulefinder
import win32com
import wx
from wx import Locale
from FelixPrefEditors import FelixPrefs


origIsSystemDLL = py2exe.build_exe.isSystemDLL
def isSystemDLL(pathname):
        lowerpath = os.path.basename(pathname).lower()
        if lowerpath in ("msvcp71.dll",):
                return 0
        return origIsSystemDLL(pathname)
py2exe.build_exe.isSystemDLL = origIsSystemDLL


for p in win32com.__path__[1:]:
    modulefinder.AddPackagePath("win32com", p)
for extra in ["win32com.shell"]:
    __import__(extra)
    m = sys.modules[extra]
    for p in m.__path__[1:]:
        modulefinder.AddPackagePath(extra, p)

from win32com.shell import shell, shellcon

DESCRIPTION = "Felix Memory Engine COM server"
WX_DIR = os.path.abspath("/Python27/lib/site-packages/wx-2.8-msw-unicode/wx")

MANIFEST_TEMPLATE = '''
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<assembly xmlns="urn:schemas-microsoft-com:asm.v1" manifestVersion="1.0">
<assemblyIdentity
     version="5.0.0.0"
     processorArchitecture="x86"
     name="%(prog)s"
     type="win32"
/>
<description>%(prog)s Program</description>
<dependency>
     <dependentAssembly>
         <assemblyIdentity
             type="win32"
             name="Microsoft.Windows.Common-Controls"
             version="6.0.0.0"
             processorArchitecture="X86"
             publicKeyToken="6595b64144ccf1df"
             language="*"
         />
     </dependentAssembly>
</dependency>
</assembly>
'''

RT_MANIFEST = 24

def CleanDir( DirName ):
    """Remove all prior fields from the dist (DirName) directory"""

    print "-" * 20
    print "Cleaning directory",DirName
    import os
    from os.path import join, getsize
    for root, dirs, files in os.walk(DirName,topdown=False):
        print root, "consumes",
        print sum([getsize(join(root, name)) for name in files]),
        print "bytes in", len(files), "files"
        for name in files:
            os.remove(join(root, name))
        for name in dirs:
            os.rmdir(join(root, name))
    print "-" * 20

class Target(object):
    def __init__(self, **kw):
        self.__dict__.update(kw)
        # for the version info resources (Properties -- Version)
        self.company_name = "Ginstrom IT Solutions (GITS)"
        self.copyright = "(C) Ginstrom IT Solutions"


def main():
    excludes = ["pywin",
                "pywin.debugger",
                "pywin.debugger.dbgcon",
                "pywin.dialogs",
                "pywin.dialogs.list",
                "win32ui",
                "tcl"]

    options = dict( optimize=2,
               dist_dir="MemoryEngineFake",
               excludes=excludes,
               packages="winpaths wx.lib.activex email lxml gzip win32api wx".split(),
               dll_excludes=["POWRPROF.dll"])

    CleanDir("MemoryEngineFake")

    memory_serves_engine = Target(
        description = "Memory Serves COM interface",
        # use module name for win32com exe/dll server
        modules = ["FelixMemoryServes.MemoryEngineFake"],
        # specify which type of com server you want (exe and/or dll)
        create_exe = True,
        create_dll = False,
        version=MemoryEngine.__version__,
        name=MemoryEngine.__progname__,
#        other_resources = [(RT_MANIFEST, 1, MANIFEST_TEMPLATE % dict(prog=MemoryEngine.__progname__))],
        )




    felix_utils = Target(
        description = "Felix Utilities",
        # use module name for win32com exe/dll server
        modules = ["FelixMemoryServes.FelixUtilities"],
        # specify which type of com server you want (exe and/or dll)
        create_exe = True,
        create_dll = False,
        version=FelixUtilities.__version__,
        name=FelixUtilities.__progname__,
#        other_resources = [(RT_MANIFEST, 1, MANIFEST_TEMPLATE % dict(prog=FelixUtilities.__progname__))],
        )


    reg_dll_manifest = MANIFEST_TEMPLATE % dict(prog="RegDll")
    reg_dll = Target(
        script=r"RegDll.py",
#        other_resources=[(RT_MANIFEST, 1, reg_dll_manifest)],
        description="RegDll application",
        name= "RegDll Application v 1.0",
        dest_base="RegDll"
        )
    unreg_dll_manifest = MANIFEST_TEMPLATE % dict(prog="UnRegDll")
    unreg_dll = Target(
        script=r"UnRegDll.py",
 #       other_resources=[(RT_MANIFEST, 1, unreg_dll_manifest)],
        description="UnRegDll application",
        name= "UnRegDll Application v 1.0",
        dest_base="UnRegDll"
        )

    setup(
        name="MemoryEngine",
        # the following two parameters embed support files within exe/dll file
        options={"py2exe":options},
        version=MemoryEngine.__version__,
        description=DESCRIPTION,
        # author, maintainer, contact go here:
        author="Ryan Ginstrom",
        author_email="info@felix-cat.com",
        packages=[],
        windows=[reg_dll,
                 unreg_dll],
        com_server=[memory_serves_engine,
                    felix_utils],
        data_files = [(".", ["MSVCR90.DLL",
                             r"C:\Program Files (x86)\MiKTeX 2.8\miktex\bin\MSVCP90.dll",
                             r"c:\python27\lib\site-packages\wx-2.8-msw-unicode\wx\gdiplus.dll"])]
        )

if __name__ == '__main__':
    if '__file__' in globals():
        os.chdir(os.path.dirname(__file__))

    # If run without args, build executables, in quiet mode.
    if len(sys.argv) == 1:
         sys.argv.append("py2exe")
         sys.argv.append("-q")

    main()
