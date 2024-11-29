#División en C con restas.

´´´ C
#include <stdio.h>

double div (double n, double n2){

    if(n<n2){
        return 0;
    }

    return 1 + div(n-n2, n2);
}

int main(){
    double n=10, n2=2;

    double r = div(n,n2);

    printf("Resultado: %f", r);
}
´´´
