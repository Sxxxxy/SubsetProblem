#include <chrono>
#include <set>

#include "subSetP.hpp"
#include <unordered_map>
#include <functional>

struct SetHash {
    std::size_t operator()(const std::set<int>& s) const {
        std::size_t hash = 0;
        for (auto& i : s) {
            hash ^= std::hash<int>()(i) + 0x9e3779b9 + (hash << 6) + (hash >> 2); // Use a simple hash combination technique
        }
        return hash;
    }
};
int main()
{
    //std::srand(std::time(nullptr)); // 使用当前时间作为随机生成器的种子
    std::srand(1024); 
    int numRows = 5;
    int numCols = 50;
    double density = 0.5;
    using OneSolution = std::set<std::set<int>>;
    std::vector<OneSolution> SolutionPool(numRows);

    auto [matrix,rhs] = Generating_example(numCols, numRows, density);
    std::cout <<"matrix size is  :"<< matrix.size() << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    for (int con = 0; con < numRows; ++con)
    {
        auto results = sum_up(matrix[con], rhs[con]);
        // 打印所有解
        for (const auto& solution : results)
        {
            std::set<int> tempsol;
            for (auto num : solution) {
                tempsol.emplace(num.id);
            }
            SolutionPool[con].emplace(tempsol);
        }
    }
	for (auto onesol : SolutionPool)
	{
		std::cout <<"size : "<< onesol.size()<< std::endl;
	}
    //std::cout<<"SolutionPool size is :"<<SolutionPool[0].size() << std::endl;
    // 找到最小元素
    auto smallest = std::min_element(SolutionPool.begin(), SolutionPool.end(),
        [](const OneSolution& a, const OneSolution& b) {
            return a.size() < b.size();
        });
    OneSolution smallestSolution = *smallest;
    std::unordered_map<std::set<int>, int, SetHash> Solcount;     //index-count
    for (auto tempsol : smallestSolution)
    {
	    Solcount[tempsol] = 1;
    }
    for (auto it = SolutionPool.begin(); it != SolutionPool.end(); ++it)
    {
		if (it == smallest)
		{
			continue;
		}
		for (auto tempsol : *it)
		{
			if (Solcount.find(tempsol) != Solcount.end())
			{
				Solcount[tempsol] += 1;
			}
		}
    }

    std::vector<std::set<int>> finialSols;
    for (auto finialSol :Solcount)
    {
	    if (finialSol.second == numRows)
	    {
		    finialSols.emplace_back(finialSol.first);
	    }
    }
    std::cout<<"finial sol size is :"<<finialSols.size()<<std::endl;
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    return 0;
}
