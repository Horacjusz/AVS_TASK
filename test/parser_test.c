#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/parser.h"
#include "../include/queue.h"
#include "../include/common.h"
#include "test_registry.h"

// `current_test_success` i `current_test_name` są globalne i dostępne z test_main.c

void test_extract_json_field() {
    const char *json = "{\"type\":\"addVehicle\",\"vehicleId\":\"v1\",\"startRoad\":\"north\",\"endRoad\":\"south\"}";

    char *type = extract_json_field(json, "type");
    char *vehicleId = extract_json_field(json, "vehicleId");
    char *startRoad = extract_json_field(json, "startRoad");
    char *endRoad = extract_json_field(json, "endRoad");
    char *missing = extract_json_field(json, "notExists");

    current_test_success =
        strcmp(type, "addVehicle") == 0 &&
        strcmp(vehicleId, "v1") == 0 &&
        strcmp(startRoad, "north") == 0 &&
        strcmp(endRoad, "south") == 0 &&
        missing == NULL;

    free(type);
    free(vehicleId);
    free(startRoad);
    free(endRoad);
    free(missing);  // NULL-safe
}

void test_split_commands_basic() {
    const char *json =
        "{ \"commands\": ["
        "{\"type\":\"addVehicle\",\"vehicleId\":\"v1\",\"startRoad\":\"north\",\"endRoad\":\"south\"},"
        "{\"type\":\"step\"}"
        "]}";

    Queue *q = split_commands(json);
    current_test_success =
        q != NULL &&
        q->length == 2 &&
        q->type == TYPE_STRING;

    free_queue_deep(q);
}
