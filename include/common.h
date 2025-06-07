#ifndef COMMON_H
#define COMMON_H

#define MAX_ID_LENGTH 64
#define PREDEFINED_DIRECTIONS {"north", "east", "south", "west"}
#define DIRECTIONS_SIZE 4

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

typedef enum ObjectType {
    TYPE_UNKNOWN,
    TYPE_VEHICLE,
    TYPE_QUEUE
} ObjectType;

#endif
