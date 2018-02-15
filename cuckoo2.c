#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    char *key;
    char *value;
} entry;

typedef struct {
    int size;
    float load_factor;
    int num_buckets;
    entry** entries;
} hashtable;

hashtable*
init_hashtable(int size) {
    hashtable *init = malloc(sizeof(hashtable));
    init->size = size;
    init->num_buckets = 0;
    init->load_factor = 0.5;
    init->entries = malloc(sizeof(entry*) * size);
    int i;
    for(i = 0; i < size; i++) {
        init->entries[i] = NULL;
    }
    return init;
}

char *
get(hashtable *table, char *key) {
}

void
place(hashtable *table, char *key, char *value) {
}

int
main() {
    hashtable *hasher =init_hashtable(8);
    if (hasher->entries[7] == NULL) {
        printf("yes\n");
    }
    return 0;
}
