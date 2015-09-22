#coding: UTF8
"""Unit test the module
"""

import os
import glob
import sys

def coverFromList(files):
    """e.g. [module.py] -> [module]"""
    out = []
    for name in files:
        if not name.lower().startswith('test') and '__init__' not in name:
            module = name.split('.')[0]
            out.append('--cover-package=' + module )
    return out


#Get the packages we want to get coverage for
def getCoverPackages():
    """Retrieve the python files in the current working directory
    that do not start with "test."
    
    These are the files for which we will collect coverage stats
    """

    os.chdir(os.path.dirname(__file__))

    coverPackages = []

    coverPackages += coverFromList(glob.glob("*.py"))
    return [name for name in coverPackages if "setup" not in name]

def main():
    # Run all our tests...
    print >> sys.stderr, "Running unit tests for module"
    import nose

    import psyco
    psyco.full()

    nose.main(argv=['-d', '--with-coverage'] + getCoverPackages() )
    #nose.main(argv=getCoverPackages() )

if __name__=='__main__':
    main()