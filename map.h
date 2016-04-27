#ifndef MAP_H
#define MAP_H
#include <vector>
#include <string>

struct vertex;

struct adjVertex{
    vertex *v;
    double weight;
};


struct vertex{
    int ID;
    vertex *parent;
    std::string name;
    bool solved;
    double distance;
    double GPS[2];
    std::vector<adjVertex> adj;
};




class map
{
    public:
        map();
        ~map();
    void addEdge(std::string v1, std::string v2, int weight);
    void addVertex(std::string name);
    void shortestPath(vertex *start_city, vertex *final_city);
    void pathfinder(std::string start_city, std::string final_city, double lat1, double lat2, double lng1, double lng2);
    void graphinit(char *file1, char *file2, char *file3);
    void printGraph();
    double GPStoDist(double lat1, double lat2, double long1, double long2);
    void weighted(char *filename);
    vertex * findVertex(std::string name);
    vertex * nearNeighbor(double lat, double lng);

    protected:
    private:
    std::vector<vertex> vertices;
    std::vector<vertex *> solved;
    void BFTraversal(std::string startingCity);
    double offmap[2];

};

#endif // MAP_H
