# coding: UTF8
"""Runs py2exe on MemoryAssist.pyw"""

## setup.py
from distutils.core import setup
import py2exe
import sys
import glob

import psyco
psyco.full()

# If run without args, build executables, in quiet mode.
if len(sys.argv) == 1:
     sys.argv.append("py2exe")
     sys.argv.append("-q")

manifest_template = '''
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
"""Resource ID in Windows executables"""

def CleanDir( DirName ):
    """Remove all prior fields from the dist (DirName) directory"""
    
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
            
        
def getSetupDict( script, app_name, app_version ):
    """Create a dictionary to pass to setup"""
    
    CleanDir( 'build' )
    CleanDir( app_name )
    
    setupDict = {}

    manifestRes = manifest_template % dict(prog=app_name)
    
    wd = {}
    wd['script'] = script
    #wd['icon_resources'] = [(1, iconFile)]
    wd['other_resources'] = [(RT_MANIFEST, 1, manifestRes)]
    wd['description'] = "%s application" % app_name
    wd['dest_base'] = app_name
    wd['version'] = app_version
    wd['company_name'] = "Ginstrom IT Solutions (GITS)"
    wd['copyright'] = "(C) 2006 Ginstrom IT Solutions (GITS)"
    wd['name'] = "%s Application v %s" % (app_name,app_version)
    
    setupDict['windows'] = [wd]
    setupDict['zipfile'] = None
    setupDict['data_files'] = [(".", glob.glob("./*.txt") + glob.glob( "./*.db" ) )]

    excludes = ["pywin", "pywin.debugger", "pywin.debugger.dbgcon",
            "pywin.dialogs", "pywin.dialogs.list", "win32com.server"]
    
    options = {"optimize":2,
             "dist_dir":app_name,
             "excludes":excludes}
    
    setupDict['options'] = {"py2exe":options}
    
    return setupDict

################################################################
# A program using wxPython
import os
os.chdir( os.path.dirname( __file__ ) )

setupDict = getSetupDict( "main.py", "CleanUpOffice", "1.0" )

print "Creating exe..."
setup(
      **setupDict  
     )
print "exe created!"


#command = r'"C:\Program Files\Inno Setup 5\ISCC.exe" "C:\Dev\Setup Scripts\AnalyzeAssist.iss"'
#print "Building setup file..."
#os.system( "buildinstaller.bat" )
#print "Finished building setup file!"

