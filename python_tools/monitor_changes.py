"""
Monitor changes in docs, and rebuild when a py or rst file changes
"""

import os
import subprocess
import time
import logging

from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler


BASEDIR = os.path.dirname(os.path.abspath(__file__))
logging.basicConfig(level=logging.DEBUG)
logger = logging.getLogger(__name__)


class Globals:
    LAST = None
    WAIT = 5  # seconds


def build_docs():  # pragma no cover
    """
    Build Sphinx documentation.
    """
    logger.info("Building docs")
    os.chdir(os.path.join(BASEDIR, "docs"))
    subprocess.call("make.bat html")


def run_tests():  # pragma no cover
    """
    Run unit tests.
    """

    current = time.time()
    if not Globals.LAST or (current - Globals.LAST) > Globals.WAIT:

        logger.info("Running unit tests")
        os.chdir(BASEDIR)
        subprocess.call("nosetests --with-doctest --with-cov --cov-report html --cov-config .coveragerc")
        Globals.LAST = time.time()


class ChangeWatcher(FileSystemEventHandler):  # pragma no cover
    """
    Watches folder for changes.
    """

    def on_any_event(self, event):
        if event.src_path.endswith(".rst"):
            build_docs()
        elif event.src_path.endswith(".py"):
            run_tests()


def main():  # pragma no cover
    logger.info("Monitoring %s for changes" % BASEDIR)
    run_tests()

    event_handler = ChangeWatcher()
    observer = Observer()
    observer.schedule(event_handler, path='.', recursive=True)
    observer.start()
    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        observer.stop()
    observer.join()


if __name__ == "__main__":  # pragma: no cover
    main()