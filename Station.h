//
// Created by USER on 5/28/2022.
//

#ifndef PUBLICTRANSFERNETWORK_STATION_H
#define PUBLICTRANSFERNETWORK_STATION_H


class Station {
public:
    char* name;
    char* type;
    Station(char* name, const char *type){
        this->name = strdup(name);
        this->type = strdup(type);
    }
};


#endif //PUBLICTRANSFERNETWORK_STATION_H
