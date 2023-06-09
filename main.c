#include <stdio.h>
#include "include/tgaimage.h"
int main() {
    printf("%llu\n", sizeof(struct TGAHeader));
    return 0;
}
