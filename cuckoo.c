
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "cuckoo.h"

/**
 * Stupid hacky hash
 */
void
hash(char *key, int *res, int size) {
    uint64_t  hashed = 0;
    for (int i = 0; i < strlen(key); i++) {
        hashed += key[i] << (8* i);
    }
   *res = hashed % size;
   *(res + 1) = (hashed / size) % size;
}


hashtable *
init_hashtable(int size) {
    hashtable *init = malloc(sizeof(hashtable));
    init->size = size;
    init->num_buckets = 0;
    init->load_factor = 0.5;
    init->entries = malloc( 2 * sizeof(entry**));
    int i;
    int j;
    for (i = 0; i < 2; i++) {
        init->entries[i] = malloc(size * sizeof(entry*));
        for(j = 0; j < size; j++) {
            init->entries[i][j] = NULL;
        }
    }
    return init;
}

int
_put(entry ***entries, char *key, char *value) {
    int SIZE = sizeof(*entries) / sizeof(**entries);
    int hasher[] = {0,0};
    hash(key, hasher, SIZE);
    for (int i = 0; i < 2; i++) {
        if (entries[i][hasher[i]] == NULL) {
            entry *new_struct = malloc(sizeof(entry));
            new_struct->key = key;
            new_struct->value = value;
            entries[i][hasher[i]] = new_struct;
            return 1;
        } else {
            if (!strcmp(entries[i][hasher[i]]->key, key)) {
                entries[i][hasher[i]]->value = value;
                return 0;
            }
        }
    }
    char *temp_key;
    char *temp_value;
    entry *curr_entry;
    int i = 0;
    while (1) {
        curr_entry = entries[i % 2][hasher[i % 2]];
        if (curr_entry == NULL) {
            entry *new_struct = malloc(sizeof(entry));
            new_struct->key = key;
            new_struct->value = value;
            entries[i % 2 ][hasher[i % 2]] = new_struct;
            return 1;
        }
        temp_key = curr_entry->key;
        temp_value = curr_entry->value;
        entries[i % 2][hasher[i %2]]->key = key;
        entries[i % 2][hasher[i %2]]->value = value;

        key = temp_key;
        value = temp_value;
        hash(key, hasher, SIZE);
        i++;
    }
}

void
resize(hashtable *table) {
    entry ***new_entry = malloc(2 * sizeof(entry**));
    int i;
    int j;
    for (i = 0; i < 2; i++) {
        new_entry[i] = malloc(2 * table->size * sizeof(entry*));
        for(j = 0; j < 2 * table->size; j++) {
            new_entry[i][j] = NULL;
        }
    }

    for (i = 0; i < 2; i++) {
        for (j = 0; j < table->size; j++) {
            if (table->entries[i][j] != NULL) {
                _put(new_entry, table->entries[i][j]->key, table->entries[i][j]->value);
                free(table->entries[i][j]);
            }
        }
        free(table->entries[i]);
    }
    free(table->entries);
    table->entries = new_entry;
}


int
get(hashtable *table, char *key, char **value) {
    int hasher[] = {0,0};
    hash(key, hasher, table->size);
    for (int i = 0; i < 2; i++) {
        if (table->entries[i][hasher[i]] != NULL) {
            entry *buck = table->entries[i][hasher[i]];
            if (!strcmp(buck->key, key)) {
                *value = buck->value;
                return 1;
            }
        }
    }
    return 0;
}

int
delete(hashtable *table, char *key) {
    int hasher[] = {0,0};
    hash(key, hasher, table->size);
    for (int i = 0; i < 2; i++) {
        if (table->entries[i][hasher[i]] != NULL) {
            entry *buck = table->entries[i][hasher[i]];
            if (!strcmp(buck->key, key)) {
                free(buck);
                table->entries[i][hasher[i]] = NULL;
                //should free sometime...
                table->num_buckets --;
                return 1;
            }
        }
    }
    return 0;
}

int
put(hashtable *table, char *key, char *value) {
    if (table->num_buckets > table->load_factor * table->size * 2) {
        resize(table);
    }
    table->size += _put(table->entries, key, value);
    return 1;
}

int
main() {
    hashtable *hasher = init_hashtable(8);
    put(hasher, "a", "b");
    put(hasher, "b", "c");
    put(hasher, "c", "b");
    put(hasher, "d", "c");
    put(hasher, "u", "c");
    delete(hasher, "u");
    return 0;
}
