#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int main() {
    int i, n = 10000000;
    int count = 0;

    srand(time(NULL)); //

    #pragma omp parallel for
    for (i = 0; i < n; i++) {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;

        if (x * x + y * y <= 1.0)
            count++;
    }

    double pi = 4.0 * count / n;
    printf("Estimativa incorreta de pi: %f\n", pi);

    return 0;
}
