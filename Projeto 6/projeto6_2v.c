#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int main() {
    int n = 10000000;
    int count = 0;
    unsigned int seed = (unsigned int)time(NULL);

    #pragma omp parallel default(none) shared(n, count, seed)
    {
        int local_count = 0;
        unsigned int thread_id = omp_get_thread_num();
        unsigned int local_seed = seed + thread_id;

        // rand() não é thread-safe, então cada thread usa seu próprio srand
        srand(local_seed);

        #pragma omp for
        for (int i = 0; i < n; i++) {
            double x = (double)rand() / RAND_MAX;
            double y = (double)rand() / RAND_MAX;

            if (x * x + y * y <= 1.0)
                local_count++;
        }

        #pragma omp critical
        {
            count += local_count;
        }
    }

    double pi = 4.0 * count / n;
    printf("Estimativa de pi correta: %f\n", pi);
    return 0;
}
