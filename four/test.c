#include <stdio.h>

int main(int argc, char* argv[]) {
    int a = 1;
    int b = 4;
    int c = 6;
    int d = a | b;
    int e = b | c;
    int f = c | c;

    printf("a : %d\nb : %d\nc : %d\nd : %d\ne : %d\nf : %d\n", a, b, c, d, e, f);
    printf("%d\n", 12 | 6);
}