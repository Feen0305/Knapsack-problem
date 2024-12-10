// #include <stdio.h>
// #include <stdlib.h>

// Function to compute the knapsack value using recursion with memoization
int DP_TopDown(int i, int j, int n, int weights[], int values[], int **KnapTable) {
    int Val;

    // Base case: if index goes below 0
    if (i < 0) {
        return 0;
    }

    // Check if value is already computed in the memoization table
    if (KnapTable[i][j] < 0) {
        if (j < weights[i]) {
            // Case 1: Skip current item
            Val = DP_TopDown(i - 1, j, n, weights, values, KnapTable);
        } else {
            // Case 2: Take the maximum of including or excluding the current item
            int exclude = DP_TopDown(i - 1, j, n, weights, values, KnapTable);
            int include = values[i] + DP_TopDown(i - 1, j - weights[i], n, weights, values, KnapTable);
            Val = (exclude > include) ? exclude : include;
        }

        // Memoize the computed value
        KnapTable[i][j] = Val;
    }

    return KnapTable[i][j];
}

// Helper function that accepts input arguments and processes computation
int DP_TopDown_Knapsack(int n, int *weights, int *values, int W) {
    // Create the memoization table dynamically
    int **KnapTable = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        KnapTable[i] = (int *)malloc((W + 1) * sizeof(int));
        for (int j = 0; j <= W; j++) {
            KnapTable[i][j] = -1; // Initialize all entries to -1 (indicating uncomputed)
        }
    }

    printf("\nStarting Dynamic programming TopDown knapsack computation...\n");
    int maxVal = DP_TopDown(n - 1, W, n, weights, values, KnapTable);

    // Free allocated memory
    for (int i = 0; i < n; i++) {
        free(KnapTable[i]);
    }
    free(KnapTable);

    return maxVal;
}

// Example for use case

// int main() {
//     int n;
//     printf("Enter the number of items: ");
//     scanf(" %d", &n);

//     int *weights = (int *)malloc(n * sizeof(int));
//     int *values = (int *)malloc(n * sizeof(int));

//     printf("Enter weights and values:\n");
//     for (int i = 0; i < n; i++) {
//         scanf(" %d %d", &weights[i], &values[i]);
//     }

//     int W;
//     printf("Enter the total weight capacity: ");
//     scanf(" %d", &W);

//     // Call the DP TopDown knapsack function
//     int result = DP_TopDown_Knapsack(n, weights, values, W);
//     printf("\nMaximum value in the knapsack: %d\n", result);

//     // Free memory
//     free(weights);
//     free(values);

//     return 0;
// }
