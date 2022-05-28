//
// Created by USER on 5/28/2022.
//

#ifndef PUBLICTRANSFERNETWORK_NEVERLAND_H
#define PUBLICTRANSFERNETWORK_NEVERLAND_H
#include <vector>
#include <iostream>
#include <fstream>
#include <memory>
#include "Transit.h"
#include "Station.h"
using namespace std;
class NeverLand {

    vector<Transit*> lines;
    vector<Station*> nodes;
    //stopping times is in minot
    // default stopping times for transit
    int tramStoppingTime = 2 ;
    int busStoppingTime = 1;
    int sprinterStoppingTime = 3;
    int railStoppingTime = 5;
    // default changing times for station
    int intercityChangeTime = 15;
    int centralChangeTime = 10;
    int stadChangeTime = 5;


    NeverLand( char** dataFiles, int numOfFiles){
        ifstream data;
        for(int i=0; i < numOfFiles; i++){
            data.open(dataFiles[i]);
            this->load(data, dataFiles[i]);
            data.close();
        }
    }
    void load(ifstream& data, const char* filename){
        char buff[100];
        char station_type[2];
        string transit_type;
        bool foundFlag = true;
        for(int i=0; i< 10; i++){
            if(filename[i] == '.'){
                break;
            }
            transit_type.push_back(filename[i]);
        }
        Station* from;
        Station* to;
        while(data >> buff){
            station_type[0] = buff[0];
            station_type[1] = buff[1];

            for(int i=0; i < nodes.size();i++){
                if(strcmp(nodes[i]->name, buff)==0){
                    from = nodes[i];
                    foundFlag = false;
                    break;
                }
            }
            if(foundFlag) {
                if (strcmp(station_type, "IC") == 0) {
                    shared_ptr<Station> sharedStation = make_shared<Station>(buff, "intercity");
                    nodes.push_back(sharedStation.get());
                    from = sharedStation.get();
                }
                else if (strcmp(station_type, "CS") == 0) {
                    shared_ptr<Station> sharedStation = make_shared<Station>(buff, "central");
                    nodes.push_back(sharedStation.get());
                    from = sharedStation.get();
                } else {
                    shared_ptr<Station> sharedStation = make_shared<Station>(buff, "stad");
                    nodes.push_back(sharedStation.get());
                    from = sharedStation.get();
                }
            }
            if(data >> buff) { ;
                station_type[0] = buff[0];
                station_type[1] = buff[1];
                int foundStationFlag = 0;
                for(int i=0; i < nodes.size();i++){
                    if(strcmp(nodes[i]->name, buff)==0){
                        to = nodes[i];
                        foundStationFlag = 1;
                        break;
                    }
                }
                if(foundStationFlag == 0) {
                    if (strcmp(station_type, "IC") == 0) {
                        shared_ptr<Station> sharedStation = make_shared<Station>(buff, "intercity");
                        nodes.push_back(sharedStation.get());
                        to = sharedStation.get();
                    }
                    else if (strcmp(station_type, "CS") == 0) {
                        shared_ptr<Station> sharedStation = make_shared<Station>(buff, "central");
                        nodes.push_back(sharedStation.get());
                        to = sharedStation.get();
                    } else {
                        shared_ptr<Station> sharedStation = make_shared<Station>(buff, "stad");
                        nodes.push_back(sharedStation.get());
                        to = sharedStation.get();
                    }
                }
            }else{
                cerr<< "faild to read"<<endl;
            }
            if (data >> buff ){

                for(int i=0; i< lines.size(); i++){
                    if(strcmp(lines[i]->from->name, from->name) == 0 &&
                        strcmp(lines[i]->to->name,to->name) == 0 &&
                        strcmp(transit_type.data(), lines[i]->type) == 0){
                        lines[i]->set_durTime(stoi(buff));
                    }
                }
                lines.push_back(new Transit(from, to, stoi(buff), transit_type.data()));
            }else{
                cerr<< "faild to read"<<endl;
            }
        }
    }


public:
    NeverLand(char* configfileName, char** dataFiles, int numOfFiles){
        ifstream config;
        config.open(configfileName);
        char buff[100];
        while(config >> buff){
            if(strcmp(buff, "bus") == 0){
                config >> buff;
                this->busStoppingTime = stoi(buff);
            }
            if(strcmp(buff, "tram") == 0){
                config >> buff;
                this->tramStoppingTime = stoi(buff);
            }
            if(strcmp(buff, "sprinter") == 0){
                config >> buff;
                this->sprinterStoppingTime = stoi(buff);
            }
            if(strcmp(buff, "rail") == 0){
                config >> buff;
                this->railStoppingTime = stoi(buff);
            }
            if(strcmp(buff, "intercity") == 0){
                config >> buff;
                this->intercityChangeTime = stoi(buff);
            }
            if(strcmp(buff, "centraal") == 0){
                config >> buff;
                this->centralChangeTime = stoi(buff);
            }
            if(strcmp(buff, "stad") == 0){
                config >> buff;
                this->stadChangeTime = stoi(buff);
            }
        }
        ifstream data;
        for(int i=0; i < numOfFiles; i++){
            data.open(dataFiles[i]);
            this->load(data, dataFiles[i]);
            data.close();
        }

    }
};


#endif //PUBLICTRANSFERNETWORK_NEVERLAND_H
