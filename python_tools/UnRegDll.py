#!/usr/bin/env python

import subprocess
import sys

command = sys.argv[-1]

sys.exit(subprocess.Popen(["regsvr32", "/u", "/s", command], shell=False).returncode)
