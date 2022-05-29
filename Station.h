//
// Created by USER on 5/28/2022.
//

#ifndef PUBLICTRANSFERNETWORK_STATION_H
#define PUBLICTRANSFERNETWORK_STATION_H
#include "Transit.h"
#include <vector>

using namespace std;

class Transit;

class Station {
public:
    std::string name;
    std::string type;
    vector<weak_ptr<Transit>> lineOut;
    vector<weak_ptr<Transit>> lineIn;

    Station(string name, string type){
        this->name = name;
        this->type = type;
    }
    void addLineOut(weak_ptr<Transit> line){
        lineOut.push_back(line);
    }
    void addLineIn(weak_ptr<Transit> line){
        lineIn.push_back(line);
    }
};


#endif //PUBLICTRANSFERNETWORK_STATION_H
