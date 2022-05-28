//
// Created by USER on 5/28/2022.
//

#ifndef PUBLICTRANSFERNETWORK_TRANSIT_H
#define PUBLICTRANSFERNETWORK_TRANSIT_H

#include "Station.h"
using namespace std;
class Transit {
public:
    int durationTime;
    Station* from;
    Station* to;
    char* type;

    void set_durTime(int t){
        if (t < 0){
            cerr<< "duration time cannot be lower then 0"<<endl;
        }
        if(t < this->durationTime){
            this->durationTime = t;
        }
    }


    Transit(Station* from, Station* to, int dTime, const char* type){
        if (dTime < 0){
            cerr<< "duration time cannot be lower then 0"<<endl;
        }

        this->from = from;
        this->to = to;
        this->durationTime = dTime;
        this->type = strdup(type);
    }
};


#endif //PUBLICTRANSFERNETWORK_TRANSIT_H
