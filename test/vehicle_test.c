#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/vehicle.h"
#include "../include/common.h"
#include "test_registry.h"

// `current_test_success` i `current_test_name` są globalne i dostępne z test_main.c

void test_create_vehicle_basic() {
    Vehicle *v = create_vehicle("v123", "north");
    current_test_success = strcmp(v->vehicle_id, "v123") == 0 && strcmp(v->target_road, "north") == 0;
    free_vehicle(v);
}

void test_get_vehicle_string() {
    Vehicle *v = create_vehicle("A7", "west");
    char *str = get_vehicle_string(v);
    current_test_success = strcmp(str, "[VEHICLE] id: A7 → west") == 0;
    free(str);
    free_vehicle(v);
}


void test_memory_allocation() {
    Vehicle *v = create_vehicle("mem", "check");
    current_test_success = v != NULL;
    free_vehicle(v);
}
