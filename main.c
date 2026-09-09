#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "simulator.h"

int main(){
    srand((unsigned int)time(NULL));

    EscortType types[MAX_ESCORT_TYPES];
    initialize_escort_types(types);

    int num_ships = 10;
    double canvas_size = 10000.0;
    EscortShip ships[10];

    Battleship b = {"USS Iowa", 'U', 500.0, 500.0, 250.0, 1.0, 0.0, 0, 5.0, 0.0};

    Point path [] = {
        {500.0, 500.0},
        {1800.0, 1800.0},
        {3200.0, 3200.0},
        {4500.0, 4500.0}
    };
    int path_len = sizeof(path) / sizeof(path[0]);
    
    generate_escort_ships(ships, num_ships, canvas_size);
    
    int choice = 0;
    printf("Select Simulation Mode:\n");
    printf("1. Part 1-A Simulation\n");
    printf("2. Part 1-B Simulation\n");
    printf("3. Part 2-A Simulation (Target Priority & Reload Time)\n");
    printf("4. Part 2-B Simulation (Continuous Escorts Reloads)\n");
    printf("Enter your choice (1 - 4): ");
    scanf("%d", &choice);

    if (choice == 1) {
        run_part_1a_simulation(b, ships, num_ships, types, canvas_size);
    } else if (choice == 2) {
        run_part_1b_simulation(b, ships, num_ships, types, path, path_len, canvas_size);
    } else if (choice == 3 || choice == 4) {
        int custom_input = 0;
        printf("Do you want to specify Battlehsip Reload Time? (1 for Yes, 0 for No): ");
        scanf("%d", &custom_input);
        if (custom_input) {
            printf("Enter Battleship Reload Time (in seconds): ");
            scanf("%lf", &b.reload_time);
        } else {
            b.reload_time = get_random_double(1.0, 10.0);
            printf("Randomly assigned Battleship Reload Time: %.2f seconds\n", b.reload_time);
        }
        if (choice == 3) {
            run_part2a_simulation(b, ships, num_ships, types, path, path_len, canvas_size);
        } else {
            run_part2b_simulation(b, ships, num_ships, types, path, path_len, canvas_size);
        }
    } else {
        printf("Invalid choice. Exiting.\n");
        return 1;
    }

        return 0;
}