#!/usr/bin/env python

from subprocess import call
import os

class cd:
    def __init__(self, newPath):
        self.newPath = os.path.expanduser(newPath)

    def __enter__(self):
        self.savedPath = os.getcwd()
        os.chdir(self.newPath)

    def __exit__(self, etype, value, traceback):
        os.chdir(self.savedPath)

print("Rinobot Lib Compiler")
print("")
toolchain = "local"
#raw_input("What is your toolchain name? ")

print("Setting up enviroment")
if(os.path.exists(".qi") == False):
    cmd = "qibuild init"
    call(cmd.split(" "))

print("Compiling RinoLib")
with cd("RinoLib"):
    cmd = "qibuild configure -c "+toolchain
    call(cmd.split(" "))
    cmd = "qibuild make -c "+toolchain
    call(cmd.split(" "))
    cmd = "qibuild package -c "+toolchain
    call(cmd.split(" "))

cmd = "qitoolchain add-package package/RinoLib-0.1-"+toolchain+".zip -c "+toolchain
call(cmd.split(" "))

print("Done")