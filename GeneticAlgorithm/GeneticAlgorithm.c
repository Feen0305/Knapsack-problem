#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "GeneticAlgorithm.h"

// Function to get input data from user
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

    // Display the input data
    printf("\n[INPUT]Data initialized:\n");
    printf("Number of items: %d\n", *n);
    printf("Knapsack capacity: %d\n", *W);
    printf("Items (weight, value):\n");
    for (int i = 0; i < *n; i++)
    {
        printf("Item %d: (%d, %d)\n", i + 1, (*weights)[i], (*values)[i]);
    }
    printf("\n");
}

int main()
{
    int *weights, *values, n, W;

    // Get input data
    initialize_sample(&weights, &values, &n, &W);

    double Time_Limit;
    printf("Enter the time limit for the GA in seconds: ");
    scanf("%lf", &Time_Limit);

    int Time_Limit_Result;

    // Run genetic algorithm
    int result1 = GreedyRandom_GA_Knapsack(n, weights, values, W, Time_Limit, &Time_Limit_Result);
    int result2 = GreedyPop_GA_Knapsack(n, weights, values, W);

    // Print result
    printf("[RESULT] Maximum value found using Greedy Random GA: %d\n", result1);
    printf("[RESULT] Maximum value found within time limit: %d\n", Time_Limit_Result);
    printf("[RESULT] Maximum value found using Greedy Population GA: %d\n", result2);

    // Free memory
    free(weights);
    free(values);

    return 0;
}