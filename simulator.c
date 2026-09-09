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
    types[0] = (EscortType){"1936A-class Destroyer", 'A', 0.08, 20.0, 40.0, 100.0, 300.0, 0.05, 4.0};
    types[1] = (EscortType){"Gabbiano-class corvette", 'B', 0.06, 30.0, 60.0, 80.0, 250.0, 0.06, 6.0};
    types[2] = (EscortType){"Matsu-class Destroyer", 'C', 0.07, 25.0, 50.0, 90.0, 280.0, 0.055, 5.0};
    types[3] = (EscortType){"F-class Escort Ship", 'D', 0.05, 50.0, 100.0, 70.0, 220.0, 0.07, 7.0};
    types[4] = (EscortType){"Japanese kaibokan", 'E', 0.04, 70.0, 140.0, 60.0, 200.0, 0.08, 8.0};
}



void generate_escort_ships(EscortShip ships[], int n, double canvas_size){
    for (int i = 0; i < n; i++){
        ships[i].id = i + 1;
        ships[i].type_index = rand() % MAX_ESCORT_TYPES;
        ships[i].x = get_random_double(0.0, canvas_size);
        ships[i].y = get_random_double(0.0, canvas_size);
        ships[i].is_destroyed = 0;
        ships[i].firings_count = 0;
        ships[i].last_firing_time = 0.0;
    }
}

int is_in_escort_range(EscortShip e, EscortType type, Battleship b){
    double dist = calculate_distance(e.x, e.y, b.x, b.y);
    double min_r = (type.min_vel * type.min_vel) * sin(2 * deg_to_rad(type.min_angle)) / GRAVITY;
    double max_r = (type.max_vel * type.max_vel) * sin(2 * deg_to_rad(type.max_angle)) / GRAVITY;

    return (dist >= min_r && dist <= max_r);

}

double calculate_time_to_hit(double distance, double v_max){
    double v = v_max * cos(deg_to_rad(45.0));
    return distance / v; 
}

int select_optimal_target(Battleship b, EscortShip ships[], int n, EscortType types[]){
    int best_target = -1;
    double max_threat = -1.0;

    for (int i = 0; i < n; i++){
        if (!ships[i].is_destroyed && is_in_battleship_range(b, ships[i])){
            double dist = calculate_distance(b.x, b.y, ships[i].x, ships[i].y);
            double threat = types[ships[i].type_index].impact_power / (dist + 1.0); // Threat level based on impact power and distance
            if (threat > max_threat){
                max_threat = threat;
                best_target = i;
            }
        }
    }
    return best_target;
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
    fprintf(file, "Battleship Health: %.2f%%\n", b.health * 100.0);
    fprintf(file, "Total damage taken by Battleship: %.2f%%\n", (1.0 - b.health) * 100.0);
    if (b_sunk){
        fprintf(file, "Result: Battleship WAS SUNK (final HIT by Escort Ship #%d)\n", sinking_e_id);
    } else {
        fprintf(file, "Result: Battleship SURVIVED!\n");
        fprintf(file, "Escort Ships taken down by Battleship:\n");

        int es_hit_count = 0;
        double max_time_to_hit = 0.0;

        for (int i = 0; i < n; i++){
            if(is_in_battleship_range(b, ships[i])){
                es_hit_count++;
                double dist = calculate_distance(b.x, b.y, ships[i].x, ships[i].y);
                double time_to_hit = calculate_time_to_hit(dist, b.v_max);

                if (time_to_hit > max_time_to_hit){
                    max_time_to_hit = time_to_hit;
                }

                fprintf(file, "Escort Ship #%d was hit by Battleship! [Position : (%.2f, %.2f)] [Time to hit : %.2fs]\n", ships[i].id, ships[i].x, ships[i].y, time_to_hit);
            }
        }
        fprintf(file, "Total Escort Ships hit by Battleship: %d / %d\n", es_hit_count, n);
        fprintf(file, "Battle Duration: %.2fs\n", max_time_to_hit);
    }

    fclose(file);
    printf("Part 1-A results saved to %s\n", filename);
}



