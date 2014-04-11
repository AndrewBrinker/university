from sys import argv
import os

length = len(argv)
if length > 2:
    print "Usage: python build.py [-t|--make-tests]"
else if length == 2:
    if argv[1] == "-t" || argv[1] == "--make-tests":
        print "Building test code..."
        os.system("iverilog -o test test/test.v")
else:
    print "Building mips..."
    os.system("iverilog -o mips src/mips.v")