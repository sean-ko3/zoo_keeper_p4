//3E33912F8BAA7542FC4A1585D2DB6FE0312725B9
#include "zoo.h"
#include "iomanip"
#include <vector>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <getopt.h>

void Zoo::get_options(int argc, char **argv){
    int choice;
    int option_index = 0;

    option long_option[]{
        {"help", no_argument, nullptr, 'h'},
        {"mode", required_argument, nullptr, 'm'},
    };

    while((choice = getopt_long(argc, argv, "hm:", long_option, &option_index)) != -1){
        switch(choice){
            case 'h':
                //help message
                exit(0);

            case 'm':
                mode = optarg;
                if(mode.empty()){
                    cerr << "Error: No mode specified\n";
                    exit(1);
                }
                break;

            default:
                cerr << "Error: Invalid command line option\n";
                exit(1);   
            
        }
    }
}

void Zoo::read_in_zoo(){
    uint32_t count;
    cin >> count;

    zoo_map.reserve(count);

    int32_t counter = 0;

    for(uint32_t i = 0; i < count; i++){
        Animal cur_a;
        cin >> cur_a.x;
        cin >> cur_a.y;

        cur_a.number = counter;
        counter++;
        if(mode == "MST"){
            if(cur_a.x < 0 && cur_a.y < 0){
                cur_a.category = 'w';
                wild_cages++;
            }
            else if((cur_a.x == 0 && cur_a.y <= 0) || (cur_a.y == 0 && cur_a.x <= 0)){
                cur_a.category = 'b';
                wall_cages++;
            }
            else{
                cur_a.category = 's';
                safe_cages++;
            }
        }
        zoo_map.push_back(cur_a);
    }
}

double Zoo::distance(const Animal &ani1, const Animal &ani2){
    
    if((ani1.category == 'w' && ani2.category == 's') || (ani1.category == 's' && ani2.category == 'w')){
        return numeric_limits<double>::infinity();
    }
    
    double diff_x = (ani1.x - ani2.x) * (ani1.x - ani2.x);
    double diff_y = (ani1.y - ani2.y) * (ani1.y - ani2.y);
   
  
    return (diff_x + diff_y);
    

}
void Zoo::MST(double &total_weight){
    
    if(safe_cages != 0 && wild_cages != 0 && wall_cages == 0){
            cerr << "Cannot construct MST\n";
            exit(1);
    }
    
    else if(!zoo_map.empty()){
        Animal *current = &zoo_map[0];
        zoo_map[0].d = 0;
        double min_distance; 
        uint32_t k_counter = 0;
        total_weight = 0;
        
        
        while(k_counter != zoo_map.size()){
            //first loop
            //rest min_distance first
            min_distance = numeric_limits<double>::infinity(); 
            for(Animal &ani: zoo_map){
                if(ani.d < min_distance && ani.k == false){
                    min_distance = ani.d;
                    current = &ani;
                }
            }
            
            current->k = true;
            total_weight += sqrt(current->d);
            k_counter++;

            for(Animal &neighbor: zoo_map){
                if(neighbor.k == false){
                    double distance_ = distance(*current, neighbor);
                    if(neighbor.d > distance_){
                        neighbor.d = distance_;
                        neighbor.p = current->number;
                    }
                }
            }
        }
    }
}
void Zoo::printMST(const double &total){
    // display answer
    cout << total << "\n";
    for(size_t i = 1; i < zoo_map.size(); i++){
        
        if(zoo_map[i].p > zoo_map[i].number){
            cout << zoo_map[i].number << " " << zoo_map[i].p << "\n";
        }
        else{
            cout <<  zoo_map[i].p << " " << zoo_map[i].number  << "\n";
        }
    } 
}

