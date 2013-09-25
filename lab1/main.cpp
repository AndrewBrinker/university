#include <iostream>
#include <fstream>
#include <string>

using namespace std;

/**
 * Checks whether the file exists
 */
bool file_exists(const char *filename) {
    ifstream file(filename);
    return file;
}


/**
 * Makes the file if it doesn't exist
 */
void create_file(const char *filename) {
    if(!file_exists(filename)) {
        ofstream file(filename);
        file.close();
    }
}


int main() {
    // Get the filename
    string filename = "";
    cout << "Filename: ";
    cin >> filename;
    
    // Makes the file if it doesn't exist and then opens it
    create_file(filename.c_str());
    fstream file(filename.c_str(), ios::in | ios::out);

    // Puts in 'a' 100 times, replacing the 50th 'a' with a 'b'
    for(unsigned int i = 0; i < 100; ++i) {
        file << "a";
    }
    file.seekg(49);
    file.put('b');

    // Closes the file
    file.close();

    return 0;
}
