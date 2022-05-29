//
// Created by USER on 5/28/2022.
//

#ifndef PUBLICTRANSFERNETWORK_NEVERLAND_H
#define PUBLICTRANSFERNETWORK_NEVERLAND_H
#include <vector>
#include <iostream>
#include <fstream>
#include <memory>
#include <algorithm>
#include "Transit.h"
#include "Station.h"
using namespace std;
class NeverLand {
public:
    vector<shared_ptr<Transit>> lines;
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
    string transits[4] = {"bus", "tram", "sprinter", "rail"};

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
        shared_ptr<Transit> current_line;
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
                current_line = make_shared<Transit>(from, to, stoi(buff), *transit_type);
                lines.push_back(current_line);
                from.lock()->addLineOut(current_line);
                to.lock()->addLineIn(current_line);

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
            if(strcmp(buff, "central") == 0){
                config >> buff;
                this->centralChangeTime = stoi(buff);
            }
            if(strcmp(buff, "stad") == 0){
                config >> buff;
                this->stadChangeTime = stoi(buff);
            }
        }


    }
    void interact(){
        char buff[100];
        while(cin >> buff){
            if (strcmp(buff , "load") == 0){
                try{
                    cin >> buff;
                    ifstream data;
                    data.open(buff);
                    this->load(data, buff);
                    data.close();
                }catch(exception e) {
                    cout<< e.what()<<endl;
                }
            }
            else if(strcmp(buff, "outbound") == 0){
                try{
                    cin >> buff;
                    vector<string> connected_stations;
                    bool station_not_exist_flag = true;
                    for(auto &station : nodes){
                        if(strcmp(buff, station->name.data()) == 0){
                            station_not_exist_flag = false;
                            for(auto &transitType : transits) {
                                outbound(station->name, station,transitType, connected_stations);
                                cout << transitType<<":";
                                if(connected_stations.empty()){
                                    cout<<"\tno outbound travel"<<endl;
                                }
                                else{
                                    for(auto &station_name : connected_stations){
                                        cout <<"\t"<<station_name;
                                    }
                                cout<<endl;
                                connected_stations.clear();
                                }
                            }

                            break;
                        }
                    }
                    if(station_not_exist_flag){
                        cerr<< "station :"<< buff<< "\t doesn't exist"<<endl;
                    }
                }catch (exception e){
                    cerr<< e.what()<<endl;
                }
            }
            else if(strcmp(buff, "inbound")==0){
                try{
                    cin >> buff;
                    vector<string> connected_stations;
                    bool station_not_exist_flag = true;
                    for(auto &station : nodes){
                        if(strcmp(buff, station->name.data()) == 0){
                            station_not_exist_flag = false;
                            for(auto &transitType : transits) {
                                inbound(station->name, station,transitType, connected_stations);
                                cout << transitType<<":";
                                if(connected_stations.empty()){
                                    cout<<"\tno outbound travel"<<endl;
                                }
                                else{
                                    for(auto &station_name : connected_stations){
                                        cout <<"\t"<<station_name;
                                    }
                                    cout<<endl;
                                    connected_stations.clear();
                                }
                            }

                            break;
                        }
                    }
                    if(station_not_exist_flag){
                        cerr<< "station :"<< buff<< "\t doesn't exist"<<endl;
                    }
                }catch (exception e){
                    cerr<<e.what()<<endl;
                }
            }
            else if(strcmp(buff, "uniExpress")==0){
                try{
                    cin >> buff;
                    string start_station_name = buff;
                    cin >> buff;
                    string target_station_name = buff;
                    weak_ptr<Station> start_station_ptr;
                    int station_not_exist_flag = 0;
                    for(auto &station : nodes) {
                        if (station->name == start_station_name) {
                            station_not_exist_flag++;
                            start_station_ptr = station;
                        }
                        if(station->name == target_station_name){
                            station_not_exist_flag++;
                        }
                        if(station_not_exist_flag == 2){
                            break;
                        }
                    }
                    int* travel_time = new int(1000000);
                    for(auto &transitType : transits){
                        *travel_time =1000000;
                        uniExpress(start_station_ptr, target_station_name,
                                   start_station_name, transitType, 0, travel_time);
                        if (*travel_time == 1000000){
                            cout<<transitType<<":"<<"\t route unavailable"<<endl;
                        }
                        else{
                            cout<<transitType<<":  "<<*travel_time<<endl;
                        }
                    }
                    delete travel_time;
                    if(station_not_exist_flag != 2){
                        cerr<< "stations :"<<start_station_name<<" or"<<target_station_name << "\t doesn't exist"<<endl;
                    }

                }catch (exception e){
                    cerr<<e.what()<<endl;
                }
            }
            else if(strcmp(buff, "multiExpress")==0){
                try{
                    cin >> buff;
                    string start_station_name = buff;
                    cin >> buff;
                    string target_station_name = buff;
                    weak_ptr<Station> start_station_ptr;
                    int station_not_exist_flag = 0;
                    for(auto &station : nodes) {
                        if (station->name == start_station_name) {
                            station_not_exist_flag++;
                            start_station_ptr = station;
                        }
                        if(station->name == target_station_name){
                            station_not_exist_flag++;
                        }
                        if(station_not_exist_flag == 2){
                            break;
                        }
                    }
                    int* travel_time = new int(1000000);
                    vector<string> route;
                    vector<string> best_route;
                    multiExpress(start_station_ptr,"", target_station_name,
                               start_station_name,&best_route, &route, 0, 0, travel_time);
                    if (*travel_time == 1000000){
                        cout<<"route unavailable"<<endl;
                    }
                    else{
                        cout<<"best route: (time: "<<*travel_time<<")\t"<<start_station_name<<endl;
                        for(int i=0; i< best_route.size() ;i+=3 ){
                            cout<<"transit type: "<< best_route[i]<<"\tto: "<< best_route[i+1]<<"\ttime: "<<best_route[i+2]<<endl;
                        }
                    }
                    delete travel_time;
                    if(station_not_exist_flag != 2){
                        cerr<< "stations :"<<start_station_name<<" or"<<target_station_name << "\t doesn't exist"<<endl;
                    }

                }catch (exception e){
                    cerr<<e.what()<<endl;
                }
            }
            else if(strcmp(buff, "print")==0){
                try{
                    fstream output;
                    output.open("MyOutput.dat");
                    output<<"from\t|\tto\t|\ttransit type\t|\ttravel time\t|"<<endl;
                    for(auto station: nodes){
                        for(auto lineOut: station->lineOut){
                            output<<station->name<<"|\t"<<lineOut.lock()->to.lock()->name<<"|\t"
                            <<lineOut.lock()->type<<"|\t"<<lineOut.lock()->durationTime<<"\n";
                        }
                    }
                    output.close();
                }catch (exception e){
                    cerr<<e.what()<<endl;
                }
            }
            else if(strcmp(buff,"quit")==0){
                cout<<"goodbye"<<endl;
                break;
            }
        }
    }
    void outbound(string check_station_name,weak_ptr<Station> currentStation, string transit_type, vector<string> &connected_stations) {
        for (auto &lineOut : currentStation.lock()->lineOut){
            if(lineOut.lock()->type == transit_type){
                if(lineOut.lock()->to.lock()->name != check_station_name) {
                    if (!(find(connected_stations.begin(), connected_stations.end(), lineOut.lock()->to.lock()->name) != connected_stations.end())) {
                        connected_stations.push_back(lineOut.lock()->to.lock()->name);
                        outbound(check_station_name, lineOut.lock()->to, transit_type, connected_stations);
                    }
                }
            }
        }
    }
    void inbound(string check_station_name,weak_ptr<Station> currentStation, string transit_type, vector<string> &connected_stations){
        for (auto &lineIn : currentStation.lock()->lineIn){
            if(lineIn.lock()->type == transit_type){
                if(lineIn.lock()->from.lock()->name != check_station_name) {
                    if (!(find(connected_stations.begin(), connected_stations.end(), lineIn.lock()->from.lock()->name) != connected_stations.end())) {
                        connected_stations.push_back(lineIn.lock()->from.lock()->name);
                        outbound(check_station_name, lineIn.lock()->from, transit_type, connected_stations);
                    }
                }
            }
        }

    }
    void uniExpress(weak_ptr<Station> currentStation, string targetStation_name, string start_station_name,string transit_type, int current_time, int* bestTime){
        if(currentStation.lock()->name == targetStation_name){
            if(transit_type == "bus"){
                current_time -=  busStoppingTime;
            }
            else if(transit_type == "tram"){
                current_time -=  tramStoppingTime;
            }
            else if(transit_type == "sprinter"){
                current_time -= sprinterStoppingTime;
            }
            else if(transit_type == "rail"){
                current_time -=  railStoppingTime;
            }
            if(current_time < *bestTime){
                *bestTime = current_time;
            }
        }
        else {
            for (auto &lineOut : currentStation.lock()->lineOut) {
                if(lineOut.lock()->type == transit_type &&
                lineOut.lock()->to.lock()->name != start_station_name){
                    if(transit_type == "bus"){
                        current_time += lineOut.lock()->durationTime + busStoppingTime;
                    }
                    else if(transit_type == "tram"){
                        current_time += lineOut.lock()->durationTime + tramStoppingTime;
                    }
                    else if(transit_type == "sprinter"){
                        current_time += lineOut.lock()->durationTime + sprinterStoppingTime;
                    }
                    else if(transit_type == "rail"){
                        current_time += lineOut.lock()->durationTime + railStoppingTime;
                    }
                    uniExpress(lineOut.lock()->to,targetStation_name,
                               start_station_name, transit_type,
                              current_time, bestTime);
                    current_time -= lineOut.lock()->durationTime;
                }
            }
        }
    }


    void multiExpress(weak_ptr<Station> currentStation,string last_transit_type,
                      string targetStation_name, string start_station_name, vector<string> *best_rout,
                      vector<string> *current_route, int current_time, int last_duration, int* bestTime){
        if(currentStation.lock()->name == targetStation_name){
            if(current_time < *bestTime){
                *bestTime = current_time;
                best_rout->clear();
                for(auto &elem: *current_route){
                    best_rout->push_back(elem);
                }
            }
            current_route->pop_back();
            current_route->pop_back();
            current_route->pop_back();
        }else{
            int current_time_in_station;
            for (auto &lineOut : currentStation.lock()->lineOut) {
               if(lineOut.lock()->to.lock()->name != start_station_name &&
               !(find(current_route->begin(), current_route->end(), lineOut.lock()->to.lock()->name) != current_route->end())){
                   current_route->push_back(lineOut.lock()->type);
                   current_route->push_back(lineOut.lock()->to.lock()->name);

                   current_time += lineOut.lock()->durationTime;

                   if(last_transit_type != "" ){
                        if(last_transit_type != lineOut.lock()->type) {
                            if (currentStation.lock()->type == "intercity") {
                                current_time += intercityChangeTime;
                                current_time_in_station = intercityChangeTime;
                            }
                            else if( currentStation.lock()->type == "central"){
                                current_time += centralChangeTime;
                                current_time_in_station = centralChangeTime;
                            }
                            else if(currentStation.lock()->type == "stad"){
                                current_time += stadChangeTime;
                                current_time_in_station = stadChangeTime;
                            }
                        }else{
                            if(last_transit_type == "bus"){
                                current_time += busStoppingTime;
                                current_time_in_station = busStoppingTime;
                            }
                            else if(last_transit_type == "tram"){
                                current_time +=  tramStoppingTime;
                                current_time_in_station = tramStoppingTime;
                            }
                            else if(last_transit_type == "sprinter"){
                                current_time +=  sprinterStoppingTime;
                                current_time_in_station = sprinterStoppingTime;
                            }
                            else if(last_transit_type == "rail"){
                                current_time +=  railStoppingTime;
                                current_time_in_station = railStoppingTime;
                            }
                        }
                   }
                   current_route->push_back(to_string(lineOut.lock()->durationTime + current_time_in_station));
                   multiExpress(lineOut.lock()->to,lineOut.lock()->type,
                                targetStation_name, start_station_name,
                                best_rout, current_route,current_time, lineOut.lock()->durationTime,bestTime);
                   current_time -= (lineOut.lock()->durationTime + current_time_in_station);

               }
            }
        }
    }


    NeverLand(char** dataFiles, int numOfFiles){

        ifstream data;
        bool confFlag = false;
        for(int i=1; i < numOfFiles; i++){
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
            if(!data){
                cout<<"failed to open file:\t"<<dataFiles[i]<<endl;
            }
            this->load(data, dataFiles[i]);
            data.close();
        }
    }
};


#endif //PUBLICTRANSFERNETWORK_NEVERLAND_H
