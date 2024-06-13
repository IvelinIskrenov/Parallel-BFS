#pragma once
#include <utility>
#include <vector>
#include <iostream>
#include <queue>
#include <random>
using namespace std;
class BFSTask
{
public:
    vector<vector<int>>& table;
    long long rows;
    long long columns;
	queue<pair<int, int>> cells_to_fill;
    BFSTask (vector<vector<int>>& _table);
    void fill(int number);
};