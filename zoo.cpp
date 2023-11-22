//3E33912F8BAA7542FC4A1585D2DB6FE0312725B9
#include "zoo.h"
#include "iomanip"

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
        
        if(cur_a.x < 0 && cur_a.y < 0){
            cur_a.category = 'w';
        }
        else if((cur_a.x == 0 && cur_a.y < 0) || (cur_a.y == 0 && cur_a.x < 0)){
            cur_a.category = 'b';
        }
        else{
            cur_a.category = 's';
        }

        zoo_map.push_back(cur_a);
    }
}

void Zoo::MST(){
    
    Animal *current = &zoo_map[0];
    zoo_map[0].d = 0;
    double min_distance; 
    uint32_t k_counter = 0;
    double total = 0;
    
    
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
        total += sqrt(current->d);
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

int main(int argc, char **argv){
    cout << std::setprecision(2); //Always show 2 decimal places
    cout << std::fixed; //Disable scientific notation for large numbers
    Zoo zoo;
    zoo.get_options(argc, argv);
    zoo.read_in_zoo();
    if(zoo.mode == "MST"){
        zoo.MST();
    }
    /*else if(zoo.mode == "FASTTSP"){
        zoo.FastTSP();
    }
    else if(zoo.mode == "OPTTSP"){
        zoo.OptTSP();
    }*/
    else{
        cerr << "Error: Invalid mode\n";
        exit(1);
    }

    return 0;
}
