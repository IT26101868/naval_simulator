#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "simulator.h"

void display_menu() {
    printf("\n========================================\n");
    printf("\n         NAVAL COMBAT SIMULATOR                  \n");
    printf("\n========================================\n");
    printf("\n1: Part 1-A : Basic & Static Simultaneous Engagement\n");
    printf("\n2: Part 1-B : Waypoint Navigation, Gun Jamming & Decay \n");
    printf("\n3: Part 2-A : Target Priority Strategy& Battleship Reload\n");
    printf("\n4: Part 2-B : Continuous Escort Firing & Reload Delays\n");
    printf("\n5: Part 2-C : Power Law Impact Power Decay Simulation\n");
    printf("\n6: Exit Simulation \n");
    printf("\nEnter your choice (1-6): ");

}

Battleship select_battleship() {
    int choice = 0;
    printf("\n**********************\n");
    printf("\nSelect Battleship:\n");
    printf("1. USS Iowa [Notation : U | Main Gun : 50-caliber Mark 7 Gun]\n");
    printf("2. MS King George V [Notation : M | Main gun : 356mm Mark VIII Gun]\n");
    printf("3. Richelieu [Notation : R | Main gun : 15 inch Mle 1935 Gun]\n");
    printf("4. Sovetsky Soyuz-class [Notation : S | Main Gun : 16 inch B-37 Gun]\n");
    printf("\nEnter your choice (default '1') :");
    scanf("%d", &choice);

    if (choice == 2) {
        return(Battleship){"MS King George V", 'M', 500.0, 500.0, 220.0, 1.0, 0.0, 0, 5.0, 0.0};
    } else if (choice == 3) {
        return(Battleship){"Richelieu", 'R', 500.0, 500.0, 240.0, 1.0, 0.0, 0, 5.0, 0.0};
    } else if (choice == 4){
        return(Battleship){"Sovetsky Soyux-Class", 'S', 500.0, 500.0, 230.0, 1.0, 0.0, 0, 5.0, 0.0};
    }else{
        return(Battleship){"USS Iowa", 'U', 500.0, 500.0, 250.0, 1.0, 0.0, 0, 5.0, 0.0};
    }


}


int main(){
    srand((unsigned int)time(NULL));

    EscortType types[MAX_ESCORT_TYPES];
    initialize_escort_types(types);

    int num_ships = 10;
    double canvas_size = 10000.0;
    EscortShip ships[100];

    Point path [] = {
        {500.0, 500.0},
        {1800.0, 1800.0},
        {3200.0, 3200.0},
        {4500.0, 4500.0}
    };
    int path_len = sizeof(path) / sizeof(path[0]);
    int choice = 0;
    while (1){
        display_menu();
        if (scanf("%d", &choice) != 1 || choice == 6){
            printf("\nExiting Naval Simulator...\n");
            break;

        }
        if (choice <1 || choice > 6) {
            printf("\nInvalid choice.. please select an option 1-6:\n");
            continue;

        }

        Battleship b = select_battleship();

        double input_canv;
        printf("\nEnter Canvas Size : ");
        scanf("%lf", &input_canv);
        if (input_canv > 0.0) {
            canvas_size = input_canv;
        
        }else{
            canvas_size = 10000.0;

        }
        printf("\nEnter a number of Escort Ships (1-100 | default 10) : ");
        scanf("%d", &num_ships);
        if (num_ships <= 0 || num_ships > 100){
            num_ships = 10;
        }

        generate_escort_ships(ships, num_ships, canvas_size);
        
        if (choice >= 3 && choice <= 5) {
            int custom_reload = 0;
            printf("\nSpecify Battleship Reload Time ? (1-yes|0-random) :");
            scanf("%d", &custom_reload);
            if (custom_reload) {
                printf("\nEnter Battleship Reload Time in seconds : ");
                scanf("%lf", &b.reload_time);

            } else {
                b.reload_time = get_random_double(1.0, 10.0);
                printf("Randomy assigned Battleship Reload Time : %.2fs\n", b.reload_time);

            }

        }

        if (choice == 1) {
            run_part_1a_simulation(b, ships, num_ships, types, canvas_size);

        }else if (choice == 2) {
            run_part_1b_simulation(b, ships, num_ships, types, path, path_len, canvas_size);
        }else if (choice == 3) {
            run_part2a_simulation(b, ships, num_ships, types, path, path_len, canvas_size);
        }else if (choice == 4) {
            run_part2b_simulation(b, ships, num_ships, types, path, path_len, canvas_size);
        }else if (choice == 5) {
            run_part2c_simulation(b, ships, num_ships, types, path, path_len, canvas_size);
        }
    }
    return 0;
}
