#include <string>
#include <iostream>
#include <cassert>
#include "Sdisk.h"

int main() {
    // Create a new disk called test 1 with
    //   16 blocks of 32 bytes
    Sdisk virtual_disk("test1", 16, 32);

    // Define input blocks
    std::string input_block_1 = "";
    std::string input_block_2 = "";

    // Define output blocks
    std::string output_block_1 = "";
    std::string output_block_2 = "";    

    // Append characters to blocks 1 and 2
    for(unsigned int i = 1; i <= 32; ++i) {
        input_block_1 = input_block_1 + "1";
    }
    for(unsigned int i = 1; i <= 32; ++i) {
        input_block_2 = input_block_2 + "2";
    }

    // Put and get the block, and verify they are identical
    virtual_disk.putblock(4, input_block_1);
    virtual_disk.getblock(4, output_block_1);
    assert(input_block_1 == output_block_1);

    // Put and get the block, and verify they are identical
    virtual_disk.putblock(8, input_block_2);
    virtual_disk.getblock(8, output_block_2);
    assert(input_block_2 == output_block_2);

    std::cout << "All tests passed.";
}
