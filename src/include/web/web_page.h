#ifndef __WEB_PAGE_H__
#define __WEB_PAGE_H__

#include "../common/configuration.h"
#include "../../../third_party/simhash/Simhasher.hpp"
#include <vector>
#include <unordered_map>
#include <string>

namespace simple{
using std::pair;
using std::string;
using std::vector;
using std::unordered_map;

struct RssItem{
    string title;
    string link;
    string author;
    string description;
};

class WebPage{
public:
    WebPage() = default;
    WebPage(string& doc, RssItem& rss_item);
    ~WebPage() = default;
    WebPage(const WebPage&) = default;
    WebPage& operator=(const WebPage&) = default;
    WebPage(WebPage&&) = default;
    WebPage& operator=(WebPage&&) = default;

    void ProcessDoc();
    //根据SimHash()的结果，生成_top_words和摘要
    void CreateAbstract();
    uint64_t GetDocId() const;
    string GetDoc() const;
    vector<pair<string, double>>& GetKeyWordsList();
    uint64_t GetSimHashValue() const;
    void SaveFmtString();

    //整篇文章，包括xml在内
    string _doc;
    //格式化后的网页内容(标题，网址，作者，描述)
    RssItem _rss_item;
    //自动生成的文档摘要
    string _doc_abstract;
    uint64_t _doc_id;
    //词频最高的词
    vector<string> _top_words;
    //simhash生成的关键词序列
    vector<pair<string, double>> _keyword_list;
    uint64_t _simhash_value;
    //保存文档的所有词语和词频(不包括停用词)
    unordered_map<string, uint64_t> _term_frequencies;
    const static int kTopKNumber = 20;
private:

};
}
#endif