void run_part_1a_simulation(Battleship b, EscortShip ships[], int n, EscortType types[], double canvas_size){
    printf("\n********Running Part 1-A Simulation********\n");
    printf("with Part 1-C Damage Mechanics\n\n)");
    save_initial_state("initial_state.txt", b, ships, n, types, canvas_size);

    double total_damage_pct = 0.0;
    int attacker_count = 0;
    int b_sunk = 0;
    int sinking_e_index = -1;
    int es_hit_count = 0;

    for (int i = 0; i < n; i++){
        if (is_in_escort_range(ships[i], types[ships[i].type_index], b)){
            attacker_count++;
            double damage = types[ships[i].type_index].impact_power * (1.0 - b.gamma);
            total_damage_pct += damage; 

            printf(">>>Escort Ship #%d HIT Battleship! (Damage: %.2f%%) | Total Damage: %.2f%%\n", ships[i].id, damage * 100.0, total_damage_pct *  100.0);

            if (total_damage_pct >= b.health){
                b_sunk = 1;
                sinking_e_index = ships[i].id;
                break;
            }
        }
    }
    b.health -= total_damage_pct;
    if (b_sunk) {
        printf("Result : Battleship %s WAS SUNK by Escort Ships !\n", b.name);
    } else {
        printf("Result : Battleship %s SURVIVED!\n", b.name);
        printf("Total Damage Taken: %.2f%% | Remaining Health: %.2f%%\n", total_damage_pct * 100.0, b.health * 100.0);
        double max_time_to_hit = 0.0;
        for (int i = 0; i < n; i++ ){
            if(is_in_battleship_range(b, ships[i])){
                es_hit_count++;
                double dist = calculate_distance(b.x, b.y, ships[i].x, ships[i].y);
                double time_to_hit = calculate_time_to_hit(dist, b.v_max);
                if (time_to_hit > max_time_to_hit){
                    max_time_to_hit = time_to_hit;
                }
            }
        }
        printf("Total Escort Ships hit by Battleship: %d / %d\n", es_hit_count, n);
        printf("Battle Duration: %.2fs\n", max_time_to_hit);
    }
    save_part_1a_results("part_1a_results.txt", b, ships, n, b_sunk, sinking_e_index);
}

int check_gun_jam(int firings_count){
    double jam_probability = 0.05 * firings_count;
    double roll = get_random_double(0.0, 1.0);
    return roll < jam_probability;
}

void save_part_1b_results(const char *filename, Battleship b, EscortShip ships[], int n, int b_sunk, int total_steps ){ 
    
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file %s for writing.\n", filename);
        return;
    }
    fprintf(file, "*******Part 1-B Simulation Results*******\n");
    fprintf(file, "Total Steps Taken: %d\n", total_steps);
    fprintf(file, "Battleship: %s (Notation: %c) | (Position: (%.2f, %.2f)) | Health: %.2f\n", b.name, b.notation, b.x, b.y, b.health * 100.0);

    if (b_sunk) {
        fprintf(file, "Result: Battleship WAS SUNK!\n");
    } else {
        fprintf(file, "Result: Battleship SURVIVED and reached the destination!\n");
        fprintf(file, " -Escort Ships Status:\n");
        int destroyed_count = 0;
        for (int i = 0; i < n; i++) {
            if(ships[i].is_destroyed){
                destroyed_count++;
                fprintf(file, "  -Escort Ship #%d: DESTROYED\n", ships[i].id);
            } else {
                fprintf(file, "  -Escort Ship #%d: SURVIVED | Position: (%.2f, %.2f))\n", ships[i].id, ships[i].x, ships[i].y);
            }
            fprintf(file, "  Total Escort Ships Destroyed: %d\n", destroyed_count);
        }
        fclose(file);
        printf("Part 1-B results saved to %s\n", filename);
    }
}

