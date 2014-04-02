#include <vector>

class VirtualMachine {
public:
	VirtualMachine() :
		VirtualMachine( REG_FILE_SIZE, MEM_SIZE )
	{}

	VirtualMachine( unsigned int reg_file_size, unsigned int mem_size ) :
		r( reg_file_size ),
		mem( mem_size )
	{}

private:
	static const int REG_FILE_SIZE = 4;
	static const int MEM_SIZE = 256;

	std::vector< int > r;
	std::vector< int > mem;
};