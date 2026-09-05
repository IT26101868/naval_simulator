#ifndef SIMULATOR_H
#define SIMULATOR_H

#define MAX_ESCORT_TYPES 5
#define MAX_BATTLESHIP_TYPES 4

typedef struct {
    char name[30];
    char notation;
    double impact_power;
    double min_angle;
    double max_angle;
    double min_vel;
    double max_vel;
    double gamma;
} EscortType;

typedef struct{
    int id;
    int type_index;
    double x, y;
    int is_destroyed;
    int firings_count;
} EscortShip;

typedef struct{
    char name[30];
    char notation;
    double x, y;
    double v_max;
    double health;
    double gamma;
    int firings_count;
    double reload_time;
    double last_firing_time;
} Battleship;

typedef struct{
    double x;
    double y;
} Point;


double calculate_distance(double x1, double y1, double x2, double y2);
double calculate_time_to_hit(double distance, double v_max);
double get_battleship_max_range(double v_max);
int is_in_battleship_range(Battleship b, EscortShip e);
int is_in_escort_range(EscortShip e, EscortType type, Battleship b);
double get_random_double(double min, double max);
void initialize_escort_types(EscortType types[]);
void generate_escort_ships(EscortShip ships[], int n, double canvas_size);

void save_initial_state(const char *filename, Battleship b, EscortShip ships[], int n, EscortType types[], double canvas_size);
void save_part_1a_results(const char *filename, Battleship b, EscortShip ships[], int n, int b_sunk, int sinking_e_id);
void run_part_1a_simulation(Battleship b, EscortShip ships[], int n, EscortType types[], double canvas_size);

int check_gun_jam(int firings_count);
void save_part_1b_results(const char *filename, Battleship b, EscortShip ships[], int n, int b_sunk, int total_steps );
void run_part_1b_simulation(Battleship b, EscortShip ships[], int n, EscortType types[], Point path[], int path_len, double canvas_size);

int select_optimal_target(Battleship b, EscortShip ships[], int n, EscortType types[]);
void run_part2a_simulation(Battleship b, EscortShip ships[], int n, EscortType types[], Point path[], int path_len, double canvas_size);

#endif
