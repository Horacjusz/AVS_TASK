#ifndef COMMON_H
#define COMMON_H

#define MAX_ID_LENGTH 64
#define PREDEFINED_DIRECTIONS {"north", "east", "south", "west"}
#define DIRECTIONS_SIZE 4

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

// If compiling for an embedded system, include embedded function overrides
#ifdef EMBEDDED
#include "embedded.h"
#endif

extern const char *directions[];

// Enum to distinguish between different types of data stored in queues
typedef enum ObjectType {
    TYPE_UNKNOWN, // Unknown type (fallback/default)
    TYPE_VEHICLE, // Represents a vehicle
    TYPE_QUEUE,    // Represents a queue of objects
    TYPE_STRING   // Represents a string (for debugging or logging)
} ObjectType;

#endif
