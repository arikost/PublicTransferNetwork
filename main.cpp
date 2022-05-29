#include <iostream>
#include <cstring>
#include "NeverLand.h"
#include <memory>
using namespace std;




int main(int argc, char** argv) {
    NeverLand neverLand(argv, argc);
    neverLand.interact();

    return 0;
}
