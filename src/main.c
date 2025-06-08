#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/parser.h"
#include "../include/vehicle.h"
#include "../include/queue.h"
#include "../include/step.h"
#include "../include/log_writer.h"
#include "../include/common.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s input.json output.json\n", argv[0]);
        return 1;
    }

    // Main queue holds traffic queues for each road
    Queue *main_queue = create_queue("main", TYPE_QUEUE);
    // Log queue stores queues of vehicles that left the intersection per step
    Queue *log_queue = create_queue("log", TYPE_QUEUE);

    // Load and parse the JSON input
    char *content = read_file(argv[1]);
    Queue *commands = split_commands(content);
    QueueNode *curr = commands->head;

    // Iterate over each parsed command
    while (curr) {
        char *cmd = (char *)curr->data;

        // Handle addVehicle command
        if (strstr(cmd, "\"type\":\"addVehicle\"")) {
            char *vehicle_id = extract_json_field(cmd, "vehicleId");
            char *start_road = extract_json_field(cmd, "startRoad");
            char *end_road = extract_json_field(cmd, "endRoad");

            if (vehicle_id && start_road && end_road) {
                Vehicle *v = create_vehicle(vehicle_id, end_road);

                // Look for existing queue for the road, create if missing
                Queue *target_queue = find_queue_in_queue(main_queue, start_road);
                if (!target_queue) {
                    target_queue = create_queue(start_road, TYPE_QUEUE);
                    enqueue(main_queue, target_queue);
                }

                // Add the vehicle to the appropriate road queue
                enqueue(target_queue, v);
            }

            // Clean up temporary fields
            free(vehicle_id);
            free(start_road);
            free(end_road);
        }

        // Handle step command – process one simulation step
        else if (strstr(cmd, "\"type\":\"step\"")) {
            step(main_queue, log_queue);
        }

        curr = curr->next;
    }

    // Output the results to JSON
    write_log_to_json(log_queue, argv[2]);

    // Clean up memory depending on the target build
    #ifdef EMBEDDED
        reset_memory();
    #else
        free_queue_deep(main_queue);
        free_queue_deep(log_queue);
        free_queue_deep(commands);
        free(content);
    #endif

    return 0;
}
