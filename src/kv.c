#include <kv.h>
#include <string.h>
#include <stdlib.h>

#define TOMBSTONE 0x01

size_t hash(char *val, int capacity) {
    // example hash function, not cryptographically secure
    size_t hash = 0x13371337deadbeef;
    
    // end of the string is zero so this loop works
    while(*val) {
        hash ^= *val;       // XOR
        hash = hash << 8;   // left shift
        hash += *val;
        val ++;
    }

    return hash % capacity; 
}

// fn kv_get
// params:
//  - db:       a pointer to the db
//  - key:      a pointer to the key value
// returns: a pointer to the key if found, NULL if not found
char *kv_get(kv_t *db, char *key)  {
    if (!db || !key) return NULL;

    size_t idx = hash(key, db -> capacity);

    for (int i = 0; i < db -> capacity -1; i++) {
        size_t real_idx  = (idx + i) % db -> capacity; 
        kv_entry_t *entry  = &db -> entries[real_idx];

        if (entry -> key == NULL) return NULL; 
        if (entry -> key 
            && entry -> key != (void*)TOMBSTONE
            && !strcmp(entry -> key, key)
        ) return entry -> value;
    }

    return NULL;
}

// fn kv_delete
// params:
//  - db:       a pointer to the db
//  - key:      a pointer to the key value
// returns: the index of deletion or 0 if not found
int kv_delete(kv_t *db, char *key)  {
    if (!db || !key) return -1;
    size_t idx = hash(key, db -> capacity);

    for (int i = 0; i < db -> capacity -1; i++) {
        size_t real_idx  = (idx + i) % db -> capacity; 
        kv_entry_t *entry  = &db -> entries[real_idx];

        if (entry -> key == NULL) return -1; 
        if (entry -> key 
            && entry -> key != (void*)TOMBSTONE
            && !strcmp(entry -> key, key)
        ) {
            free(entry -> key);   
            free(entry -> value);
            db -> count --;
            entry -> key = (void*)TOMBSTONE;
            entry -> value = NULL;
            return 0;
        }
    }
    return -1;
}

// fn kv_put 
// params:
//  - db:       a pointer to the db
//  - key:      a pointer to the key value
//  - value:    a pointer to the value itself
// returns: the index of the key
// error:
//  -1:         Error
//  -2:         Not found
int kv_put(kv_t *db, char *key, char *value) {
    if (!db || !key || !value) return -1;

    size_t idx = hash(key, db -> capacity);

    for (int i = 0; i < db -> capacity -1; i++) {
        size_t real_idx = (idx + i) % db -> capacity;
        kv_entry_t *entry = &db -> entries[real_idx];

        if (entry -> key && entry -> key != (void*)TOMBSTONE && !strcmp(entry -> key, key)) {
            // stack allocated values may be freed at some point therefore move to heap to give control of lifetime
            char *newval = strdup(value);
            if (!newval) {
                return -1;
            }
            free(entry -> value);
            entry -> value = newval;
            return 0;
        } 

        if (!entry -> key || entry -> key == (void*)TOMBSTONE) {
            // stack allocated values may be freed at some point therefore move to heap to give control of lifetime
            char *newval = strdup(value);
            char *newkey = strdup(key);
            if (!newval || !newkey) {
                free(newkey);
                free(newval); 
                return -1;
            }
            entry -> value = newval;
            entry -> key = newkey;
            db -> count++;
            return 0;
        }
    }

    // database is full
    return -2;
}

kv_t *kv_init(size_t capacity) {
    if (capacity == 0) return NULL;

    kv_t *table = malloc(sizeof(kv_t));
    if (table == NULL) {
        return NULL;
    }

    table -> capacity = capacity;
    table -> count = 0;
    // calloc allocates and zeoes memory
    table -> entries = calloc(sizeof(kv_entry_t), capacity);

    if (table -> entries == NULL) {
        return NULL;
    } else {
        return table;
    }
}

void kv_free(kv_t *db) {
    // if (!db) return -1;

    // free entries
    for (int i = 1; i < db -> capacity -1; i++) {
        kv_entry_t *e = &db -> entries[i];
        if (e -> key && e -> key != (void*)TOMBSTONE) {
            free(e -> key);
            free(e -> value);
            e -> key = NULL;
            e -> value = NULL;
            db -> count --;
        }
    }

    free(db -> entries);
    free(db);
}