#ifndef __PAGE_LIB_PREPROCESSOR_H__
#define __PAGE_LIB_PREPROCESSOR_H__

#include "../common/split_tool.h"
#include "page_lib.h"
#include "web_page.h"
#include "caculate.h"
#include <unordered_map>
#include <cmath>
#include <memory>
#include <fstream>

namespace simple{
class PageLibPreprocessor{
public:
    PageLibPreprocessor(PageLib& _page_lib, Logger& logger, IConfiguration& config);
    void Do();
    void CreatePageLibList();
    void Cut();
    vector<WebPage>& GetPageLibList();
    void RmRedundantPages();
    void BuildInvertIndexTable();
    //将预处理后的网页库和网页偏移库存储到磁盘上
    void StoreOnDisk();
    //使用simhash计算出网页描述的simhash值和关键词序列
    void SimHash(WebPage& web_page);

    void LoadIndexPageList();

    Logger& _logger;
    IConfiguration& _config;
    PageLib& _page_lib;
    vector<WebPage> _page_lib_list;
    unordered_map<DocId, WebPage> _indexed_page_lib_list;
    //存放每篇文档的网页库的位置信息
    unordered_map<DocId, pair<uint64_t, uint64_t>> _offset_lib;
    //存放包含某个单词的文档数
    unordered_map<string, uint64_t> _document_frequencies;
    //倒排索引表
    unordered_map<string, set<pair<DocId, TF_IDF_Weight>>> _invert_index_table;
};
}
#endif
