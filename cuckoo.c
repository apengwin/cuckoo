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

hashtable *
init_hashtable(int size) {
    hashtable *init = malloc(sizeof(hashtable));
    init->size = size;
    init->num_buckets = 0;
    init->load_factor = 0.5;
    init->entries = malloc(size * sizeof(entry*));
    int i;
    for(i = 0; i < size; i++) {
        init->entries[i] = NULL;
    }
    return init;
}

char *
get(hashtable *table, char *key) {
    int hasher[] = {0,0};
    hash(key, hasher);
    for (int i = 0; i < 2; i++) {
        if (table->entries[hasher[i]] != NULL) {
            entry *buck = table->entries[hasher[i]];
            if (!strcmp(buck->key, key)) {
                return buck->value;
            }
        }
    }
    return NULL;
}

int
delete(hashtable *table, char *key) {
    int hasher[] = {0,0};
    hash(key, hasher);
    for (int i = 0; i < 2; i++) {
        if (table->entries[hasher[i]] != NULL) {
            entry *buck = table->entries[hasher[i]];
            if (!strcmp(buck->key, key)) {
                buck = NULL;
                table->num_buckets --;
                return 1;
            }
        }
    }
    return 0;
}


int
insert(hashtable *table, char *key, char *value) {
    int hasher[] = {0,0};
    hash(key, hasher);
    for (int i = 0; i < 2; i++) {
        if (table->entries[hasher[i]] == NULL) {
            *table->entries[hasher[i]] = entry{key, value};
            return 1;
        }
    }
}

int
main() {
    hashtable *hasher =init_hashtable(8);
    if (hasher->entries[7] == NULL) {
        printf("yes\n");
    }
    return 0;
}
