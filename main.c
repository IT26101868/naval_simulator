#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "simulator.h"

int main(){
    srand((unsigned int)time(NULL));

    EscortType types[MAX_ESCORT_TYPES];
    initialize_escort_types(types);

    int num_ships = 20;
    double canvas_size = 10000.0;
    EscortShip ships[20];

    Battleship b = {"USS Iowa", 'U', 500.0, 500.0, 250.0, 1.0, 0.0, 0};

    Point path [] = {
        {100.0, 100.0},
        {300.0, 400.0},
        {600.0, 700.0},
        {900.0, 900.0}
    };
    int path_len = sizeof(path) / sizeof(path[0]);



    generate_escort_ships(ships, num_ships, canvas_size);
    run_part_1b_simulation(b, ships, num_ships, types, path, path_len, canvas_size);

        return 0;
}