void run_part_1b_simulation(Battleship b, EscortShip ships[], int n, EscortType types[], Point path[], int path_len, double canvas_size){
    printf("\n####### Running Part 1-B Simulation #######\n");

    save_initial_state("initial_state_1b.txt", b, ships, n, types, canvas_size);

    int b_sunk = 0;
    int step = 0;

    for (step = 0; step < path_len; step++) {
        b.x = path[step].x;
        b.y = path[step].y;
        printf("Step %d: Battleship's current position : %.2f, %.2f \n", step + 1, b.x, b.y);

        for (int i = 0; i < n; i++){
            if(!ships[i].is_destroyed){
                if (is_in_escort_range(ships[i], types[ships[i].type_index],b)){
                    ships[i].firings_count++;
                    
                    if (check_gun_jam(ships[i].firings_count)){
                        printf("***Escort Ship #%d GUN JAMMED! \n", ships[i].id);
                    } else{
                        double damage = types[ships[i].type_index].impact_power * (1.0 - b.gamma);
                        b.health -= damage;
                        printf(">>Escort Ship #%d HIT Battleship! (B health: %.2f%%)\n)", ships[i].id, b.health * 100.0);

                        if (b.health <= 0.0){
                            b_sunk = 1;
                            break;
                        }
                    }
                }
            }
        }

        if (b_sunk){
            printf("\nResult: Battleship %s was SUNK at step %d!\n", b.name, step + 1);
            break;
        }

        int active_escorts_exist = 0;
        for (int i = 0; i < n; i++){
            if (!ships[i].is_destroyed){
                active_escorts_exist = 1;
                break;
            }
        }
        if (active_escorts_exist) {
            if (check_gun_jam(b.firings_count)) {
            printf(">>>Battleship %s GUN JAMMED this turn!\n", b.name);
            } else {
                b.firings_count++;
                for (int i = 0; i < n; i++){
                    if (!ships[i].is_destroyed && is_in_battleship_range(b, ships[i])) {
                        ships[i].is_destroyed = 1;
                        printf(">>> Battleship DESTROYED Escort Ship #%d!\n", ships[i].id);
                    }
                }
            }
        }

    }

    if (!b_sunk) {
        printf("\nResult: Battleship %s SURVIVED all %d path steps!\n", b.name, path_len);
    }
    save_part_1b_results("part_1b_results.txt", b, ships, n, b_sunk, step + (b_sunk ? 1:0));
    
}

