//3E33912F8BAA7542FC4A1585D2DB6FE0312725B9
#include <vector>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <getopt.h>
#include <limits>
#include <cstdint>
#include <string>
#include <cstdlib>
#include <numeric>


using namespace std;

struct Animal{
    int32_t number;
    double x;
    double y;
    char category;

    double d;
    int32_t p;
    bool k;

    Animal():number{-1},x{0.0}, y{0.0}, category{'n'}, d{numeric_limits<double>::infinity()}, p{-1}, k{false}{}
    
};



class Zoo{
    private:
    
    //MST
    uint32_t safe_cages = 0;
    uint32_t wall_cages = 0;
    uint32_t wild_cages = 0;
    
    //FastTSP
    vector<int32_t> fast_route;
    
    //OptTSP
    vector<int32_t> best_route;
    double best_cost = 0;
    vector<int32_t> cur_route;
    double current_cost = 0;
    bool promising(vector<int32_t>&path, size_t permlength);
    vector<int32_t> not_fixed; 

    public:
    //General
    vector<Animal> zoo_map;
    string mode;
    //distance function
    double distance(const Animal &ani1, const Animal &ani2);

    void get_options(int argc, char **argv);
    void read_in_zoo();

    void MST(double &total_weight);
    void printMST(const double &total);
    
    void FastTSP();
    void printFASTTSP();

    double OptMST();
    void OptTSP();
    vector<int32_t> map;
    void printOPTTSP();
    void genPerms(size_t permLength);
         
};
