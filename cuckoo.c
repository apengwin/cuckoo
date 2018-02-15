#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *key;
    char *value;
} object;

typedef struct {
    int size;
    int num_buckets;
    float load_factor;
    object **entries;
} hashtable;

void
hash(hashtable *ht, int*pos, char *key) {
    int MODULUS = ht->num_buckets;
    int sol = 0;
    int i;
    for (i = 0; i < strlen(key); i++) {
        sol += (key[i] << 8 * i);
    }
    *pos = sol % MODULUS;
    *(pos + 1) = (sol / MODULUS) % MODULUS;
}

hashtable *
init_hashtable(int size) {
    hashtable *initted = malloc(sizeof(hashtable));
    initted->size = 0;
    initted->num_buckets = size;
    initted->load_factor = 0.75;
    initted->entries = malloc(sizeof(object*) * size);
    int i;
    for (i = 0; i < size; i++) {
        initted->entries[i] = NULL;
    }
    return initted;
}

char *
get(hashtable * ht, char * key) {
    int hashed[2] = {0,0};
    hash(ht, hashed, key);
    int index = *hashed;
    object kv = ht->entries[index];
    if (kv != NULL && !strcmp(kv.key, key)) {
        return kv.value;
    }
    index = *(hashed + 1);
    kv = ht->entries[index];
    if (kv != NULL & !strcmp(kv.key, key)) {
        return kv.value;
    }
    return NULL;
}

int
delete(hashtable * ht, char * key) {
    int hashed[2] = {0,0};
    hash(ht, hashed, key);
    int index = *hashed;
    object *kv = ht->entries[index];
    if (kv != NULL && !strcmp(kv.key, key)) {
        ht->entries[index] = NULL;
        return 1;
    }
    index = *(hashed + 1);
    kv = ht->entries[index];
    if (kv != NULL & !strcmp(kv.key, key)) {
        ht->entries[index] = NULL;
        return 1;
    }
    return 0;
}

void
insert(hashtable * ht, char * key, char *value) {
    int hashed[2] = {0,0};
    hash(ht, hashed, key);
    object *new = {key, value};
    if (ht->entries(*hashed) == NULL) {
        ht->entries(*hashed) = new;
        return;
    }
    if (ht->entries(*(hashed + 1)) == NULL) {
        ht->entries(*(hashed + 1)) = new;
        return;
    }
    int picker = 0;
    object temp;
    while (1) {
        temp = ht->entries(*(hashed + picker));
        ht->entries(*(hashed + picker)) = new;
        if (temp == NULL) {
            break;
        }
        new = temp;
        hash(ht, hashed, new.key);
    }
}
