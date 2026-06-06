#ifndef __RSS_PARSER_H__
#define __RSS_PARSER_H__
#include "../common/configuration.h"
#include "web_page.h"
#include "../../tinyxml2/tinyxml2.h"
#include <re2/re2.h>
#include <memory>
#include <string>
#include <fstream>
#include <vector>

namespace simple{
using tinyxml2::XMLDocument;
using tinyxml2::XMLElement;
using std::string;
using std::vector;

class RssParser{
public:
    explicit RssParser(Logger& Logger);
    void Load(XMLDocument &doc);
    vector<RssItem>& GetRssItems();

    static string FormatText(string s){
        RE2::GlobalReplace(&s, "<[^>]*>", " ");
        return s;
    }

    //debug用
    void Output(const string& filename);

private:
    vector<RssItem> _rss;
    [[maybe_unused]]Logger& _logger;
};

int LoadXML(const string& filename, XMLDocument &doc);
}
#endif
