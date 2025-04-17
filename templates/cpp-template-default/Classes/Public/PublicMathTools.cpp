#include"PublicMathTools.h"


std::vector<std::vector<int>> PublicCorrelate2d(const std::vector<std::vector<int>>& input, const std::vector<std::vector<int>>& kernel, int fillValue)
{
	std::vector<std::vector<int>> res(input.size());
	for (int i = 0; i < input.size(); ++i)
	{
		res[i].resize(input[i].size(), 0);
		for (int j = 0; j < input[i].size(); ++j)
		{
			for (int p = 0; p < kernel.size(); ++p)
			{
				for (int q = 0; q < kernel[p].size(); ++q)
				{
					if (i + p >= input.size() || j + q >= input[i].size())
					{
						res[i][j] += fillValue * kernel[p][q];
					}
					else
					{
						res[i][j] += input[i + p][j + q] * kernel[p][q];
					}
				}
			}
		}
	}
	return res;
}