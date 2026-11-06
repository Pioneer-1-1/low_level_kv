#include <stdio.h>
#include <kv.h>

int main() {
    kv_t  *table = kv_init(3);
    printf("%p", table);
    printf("%ld\n", table -> capacity);

    kv_put(table, "hehe", "haha");
    kv_put(table, "hehe", "hoho");
    kv_put(table, "lala", "hoho");

    for (int i = 0; i < table -> capacity; i++) {
        if (table -> entries[i].key) {
            printf("%s: %s\n", table -> entries[i].key, table -> entries[i].value);
        }
    }
}