double distance_no_walls(const Animal &ani1, const Animal &ani2){
    double diff_x = (ani1.x - ani2.x) * (ani1.x - ani2.x);
    double diff_y = (ani1.y - ani2.y) * (ani1.y - ani2.y);
   
  
    return (diff_x + diff_y);
    
}
void Zoo::FastTSP(){
    //step 1
    fast_route.reserve(zoo_map.size() + 1);
    
    //partial tour: {0}
    fast_route.push_back(0);
    //step 2 : randomly selecting the 2nd elemnt of vector
    fast_route.push_back(1);
    fast_route.push_back(0);
    
    //start at zoo_map[0]
    zoo_map[0].k = true;
    zoo_map[1].k = true;
    
    ;
   //step 3: arbitrary select in order
   for(size_t counter = 2; counter < zoo_map.size(); counter++){
        zoo_map[counter].k = true;
        // now we need to think about exactly where to add it 
        //step 4
        size_t best_spot = 0; //holds the index in fast_route you put next_animal right after
        double d1, d2, d3;
        double dist_min = numeric_limits<double>::infinity();
        
        for(size_t i = 0; i < fast_route.size() - 1; i++){
                d1 = sqrt(distance_no_walls(zoo_map[static_cast<size_t>(fast_route[i])], zoo_map[static_cast<size_t>(fast_route[i + 1])])); //old path
                d2 = sqrt(distance_no_walls(zoo_map[counter], zoo_map[static_cast<size_t>(fast_route[i])])); //left path to next
                d3 = sqrt(distance_no_walls(zoo_map[counter], zoo_map[static_cast<size_t>(fast_route[i + 1])])); //right path from next
                
                //the distance you want to minimize
                double d4 =  d2 + d3 - d1;
                if (d4 < dist_min){
                    dist_min = d4;
                    best_spot = i;
                }
        }

        //put it at the desired spot 
        fast_route.insert(fast_route.begin() + int(best_spot) + 1, int32_t(counter));
    }      
     
}
void Zoo::printFASTTSP(){
        double total = 0;
        for(size_t i = 0; i < fast_route.size() - 1; i++){
            total += sqrt(distance_no_walls(zoo_map[size_t(fast_route[i])], zoo_map[size_t(fast_route[i+1])]));
        }
        fast_route.pop_back();

        cout << total << "\n";
        for(int32_t &vertex: fast_route){
            
            cout << vertex << " ";
        
        }
}

double Zoo::OptMST(){
        for(int32_t &vertex: not_fixed){
            zoo_map[size_t(vertex)].k = false;
            zoo_map[size_t(vertex)].d = numeric_limits<double>::infinity();
        }
        double total_weight;
        Animal *current = &zoo_map[size_t(not_fixed[0])];
        zoo_map[size_t(not_fixed[0])].d = 0;
        double min_distance; 
        uint32_t k_counter = 0;
        total_weight = 0;
        
        
        while(k_counter != not_fixed.size()){
            //first loop
            //rest min_distance first
            min_distance = numeric_limits<double>::infinity(); 
            for(int32_t &ani_index: not_fixed){
                if(zoo_map[size_t(ani_index)].d < min_distance && zoo_map[size_t(ani_index)].k == false){
                    min_distance = zoo_map[size_t(ani_index)].d;
                    current = &zoo_map[size_t(ani_index)];
                }
            }
            
            current->k = true;
            total_weight += (current->d);
            k_counter++;

            for(int32_t &neighbor: not_fixed){
                if(zoo_map[size_t(neighbor)].k == false){
                    double distance_ = sqrt(distance_no_walls(*current, zoo_map[size_t(neighbor)]));
                    if(zoo_map[size_t(neighbor)].d > distance_){
                        zoo_map[size_t(neighbor)].d = (distance_);
                        zoo_map[size_t(neighbor)].p = current->number;
                    }
                }
            }
        }
        return total_weight;
}


