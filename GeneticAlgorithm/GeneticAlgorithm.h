#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Definitions for genetic algorithm parameters
#define POP_SIZE 100 // Size of the population (number of individuals)
#define MAX_GEN 1000 // Maximum number of generations
#define MUTATION_RATE 0.01 // Probability of mutation per gene
#define KNAPSACK_CAPACITY 7 // Maximum weight the knapsack can hold
#define NUM_ITEMS 3 // Number of items available

// Structure to represent an item with weight and value
typedef struct {
    int weight; 
    int value; 
} Item;

// Structure to represent an individual solution in the population
typedef struct {
    int genes[NUM_ITEMS]; // Binary representation of selected items (1 = selected, 0 = not selected)
    int fitness; // Fitness score of the individual (higher is better)
} Individual;

// Global arrays for items and population
Item items[NUM_ITEMS]; 
Individual population[POP_SIZE]; // Array to store the current population
Individual new_population[POP_SIZE]; // Array to store the new generation of population

// Function to initialize items with random weights and values
void initialize_items() {
    printf("Enter the weight and value of each item:\n");
    for (int i = 0; i < NUM_ITEMS; i++) {
        printf("Item %d:\n", i + 1);
        printf("Weight: ");
        scanf("%d", &items[i].weight);
        printf("Value: ");
        scanf("%d", &items[i].value);
    }
}

// Function to initialize the population with random genes
void initialize_population() {
    for (int i = 0; i < POP_SIZE; i++) {
        for (int j = 0; j < NUM_ITEMS; j++) {
            population[i].genes[j] = rand() % 2; // Randomly set each gene to 0 or 1
        }
    }
}

// Function to calculate the fitness of an individual
int calculate_fitness(Individual *ind) {
    int total_weight = 0; // Total weight of selected items
    int total_value = 0; // Total value of selected items
    for (int i = 0; i < NUM_ITEMS; i++) {
        if (ind->genes[i] == 1) { // If the item is selected
            total_weight += items[i].weight; // Add item's weight
            total_value += items[i].value; // Add item's value
        }
    }
    // If total weight exceeds the knapsack capacity, fitness is 0
    if (total_weight > KNAPSACK_CAPACITY) {
        return 0;
    }
    return total_value; // Fitness is the total value of selected items
}

// Function to evaluate the fitness of all individuals in the population
void evaluate_population() {
    for (int i = 0; i < POP_SIZE; i++) {
        population[i].fitness = calculate_fitness(&population[i]);
    }
}

// Function to select a parent using roulette wheel selection
Individual select_parent() {
    int total_fitness = 0;
    // Calculate total fitness of the population
    for (int i = 0; i < POP_SIZE; i++) {
        total_fitness += population[i].fitness;
    }
    // Select a random point in the fitness space
    int random_value = rand() % total_fitness;
    int running_sum = 0;
    // Iterate through the population to find the selected parent
    for (int i = 0; i < POP_SIZE; i++) {
        running_sum += population[i].fitness;
        if (running_sum > random_value) {
            return population[i]; // Return the selected parent
        }
    }
    return population[POP_SIZE - 1]; // Fallback return in case of rounding errors
}

// Function to perform crossover between two parents to create two children
void crossover(Individual *parent1, Individual *parent2, Individual *child1, Individual *child2) {
    int crossover_point = rand() % NUM_ITEMS; // Random crossover point
    for (int i = 0; i < NUM_ITEMS; i++) {
        if (i < crossover_point) { // Genes before the crossover point
            child1->genes[i] = parent1->genes[i];
            child2->genes[i] = parent2->genes[i];
        } else { // Genes after the crossover point
            child1->genes[i] = parent2->genes[i];
            child2->genes[i] = parent1->genes[i];
        }
    }
}

// Function to mutate an individual's genes
void mutate(Individual *ind) {
    for (int i = 0; i < NUM_ITEMS; i++) {
        // Flip the gene with a probability equal to the mutation rate
        if ((double)rand() / RAND_MAX < MUTATION_RATE) {
            ind->genes[i] = !ind->genes[i];
        }
    }
}

// Function to create a new generation of the population
void create_new_population() {
    for (int i = 0; i < POP_SIZE; i += 2) {
        // Select two parents using selection
        Individual parent1 = select_parent();
        Individual parent2 = select_parent();
        // Perform crossover to create two children
        crossover(&parent1, &parent2, &new_population[i], &new_population[i + 1]);
        // Apply mutation to the children
        mutate(&new_population[i]);
        mutate(&new_population[i + 1]);
    }
    // Replace the old population with the new generation
    for (int i = 0; i < POP_SIZE; i++) {
        population[i] = new_population[i];
    }
}


// // Main function
// int main() {
//     srand(time(NULL)); // Seed the random number generator
//     initialize_items(); // Initialize items with random values and weights
//     initialize_population(); // Initialize population with random solutions

//     // Run the genetic algorithm for a fixed number of generations
//     for (int gen = 0; gen < MAX_GEN; gen++) {
//         evaluate_population(); // Evaluate fitness of the population
//         create_new_population(); // Create the next generation
//     }

//     // Evaluate the final population and find the best individual
//     evaluate_population();
//     Individual best_individual = population[0];
//     for (int i = 1; i < POP_SIZE; i++) {
//         if (population[i].fitness > best_individual.fitness) {
//             best_individual = population[i];
//         }
//     }

//     // Print the best solution found
//     printf("Best solution found:\n");
//     for (int i = 0; i < NUM_ITEMS; i++) {
//         printf("%d ", best_individual.genes[i]); // Display the gene (0 or 1)
//     }
//     printf("\nFitness: %d\n", best_individual.fitness); // Display the fitness value

//     return 0;
// }
