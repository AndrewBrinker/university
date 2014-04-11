from sys import argv
import os

HELP_STRING = "Usage: python build.py [-t | --make-tests] [-h | --help]"

length = len(argv)
if length > 2:
    print HELP_STRING
elif length == 2:
    if argv[1] == "-t" or argv[1] == "--make-tests":
        print "Building test code..."
        os.system("iverilog -o testbench test/test.v")
        print "Built successfully"
    if argv[1] == "-h" or argv[1] == "--help":
        print HELP_STRING
else:
    print "Building mips..."
    os.system("iverilog -o mips src/mips.v")
    print "Built successfully"