void run_part2a_simulation(Battleship b, EscortShip ships[], int n, EscortType types[], Point path[], int path_len, double canvas_size){
    printf("\n******** Running Part 2-A Simulation ********\n");

    FILE *file = fopen("part_2a_results.txt", "w");
    if (file == NULL){
        printf("Error opening file part_2a_results.txt for writing.\n");
        return;
    }
    save_initial_state("initial_state_part2a.txt", b, ships, n, types, canvas_size);

    fprintf(file, "******* Part 2-A Simulation Results *******\n");
    fprintf(file, "Battleship Reload Time: %.2fs\n", b.reload_time);
    fprintf(file, "Attack Strategy: Priority based on Threat Level (Impact Power / Distance)\n\n");

    int escort_destroyed_count = 0;
    double simulation_time = 0.0;
    int b_sunk = 0;

    for (int step = 0; step < path_len && !b_sunk; step++){
        b.x = path[step].x;
        b.y = path[step].y;
        printf("\nStep %d: Battleship's current position : (%.2f, %.2f)\n", step + 1, b.x, b.y);
        fprintf(file, "--- Step %d [Position: (%.2f, %.2f)] ---\n", step + 1, b.x, b.y);

        // 1. All Escort Ships in range return fire
        for (int i = 0; i < n; i++){
            if (!ships[i].is_destroyed && is_in_escort_range(ships[i], types[ships[i].type_index], b)){
                double damage = types[ships[i].type_index].impact_power * (1.0 - b.gamma);
                b.health -= damage;
                printf(">> Escort Ship #%d HIT Battleship! | Damage: %.2f%% | Health: %.2f%%\n", 
                       ships[i].id, damage * 100.0, b.health * 100.0);
                
                if (b.health <= 0.0){
                    b_sunk = 1;
                    printf("Result: Battleship %s WAS SUNK by Escort Ship #%d at step %d!\n", b.name, ships[i].id, step + 1);
                    fprintf(file, "Result: Battleship %s WAS SUNK by Escort Ship #%d at step %d!\n", b.name, ships[i].id, step + 1);
                    break;
                }
            }
        }
        
        while (!b_sunk) {
            int target_index = select_optimal_target(b, ships, n, types);
            if (target_index == -1) {
                printf("  [STRATEGY] No more Escort Ships in range at this waypoint.\n");
                break;
            }

            ships[target_index].is_destroyed = 1;
            escort_destroyed_count++;
            b.firings_count++;

            double dist = calculate_distance(b.x, b.y, ships[target_index].x, ships[target_index].y);
            double time_to_hit = calculate_time_to_hit(dist, b.v_max);
            simulation_time += time_to_hit + b.reload_time;

            printf("  [STRATEGY] Target Priority Selected: Escort Ship #%d (Type: %c | Dist: %.2fm)\n",
                   ships[target_index].id, types[ships[target_index].type_index].notation, dist);
            printf("  -> [Time: %.2fs] Battleship FIRED at Escort #%d! (Flight: %.2fs | Reload: %.2fs)\n",
                   simulation_time, ships[target_index].id, time_to_hit, b.reload_time);           
            
            fprintf(file, "Attack Order #%d -> Escort #%d | Dist: %.2fm | Flight Time: %.2fs | Reload Time: %.2fs | Total Simulation Time: %.2fs\n",
                    b.firings_count, ships[target_index].id, dist, time_to_hit, b.reload_time, simulation_time);
        }
    }

    // 3. Print Survival Summary if Battleship reached the destination
    if (!b_sunk) {
        printf("\nResult: Battleship %s SURVIVED all %d path steps! (Remaining Health: %.2f%%)\n", 
               b.name, path_len, b.health * 100.0);
        fprintf(file, "\nResult: Battleship %s SURVIVED all %d path steps! (Remaining Health: %.2f%%)\n", 
                b.name, path_len, b.health * 100.0);
    }

    fprintf(file, "Total Escorts Destroyed: %d / %d\n", escort_destroyed_count, n);
    fprintf(file, "Total Battle Time: %.2fs\n", simulation_time);
    if (file) fclose(file);
    printf("Part 2-A results saved to part_2a_results.txt\n");
}

