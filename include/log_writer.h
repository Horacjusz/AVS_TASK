#ifndef LOG_WRITER_H
#define LOG_WRITER_H

#include "queue.h"

void write_log_to_json(Queue *log_queue, const char *output_path);

#endif
