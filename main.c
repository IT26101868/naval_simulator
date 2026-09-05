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
        {500.0, 400.0},
        {3000.0, 3000.0},
        {6000.0, 5500.0},
        {9000.0, 8500.0}
    };
    int path_len = sizeof(path) / sizeof(path[0]);
    
    generate_escort_ships(ships, num_ships, canvas_size);
    
    int choice = 0;
    printf("Select Simulation Mode:\n");
    printf("1. Part 1-A Simulation\n");
    printf("2. Part 1-B Simulation\n");
    printf("Enter your choice (1 or 2): ");
    scanf("%d", &choice);

    if (choice == 1) {
        run_part_1a_simulation(b, ships, num_ships, types, canvas_size);
    } else if (choice == 2) {
        run_part_1b_simulation(b, ships, num_ships, types, path, path_len, canvas_size);
    } else {
        printf("Invalid choice. Exiting.\n");
        return 1;
    }

        return 0;
}