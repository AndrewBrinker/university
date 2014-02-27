/**
 * Copyright 2014 Andrew Brinker
 */

#include "./preprocessor.h"
#include <string>
#include <cstdio>

namespace gram {

preprocessor::preprocessor(std::string file_name) {
    printf("%s", file_name.c_str());
}

}  // namespace gram
