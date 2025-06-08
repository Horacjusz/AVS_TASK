#include "test_registry.h"
#include <stdio.h>

bool current_test_success = false;
const char *current_test_name = NULL;

void report_result() {
}

TestEntry TESTS[] = {
    { "test_create_vehicle_basic", test_create_vehicle_basic },
    { "test_get_vehicle_string", test_get_vehicle_string },
    { "test_memory_allocation", test_memory_allocation },
    { "test_direction_val", test_direction_val },
    { "test_are_colliding_false", test_are_colliding_false },
    { "test_are_colliding_true", test_are_colliding_true },
    { "test_step_basic", test_step_basic },
    { "test_enqueue_and_dequeue", test_enqueue_and_dequeue },
    { "test_add_queue_if_missing", test_add_queue_if_missing },
    { "test_find_queue", test_find_queue },
    { "test_get_queue_string", test_get_queue_string },
    { "test_find_queue_in_queue", test_find_queue_in_queue },
    { "test_extract_json_field", test_extract_json_field },
    { "test_split_commands_basic", test_split_commands_basic },
    { "test_write_log_to_json_basic", test_write_log_to_json_basic }
};
int NUM_TESTS = sizeof(TESTS) / sizeof(TESTS[0]);

int main() {
    int passed = 0;
    for (int i = 0; i < NUM_TESTS; ++i) {
        current_test_success = false;
        current_test_name = TESTS[i].name;
        printf("Running test: %s... ", current_test_name);
        TESTS[i].func();
        if (current_test_success) {
            passed++;
            printf("SUCCESS\n");
        } else {
            printf("FAILURE [%s]\n", current_test_name);
        }
    }
    printf("\nTotal: %d / %d tests passed\n", passed, NUM_TESTS);
    return 0;
}
