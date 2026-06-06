#include "../include/web/page_lib.h"

namespace simple{
using std::ios;
using std::make_pair;
using std::ofstream;

PageLib::PageLib(Logger& logger, IConfiguration& config)
: _logger{logger}
, _config{config}
, _dir_scanner{_logger, _config}
, _rss_parser(_logger)
, _simhasher(_config.GetConfig()["jieba.dict"],
            _config.GetConfig()["hmm_model"],
            _config.GetConfig()["idf"],
            _config.GetConfig()["stop_words"]){
    _ripe_pages.reserve(5000);
}

void PageLib::Load(){
    int docid = 0;
    _dir_scanner();
    vector<string> files = _dir_scanner.GetFilesPath();
    for(const string& file : files){
        XMLDocument doc;
        LoadXML(file, doc);
        _rss_parser.Load(doc);
    }

    for(const RssItem& rss_item : _rss_parser.GetRssItems()){
        string page;
        page.append("<doc>");
        page.append("<docid>");
        page.append(std::to_string(++docid));
        page.append("</docid>");
        page.append("<title>");
        page.append(rss_item.title);
        page.append("</title>");
        page.append("<link>");
        page.append(rss_item.link);
        page.append("</link>");
        page.append("<author>");
        page.append(rss_item.author);
        page.append("</author>");
        page.append("<description>");
        page.append(rss_item.description);
        page.append("</description>");
        page.append("</doc>");
        _ripe_pages.emplace_back(page);
    }
}

size_t PageLib::GetPageNum() const {
    return _ripe_pages.size();
}

vector<RssItem>& PageLib::GetRssItems(){
    return _rss_parser.GetRssItems();
}

vector<string>& PageLib::GetRipePages(){
    return _ripe_pages;
}

Simhasher& PageLib::GetSimhasher(){
    return _simhasher;
}

void PageLib::StoreOnDisk(){
    //存储网页库，目标文件夹必须存在
    ofstream ofs("../../data/database/ripe_page.dat", ios::binary);
    for(const string& page : _ripe_pages){
        ofs.write(page.c_str(), static_cast<int64_t>(page.size()));
    }
    ofs.close();
    //存储位置偏移库
    ofstream ofs_offset("../../data/database/offset_lib.dat", ios::binary);
    uint64_t doc_id = 1;
    uint64_t offset = 0;
    uint64_t doc_size = 0;
    for(const string& page : _ripe_pages){
        ofs_offset.write(reinterpret_cast<const char*>(&doc_id), sizeof(uint64_t));
        ofs_offset.write(reinterpret_cast<const char*>(&offset), sizeof(uint64_t));
        offset += page.size();

        doc_size = _ripe_pages[doc_id - 1].size();
        ofs_offset.write(reinterpret_cast<const char*>(&doc_size),
                         sizeof(uint64_t));
        ++doc_id;
        _offset_lib.insert(make_pair(doc_id, make_pair(offset, doc_size)));
    }
    ofs_offset.close();
}
}
