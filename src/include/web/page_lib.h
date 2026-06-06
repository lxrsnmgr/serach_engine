#ifndef __PAGE_LIB_H__
#define __PAGE_LIB_H__

#include "../common/dir_scanner.h"
#include  "rss_parser.h"
#include "web_page.h"
#include <unordered_map>
#include <memory>
#include <fstream>

namespace simple{
using std::unordered_map;
using std::pair;
using simhash::Simhasher;

class PageLib{
public:
    PageLib(Logger& logger, IConfiguration& config);
    /// @brief 从文件中加载网页库
    void Load();
    /// @brief 获取网页库中的网页数量
    [[nodiscard]] size_t GetPageNum() const;
    vector<RssItem>& GetRssItems();
    vector<string>& GetRipePages();
    Simhasher& GetSimhasher();
    /// @brief 离线存储网页库`ripe_page.dat`和位置偏移库`offset_lib.dat`
    void StoreOnDisk();

private:
    Logger& _logger;
    IConfiguration& _config;
    DirScanner _dir_scanner;
    //解析单个rss文件
    RssParser _rss_parser;
    simhash::Simhasher _simhasher;
    //存放每篇文档的网页库的位置信息
    unordered_map<uint64_t, pair<uint64_t, uint64_t>> _offset_lib;
    //存放格式化后的网页
    vector<string> _ripe_pages;
};
}
#endif
