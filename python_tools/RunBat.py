#!/usr/bin/env python

import subprocess
import sys

command = sys.argv[-1]

sys.exit(subprocess.Popen(["regsvr32", "/s", r"c:\program files\assistant suite\felix\WordAssist.dll"], shell=False).returncode)

assert command.lower().endswith("bat")

sys.exit(subprocess.Popen([command], shell=False).returncode)
