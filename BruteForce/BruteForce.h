// #include <stdio.h>
// #include <stdlib.h>

// Function to solve the 0/1 Knapsack Problem using brute force iterative approach
int BruteForce_Knapsack(int n, int weights[], int values[], int capacity) {
    printf("\nStarting Brute force knapsack computation...\n");
    int maxValue = 0; // To track the maximum value found
    unsigned long long int totalSubsets = (1ULL << n); // Total number of subsets is 2^n

    // Iterate through all subsets (bitmask from 0 to 2^n - 1)
    for (unsigned long long int subset = 0; subset < totalSubsets; subset++) {
        int currentWeight = 0;
        int currentValue = 0;

        // Check each bit in the bitmask to include/exclude items
        for (int i = 0; i < n; i++) {
            if (subset & (1ULL << i)) { // If the i-th bit is set, include item i
                currentWeight += weights[i];
                currentValue += values[i];
            }
        }

        // Check if the subset is valid and update the maximum value
        if (currentWeight <= capacity && currentValue > maxValue) {
            maxValue = currentValue;
        }
    }

    return maxValue;
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

//     // Call the BruteForce Knapsack function
//     int result = BruteForce_Knapsack(n, weights, values, W);
//     printf("\nMaximum value in the knapsack: %d\n", result);

//     // Free memory
//     free(weights);
//     free(values);

//     return 0;
// }
