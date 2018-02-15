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
    // lmao
    entry*** entries;
} hashtable;

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

char *
get(hashtable *table, char *key) {
    int hasher[] = {0,0};
    hash(key, hasher);
    for (int i = 0; i < 2; i++) {
        if (table->entries[i][hasher[i]] != NULL) {
            entry *buck = table->entries[i][hasher[i]];
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
        if (table->entries[i][hasher[i]] != NULL) {
            entry *buck = table->entries[i][hasher[i]];
            if (!strcmp(buck->key, key)) {
                *(&buck) = NULL;
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
        if (table->entries[i][hasher[i]] == NULL) {
            *table->entries[i][hasher[i]] = entry{key, value};
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
        curr_entry = table->entries[i % 2][hasher[i % 2]]
        if (curr_entry != NULL) {
            temp_key = curr_entry->key;
            temp_value = curr_entry->value;
        }
        *table->entries[hasher[i %2]] = key;
        *table->entries[hasher[i %2]] = value;
        if (curr_entry == NULL) {
            return 1;
        }
        key = temp_key;
        value = temp_value;
        hash(key, hasher);
        i++;
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
