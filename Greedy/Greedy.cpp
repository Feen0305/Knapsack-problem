#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>

using namespace std;

struct Item {
    int value, weight;
    Item(int v, int w) : value(v), weight(w) {}
};

// Comparison function to sort items by value-to-weight ratio
bool compare(Item a, Item b) {
    double r1 = (double)a.value / a.weight;
    double r2 = (double)b.value / b.weight;
    return r1 > r2;
}

// Greedy approximation for 0/1 Knapsack Problem
int knapsackGreedyApproximation(std::vector<Item>& items, int W) {
    // Sort items by value-to-weight ratio
    sort(items.begin(), items.end(), compare);

    int currentWeight = 0;
    int finalValue = 0;

    // Iterate through items, include them fully if possible
    for (auto& item : items) {
        if (currentWeight + item.weight <= W) {
            currentWeight += item.weight;
            finalValue += item.value;
        }
        // Stop adding items when weight limit is reached
        if (currentWeight == W) {
            break;
        }
    }

    return finalValue; // Return the approximate maximum value
}

int main() {
    int W = 50; // Maximum weight of knapsack
    vector<Item> items;

    string filePath = "../testcase/knapsack_100_items.txt";
    //cout << "Trying to open file at: " << std::filesystem::absolute(filePath) << endl;

    // Open the file
    ifstream inputFile(filePath);
    if (!inputFile) {
        cerr << "Error: Could not open file." << endl;
        return 1;
    }

    // Read file line by line
    string line;
    while (getline(inputFile, line)) {
        stringstream ss(line);
        int value, weight;
        ss >> value >> weight; // Extract value and weight
        items.emplace_back(value, weight); // Add item to vector
    }
    inputFile.close();

    // Calculate the approximate maximum value
    cout << "Approximate Maximum Value in Knapsack = " << knapsackGreedyApproximation(items, W) << endl;

    return 0;
}
