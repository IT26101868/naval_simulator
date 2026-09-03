#include <stdio.h>
#include <math.h>
#include "simulator.h"

#define GRAVITY 9.81

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
