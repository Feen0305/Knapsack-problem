#include <stdio.h>
#include <stdlib.h>

#include "BruteForce\BruteForce.h"
#include "DP_ButtomUp\DP_ButtomUp.h"
#include "DP_TopDown\DP_TopDown.h"

int main() {
    int n;
    printf("Enter the number of items: ");
    scanf(" %d", &n);

    int *weights = (int *)malloc(n * sizeof(int));
    int *values = (int *)malloc(n * sizeof(int));

    printf("Enter weights and values:\n");
    for (int i = 0; i < n; i++) {
        scanf(" %d %d", &weights[i], &values[i]);
    }

    int W;
    printf("Enter the total weight capacity: ");
    scanf(" %d", &W);

    int result;
    // Call the BruteForce Knapsack function
    result = BruteForce_Knapsack(n, weights, values, W);
    printf("Maximum value in the knapsack(BF): %d\n", result);

    // Call the DP BottomUp Knapsack function
    result = DP_BottomUp_Knapsack(n, weights, values, W);
    printf("Maximum value in the knapsack(DP-BU): %d\n", result);

    // Call the DP TopDown Knapsack function
    result = DP_TopDown_Knapsack(n, weights, values, W);
    printf("Maximum value in the knapsack(DP-TD): %d\n", result);

    // Free memory
    free(weights);
    free(values);

    return 0;
}