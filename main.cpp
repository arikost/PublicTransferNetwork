#include <iostream>
#include <cstring>
#include "NeverLand.h"
#include <memory>
using namespace std;




int main(int argc, char** argv) {
    char** dataFiles;
    char* config_file;
    if(strcmp("config.dat", argv[1]) == 0) {
        config_file = strdup(argv[1]);
        for (int i = 2; i < argc; i++) {
            dataFiles[i] = strdup(argv[i]);
        }
        NeverLand neverLand(config_file, dataFiles, argc-2);
    }
    else{
        for (int i = 1; i < argc; i++) {
            dataFiles[i] = strdup(argv[i]);
        }
        NeverLand neverLand(config_file, dataFiles, argc-1);
    }

    return 0;
}
