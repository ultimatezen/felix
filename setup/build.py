# coding: UTF8
"""
Build the installer program

Calls buildinstaller.bat to get the job done.

"""

from datetime import date
import datetime
import glob
import logging
import os
import shutil
import subprocess
import sys
import time

import win32api
from jinja2 import Template

import sign_felix

HERE = os.path.dirname(os.path.abspath(__file__))
PARENT = os.path.dirname(HERE)

EXE_FILENAME = os.path.join(PARENT, "Felix", "Unicode_Release", "Felix.exe")
EXCEL_FILENAME = os.path.join(PARENT, "ExcelAssist", "ExcelAssist", "Release", "ExcelAssist.dll")
PPT_FILENAME = os.path.join(PARENT, "PowerPointAssist", "PowerPointAssist", "Release", "PowerPointAssist.dll")
WORD_FILENAME = os.path.join(PARENT, "WordAssist", "WordAssist", "Release", "WordAssist.dll")
HTML_DIR = os.path.join(PARENT, "Felix")

LOGGER = logging.getLogger(__name__)
LOGGER.setLevel(logging.DEBUG)

ch = logging.StreamHandler(sys.stdout)
ch.setLevel(logging.DEBUG)
formatter = logging.Formatter('%(asctime)s :: %(levelname)s :: %(message)s')
ch.setFormatter(formatter)
LOGGER.addHandler(ch)


def debug():
    """
    For debugging until script is working
    """
    LOGGER.info('HERE: %s', HERE)
    LOGGER.info('PARENT: %s', PARENT)
    LOGGER.info('EXE: %s', EXE_FILENAME)
    LOGGER.info('EXCEL: %s', EXCEL_FILENAME)
    LOGGER.info('PPT: %s', PPT_FILENAME)
    LOGGER.info('WORD: %s', WORD_FILENAME)


def get_now():
    """For time-stamping purposes"""

    format = "%Y/%m/%d %H:%M:%S"
    return datetime.datetime(*time.localtime()[:6]).strftime(format)


def write_version_file(data):
    LOGGER.info('Writing version file with data %s', data)
    ver_out = open(os.path.join(HERE, "version.txt"), "w")
    for pair in data.items():
        print >> ver_out, "%s=%s" % pair
    ver_out.close()


def get_felix_version():
    info = win32api.GetFileVersionInfo(EXE_FILENAME, "\\")
    ms = info['FileVersionMS']
    ls = info['FileVersionLS']
    nums = win32api.HIWORD(ms), win32api.LOWORD(ms), win32api.HIWORD(ls), win32api.LOWORD(ls)
    while nums and not nums[-1]:
        nums = nums[:-1]
    version = '.'.join([str(n) for n in nums])
    LOGGER.info("version: %s" % version)
    return version


def get_installer_name(version):
    return r"Felix_Setup_%s.exe" % version


def run_command(command):
    data = subprocess.call(command)
    LOGGER.info(data)


def make_setup_script(data):
    text = open(os.path.join(HERE, "FelixSetup.tpl")).read()
    template = Template(text)
    out = open(os.path.join(HERE, "FelixSetup.iss"), "w")
    out.write(template.render(data).encode("utf-8"))
    out.close()


def get_data():
    version = get_felix_version()
    data = dict(version=version,
                build_date=date.today(),
                executable=get_installer_name(version),
                felix_dir=os.path.dirname(EXE_FILENAME),
                html_dir=HTML_DIR,
                base_dir=PARENT,
                redist_dir=r'D:\dev\cpp\Common DLLs',
                word_dir=os.path.dirname(WORD_FILENAME),
                excel_dir=os.path.dirname(EXCEL_FILENAME),
                ppt_dir=os.path.dirname(PPT_FILENAME),
                mem_engine_dir=r'D:\dev\python\FelixMemoryServes\MemoryEngine')
    print "App data:", data
    return data


def make_html_files(data):
    for lang in "en jp".split():
        outdir = r"D:\dev\Cpp\Assistant Suite\Felix\html\%s" % lang
        htmlfiles = glob.glob("html-templates/%s/*.html" % lang)
        for htmlfile in htmlfiles:
            template = Template(open(htmlfile).read().decode("utf-8"))
            base = os.path.split(htmlfile)[-1]
            LOGGER.info("creating: html/%s/%s" % (lang, base))
            out = open(os.path.join(outdir, base), "w")
            out.write(template.render(data).encode("utf-8"))
            out.close()


def build_docs():
    print "Building docs..."
    os.chdir(r"D:\dev\manuals\english")
    subprocess.call("make clean")
    os.chdir(r"D:\dev\manuals\english")
    subprocess.call("make html")
    os.chdir(r"D:\dev\manuals\english")
    subprocess.call("make htmlhelp")
    sys.stdout.flush()


def sign_exes():
    sign_felix.sign_file(EXE_FILENAME)
    sign_felix.sign_file(EXCEL_FILENAME)
    sign_felix.sign_file(PPT_FILENAME)
    sign_felix.sign_file(WORD_FILENAME)


def create_installer(data):

    os.chdir(HERE)
    make_setup_script(data)
    write_version_file(data)
    make_html_files(data)
    LOGGER.info("*" * 30)
    LOGGER.info("Building setup file...")
    run_command("buildinstaller.bat")
    LOGGER.info("Finished building setup file!")
    LOGGER.info("*" * 30)
    LOGGER.info("")


def sign_installer():
    # allow file to be released so it can be signed
    time.sleep(1)
    sign_felix.sign_file(sign_felix.get_installer_filename())


def main(args):
    """Builds the installer by calling a .bat file"""

    if "debug" in args:
        debug()

    if "docs" in args or "all" in args:
        build_docs()

    if "sign" in args or "all" in args:
        sign_exes()

    data = get_data()

    if "installer" in args or "all" in args:
        create_installer(data)

        sign_installer()

    return data


def copy_installer(data):
    if not data:
        LOGGER.info('No installer data')
        return

    LOGGER.info("Copying version file...")
    version_source = os.path.join(HERE, "version.txt")
    version_dest = os.path.join(HERE, "Felix", "version.txt")
    shutil.copy(version_source, version_dest)

    LOGGER.info("... Copy finished")


if __name__ == '__main__':
    data = main(sys.argv[:])
    copy_installer(data)
