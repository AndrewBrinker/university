#include <iostream>

#include "../src/vm/VirtualMachine.h"

int main( int argc, char* argv[] )
{
  if( argc != 2 ) {
    std::cerr << "Use: VMTest [obj file]" << std::endl;
    return 1;
  }

  VirtualMachine vm;

  vm.run( argv[1] );

  return 0;
}