void Zoo::genPerms(size_t permLength) {
        if (permLength == cur_route.size()) {
        // Do something with the path
            double closing_edge = sqrt(distance_no_walls(zoo_map[size_t(cur_route[cur_route.size() - 1])], zoo_map[0]));
            current_cost += closing_edge;
            if(current_cost < best_cost){
                best_route = cur_route;
                best_cost = current_cost;
            }
            current_cost -= closing_edge;
            return;
        }  // if ..complete path

        if (!promising(cur_route, permLength)) {
            return;
        }  // if ..not promising

        for (size_t i = permLength; i < cur_route.size(); ++i) {
            swap(cur_route[permLength], cur_route[i]);
            double new_edge_cost = sqrt(distance_no_walls(zoo_map[size_t(cur_route[permLength - 1])], zoo_map[size_t(cur_route[permLength])]));
            current_cost += new_edge_cost;
            genPerms(permLength + 1);
            current_cost -= new_edge_cost;
            swap(cur_route[permLength],cur_route[i]);
        }  // for ..unpermuted elements
    }  // genPerms()


bool Zoo::promising(vector<int32_t>&path, size_t permlength){
    /*if(path.size() - permlength < 5){
        return true;
    }*/
    //use MST to find total cost of the rest of the other
    /*vector<int32_t> test = {0,6,1,2,5,10,3,4,8,7,9};
    if(cur_route == test && permlength == 10)
      cout << "here";
    }*/
    double s_min_d = numeric_limits<double>::infinity();
    double e_min_d = numeric_limits<double>::infinity();
    not_fixed.reserve(path.size() - permlength);
    for(size_t i = permlength; i < path.size(); i++){
        not_fixed.push_back(path[i]);
        double s_temp_d = distance_no_walls(zoo_map[size_t(path[0])], zoo_map[size_t(path[i])]);
        double e_temp_d = distance_no_walls(zoo_map[size_t(path[permlength - 1])], zoo_map[size_t(path[i])]);
        
        s_min_d = min(s_temp_d, s_min_d);
        e_min_d = min(e_temp_d, e_min_d);
                    
    }

    double MSTweight = OptMST();
    
    
    double total_cost = current_cost + sqrt(s_min_d) + sqrt(e_min_d) + MSTweight;
    //size_t unvisited = path.size() - permlength;
    //Error checks
    /*cout << unvisited << setw(10) << MSTweight << "\n";
    for(size_t i = 0; i < permlength; i++){
        cout << cur_route[i] << " ";
    }*/
    not_fixed.clear();
    //cout << "\n" << s_min_d << setw(10) << e_min_d << setw(10) << total_cost << "\n";
    //End of Error check
    if(total_cost < best_cost){
        return true;
    }
    return false;

    
}
    
void Zoo::OptTSP(){
    FastTSP(); //we need to have a good base --> fast_route
    best_route = fast_route;
    for(size_t i = 0; i < fast_route.size() - 1; i++){
            best_cost += sqrt(distance_no_walls(zoo_map[size_t(fast_route[i])], zoo_map[size_t(fast_route[i+1])]));
    }
    fast_route.pop_back();
    best_route.pop_back();

    //start path with 0 and permLength 1
    cur_route.reserve(best_route.size());
    cur_route = best_route;

    size_t permlength = 1;
    genPerms(permlength);

}

void Zoo::printOPTTSP(){
    cout << best_cost << "\n";
    for(int32_t &vertex: best_route){
        cout << vertex << " ";
    }
}
    


int main(int argc, char **argv){
    cout << std::setprecision(2); //Always show 2 decimal places
    cout << std::fixed; //Disable scientific notation for large numbers
    Zoo zoo;
    zoo.get_options(argc, argv);
    zoo.read_in_zoo();
    if(zoo.mode == "MST"){
        double total;
        vector<int32_t> whole_zoo;
        whole_zoo.reserve(zoo.zoo_map.size());
        for(size_t i = 0; i < zoo.zoo_map.size(); i++){
            whole_zoo.push_back(int32_t(i));
        }
        zoo.MST(total);
        zoo.printMST(total);
    }
    else if(zoo.mode == "FASTTSP"){
        zoo.FastTSP();
        zoo.printFASTTSP();
    }
    else if(zoo.mode == "OPTTSP"){
        zoo.OptTSP();
        zoo.printOPTTSP();
    }
    else{
        cerr << "Error: Invalid mode\n";
        exit(1);
    }

    return 0;
}
