#include <stdio.h>
#include <stdlib.h>

#include "BruteForce\BruteForce.h"
#include "DP_ButtomUp\DP_ButtomUp.h"
#include "DP_TopDown\DP_TopDown.h"
#include "Greedy\Greedy.h"

// Function to read weights, values, and weight capacity from a file
void read_knapsack_input(const char *filename, int **weights, int **values, int *n, int *W) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file %s\n", filename);
        exit(1);
    }

    // Read the number of items
    fscanf(file, "%d", n);

    // Allocate memory for weights and values
    *weights = (int *)malloc(*n * sizeof(int));
    *values = (int *)malloc(*n * sizeof(int));

    printf("[FILE]Reading weights and values from file...\n");

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

    for (int i = 0; i < num_files; i++) {
        printf("\n[FILE]Processing file: %s\n", filenames[i]);

        // Read data from the current file
        read_knapsack_input(filenames[i], &weights, &values, &n, &W);
        
        // Call the BruteForce Knapsack function
        result = BruteForce_Knapsack(n, weights, values, W);
        printf("[RESULT]Maximum value in the knapsack(BF): %d\n", result);

        // Call the DP BottomUp Knapsack function
        result = DP_BottomUp_Knapsack(n, weights, values, W);
        printf("[RESULT]Maximum value in the knapsack(DP-BU): %d\n", result);

        // Call the DP TopDown Knapsack function
        result = DP_TopDown_Knapsack(n, weights, values, W);
        printf("[RESULT]Maximum value in the knapsack (DP-TD): %d\n", result);

        result = Greedy_Knapsack(n, weights, values, W);
        printf("[RESULT]Approximate Maximum value in the knapsack (Greedy): %d\n", result);

        // Free dynamically allocated memory
        free(weights);
        free(values);
    }

    return 0;
}
