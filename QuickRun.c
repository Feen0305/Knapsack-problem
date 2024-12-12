#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Include algorithm headers
// #include "BruteForce/BruteForce.h"
#include "DP_ButtomUp/DP_ButtomUp.h"
#include "DP_TopDown/DP_TopDown.h"
#include "Greedy\Greedy.h"
#include "GeneticAlgorithm\GeneticAlgorithm.h"

void initialize_sample(int **weights, int **values, int *n, int *W)
{
    // Get number of items
    printf("Enter the number of items: ");
    scanf("%d", n);

    // Allocate memory
    *weights = (int *)malloc(*n * sizeof(int));
    *values = (int *)malloc(*n * sizeof(int));

    // Get weights and values
    printf("Enter the weight and value of each item:\n");
    for (int i = 0; i < *n; i++)
    {
        printf("Item %d:\n", i + 1);
        printf("Weight: ");
        scanf("%d", &(*weights)[i]);
        printf("Value: ");
        scanf("%d", &(*values)[i]);
    }

    // Get knapsack capacity
    printf("Enter the knapsack capacity: ");
    scanf("%d", W);

    printf("\n");
}

int main()
{
    int *weights, *values, n, W;

    // Get input data
    initialize_sample(&weights, &values, &n, &W);

    // double Temp = 0;
    // int Tmp = 0;

    // Run genetic algorithm
    int result1 = DP_TopDown_Knapsack(n, weights, values, W);
    // int result2 = GreedyRandom_GA_Knapsack(n, weights, values, W, Temp, &Tmp);

    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);
    int result3 = GreedyPop_GA_Knapsack(n, weights, values, W);
    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    // Print result
    printf("[RESULT] Maximum value found using Dynamic Programming: %d\n", result1);
    // printf("[RESULT] Maximum value found using Greedy Random Genetic Algorithm: %d\n", result2);
    printf("[RESULT] Maximum value found using Greedy Population Genetic Algorithm: %d, Compute within Time %.3f\n\n", result3, elapsed_time);
    printf("Accuracy = %.2f", (float)result3 / (float)result1);

    // Free memory
    free(weights);
    free(values);

    return 0;
}