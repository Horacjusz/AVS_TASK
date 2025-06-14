#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/queue.h"
#include "../include/common.h"

#define MAX_COMMANDS 100

long input_length = 0;
int command_count = 0;

// Reads the entire content of a file into memory
char *read_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Cannot open input file");
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    input_length = ftell(file);

    rewind(file);

    char *buffer = (char *)malloc(input_length + 1);
    if (!buffer) {
        perror("Memory allocation failed");
        exit(1);
    }

    fread(buffer, 1, input_length, file);
    buffer[input_length] = '\0';
    fclose(file);
    return buffer;
}

// Splits the input JSON into individual command strings
Queue *split_commands(const char *json) {
    const char *ptr = strstr(json, "\"commands\"");
    if (!ptr) {
        printf("No commands found.\n");
        return NULL;
    }

    ptr = strchr(ptr, '[');
    if (!ptr) return NULL;
    ptr++;

    // Initializes a queue to hold extracted command objects
    Queue *commands_queue = create_queue("commands", TYPE_STRING);

    while (*ptr && *ptr != ']') {
        if (*ptr == '{') {
            const char *start = ptr;
            int brace_count = 1;
            ptr++;

            // Finds the matching closing brace for each JSON object
            while (*ptr && brace_count > 0) {
                if (*ptr == '{') brace_count++;
                if (*ptr == '}') brace_count--;
                ptr++;
            }

            if (brace_count == 0) {
                int len = ptr - start;
                char *raw = malloc(len + 1);
                if (!raw) {
                    perror("Memory allocation failed");
                    exit(1);
                }
                strncpy(raw, start, len);
                raw[len] = '\0';

                // Cleans the command string from unnecessary whitespace
                char *cleaned = malloc(len + 1);
                if (!cleaned) {
                    perror("Memory allocation failed");
                    exit(1);
                }

                int ci = 0;
                int in_braces = 0;
                for (int i = 0; i < len; i++) {
                    char c = raw[i];

                    if (c == '{') {
                        in_braces++;
                        continue;
                    } else if (c == '}') {
                        in_braces--;
                        continue;
                    } else if (c == ' ' || c == '\n' || c == '\t') {
                        continue;
                    }

                    if (c == ',' && raw[i + 1] == '\n') {
                        continue;
                    }

                    cleaned[ci++] = c;
                }
                cleaned[ci] = '\0';

                if (cleaned[0] == '\n') {
                    memmove(cleaned, cleaned + 1, strlen(cleaned));
                }

                free(raw);

                // Adds the cleaned command to the queue
                enqueue(commands_queue, cleaned);
            }
        } else {
            ptr++;
        }
    }

    return commands_queue;
}

// Extracts a string value for a given key from a JSON-like command string
char *extract_json_field(const char *cmd, const char *key) {
    char pattern[MAX_ID_LENGTH + 10];
    snprintf(pattern, sizeof(pattern), "\"%s\":\"", key);
    char *start = strstr(cmd, pattern);
    if (!start) return NULL;

    start += strlen(pattern);
    char *end = strchr(start, '"');
    if (!end) return NULL;

    size_t len = end - start;
    char *result = malloc(len + 1);
    if (!result) return NULL;

    strncpy(result, start, len);
    result[len] = '\0';
    return result;
}
