#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Include algorithm headers
#include "BruteForce/BruteForce.h"
#include "DP_ButtomUp/DP_ButtomUp.h"
#include "DP_TopDown/DP_TopDown.h"
#include "Greedy/Greedy.h"
#include "GeneticAlgorithm/GeneticAlgorithm.h"

// Function to read weights, values, and weight capacity from a file
void read_knapsack_input(const char *filename, int **weights, int **values, int *n, int *W) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("\033[1;31mError: Could not open file %s\033[0m\n", filename); // Red text for errors
        exit(1);
    }

    // Read the number of items
    fscanf(file, "%d", n);

    // Allocate memory for weights and values
    *weights = (int *)malloc(*n * sizeof(int));
    *values = (int *)malloc(*n * sizeof(int));

    printf("\033[1;34m[FILE]\033[0m Reading weights and values from file...\n"); // Blue text for file info

    // Read weights and values from the file
    for (int i = 0; i < *n; i++) {
        fscanf(file, "%d %d", &(*weights)[i], &(*values)[i]);
    }

    // Read the weight capacity
    fscanf(file, "%d", W);

    fclose(file);
}

int main() {
    int *weights, *values, n, W;

    // List of filenames to loop through
    const char *filenames[] = {
        "testcase/knapsack_25_items.txt",
        "testcase/knapsack_50_items.txt",
        "testcase/knapsack_100_items.txt",
        "testcase/knapsack_500_items.txt",
        "testcase/knapsack_1000_items.txt"
    };
    int num_files = sizeof(filenames) / sizeof(filenames[0]);
    int result;
    clock_t start, end;
    double elapsed_time;

    printf("\033[1;32m========== Knapsack Problem Solver ==========\033[0m\n"); // Green title banner

    for (int i = 0; i < num_files; i++) {
        printf("\n\033[1;36m[FILE] Processing file: %s\033[0m\n", filenames[i]); // Cyan for file being processed

        // Read data from the current file
        read_knapsack_input(filenames[i], &weights, &values, &n, &W);

        printf("\033[1;33mNumber of items:\033[0m %d, \033[1;33mCapacity:\033[0m %d\n", n, W); // Yellow for summary

        // Uncomment for BruteForce if implemented
        // start = clock();
        // result = BruteForce_Knapsack(n, weights, values, W);
        // end = clock();
        // elapsed_time = ((double)(end - start)) / CLOCKS_PER_SEC;
        // printf("\033[1;31m[RESULT]\033[0m Maximum value (BruteForce): \033[1;31m%d\033[0m | \033[1;31mTime: %.4f seconds\n\033[0m", result, elapsed_time);

        // Measure time for DP Bottom-Up
        start = clock();
        result = DP_BottomUp_Knapsack(n, weights, values, W);
        end = clock();
        elapsed_time = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("\033[1;32m[RESULT]\033[0m Maximum value (DP-BU): \033[1;32m%d\033[0m | \033[1;32mTime: %.4f seconds\n\033[0m", result, elapsed_time);

        // Measure time for DP Top-Down
        start = clock();
        result = DP_TopDown_Knapsack(n, weights, values, W);
        end = clock();
        elapsed_time = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("\033[1;32m[RESULT]\033[0m Maximum value (DP-TD): \033[1;32m%d\033[0m | \033[1;32mTime: %.4f seconds\n\033[0m", result, elapsed_time);

        // Measure time for Greedy
        start = clock();
        result = Greedy_Knapsack(n, weights, values, W);
        end = clock();
        elapsed_time = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("\033[1;33m[RESULT]\033[0m Approx. value (Greedy): \033[1;33m%d\033[0m | \033[1;32mTime: %.4f seconds\n\033[0m", result, elapsed_time);

        // Measure time for Genetic Algorithm
        start = clock();
        result = GA_Knapsack(n, weights, values, W);
        end = clock();
        elapsed_time = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("\033[1;34m[RESULT]\033[0m Approx. value (GA): \033[1;34m%d\033[0m | \033[1;32mTime: %.4f seconds\n\033[0m", result, elapsed_time);

        // Free dynamically allocated memory
        free(weights);
        free(values);
    }

    printf("\n\033[1;32m========== Execution Complete ==========\033[0m\n"); // Green footer banner
    return 0;
}
