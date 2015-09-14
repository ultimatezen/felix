"""
Monitor changes in docs, and rebuild when a py or rst file changes
"""

import os
import subprocess
import time
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler

BASEDIR = os.path.abspath(os.path.dirname(__file__))

def build_docs():
    os.chdir(os.path.join(BASEDIR, "english"))
    subprocess.call("make.bat html")

class ChangeWatcher(FileSystemEventHandler):
    def on_any_event(self, event):
        if event.src_path.endswith(".rst"):
            build_docs()
        elif event.src_path.endswith(".py"):
            build_docs()


if __name__ == "__main__":
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
