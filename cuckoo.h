
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
