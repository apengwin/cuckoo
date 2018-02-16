
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

void
resize(hashtable *table) {
    hashtable *new_table = init_hashtable(table->size * 2);
    int i;
    int j;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < table->size; j++) {
            if (table->entries[i][j] != NULL) {
                put(new_table, table->entries[i][j]->key, table->entries[i][j]->value);
                free(table->entries[i][j]);
            }
        }
        free(table->entries[i]);
    }
    free(table->entries);
    free(table);
    *(&table) = new_table;
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
    int hasher[] = {0,0};
    hash(key, hasher, table->size);
    for (int i = 0; i < 2; i++) {
        if (table->entries[i][hasher[i]] == NULL) {
            entry *new_struct = malloc(sizeof(entry));
            new_struct->key = key;
            new_struct->value = value;
            table->entries[i][hasher[i]] = new_struct;
            table->num_buckets++;
            return 1;
        } else {
            if (!strcmp(table->entries[i][hasher[i]]->key, key)) {
                table->entries[i][hasher[i]]->value = value;
                return 1;
            }
        }
    }
    char *temp_key;
    char *temp_value;
    entry *curr_entry;
    int i = 0;
    while (1) {
        curr_entry = table->entries[i % 2][hasher[i % 2]];
        if (curr_entry == NULL) {
            entry *new_struct = malloc(sizeof(entry));
            new_struct->key = key;
            new_struct->value = value;
            table->entries[i % 2 ][hasher[i % 2]] = new_struct;
            table->num_buckets++;
            return 1;
        }
        temp_key = curr_entry->key;
        temp_value = curr_entry->value;
        table->entries[i % 2][hasher[i %2]]->key = key;
        table->entries[i % 2][hasher[i %2]]->value = value;

        key = temp_key;
        value = temp_value;
        hash(key, hasher, table->size);
        i++;
    }
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
