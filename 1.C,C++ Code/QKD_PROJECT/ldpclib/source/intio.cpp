/* INTIO.CPP - Routines to read and write integers one byte at a time. */

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>  // for INT_MIN and INT_MAX

#include "../include/intio.h"

// Enable debug logging for integer I/O
#define INTIO_DEBUG 1

int intio_read(FILE *f)
{
    unsigned char b[4];
    int top;
    int i;

    for (i = 0; i < 4; i++) {
        if (fread(&b[i], 1, 1, f) != 1) {
            if (feof(f)) {
                fprintf(stderr, "[intio_read] Error: Unexpected end-of-file while reading byte %d\n", i);
            } else if (ferror(f)) {
                perror("[intio_read] Error: Failed to read from file");
            } else {
                fprintf(stderr, "[intio_read] Error: Unknown error reading byte %d\n", i);
            }
            return 0;
        }
    }

    // Convert to signed integer using 2's complement
    top = b[3] > 127 ? (int)b[3] - 256 : b[3];

    int result = (top << 24) + (b[2] << 16) + (b[1] << 8) + b[0];

#if INTIO_DEBUG
    printf("[intio_read] Read integer: %d (bytes: %u %u %u %u)\n", result, b[0], b[1], b[2], b[3]);
#endif

    return result;
}

void intio_write(FILE *f, int v)
{
    unsigned char b;
    int i;

#if INTIO_DEBUG
    printf("[intio_write] Writing integer: %d\n", v);
#endif

    for (i = 0; i < 3; i++) {
        b = v & 0xff;
        if (fwrite(&b, 1, 1, f) != 1) {
            perror("[intio_write] Error: Failed to write byte");
        }
        v >>= 8;
    }

    // Write final byte as signed value
    b = v > 0 ? v : v + 256;
    if (fwrite(&b, 1, 1, f) != 1) {
        perror("[intio_write] Error: Failed to write final byte");
    }
}
