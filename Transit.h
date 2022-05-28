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
    weak_ptr<Station> from;
    weak_ptr<Station> to;
    string type;

    void set_durTime(int t){
        if (t < 0){
            cerr<< "duration time cannot be lower then 0"<<endl;
        }
        if(t < this->durationTime){
            this->durationTime = t;
        }
    }


    Transit(weak_ptr<Station> from, weak_ptr<Station> to, int dTime, string type){
        if (dTime < 0){
            cerr<< "duration time cannot be lower then 0"<<endl;
        }

        this->from = from;
        this->to = to;
        this->durationTime = dTime;
        this->type = type;
    }
};


#endif //PUBLICTRANSFERNETWORK_TRANSIT_H
