#include <stdio.h>
#include <kv.h>

int main() {
    kv_t  *table = kv_init(1024);
    printf("%p\n", table);
    printf("%ld\n", table -> capacity);

    int a = kv_put(table, "color", "red");
    int b = kv_put(table, "color", "blue");
    int c = kv_put(table, "lala", "hoho");

    printf("%d, %d, %d\n", a, b, c);

    for (int i = 0; i < table -> capacity; i++) {
        if (table -> entries[i].key) {
            printf("%s: %s\n", table -> entries[i].key, table -> entries[i].value);
        }
    }
}