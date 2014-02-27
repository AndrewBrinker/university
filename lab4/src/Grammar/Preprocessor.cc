
#include "./Preprocessor.h"
#include <string>
#include <cstdio>

namespace grammar {

Preprocessor::Preprocessor(std::string file_name) {
    printf("%s", file_name.c_str());
}

}