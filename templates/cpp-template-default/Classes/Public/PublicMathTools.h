#pragma once
#include<vector>

// 互相关卷积操作
std::vector<std::vector<int>> PublicCorrelate2d(const std::vector<std::vector<int>>& input, const std::vector<std::vector<int>>& kernel, int fillValue = 0);