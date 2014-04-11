from sys import argv
import os
import os.path
import re
import glob
import argparse

#HELP_STRING = "Usage: python build.py [-t | --make-tests] [-h | --help] [-c | --clean]"

def make_tests():
    print "Building test code..."
    success = True
    if not os.path.isdir( 'bin' ):
        os.mkdir( 'bin' )
    for root, dirs, files in os.walk( 'test' ):
        for filename in files:
            match = re.match( "(\w*)_test\.v", filename )
            if match:
                module_name = match.group( 1 )
                sources = glob.glob( "src/{}.v".format( module_name ) )
                if sources:
                    if os.system( "iverilog -o bin/{name}_test test/{name}_test.v {source}".format(
                                    name = module_name,
                                    source = sources[0] ) ) != 0:
                        success = False
                    else:
                        os.symlink( "bin/{}_test".format( module_name ),
                                    "{}_test".format( module_name ) )
                else:
                    print "Could not find src/{}.v".format( module_name )
                    success = False
    if success:
        print "Tests built successfully"
    else:
        try:
            os.rmdir( 'bin' )
        except OSError: # Directory is non-empty, so something built successfully
            pass

def make_clean():
    print "Cleaning..."
    binaries = glob.glob( "bin/*" )
    binaries += glob.glob( "*_test" )
    if len( binaries ) == 0:
        print "No binaries to clean"
        return
    for binary in binaries:
        os.remove( binary )
    os.rmdir( "bin" )
    print "Cleaned successfully"

#if __name__ == '__main__':
#    if len( argv ) > 2:
#        print HELP_STRING
#    elif len( argv ) == 2:
#        if argv[1] == "-t" or argv[1] == "--make-tests":
#            make_tests()
#        if argv[1] == "-h" or argv[1] == "--help":
#            print HELP_STRING
#        if argv[1] == "-c" or argv[1] == "--clean":
#            make_clean()
#    else:
#        print "Building mips..."
#        os.system("iverilog -o mips src/mips.v")
#        print "Built successfully"
if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument( "-c", "--clean", help = "clean binaries",
                         action = "store_true" )
    args = parser.parse_args()
    if args.clean:
        make_clean()
    else:
        make_tests()