void run_part2b_simulation(Battleship b, EscortShip ships[], int n, EscortType types[], Point path[], int path_len, double canvas_size){
    printf("\n******** Running Part 2-B Simulation ********\n");
    FILE *file = fopen("part_2b_results.txt", "w");
    if (file == NULL){
        printf("Error opening file part_2b_results.txt for writing.\n");
        return; 
    }
    save_initial_state("initial_state_part2b.txt", b, ships, n, types, canvas_size);
    fprintf(file, "******* Part 2-B Simulation Results *******\n");
    fprintf(file, "Battleship Reload Time: %.2fs\n", b.reload_time);
    fprintf(file, "Escortship Reload Time: \n");
    for (int i = 0; i < MAX_ESCORT_TYPES; i++){
        fprintf(file, "  - Type %c: %.2fs\n", types[i].notation, types[i].relod_time);
    }
    fprintf(file, "Attack and Continuous Fire Strategy: Battleship prioritizes targets based on Threat Level (Impact Power / Distance)\n\n");

    int escort_destroyed_count = 0;
    double simulation_time = 0.0;
    int b_sunk = 0;

    for (int step = 0; step < path_len && !b_sunk; step++){
        b.x = path[step].x;
        b.y = path[step].y;
        printf("\nStep %d: Battleship's current position : (%.2f, %.2f)\n", step + 1, b.x, b.y);
        fprintf(file, "--- Step %d [Position: (%.2f, %.2f)] ---\n", step + 1, b.x, b.y);

        while (!b_sunk) {
            for (int i = 0; i < n; i++){
                if (!ships[i].is_destroyed && is_in_escort_range(ships[i], types[ships[i].type_index], b)) {
                    double reload = types[ships[i].type_index].relod_time;
                    if (simulation_time == 0.0 || (simulation_time - ships[i].last_firing_time) >= reload) {
                        ships[i].last_firing_time = simulation_time;
                        ships[i].firings_count++;
                        double damage = types[ships[i].type_index].impact_power * (1.0 - b.gamma);
                        b.health -= damage;
                        printf(">>> Escort Ship #%d HIT Battleship ! at Time: %.2fs | Damage: %.2f%% | Health: %.2f%%\n", ships[i].id, simulation_time, damage * 100.0, b.health * 100.0);
                        fprintf(file, ">>> Escort Ship #%d FIRED! | Damage: %.2f%% | Battleship Health: %.2f%% | Time: %.2fs\n", ships[i].id, damage * 100.0, b.health * 100.0, simulation_time);

                        if (b.health <= 0.0){
                            b_sunk = 1;
                            printf("Result: Battleship %s WAS SUNK by Escort Ship #%d at step %d!\n", b.name, ships[i].id, step + 1);
                            fprintf(file, "Result: Battleship %s WAS SUNK by Escort Ship #%d at step %d!\n", b.name, ships[i].id, step + 1);
                            break;
                        }
                        
                    }
                }
            }

            if (b_sunk) break;

            int target_index = select_optimal_target(b, ships, n, types);
            if (target_index == -1) {
                printf("  [STRATEGY] No more Escort Ships in range at this waypoint.\n");
                break;

        }
        ships[target_index].is_destroyed = 1;
        escort_destroyed_count++;
        b.firings_count++;

        double dist = calculate_distance(b.x, b.y, ships[target_index].x, ships[target_index].y);
        double time_to_hit = calculate_time_to_hit(dist, b.v_max);
        simulation_time += time_to_hit + b.reload_time;

        printf("  [STRATEGY] Target Priority Selected: Escort Ship #%d (Type: %c | Dist: %.2fm)\n",
               ships[target_index].id, types[ships[target_index].type_index].notation, dist);
        printf("  -> [Time: %.2fs] Battleship FIRED at Escort #%d! (Flight: %.2fs | Reload: %.2fs)\n",
               simulation_time, ships[target_index].id, time_to_hit, b.reload_time);
        fprintf(file, "Attack Order #%d -> Escort #%d | Dist: %.2fm | Flight Time: %.2fs | Reload Time: %.2fs | Simulation Time: %.2fs\n",
                b.firings_count, ships[target_index].id, dist, time_to_hit, b.reload_time, simulation_time);
            
        }

    }

    if (!b_sunk) {
        printf("\nResult: Battleship %s SURVIVED the attack! | Remaining health: %.2f%%\n", b.name, b.health * 100.0);
        fprintf(file, "\nResult: Battleship %s SURVIVED the attack! | Remaining health: %.2f%%\n", b.name, b.health * 100.0);
    
    }
    
    fprintf(file, "Total Escorts Destroyed: %d / %d\n", escort_destroyed_count, n);
    fprintf(file, "Total Battle Time: %.2fs\n", simulation_time);
    if (file) fclose(file);
    printf("Part 2-B results saved to part_2b_results.txt\n");

}

      
