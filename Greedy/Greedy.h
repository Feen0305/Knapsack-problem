#include <stdio.h>
#include <stdlib.h>

// Structure to store value-to-weight ratio with index
typedef struct {
    double ratio;
    int index;
} ItemRatio;

// Comparison function for qsort
int compare(const void* a, const void* b) {
    double r1 = ((ItemRatio*)a)->ratio;
    double r2 = ((ItemRatio*)b)->ratio;
    return (r1 > r2) ? -1 : (r1 < r2) ? 1 : 0;
}

// Greedy approximation for Knapsack Problem
int Greedy_Knapsack(int n, int weights[], int values[], int capacity) {
    printf("[COMP]Starting Greedy knapsack computation...\n");
    ItemRatio* itemRatios = (ItemRatio*)malloc(n * sizeof(ItemRatio));

    // Calculate value-to-weight ratio for each item
    for (int i = 0; i < n; i++) {
        itemRatios[i].ratio = (double)values[i] / weights[i];
        itemRatios[i].index = i;
    }

    // Sort items by value-to-weight ratio using qsort
    qsort(itemRatios, n, sizeof(ItemRatio), compare);

    int currentWeight = 0;
    int finalValue = 0;

    // Iterate through sorted items and select them greedily
    for (int i = 0; i < n; i++) {
        int idx = itemRatios[i].index;
        if (currentWeight + weights[idx] <= capacity) {
            currentWeight += weights[idx];
            finalValue += values[idx];
        }
        if (currentWeight == capacity) {
            break;
        }
    }

    free(itemRatios); // Free dynamically allocated memory
    return finalValue;
}

// int main() {
//     int capacity = 50; // Maximum weight of knapsack
//     char filePath[] = "../testcase/knapsack_1000_items.txt";

//     // Open the testcase file
//     FILE* inputFile = fopen(filePath, "r");
//     if (!inputFile) {
//         perror("Error: Could not open file");
//         return 1;
//     }

//     // Read items from the file
//     int weights[1000];
//     int values[1000];
//     int n = 0;

//     while (fscanf(inputFile, "%d %d", &values[n], &weights[n]) == 2) {
//         n++;
//     }
//     fclose(inputFile);

//     // Calculate the approximate maximum value
//     int maxValue = Greedy_Knapsack(n, weights, values, capacity);
//     printf("Approximate Maximum Value in Knapsack = %d\n", maxValue);

//     return 0;
// }
