#ifndef PARSER_H
#define PARSER_H
#include "common.h"
#include "queue.h"

extern long input_length;
extern int command_count;

char *read_file(const char *filename);
Queue *split_commands(const char *json);
char *extract_json_field(const char *cmd, const char *key);

#endif
