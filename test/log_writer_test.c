#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/log_writer.h"
#include "../include/queue.h"
#include "../include/vehicle.h"
#include "../include/common.h"
#include "test_registry.h"

#define OUTPUT_FILE "test_output.json"

void test_write_log_to_json_basic() {
    Queue *log_queue = create_queue("log", TYPE_QUEUE);

    Queue *step1 = create_queue("step1", TYPE_VEHICLE);
    enqueue(step1, create_vehicle("vehicle1", "north"));
    enqueue(step1, create_vehicle("vehicle2", "south"));

    Queue *step2 = create_queue("step2", TYPE_VEHICLE);

    enqueue(log_queue, step1);
    enqueue(log_queue, step2);

    write_log_to_json(log_queue, OUTPUT_FILE);

    FILE *fp = fopen(OUTPUT_FILE, "r");
    if (!fp) {
        current_test_success = 0;
        return;
    }

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    char *content = malloc(size + 1);
    fread(content, 1, size, fp);
    content[size] = '\0';
    fclose(fp);

    current_test_success = strstr(content, "\"vehicle1\"") != NULL &&
                           strstr(content, "\"vehicle2\"") != NULL &&
                           strstr(content, "\"leftVehicles\": []") != NULL;

    free(content);
    free_queue_deep(log_queue);
    remove(OUTPUT_FILE);
}
