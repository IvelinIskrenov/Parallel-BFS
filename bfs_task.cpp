#include "bfs_task.h"





BFSTask::BFSTask (vector<vector<int>>& _table): table(_table)
{
	rows = table.size();
	if (rows > 0)
	{
		columns = table[0].size();
	}
	else
	{
		columns = 0;
	}
	//generate random cell to start filling
	random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<mt19937::result_type> dist_rows(0,rows);
	uniform_int_distribution<mt19937::result_type> dist_columnns(0,columns);
	cells_to_fill.push({dist_rows(rng), dist_columnns(rng)});
}

void BFSTask::fill(int number_to_fill)
{
	while(!cells_to_fill.empty())
	{
		long long row = cells_to_fill.front().first;
		long long column = cells_to_fill.front().second;
		cells_to_fill.pop();
		if (row < 0 || column < 0 || row >= rows || column >= columns || table[row][column] != 0)
		{
			continue;
		}
		table[row][column] = number_to_fill;
		cells_to_fill.push({row + 1, column});
		cells_to_fill.push({row - 1, column});
		cells_to_fill.push({row, column + 1});
		cells_to_fill.push({row, column - 1});
	}
}