#include <stdio.h>
#include "../include/vehicle.h"
#include "../include/queue.h"
#include "../include/common.h"

// Saves the log of vehicle movements to a JSON file.
// {
//   "stepStatuses": [
//     { "leftVehicles": ["vehicle1", "vehicle2"] },
//     ...
//   ]
// }
void write_log_to_json(Queue *log_queue, const char *output_path) {
    FILE *fp = fopen(output_path, "w");
    if (!fp) {
        perror("Failed to open output file");
        return;
    }

    fprintf(fp, "{\n  \"stepStatuses\": [\n");

    QueueNode *step_node = log_queue->head;

    while (step_node) {
        Queue *left = (Queue *)step_node->data; // Each step contains a queue of vehicles that left

        fprintf(fp, "    {\n      \"leftVehicles\": [");

        if (left->length == 0) {
            // Compact empty array
            fprintf(fp, "]\n");
        } else {
            // Non-empty array with indented items
            fprintf(fp, "\n");

            QueueNode *veh_node = left->head; // Iterate over vehicles in the current step
            while (veh_node) {
                Vehicle *v = (Vehicle *)veh_node->data;
                fprintf(fp, "        \"%s\"%s\n", v->vehicle_id, veh_node->next ? "," : "");
                veh_node = veh_node->next;
            }

            fprintf(fp, "      ]\n");
        }

        fprintf(fp, "    }");

        if (step_node->next) {
            fprintf(fp, ",\n");
        } else {
            fprintf(fp, "\n");
        }

        step_node = step_node->next;
    }

    fprintf(fp, "  ]\n}\n");
    fclose(fp);
}

