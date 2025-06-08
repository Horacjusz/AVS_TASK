#ifndef TEST_REGISTRY_H
#define TEST_REGISTRY_H

#include <stdbool.h>

typedef void (*TestFunc)();

typedef struct {
    const char *name;
    TestFunc func;
} TestEntry;

extern bool current_test_success;
extern const char *current_test_name;
extern void report_result();

extern TestEntry TESTS[];
extern int NUM_TESTS;

void test_create_vehicle_basic();
void test_get_vehicle_string();
void test_memory_allocation();
void test_direction_val();
void test_are_colliding_false();
void test_are_colliding_true();
void test_step_basic();
void test_enqueue_and_dequeue();
void test_add_queue_if_missing();
void test_find_queue();
void test_get_queue_string();
void test_find_queue_in_queue();
void test_extract_json_field();
void test_split_commands_basic();
void test_write_log_to_json_basic();

#endif
