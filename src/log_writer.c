#include <stdio.h>
#include "vehicle.h"
#include "queue.h"

void write_log_to_json(Queue *log_queue, const char *output_path) {
    FILE *fp = fopen(output_path, "w");
    if (!fp) {
        perror("Failed to open output file");
        return;
    }

    fprintf(fp, "{\n  \"stepStatuses\": [\n");

    QueueNode *step_node = log_queue->head;
    int step_index = 0;

    while (step_node) {
        Queue *left = (Queue *)step_node->data;

        fprintf(fp, "    {\n      \"leftVehicles\": [\n");

        QueueNode *veh_node = left->head;
        int vehicle_count = 0;
        while (veh_node) {
            Vehicle *v = (Vehicle *)veh_node->data;
            fprintf(fp, "        \"%s\"%s\n", v->vehicle_id, veh_node->next ? "," : "");
            veh_node = veh_node->next;
            vehicle_count++;
        }

        fprintf(fp, "      ]\n    }");


        if (step_node->next) {
            fprintf(fp, ",\n");
        } else {
            fprintf(fp, "\n");
        }

        step_node = step_node->next;
        step_index++;
    }

    fprintf(fp, "  ]\n}\n");

    fclose(fp);
}
