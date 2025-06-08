#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/vehicle.h"
#include "../include/common.h"

// Creates a new vehicle object with given ID and destination
Vehicle *create_vehicle(const char *id, const char *target) {
    Vehicle *v = malloc(sizeof(Vehicle));
    v->type = TYPE_VEHICLE;
    strncpy(v->vehicle_id, id, MAX_ID_LENGTH);
    strncpy(v->target_road, target, MAX_ID_LENGTH);
    return v;
}

// Generates a string representation of the vehicle (ID and target road)
char *get_vehicle_string(void *v) {
    Vehicle *veh = (Vehicle *)v;

    int size = snprintf(NULL, 0, "[VEHICLE] id: %s → %s", veh->vehicle_id, veh->target_road);
    if (size < 0) return NULL;

    char *buffer = malloc(size + 1);
    if (!buffer) return NULL;

    snprintf(buffer, size + 1, "[VEHICLE] id: %s → %s", veh->vehicle_id, veh->target_road);

    return buffer;
}

// Prints vehicle's string representation to stdout
void print_vehicle(void *v) {
    char *vehicle_str = get_vehicle_string(v);
    printf("%s\n", vehicle_str);
    free(vehicle_str);
}

// Frees memory allocated for a vehicle object
void free_vehicle(void *v) {
    free(v);
}
