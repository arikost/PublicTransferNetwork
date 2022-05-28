//
// Created by USER on 5/28/2022.
//

#ifndef PUBLICTRANSFERNETWORK_STATION_H
#define PUBLICTRANSFERNETWORK_STATION_H

using namespace std;
class Station {
public:
    std::string name;
    std::string type;
    Station(string name, string type){
        this->name = name;
        this->type = type;
    }
};


#endif //PUBLICTRANSFERNETWORK_STATION_H
