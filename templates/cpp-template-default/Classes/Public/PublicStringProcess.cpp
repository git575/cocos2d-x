#include "PublicStringProcess.h"

std::vector<std::string> PublicSplit(const std::string& str, const std::string& separator)
{
	std::vector<std::string> ret;
	size_t sp_len = separator.size();
	size_t pre = 0;
	size_t cut_at = str.find(separator);
	while (cut_at != std::string::npos)
	{
		ret.push_back(str.substr(pre, cut_at - pre));
		pre = cut_at + sp_len;
		cut_at = str.find(separator, pre);
	}
	if (pre < str.size()) {
		ret.push_back(str.substr(pre));
	}
	return ret;
}

