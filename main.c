#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "simulator.h"

int main(){
    srand((unsigned int)time(NULL));

    EscortType types[MAX_ESCORT_TYPES];
    initialize_escort_types(types);

    int num_ships = 10;
    double canvas_size = 1000.0;
    EscortShip ships[10];

    Battleship b = {"USS Iowa", 'U', 500.0, 500.0, 250.0, 1.0, 0.0, 0};

    generate_escort_ships(ships, num_ships, canvas_size);
    run_part_1a_simulation(b, ships, num_ships, types, canvas_size);

    return 0;
}