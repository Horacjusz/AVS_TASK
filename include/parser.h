#ifndef PARSER_H
#define PARSER_H

extern long input_length;
extern int command_count;

char *read_file(const char *filename);
char **split_commands(const char *json);
char *extract_json_field(const char *cmd, const char *key);

#endif
