//3E33912F8BAA7542FC4A1585D2DB6FE0312725B9
#include <vector>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <getopt.h>


using namespace std;

struct Animal{
    int32_t number;
    double x;
    double y;
    char category;

    double d;
    int32_t p;
    bool k;

    Animal():x{0.0}, y{0.0}, category{'n'}, d{numeric_limits<double>::infinity()}, p{-1}, k{false}{}
    
};

double distance(const Animal &ani1, const Animal &ani2){
    if((ani1.category == 'w' && ani2.category == 's') || (ani1.category == 's' && ani2.category == 'w')){
        return numeric_limits<double>::infinity();
    }
    
    double diff_x = (ani1.x - ani2.x) * (ani1.x - ani2.x);
    double diff_y = (ani1.y - ani2.y) * (ani1.y - ani2.y);

    return (diff_x + diff_y);

}

class Zoo{
    vector<Animal> zoo_map;

    public:
    string mode;
    void get_options(int argc, char **argv);
    void read_in_zoo();

    void MST();
    void FastTSP();
    void OptTSP();
};
