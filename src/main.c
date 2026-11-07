#include <stdio.h>
#include <kv.h>

int main() {
    printf("Hello world 2\n");
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

    char *val = kv_get(table, "color");
    char *val2 = kv_get(table, "lala");
    char *val3 = kv_get(table, "doesn't exist");

    printf("%s, %s, %s\n", val, val2, val3);
}