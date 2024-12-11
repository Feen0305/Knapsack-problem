// #include <stdio.h>
// #include <stdlib.h>
// #include <time.h>
// #include <string.h>

// Parameters
#define POP_SIZE 100       // ขนาดประชากร
#define MAX_GEN 1000       // จำนวนรอบ
#define MUTATION_RATE 0.02 // อัตราการกลายพันธุ์
#define ELITE_SIZE 30      // จำนวน elite
#define MULTI_PARENT 5     // จำนวน parents ในการทำ multi-parent crossover

typedef struct
{
    int weight;
    int value;
    double ratio; // value/weight ratio
} Item;

typedef struct
{
    int *genes;
    int fitness;
} Individual;

// ฟังก์ชันเปรียบเทียบสำหรับการเรียงลำดับประชากร
static int compare_items(const void *a, const void *b)
{
    Item *item1 = (Item *)a;
    Item *item2 = (Item *)b;
    if (item1->ratio < item2->ratio)
        return 1;
    if (item1->ratio > item2->ratio)
        return -1;
    return 0;
}

static int calculate_fitness(Individual *ind, Item *items, int n, int W)
{
    int total_weight = 0;
    int total_value = 0;

    for (int i = 0; i < n; i++)
    {
        if (ind->genes[i])
        {
            total_weight += items[i].weight;
            total_value += items[i].value;
        }
    }

    // ถ้าน้ำหนักเกิน มี penalty
    if (total_weight > W)
    {
        double overflow_ratio = (double)(total_weight - W) / W;
        if (overflow_ratio > 0.5)
            return 0;
        return (int)(total_value * (1.0 - overflow_ratio));
    }
    return total_value;
}

// Roulette Wheel Selection
static Individual *select_parent(Individual *population, int total_fitness)
{
    if (total_fitness == 0)
    {
        return &population[rand() % POP_SIZE];
    }

    int random_value = rand() % total_fitness;
    int running_sum = 0;

    for (int i = 0; i < POP_SIZE; i++)
    {
        running_sum += population[i].fitness;
        if (running_sum > random_value)
        {
            return &population[i];
        }
    }
    return &population[POP_SIZE - 1];
}

// ฟังก์ชัน repair solution
static void repair_solution(Individual *ind, Item *items, int n, int W)
{
    int total_weight = 0;
    for (int i = 0; i < n; i++)
    {
        if (ind->genes[i])
        {
            total_weight += items[i].weight;
        }
    }

    // ถ้าน้ำหนักเกิน ให้ลบ items ที่มี ratio ต่ำออก
    if (total_weight > W)
    {
        for (int i = n - 1; i >= 0 && total_weight > W; i--)
        {
            if (ind->genes[i])
            {
                ind->genes[i] = 0;
                total_weight -= items[i].weight;
            }
        }
    }

    // ถ้าน้ำหนักเหลือ ให้พยายามเพิ่ม items ที่มี ratio สูง
    for (int i = 0; i < n && total_weight < W; i++)
    {
        if (!ind->genes[i] && total_weight + items[i].weight <= W)
        {
            ind->genes[i] = 1;
            total_weight += items[i].weight;
        }
    }
}

// Multi-parent crossover
static void advanced_crossover(Individual **parents, Individual *child, int n)
{
    int *vote = (int *)calloc(n, sizeof(int));

    // นับ vote จาก parents ทั้งหมด
    for (int i = 0; i < MULTI_PARENT; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (parents[i]->genes[j])
            {
                vote[j]++;
            }
        }
    }

    // เลือกยีนที่ได้รับ vote มากกว่าครึ่ง
    for (int i = 0; i < n; i++)
    {
        child->genes[i] = (vote[i] > MULTI_PARENT / 2);
    }

    free(vote);
}

static int GA_Process(int n, int *weights, int *values, int W, double Time_Limit, int *Time_Limit_Result)
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

        // สร้างโซลูชันแบบ greedy-random
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
                if ((rand() % 100 < 50) && // โอกาส 50-50
                    current_weight + items[j].weight <= W)
                {
                    population[i].genes[j] = 1;
                    current_weight += items[j].weight;
                }
            }
        }
    }

    int generations_without_improvement = 0;

    struct timespec CurrentTime;

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

        clock_gettime(CLOCK_MONOTONIC, &CurrentTime);
        double elapsed_time = CurrentTime.tv_sec / 1e9;
        if (elapsed_time > Time_Limit)
        {
            *Time_Limit_Result = best_ever.fitness;
        }

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

int GA_Knapsack(int n, int *weights, int *values, int W, double Time_Limit, int *Time_Limit_Result)
{
    // printf("[COMP]Starting Genetic Algorithm knapsack computation...\n");
    return GA_Process(n, weights, values, W, Time_Limit, Time_Limit_Result);
}