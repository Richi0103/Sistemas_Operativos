#include <stdio.h>

int main() {
    int n, i;
    unsigned long long t1 = 0, t2 = 1, sig;

    printf("Ingrese el número de términos: ");
    scanf("%d", &n);

    printf("Serie de Fibonacci: ");

    for (i = 1; i <= n; ++i) {
        printf("%llu ", t1);
        sig = t1 + t2;
        t1 = t2;
        t2 = sig;
    }

    return 0;
}
