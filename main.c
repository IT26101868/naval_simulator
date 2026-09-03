#include <stdio.h>
#include "simulator.h"

int main(){
    Battleship b = {"USS Iowa", 'U', 0.0, 0.0, 100.0, 1.0, 0.0, 0};
    EscortShip e = {1, 0, 300.0, 400, 0, 0};

    double dist = calculate_distance(b.x, b.y, e.x, e.y );
    double max_r = get_battleship_max_range(b.v_max);

    printf("distance to escort Ship %d : %.3f m\n", e.id, dist);
    printf("Battleship max range : %.2f m\n", max_r);

if (is_in_battleship_range(b, e)){
    printf("Result: Escort Ship is within attack range!\n");
}  else {
    printf("Result: Escort Ship is out of attack range!\n");
}   
return 0;
}