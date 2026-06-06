#include "../include/web/web_page.h"

namespace simple{
WebPage::WebPage(string& doc, RssItem& rss_item)
: _doc{std::move(doc)}
, _rss_item{std::move(rss_item)}{
    _top_words.reserve(kTopKNumber);
    _keyword_list.reserve(kTopKNumber);
}

void WebPage::ProcessDoc(){
    CreateAbstract();
}

void WebPage::CreateAbstract(){
    for(const auto& word : _keyword_list){
        _top_words.push_back(word.first);
        _doc_abstract.append(word.first);
        _doc_abstract.append(" ");
    }
}

uint64_t WebPage::GetDocId() const {
    return _doc_id;
}

string WebPage::GetDoc() const {
    return _doc;
}

vector<pair<string, double>>& WebPage::GetKeyWordsList(){
    return _keyword_list;
}

uint64_t WebPage::GetSimHashValue() const {
    return _simhash_value;
}
}
