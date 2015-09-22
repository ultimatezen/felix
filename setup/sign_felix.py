#!/usr/bin/env python

import os
import subprocess
import logging

from FelixAdmin.utils import text2data

HERE = os.path.dirname(os.path.abspath(__file__))

LOGGER = logging.getLogger(__name__)


def run_command(command):
    """Run the specified command using subprocess module"""

    proc = subprocess.Popen(command,
                            shell=True,
                            stdout=subprocess.PIPE,
                            stderr=subprocess.PIPE
                           )

    for line in proc.stdout:
        LOGGER.info(line.strip().decode("sjis"))

    for line in proc.stderr:
        LOGGER.info("ERRROR: %s" % line.strip().decode("sjis"))

    assert not proc.returncode


def sign_file(filename):
    os.chdir(HERE)
    LOGGER.info("Signing %s" % filename)
    run_command(["sign.bat", filename])


def get_installer_filename():
    version_filename = os.path.join(HERE, "version.txt")
    assert os.path.exists(version_filename), version_filename

    versiondata = text2data(version_filename)
    return os.path.join(HERE, "Felix", versiondata["executable"])
