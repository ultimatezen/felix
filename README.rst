Felix CAT System
================

Felix is a computer assisted translation (CAT) tool for Windows. It is also called a "translation memory" system.

This package consists of the following tools:

- Felix (main program)
- Word Assist (MS Word add-in for Felix)
- Excel Assist (MS Excel add-in for Felix)
- PowerPoint Assist (MS PowerPoint add-in for Felix)

Felix is hosted at http://felix-cat.com/

Build Requirements
------------------

Felix is built using Visual Studio 2013 Community Edition. I plan to migrate to Visual Studio 2015.

Building Felix requires the following separate libraries:

- Windows Template Library (WTL): http://sourceforge.net/projects/wtl/
- Boost: http://www.boost.org/
- Lua: http://www.lua.org/
- LuaBridge: https://github.com/vinniefalco/LuaBridge
- Python: https://www.python.org/

Felix also links to the following libraries:

- SciLexer.dll, from http://www.scintilla.org/
- DbgHelp.dll

Distributing Felix requires the VS 2010 and VS 2013 redistributable packages. It also requires the MSVCP90 and MSVCR90 DLLs for the python components.

Documentation
-------------

The code documentation can be found here (generated using doxygen):
http://felix-cat.com/media/docs/

The user's manual can be found here:
http://felix-cat.com/media/manuals/felix/

(Japanese translation)
http://jp.felix-cat.com/media/manuals/jp/felix/