#include <iostream>
#include <sstream>
#include <string>
#include "map.h"



using namespace std;


int menu() {
    cout << "\n" << endl;
    cout << "======Road Trip Planner 2016======" << endl;
    cout << "1. Print Map" << endl;
    cout << "2. Plan your Route" << endl;
    cout << "3. Quit" << endl;

    int intInput;
    cin >> intInput;
    if(intInput<1||intInput>3) {
        cout << "wrong input, please choose 1-3" << endl;
        cin >> intInput;
    }
    return intInput;
}

int main(int argc, char *argv[])
{
    if(argc!=4) {
        cout << "./program cities.txt adj_list.txt gps.txt" <<endl;
        return 0;
    }
    map m;
    m.graphinit(argv[1], argv[2], argv[3]);

    int start = menu();

    while(start!=-999) {

        if(start==1) {
            m.printGraph();
        }

        if(start==2) {
            string start_dest;
            string str_gps;
            string final_dest;
            double lat1, lng1, lat2, lng2;
            bool bounds = false;
            bool bounds2 = false;
            cin.clear();
            cin.ignore();

            cout << "Give the name of your starting destination: " << endl;
            getline(cin, start_dest);

            if(m.findVertex(start_dest)==NULL) {
                vertex *test = m.findVertex(start_dest);
                while(!bounds) {
                    cout << "Give GPS coordinates in (latitude, longitude) \n ex: 37.54, 90.51" << endl;
                    getline(cin, str_gps, ',');
                    stringstream boosh(str_gps);
                    boosh >> lat1;
                    getline(cin,str_gps);
                    stringstream boosh2(str_gps);
                    boosh2 >> lng1;
                    bounds = true;
                    if((lat1<25||lat1>50)||(lng1<65||lng1>125)) {
                        bounds = false;
                        cout << "Please give coordinates for a location within the contiguous US" << endl;
                    }
                }
            }
            else {
                lat1 = 0;
                lng1 = 0;
            }

            cout << "Give the name of your final destination: " << endl;
            getline(cin, final_dest);

            if(m.findVertex(final_dest)==NULL) {
                while(!bounds2) {
                    cout << "Give GPS coordinates in (latitude, longitude) \n ex: 37.54, 90.51" << endl;
                    getline(cin, str_gps, ',');
                    stringstream boosh3(str_gps);
                    boosh3 >> lat2;
                    getline(cin,str_gps);
                    stringstream boosh4(str_gps);
                    boosh4 >> lng2;
                    bounds = true;
                    if((lat2<25||lat2>50)||(lng2<65||lng2>125)) {
                        bounds = false;
                        cout << "Please give coordinates for a location within the contiguous US" << endl;
                    }
                }
            }
            else {
                    lat2 = 0;
                    lng2 = 0;
            }
            m.pathfinder(start_dest, final_dest, lat1, lat2, lng1, lng2);

        }

        if(start==3) {
            cout<<"Goodbye!"<<endl;
            return 0;
        }

        start = menu();
    }

}
