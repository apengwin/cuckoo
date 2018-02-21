
typedef struct {
    char *key;
    char *value;
} entry;

typedef struct {
    // I fucked up and mixed up the definitions of size and num_buckets. Oh well.
    int size;
    float load_factor;
    int num_buckets;
    entry*** entries;
} hashtable;

void
hash(char *key, int *res, int size);

hashtable *
init_hashtable(int size);

void
resize(hashtable *table);

int
get(hashtable *table, char *key, char **value);

int
delete(hashtable *table, char *key);

int
put(hashtable *table, char *key, char *value);
