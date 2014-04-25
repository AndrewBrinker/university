from os import system
from sys import argv

if __name__ == "__main__":
    if len(argv) > 2:
        print "Usage: python build.py [clean]"
    elif len(argv) == 2:
        if argv[1] == "clean":
            system("rm pipeline")
        else:
            print "Usage: python build.py [clean]"
    else:
        system("iverilog -o pipeline src/pipeline.v")