#ifndef __CACULATE_H__
#define __CACULATE_H__

#include <string>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <stdint.h>

namespace simple{
using std::string;
using std::unordered_map;
using TF_IDF_Weight = double;

///@brief 计算一篇文档中所有单词TF_IDF的值
///
///@param terms_frequency_map 词频映射表
///@param document_frequency 包含该单词的文档数量
///@param total_docs_num 总文档数量
unordered_map<string, TF_IDF_Weight> CaculateValue(
        const unordered_map<string, uint64_t>& terms_frequency_map,
        unordered_map<string, uint64_t> document_frequencies,
        uint64_t total_docs_num);
}
#endif
