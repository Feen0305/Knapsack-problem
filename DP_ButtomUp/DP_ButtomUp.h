// #include <stdio.h>
// #include <stdlib.h>

// Bottom-Up DP implementation for the 0/1 knapsack problem
int DP_BottomUp_Knapsack(int n, int *weights, int *values, int W) {
    // printf("[COMP]Starting Dynamic programming BottomUp knapsack computation...\n");
    // Create a 2D dp table (n + 1 rows x W + 1 columns)
    int **dp = (int **)malloc((n + 1) * sizeof(int *));
    for (int i = 0; i <= n; i++) {
        dp[i] = (int *)malloc((W + 1) * sizeof(int));
    }

    // Initialize the dp table
    for (int i = 0; i <= n; i++) {
        for (int w = 0; w <= W; w++) {
            if (i == 0 || w == 0) {
                dp[i][w] = 0; // Base case: no items or zero weight capacity results in 0 value
            } else if (weights[i - 1] <= w) {
                // Case: Either take or don't take the current item
                dp[i][w] = (values[i - 1] + dp[i - 1][w - weights[i - 1]] > dp[i - 1][w]) ?
                            values[i - 1] + dp[i - 1][w - weights[i - 1]] : dp[i - 1][w];
            } else {
                // Case: Cannot include the item
                dp[i][w] = dp[i - 1][w];
            }
        }
    }

    int result = dp[n][W]; // The final answer
    // Free allocated memory
    for (int i = 0; i <= n; i++) {
        free(dp[i]);
    }
    free(dp);

    return result;
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

//     // Call the DP Bottom-Up knapsack function
//     int result = DP_BottomUp_Knapsack(n, weights, values, W);
//     printf("\nMaximum value in the knapsack: %d\n", result);

//     // Free memory
//     free(weights);
//     free(values);

//     return 0;
// }
