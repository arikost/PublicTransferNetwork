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
public:
    vector<Transit*> lines;
    vector<shared_ptr<Station>> nodes;
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


    void load(ifstream& data, const char* filename){
        char buff[100];
        char station_type[2];
        string *transit_type;
        bool foundFlag = true;
        string filename_str(filename);
        if( filename_str.find("bus") != string::npos){
            transit_type = new string("bus");
        }
        else if( filename_str.find("tram") != string::npos){
            transit_type = new string("tram");
        }
        else if( filename_str.find("rail") != string::npos){
            transit_type = new string("rail");
        }
        else if( filename_str.find("sprinter") != string::npos){
            transit_type = new string("sprinter");
        }
        weak_ptr<Station>from;
        weak_ptr<Station>to;
        while(data >> buff){
            station_type[0] = buff[0];
            station_type[1] = buff[1];

            for(int i=0; i < nodes.size();i++){
                if(strcmp(nodes[i]->name.data(), buff)==0){
                    from = nodes[i];
                    foundFlag = false;
                    break;
                }
            }
            if(foundFlag) {
                if (strcmp(station_type, "IC") == 0) {
                    shared_ptr<Station> sharedPtr = make_shared<Station>(buff, "intercity");
                    nodes.push_back(sharedPtr);
                    from = sharedPtr;
                }
                else if (strcmp(station_type, "CS") == 0) {
                    shared_ptr<Station> sharedPtr = make_shared<Station>(buff, "central");
                    nodes.push_back(sharedPtr);
                    from = sharedPtr;
                } else {
                    shared_ptr<Station> sharedPtr = make_shared<Station>(buff, "stad");
                    nodes.push_back(sharedPtr);
                    from = sharedPtr;
                    cout<< from.lock()->name<<endl;
                }
            }
            if(data >> buff) {
                station_type[0] = buff[0];
                station_type[1] = buff[1];
                int foundStationFlag = 0;
                for(int i=0; i < nodes.size();i++){
                    if(strcmp(nodes[i]->name.data(), buff)==0){
                        to = nodes[i];
                        foundStationFlag = 1;
                        break;
                    }
                }
                if(foundStationFlag == 0) {
                    if (strcmp(station_type, "IC") == 0) {
                        shared_ptr<Station> sharedPtr = make_shared<Station>(buff, "intercity");
                        nodes.push_back(sharedPtr);
                        to = sharedPtr;
                    }
                    else if (strcmp(station_type, "CS") == 0) {
                        shared_ptr<Station> sharedPtr = make_shared<Station>(buff, "central");
                        nodes.push_back(sharedPtr);
                        to = sharedPtr;
                    } else {
                        shared_ptr<Station> sharedPtr = make_shared<Station>(buff, "stad");
                        nodes.push_back(sharedPtr);
                        to = sharedPtr;
                    }
                }
            }else{
                cerr<< "failed to read"<<endl;
            }
            if (data >> buff ){

                for(int i=0; i< lines.size(); i++){
                    if(lines[i]->from.lock()->name == from.lock()->name &&
                        lines[i]->to.lock()->name == to.lock()->name &&
                        *transit_type == lines[i]->type){
                        lines[i]->set_durTime(stoi(buff));
                    }
                }
                lines.push_back(new Transit(from, to, stoi(buff), *transit_type));
            }else{
                cerr<< "failed to read"<<endl;
            }
        }
    }



    void load_config(char* configfileName){
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


    }
    void

    NeverLand(char** dataFiles, int numOfFiles){

        ifstream data;
        bool confFlag = false;
        for(int i=1; i < numOfFiles; i++){
            cout << dataFiles[i]<<endl;
            if( strcmp(dataFiles[i], "-c")==0){
                confFlag = true;
                continue;
            }
            if( confFlag){
                this->load_config(dataFiles[i]);
                confFlag = false;
                continue;
            }
            data.open(dataFiles[i]);
            if(data){
                cout<<"file opened"<<endl;
            }
            this->load(data, dataFiles[i]);
            data.close();
        }
    }
};


#endif //PUBLICTRANSFERNETWORK_NEVERLAND_H
