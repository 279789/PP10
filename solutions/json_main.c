#include <jansson.h>
#include <stdio.h>
int main(void) {
    json_t *root = json_pack("{s:i, s:s}", "id", 1, "name", "Alice");
    char *dump = json_dumps(root, 0);
    printf("%s\n", dump);
    free(dump);
    json_decref(root);
    return 0;
}
