# Potencias con sumas

```c
#include <stdio.h>
double mult(double a, double b){
    if (b==0) return 0;
    
    return a + mult(a, b-1);
}
double pot(double n, double n2){
    if (n2==0)
    
        return 1;
    
    
    return mult(n,pot(n, n2-1));
}
int main(){
    double n=5, n2=3;
    double r = pot(n,n2);
    printf("Resultado: %f", r);
}
```
