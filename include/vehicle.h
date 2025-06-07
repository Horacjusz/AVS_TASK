#ifndef VEHICLE_H
#define VEHICLE_H

#include "common.h"

typedef struct Vehicle {
    ObjectType type;
    char vehicle_id[MAX_ID_LENGTH];
    char target_road[MAX_ID_LENGTH];
} Vehicle;

Vehicle *create_vehicle(const char *id, const char *target);
void print_vehicle(void *v);
char *get_vehicle_string(void *v);

void free_vehicle(void *v);


#endif
