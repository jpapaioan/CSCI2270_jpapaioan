#include "map.h"
#include <string>
#include <vector>
#include <stack>
#include "math.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
#define d2r (M_PI / 180.0)

map::map()
{
    //ctor
}

map::~map()
{
    //dtor
}

void map::graphinit(char *file1, char *file2, char *file3) {
    ifstream inFile;
    inFile.open(file1);

    string tmp_city;

    while(getline(inFile, tmp_city, ',')) {
        addVertex(tmp_city);
    }
    inFile.close();

    weighted(file3);

    inFile.open(file2);
    string tmp_weight;
    int int_weight;
    for(int i=0; i<vertices.size(); i++) {
        for(int j=0; j<vertices.size(); j++) {
            getline(inFile,tmp_weight, ',');
            stringstream weight_conv(tmp_weight);
            weight_conv >> int_weight;
            if(int_weight==1) {
                addEdge(vertices[i].name, vertices[j].name, int_weight);
            }
        }
    }
    for(int i=0; i<vertices.size(); i++) {
        double lat1 = vertices[i].GPS[0];
        double long1 = vertices[i].GPS[1];

        for(int j=0; j<vertices[i].adj.size(); j++) {
            double lat2 = vertices[i].adj[j].v->GPS[0];
            double long2 = vertices[i].adj[j].v->GPS[1];
            vertices[i].adj[j].weight = GPStoDist(lat1,lat2,long1,long2);
        }
    }

    inFile.close();
}

void map::addEdge(string v1, string v2, int weight){

    for(int i = 0; i < vertices.size(); i++){
        if(vertices[i].name == v1){
            for(int j = 0; j < vertices.size()-1; j++){
                if(vertices[j].name == v2 && i != j){
                    adjVertex av;
                    av.v = &vertices[j];
                    av.weight = weight;
                    vertices[i].adj.push_back(av);
                    //another vertex for edge in other direction
                    //adjVertex av2;
                    //av2.v = &vertices[i];
                    //av2.weight = weight;
                    //vertices[j].adj.push_back(av2);
                }
            }
        }
    }
}
void map::addVertex(std::string name) {
    vertex v;
    v.name = name;
    vertices.push_back(v);
}

double map::GPStoDist(double lat1, double lat2, double long1, double long2) {
    double dlong = (long2 - long1) * d2r;
    double dlat = (lat2 - lat1) * d2r;
    double a = pow(sin(dlat/2.0), 2) + cos(lat1*d2r) * cos(lat2*d2r) * pow(sin(dlong/2.0), 2);
    double c = 2 * atan2(sqrt(a), sqrt(1-a));
    double d = 3956 * c;

    return d;
}

void map::weighted(char *filename) {
    ifstream inFile;
    inFile.open(filename);

    string tmp_coords;
    double lat;
    double lng;
    for(int i=0; i<vertices.size(); i++){
        getline(inFile,tmp_coords, ',');
        stringstream ss1(tmp_coords);
        ss1 >> lat;
        getline(inFile,tmp_coords);
        stringstream ss2(tmp_coords);
        ss2 >> lng;
        vertices[i].GPS[0] = lat;
        vertices[i].GPS[1] = lng;
    }
}

void map::printGraph() {
    for(int i=0; i<vertices.size(); i++) {
    cout<<vertices[i].name<<"-->";
        for(int j=0; j<vertices[i].adj.size(); j++) {
            cout<<vertices[i].adj[j].v->name;
            if (j != vertices[i].adj.size()-1) {
                cout<<"***";
            }
        }
    cout << "\n" << endl;
    }
}

vertex * map::nearNeighbor(double lat, double lng) {
    vertex *tmp = NULL;
    double dist = 10000;
    double tmp_dist = 10000;
    double lat2, lng2;
    for(int i=0; i<vertices.size(); i++) {
        lat2 = vertices[i].GPS[0];
        lng2 = vertices[i].GPS[1];
        tmp_dist = GPStoDist(lat,lat2,lng,lng2);
        if(tmp_dist < dist) {
            dist = tmp_dist;
            tmp = &vertices[i];
        }
    }
    cout << "closest city: " << tmp->name << "  dist = " << dist << endl;
    offmap[1] = dist;
    return tmp;
}

