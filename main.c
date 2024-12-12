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

#define iterateRound 3

// Function to read weights, values, and weight capacity from a file
void read_knapsack_input(const char *filename, int **weights, int **values, int *n, int *W)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("\033[1;31mError: Could not open file %s\033[0m\n", filename);
        exit(1);
    }

    fscanf(file, "%d", n);
    *weights = (int *)malloc(*n * sizeof(int));
    *values = (int *)malloc(*n * sizeof(int));

    printf("\033[1;34m[FILE]\033[0m Reading weights and values from file...\n");

    for (int i = 0; i < *n; i++)
    {
        fscanf(file, "%d %d", &(*weights)[i], &(*values)[i]);
    }

    fscanf(file, "%d", W);
    fclose(file);
}

int main()
{
    int *weights, *values, n, W;
    const char *filenames[] = {
        "testcase/knapsack_25_items.txt",
        "testcase/knapsack_50_items.txt",
        "testcase/knapsack_100_items.txt",
        "testcase/knapsack_500_items.txt",
        "testcase/knapsack_1000_items.txt"};
    int num_files = sizeof(filenames) / sizeof(filenames[0]);

    int result;
    int greedyrandom_ga_timelimit_result;
    double elapsed_time;

    // Open CSV file for writing results
    FILE *csv_file = fopen("results.csv", "w");
    if (!csv_file)
    {
        printf("\033[1;31mError: Could not open results.csv for writing\033[0m\n");
        exit(1);
    }

    // Write the header row to the CSV
    fprintf(csv_file, "File,DP-BottomUp Avg Value,DP-BottomUp Avg Time (ms),DP-TopDown Avg Value,DP-TopDown Avg Time (ms),Greedy Avg Value,Greedy Avg Time (ms),GA Greedy Random Avg Value,GA Greedy Random within Time Avg Value,GA Greedy Random Avg Time (ms),GA Greedy Population Avg Value,GA Greedy Population Avg Time (ms)\n");

    printf("\033[1;32m========== Knapsack Problem Solver ==========\033[0m\n");

    for (int i = 0; i < num_files; i++)
    {
        printf("\n\033[1;36m[FILE] Processing file: %s\033[0m\n", filenames[i]);

        // Read data from the current file
        read_knapsack_input(filenames[i], &weights, &values, &n, &W);
        printf("\033[1;33mNumber of items:\033[0m %d, \033[1;33mCapacity:\033[0m %d\n", n, W);

        double total_time_dp_bu = 0, total_time_dp_td = 0, total_time_greedy = 0, total_time_greedyrandom_ga = 0, total_time_greedypop_ga = 0;
        int total_value_dp_bu = 0, total_value_dp_td = 0, total_value_greedy = 0, total_value_greedyrandom_ga = 0, total_timelimit_value_greedyrandom_ga = 0, total_value_greedypop_ga = 0;
        struct timespec start, end;

        // Run DP Bottom-Up multiple times
        for (int j = 0; j < iterateRound; j++)
        {
            clock_gettime(CLOCK_MONOTONIC, &start);
            result = DP_BottomUp_Knapsack(n, weights, values, W);
            clock_gettime(CLOCK_MONOTONIC, &end);

            double elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
            total_time_dp_bu += elapsed_time;
            total_value_dp_bu += result;
        }

        // Run DP Top-Down multiple times
        for (int j = 0; j < iterateRound; j++)
        {
            clock_gettime(CLOCK_MONOTONIC, &start);
            result = DP_TopDown_Knapsack(n, weights, values, W);
            clock_gettime(CLOCK_MONOTONIC, &end);
            double elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
            total_time_dp_td += elapsed_time;
            total_value_dp_td += result;
        }

        // Run Greedy Knapsack multiple times
        for (int j = 0; j < iterateRound; j++)
        {
            clock_gettime(CLOCK_MONOTONIC, &start);
            result = Greedy_Knapsack(n, weights, values, W);
            clock_gettime(CLOCK_MONOTONIC, &end);
            double elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
            total_time_greedy += elapsed_time;
            total_value_greedy += result;
        }

        // Calculate greedy averages for used in Greedy Random GA algorithm
        double avg_time_greedy = (total_time_greedy / iterateRound);

        // Run Greedy Random Genetic Algorithm multiple times
        for (int j = 0; j < iterateRound; j++)
        {
            clock_gettime(CLOCK_MONOTONIC, &start);
            result = GreedyRandom_GA_Knapsack(n, weights, values, W, avg_time_greedy, &greedyrandom_ga_timelimit_result);
            clock_gettime(CLOCK_MONOTONIC, &end);
            double elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
            total_time_greedyrandom_ga += elapsed_time;
            total_value_greedyrandom_ga += result;
            total_timelimit_value_greedyrandom_ga += greedyrandom_ga_timelimit_result;
        }

        // Run Greedy Population Genetic Algorithm multiple times
        for (int j = 0; j < iterateRound; j++)
        {
            clock_gettime(CLOCK_MONOTONIC, &start);
            result = GreedyPop_GA_Knapsack(n, weights, values, W);
            clock_gettime(CLOCK_MONOTONIC, &end);
            double elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
            total_time_greedypop_ga += elapsed_time;
            total_value_greedypop_ga += result;
        }

        // Calculate averages and convert to ms
        double avg_time_dp_bu = (total_time_dp_bu / iterateRound) * 1000;
        double avg_time_dp_td = (total_time_dp_td / iterateRound) * 1000;
        avg_time_greedy = (total_time_greedy / iterateRound) * 1000;
        double avg_time_greedyrandom_ga = (total_time_greedyrandom_ga / iterateRound) * 1000;
        double avg_time_greedypop_ga = (total_time_greedypop_ga / iterateRound) * 1000;

        int avg_value_dp_bu = total_value_dp_bu / iterateRound;
        int avg_value_dp_td = total_value_dp_td / iterateRound;
        int avg_value_greedy = total_value_greedy / iterateRound;
        int avg_value_greedyrandom_ga = total_value_greedyrandom_ga / iterateRound;
        int avg_timelimit_value_greedyrandom_ga = total_timelimit_value_greedyrandom_ga / iterateRound;
        int avg_value_greedypop_ga = total_value_greedypop_ga / iterateRound;

        printf("\033[1;32m[RESULT]\033[0m DP-BottomUp Avg Value: %d | Avg Time: %.3f ms\n", avg_value_dp_bu, avg_time_dp_bu);
        printf("\033[1;33m[RESULT]\033[0m DP-TopDown Avg Value: %d | Avg Time: %.3f ms\n", avg_value_dp_td, avg_time_dp_td);
        printf("\033[1;34m[RESULT]\033[0m Greedy Avg Value: %d | Avg Time: %.3f ms\n", avg_value_greedy, avg_time_greedy);
        printf("\033[1;36m[RESULT]\033[0m Greedy Random GA Avg Value: %d | within Time Value: %d | Avg Time: %.3f ms\n", avg_value_greedyrandom_ga, avg_timelimit_value_greedyrandom_ga, avg_time_greedyrandom_ga);
        printf("\033[1;36m[RESULT]\033[0m Greedy Population GA Avg Value: %d | Avg Time: %.3f ms\n", avg_value_greedypop_ga, avg_time_greedypop_ga);

        // Write results to CSV
        fprintf(csv_file, "%s,%d,%.3f,%d,%.3f,%d,%.3f,%d,%d,%.3f,%d,%.3f\n",
                filenames[i], avg_value_dp_bu, avg_time_dp_bu,
                avg_value_dp_td, avg_time_dp_td,
                avg_value_greedy, avg_time_greedy,
                avg_value_greedyrandom_ga, avg_timelimit_value_greedyrandom_ga, avg_time_greedyrandom_ga,
                avg_value_greedypop_ga, avg_time_greedypop_ga);
        free(weights);
        free(values);
    }

    printf("\n\033[1;32m========== Execution Complete ==========\033[0m\n");
    fclose(csv_file);
    printf("\033[1;32mResults saved in 'results.csv'\033[0m\n");

    return 0;
}