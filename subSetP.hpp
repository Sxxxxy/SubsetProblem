#include <cstdio>
#include <vector>
#include <algorithm>
#include <iostream>
#include <random>
#include <unordered_map>
#include <functional>
#include <numeric>

std::vector<int> generateRandomIndexes(int size, int selectnum)
{
    std::vector<int> indexes(size);
    // 生成0到size - 1的连续整数
    for (int i = 0; i < size; ++i) 
    {
        indexes[i] = i;
    }

    // 生成随机数引擎
    std::random_device rd;
    std::mt19937 g(rd());

    // 打乱indexes
    std::shuffle(indexes.begin(), indexes.end(), g);

    // 截取前selectnum个元素
    indexes.resize(selectnum);

    return indexes;
}



class Variable
{
public:
    int id;
    int coef;
    Variable(int id, int coef) : id(id), coef(coef) {}
};

void sum_up_recursive(std::vector<Variable>& num, int tar, std::vector<Variable>& partial, std::vector<std::vector<Variable>>& solutions)
{
    int sum = 0;
    for (const auto& v : partial)
    {
        sum += v.coef;
    }
    if (sum == tar)
    {
        solutions.push_back(partial);  // Store the solution
    }
    if (sum >= tar)
    {
        return;
    }
    for (size_t i = 0; i < num.size(); i++)
    {
	    std::vector<Variable> remaining;
        Variable n = num[i];
        for (size_t j = i + 1; j < num.size(); j++)
        {
            remaining.push_back(num[j]);
        }
	    std::vector<Variable> partial_rec(partial);
        partial_rec.push_back(n);
        sum_up_recursive(remaining, tar, partial_rec, solutions);
    }
}

std::vector<std::vector<Variable>> sum_up(std::vector<Variable>& num, int tar)
{
    std::sort(num.begin(), num.end(), [](const Variable& a, const Variable& b) {
        return a.coef < b.coef;
        });
	std::vector<Variable> partial;
    std::vector<std::vector<Variable>> solutions;
    sum_up_recursive(num, tar, partial, solutions);
    return solutions;
}


std::tuple<std::vector<std::vector<Variable>>,std::vector<int>> Generating_example(int ncols,int nrows,double density)
{
	std::vector<std::vector<Variable>> matrix(nrows);
    std::vector<int> rhs(nrows);
	for (int i = 0; i < nrows; i++)
	{
		std::vector<Variable> row;
		for (int j = 0; j < ncols; j++)
		{
			if (rand() % 100 < density * 100)
			{
                int randNum = std::rand() % (2 * ncols + 1) - ncols; // 生成 -n 到 n 的随机数
				row.emplace_back(j, randNum);
			}
		}
		matrix[i] = row;
	}

    int rowsize = ncols;
    //int selectnum = std::rand() % (rowsize)+1;
    int selectnum = rowsize / 3;
    auto indexes = generateRandomIndexes(rowsize, selectnum);
	for (int i = 0; i < nrows; i++)
	{
		for (int j = 0; j < matrix[i].size(); ++j)
		{
			if (std::find(indexes.begin(), indexes.end(), matrix[i][j].id) != indexes.end())
			{
				rhs[i] += matrix[i][j].coef;
			}
		}
	}
	return { matrix , rhs};
}
