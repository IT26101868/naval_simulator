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
} Battleship;

double calculate_distance(double x1, double y1, double x2, double y2);
double get_battleship_max_range(double v_max);
int is_in_battleship_range(Battleship b, EscortShip e);

#endif
