#ifndef __WEB_PAGE_QUERY_H__
#define __WEB_PAGE_QUERY_H__
#include "page_lib_preprocessor.h"
#include "caculate.h"
#include <map>
#include <algorithm>

namespace simple{
using Cosine = double;
using DocsVector = vector<pair<DocId, unordered_map<string, TF_IDF_Weight>>>;
class WebPageQuery{
public:
    WebPageQuery(const PageLibPreprocessor &pre, Logger &logger, IConfiguration &config);
    //加载库文件
    void LoadLobrary();
    //执行查询返回结果
    string DoQuery(const string&);
    //通过TF-IDF算法算出每个关键词的权重系数
    unordered_map<string, TF_IDF_Weight> CaculateQueryWeights(
        const unordered_map<string, uint64_t>& query_words);
    //通过倒排索引表去寻找包含所有关键词的网页：只要其中有一个关键词不在索引表中，
    //就认为没有找到相关网页
    vector<DocId> GetRelativePages(
        const unordered_map<string, uint64_t> &query_words);
    //算出相关网页的查询词向量集合
    DocsVector CaculateRelativePagesVectors(
        const unordered_map<string, uint64_t> &query_words);

};
}
#endif
