#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../include/parser.h"
#include "../include/vehicle.h"
#include "../include/queue.h"
#include "../include/step.h"
#include "../include/log_writer.h"
#include "../include/common.h"

void tell_lights(int lights) {
    for (int i = 0; i < DIRECTIONS_SIZE; i++) {
        if (lights & (1 << i)) {
            if (lights & (1 << DIRECTIONS_SIZE)) {
                printf("Direction %s: GREEN\n", directions[i]);
            } else {
                printf("Direction %s: YELLOW\n", directions[i]);
            }
        } else {
            printf("Direction %s: RED\n", directions[i]);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s input.json output.json\n", argv[0]);
        return 1;
    }
    printf("Starting simulation. Showing commands, traffic lights and vehicles.\n");
    // Main queue holds traffic queues for each road
    Queue *main_queue = create_queue("main", TYPE_QUEUE);
    // Log queue stores queues of vehicles that left the intersection per step
    Queue *log_queue = create_queue("log", TYPE_QUEUE);

    int lights = 0;

    // Load and parse the JSON input
    char *content = read_file(argv[1]);
    Queue *commands = split_commands(content);
    QueueNode *curr = commands->head;

    // Iterate over each parsed command
    while (curr) {
        char *cmd = (char *)curr->data;
        lights |= (1 << DIRECTIONS_SIZE);

        // Handle addVehicle command
        if (strstr(cmd, "\"type\":\"addVehicle\"")) {
            char *vehicle_id = extract_json_field(cmd, "vehicleId");
            char *start_road = extract_json_field(cmd, "startRoad");
            char *end_road = extract_json_field(cmd, "endRoad");

            lights &= ~(1 << DIRECTIONS_SIZE);

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

        lights = compute_best_mask(main_queue, lights);
        #ifdef DEBUG
            printf("\nCommand: %s\n", cmd);
            tell_lights(lights);
        #endif

        // Handle step command – process one simulation step
        if (strstr(cmd, "\"type\":\"step\"")) {
            Queue *left = create_queue("left", TYPE_QUEUE);
            
            for (int i = 0; i < DIRECTIONS_SIZE; i++) {
                if (lights & (1 << i)) {
                    Vehicle *v = (Vehicle *)dequeue(find_queue_in_queue(main_queue, directions[i]));
                    enqueue(left, v);

                    #ifdef DEBUG
                        printf("Vehicle %s went from %s to %s\n", v->vehicle_id, directions[i], v->target_road);
                    #endif
                }
            }
            enqueue(log_queue, left);
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
