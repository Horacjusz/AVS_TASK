#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "vehicle.h"
#include "queue.h"
#include "step.h"
#include "log_writer.h"
#include "common.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s input.json output.json\n", argv[0]);
        return 1;
    }

    Queue *main_queue = create_queue("main", TYPE_QUEUE);
    Queue *log_queue = create_queue("log", TYPE_QUEUE);

    char *content = read_file(argv[1]);
    char **commands = split_commands(content);

    for (int i = 0; i < command_count; i++) {
        char *cmd = commands[i];

        if (strstr(cmd, "\"type\":\"addVehicle\"")) {
            char *vehicle_id = extract_json_field(cmd, "vehicleId");
            char *start_road = extract_json_field(cmd, "startRoad");
            char *end_road = extract_json_field(cmd, "endRoad");

            if (vehicle_id && start_road && end_road) {
                Vehicle *v = create_vehicle(vehicle_id, end_road);

                Queue *target_queue = find_queue_in_queue(main_queue, start_road);
                if (!target_queue) {
                    target_queue = create_queue(start_road, TYPE_QUEUE);
                    enqueue(main_queue, target_queue);
                }

                enqueue(target_queue, v);
            }

            free(vehicle_id);
            free(start_road);
            free(end_road);
        }

        else if (strstr(cmd, "\"type\":\"step\"")) {
            step(main_queue, log_queue);
        }

        free(commands[i]);
    }


    printf("\nMAIN QUEUE:\n");
    print_queue(main_queue);
    printf("\nLOG QUEUE:\n");
    print_queue(log_queue);

    write_log_to_json(log_queue, argv[2]);

    #ifdef EMBEDDED
        reset_memory();
    #else
        free_queue_deep(main_queue);
        free_queue_deep(log_queue);
        free(commands);
        free(content);
    #endif

    return 0;
}
