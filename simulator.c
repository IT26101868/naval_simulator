#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "simulator.h"

#define GRAVITY 9.81
#define PI 3.14159265358979323846

double deg_to_rad(double deg){
    return deg * (PI / 180.0);
}

double calculate_distance(double x1, double y1, double x2, double y2){
    return sqrt((x2 - x1) * (x2 - x1) + (y2 -y1)*(y2 - y1));
}

double get_battleship_max_range(double v_max){
    return (v_max * v_max) / GRAVITY;
}

int is_in_battleship_range(Battleship b, EscortShip e){
    double dist = calculate_distance(b.x, b.y, e.x, e.y);
    double max_range = get_battleship_max_range(b.v_max);
    return dist <= max_range;
}

double get_random_double(double min, double max){
    return min + (rand() / (RAND_MAX / (max - min)));
}

void initialize_escort_types(EscortType types[]){
    // Initialize Escort Types
    types[0] = (EscortType){"1936A-class Destroyer", 'A', 0.08, 20.0, 40.0, 100.0, 300.0, 0.05};
    types[1] = (EscortType){"Gabbiano-class corvette", 'B', 0.06, 30.0, 60.0, 80.0, 250.0, 0.06};
    types[2] = (EscortType){"Matsu-class Destroyer", 'C', 0.07, 25.0, 50.0, 90.0, 280.0, 0.055};
    types[3] = (EscortType){"F-class Escort Ship", 'D', 0.05, 50.0, 100.0, 70.0, 220.0, 0.07};
    types[4] = (EscortType){"Japanese kaibokan", 'E', 0.04, 70.0, 140.0, 60.0, 200.0, 0.08};
}

void generate_escort_ships(EscortShip ships[], int n, double canvas_size){
    for (int i = 0; i < n; i++){
        ships[i].id = i + 1;
        ships[i].type_index = rand() % MAX_ESCORT_TYPES;
        ships[i].x = get_random_double(0.0, canvas_size);
        ships[i].y = get_random_double(0.0, canvas_size);
        ships[i].is_destroyed = 0;
        ships[i].firings_count = 0;
    }
}

int is_in_escort_range(EscortShip e, EscortType type, Battleship b){
    double dist = calculate_distance(e.x, e.y, b.x, b.y);
    double min_r = (type.min_vel * type.min_vel) * sin(2 * deg_to_rad(type.min_angle)) / GRAVITY;
    double max_r = (type.max_vel * type.max_vel) * sin(2 * deg_to_rad(type.max_angle)) / GRAVITY;

    return (dist >= min_r && dist <= max_r);

}

void save_initial_state(const char *filename, Battleship b, EscortShip ships[], int n, EscortType types[], double canvas_size){
    FILE *file = fopen(filename, "w");
    if (file == NULL){
        printf("Error opening file %s for writing.\n", filename);
        return;
    }

    fprintf(file, "Initial State of Simulation\n");
    fprintf(file, "Canvas Size: %.2f m x %.2f m\n", canvas_size, canvas_size);
    fprintf(file, "Battleship: %s (Notation: %c) at (%.2f, %.2f) with max shell velocity %.2f m/s\n", b.name, b.notation, b.x, b.y, b.v_max);
    fprintf(file, "Escort Ships:\n");
    for (int i = 0; i < n; i++){
        fprintf(file, "Escort Ship #%d: Type: %s (Notation: %c) at (%.2f, %.2f) | Max angle: %.2f degrees, Min angle: %.2f degrees | v_max: %.2f m/s, v_min: %.2f m/s\n", ships[i].id, types[ships[i].type_index].name, types[ships[i].type_index].notation, ships[i].x, ships[i].y, types[ships[i].type_index].max_angle, types[ships[i].type_index].min_angle, types[ships[i].type_index].max_vel, types[ships[i].type_index].min_vel);
    }

    fclose(file);
    printf("Initial state saved to %s\n", filename);
}

void save_part_1a_results(const char *filename, Battleship b, EscortShip ships[], int n, int b_sunk, int sinking_e_id){
    FILE *file = fopen(filename, "w");
    if (file == NULL){
        printf("Error opening file %s for writing.\n", filename);
        return;
    }

    fprintf(file, "Part 1-A Simulation Results\n");
    fprintf(file, "Battleship: %s (Notation: %c) at (%.2f, %.2f)\n", b.name, b.notation, b.x, b.y);
    if (b_sunk){
        fprintf(file, "Result: Battleship WAS SUNK by Escort Ship #%d!\n", sinking_e_id);
    } else {
        fprintf(file, "Result: Battleship SURVIVED!\n");
        int es_hit_count = 0;
        for (int i = 0; i < n; i++){
            if(is_in_battleship_range(b, ships[i])){
                es_hit_count++;
                fprintf(file, "Escort Ship #%d was hit by Battleship!\n", ships[i].id);
            }
        }
        fprintf(file, "Total Escort Ships hit by Battleship: %d / %d\n", es_hit_count, n);
    }

    fclose(file);
    printf("Part 1-A results saved to %s\n", filename);
}

void run_part_1a_simulation(Battleship b, EscortShip ships[], int n, EscortType types[], double canvas_size){
    printf("\n********Running Part 1-A Simulation********\n");
    
    save_initial_state("initial_state.txt", b, ships, n, types, canvas_size);

    int b_sunk = 0;
    int sinking_e_index = -1;
    int es_hit_count = 0;

    for (int i = 0; i < n; i++){
        if (!ships[i].is_destroyed){
            if(is_in_escort_range(ships[i], types[ships[i].type_index], b)){
                b_sunk = 1;
                sinking_e_index = ships[i].id;
                break;            
            }
        }
    }
    if (b_sunk) {
        printf("Result : Battleship %s WAS SUNK by Escort Ship #%d !\n", b.name, sinking_e_index);
    } else {
        printf("Result : Battleship %s SURVIVED!\n", b.name);
        for (int i = 0; i < n; i++ ){
            if(is_in_battleship_range(b, ships[i])){
                es_hit_count++;
            }
        }
        printf("Total Escort Ships hit by Battleship: %d / %d\n", es_hit_count, n);

    }
    save_part_1a_results("part_1a_results.txt", b, ships, n, b_sunk, sinking_e_index);
}