void map::pathfinder(std::string start_city, std::string final_city, double lat1, double lat2, double lng1, double lng2) {
    cout << "starting from: " << start_city << endl;
    cout << "ending at: " << final_city << endl;
    bool strt = false;
    bool fnl = false;
    vertex *strt_city = NULL;
    vertex *fnl_city = NULL;
    if(findVertex(start_city)==NULL) {
        strt_city = nearNeighbor(lat1, lng1);
        offmap[0] = offmap[1];
    }
    else {
        strt_city = findVertex(start_city);
        offmap[0] = 0;
        strt = true;
    }

    if(findVertex(final_city)==NULL) {
        fnl_city = nearNeighbor(lat2, lng2);
    }
    else {
        fnl_city = findVertex(final_city);
        offmap[1] = 0;
        fnl = true;
    }
    shortestPath(strt_city, fnl_city);
    cout  << endl;
    fnl_city->distance = fnl_city->distance + offmap[0] + offmap[1];
    cout << "total distance of trip: " << fnl_city->distance << endl;
    cout << endl;
    stack <vertex *> pathstack;
    vertex *path = solved.back();

    while(path->parent!=strt_city) {
        pathstack.push(path->parent);
        path = path->parent;
        //cout << "path's parent: " << path->name << endl;
    }
    pathstack.push(path->parent);
    if(strt==true && fnl ==true) {
        while(!pathstack.empty()) {
            path = pathstack.top();
            pathstack.pop();
            cout << path->name << "->";
        }
        cout << fnl_city->name << endl;
    }
    else if(strt==false && fnl == true) {
        cout << start_city << "->";
        while(!pathstack.empty()) {
            path = pathstack.top();
            pathstack.pop();
            cout << path->name << "->";
        }
        cout << fnl_city->name << endl;
    }
    else if(strt==true && fnl ==false) {
        while(!pathstack.empty()) {
            path = pathstack.top();
            pathstack.pop();
            cout << path->name << "->";
        }
        cout << fnl_city->name << "->" << final_city << endl;
    }
    else {
        cout << start_city << "->";
        while(!pathstack.empty()) {
            path = pathstack.top();
            pathstack.pop();
            cout << path->name << "->";
        }
        cout << fnl_city->name << "->" << final_city << endl;
    }
    cout << endl;
    double hours_per_day;
    double tot_dist = fnl_city->distance;
    string tmp_hours;
    cout << "How many hours on average do you plan on driving per day?" << endl;
    getline(cin,tmp_hours);
    stringstream th(tmp_hours);
    th >> hours_per_day;
    double days = tot_dist/(hours_per_day*60.0);
    double days_int,days_frac;
    days_frac = modf(days,&days_int);
    days_frac = days_frac*24;
    int days_frac_int = (int) (days_frac+0.5);
    cout << endl;
    cout << "Estimated travel time: " << days_int << " day(s) and " << days_frac_int << " hour(s)." << endl;
}

void map::shortestPath(vertex *start_city, vertex *final_city) {
    double dist,minDist;
    solved.clear();
    vertex *tmp_parent = NULL;
    vertex *tmp = NULL;
    for(int i=0; i<vertices.size(); i++) {
        vertices[i].solved = false;
    }
    start_city->solved = true;
    start_city->distance = 0;
    solved.push_back(start_city);
    while(!final_city->solved) {
        vertex *solvedV = NULL;
        minDist = 10000000;
        for(int i=0; i<solved.size(); i++) {
            tmp = solved[i];
            for(int j=0; j<tmp->adj.size(); j++) {
                if(!tmp->adj[j].v->solved) {
                    dist = tmp->distance + tmp->adj[j].weight;
                    if(dist < minDist) {
                        solvedV = tmp->adj[j].v;
                        minDist = dist;
                        tmp_parent = tmp;
                    }
                }
            }
        }
        solvedV->distance = minDist;
        solvedV->parent = tmp_parent;
        solvedV->solved = true;
        solved.push_back(solvedV);

    }
}

vertex * map::findVertex(string name) {
    for(int i=0; i<vertices.size(); i++) {
        if(vertices[i].name==name) {
            return &vertices[i];
        }
    }
    return NULL;
}


