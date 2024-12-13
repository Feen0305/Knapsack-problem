static int GreedyPop_GA_Process(int n, int *weights, int *values, int W)
{
    srand(time(NULL));

    // สร้างและเรียงลำดับ items ตาม ratio
    Item *items = (Item *)malloc(n * sizeof(Item));
    for (int i = 0; i < n; i++)
    {
        items[i].weight = weights[i];
        items[i].value = values[i];
        items[i].ratio = (double)values[i] / weights[i];
    }
    qsort(items, n, sizeof(Item), compare_items);

    Individual *population = (Individual *)malloc(POP_SIZE * sizeof(Individual));
    Individual *new_population = (Individual *)malloc(POP_SIZE * sizeof(Individual));
    Individual **selected_parents = (Individual **)malloc(MULTI_PARENT * sizeof(Individual *));
    Individual best_ever = {NULL, 0};
    best_ever.genes = (int *)calloc(n, sizeof(int));

    // Initialize population
    for (int i = 0; i < POP_SIZE; i++)
    {
        population[i].genes = (int *)calloc(n, sizeof(int));
        new_population[i].genes = (int *)calloc(n, sizeof(int));

        // 30% แรกใช้ greedy approach
        if (i < POP_SIZE * 0.3)
        {
            int current_weight = 0;
            // เรียงตาม value/weight ratio แล้ว
            for (int j = 0; j < n && current_weight < W; j++)
            {
                if (current_weight + items[j].weight <= W)
                {
                    population[i].genes[j] = 1;
                    current_weight += items[j].weight;
                }
            }
        }
        // 70% ที่เหลือสร้างแบบ random
        else
        {
            int current_weight = 0;
            for (int j = 0; j < n; j++)
            {
                if ((rand() % 100 < (70 - (j * 50 / n))) && // โอกาสลดลงตามลำดับ
                    current_weight + items[j].weight <= W)
                {
                    population[i].genes[j] = 1;
                    current_weight += items[j].weight;
                }
            }
        }
    }

    int generations_without_improvement = 0;

    // Main GA loop
    for (int gen = 0; gen < MAX_GEN; gen++)
    {
        int total_fitness = 0;

        // Calculate fitness
        for (int i = 0; i < POP_SIZE; i++)
        {
            population[i].fitness = calculate_fitness(&population[i], items, n, W);
            total_fitness += population[i].fitness;
        }

        // Update best solution
        for (int i = 0; i < POP_SIZE; i++)
        {
            if (population[i].fitness > best_ever.fitness)
            {
                memcpy(best_ever.genes, population[i].genes, n * sizeof(int));
                best_ever.fitness = population[i].fitness;
                generations_without_improvement = 0;
                break;
            }
        }

        generations_without_improvement++;

        // Elitism
        for (int i = 0; i < ELITE_SIZE; i++)
        {
            memcpy(new_population[i].genes, best_ever.genes, n * sizeof(int));
            new_population[i].fitness = best_ever.fitness;
        }

        // Create new population
        for (int i = ELITE_SIZE; i < POP_SIZE; i++)
        {
            // Select parents using Roulette Wheel
            for (int j = 0; j < MULTI_PARENT; j++)
            {
                selected_parents[j] = select_parent(population, total_fitness);
            }

            // Perform multi-parent crossover
            advanced_crossover(selected_parents, &new_population[i], n);

            // Mutation
            for (int j = 0; j < n; j++)
            {
                if ((double)rand() / RAND_MAX < MUTATION_RATE)
                {
                    new_population[i].genes[j] = !new_population[i].genes[j];
                }
            }

            // Repair invalid solutions
            repair_solution(&new_population[i], items, n, W);
        }

        // Swap populations
        Individual *temp = population;
        population = new_population;
        new_population = temp;

        // Early stopping
        if (generations_without_improvement > 300 && gen > MAX_GEN / 2)
        {
            break;
        }
    }

    int result = best_ever.fitness;

    // Cleanup
    for (int i = 0; i < POP_SIZE; i++)
    {
        free(population[i].genes);
        free(new_population[i].genes);
    }
    free(population);
    free(new_population);
    free(selected_parents);
    free(best_ever.genes);
    free(items);

    return result;
}

int GreedyPop_GA_Knapsack(int n, int *weights, int *values, int W)
{
    // printf("[COMP]Starting Genetic Algorithm knapsack computation...\n");
    return GreedyPop_GA_Process(n, weights, values, W);
}