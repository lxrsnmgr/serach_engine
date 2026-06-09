#include "../include/web/web_page_query.h"

namespace simple{
using std::map;

WebPageQuery::WebPageQuery(const PageLibPreprocessor& pre, Logger& logger,
                           IConfiguration& config)
    : _logger(logger),
      _config(config),
      _pre(pre),
      _split_tool(_logger, _config){}

string WebPageQuery::DoQuery(const string& sought){
    unordered_map<string, uint64_t> query_words = Cut(sought);

    unordered_map<string, TF_IDF_Weight> base_vec = CaculateQueryWeights(query_words);
    auto relative_pages_vectors = CaculateRelativePagesVectors(query_words);
    if(relative_pages_vectors.empty()){
        return NoAnswer();
    }
    vector<DocId> ordered_pages = GetPagesOrder(base_vec, relative_pages_vectors);
    return CreateJson(ordered_pages);
}

unordered_map<string, TF_IDF_Weight> WebPageQuery::CaculateQueryWeights(const unordered_map<string, uint64_t>& query_words){
    return CaculateValue(query_words, _pre._document_frequencies, _pre._page_lib_list. size());
}

vector<DocId> WebPageQuery::GetRelativePages(const unordered_map<string, uint64_t>& query_words){
    vector<DocId> relative_pages;
    unordered_map<DocId, uint64_t> document_frequencies;
    //获取查询词个数
    uint64_t query_words_num = query_words.size();
    document_frequencies.reserve(query_words_num);

    //查询每一个查询词
    for(const auto& pair : query_words){
        auto it = _pre._invert_index_table.find(pair.first);
        //只要有一个查询词不在索引表，救人位没有找到相关网页
        if(it == _pre._invert_index_table.end()){
            return vector<DocId>();
        }
        //查找包含查询词的所有网页
        for(const auto& doc_id_set : it->second){
            //记录相关网页是否在结构集合中
            auto it_doc_freq = document_frequencies.find(doc_id_set.first);
            if(it_doc_freq == document_frequencies.end()){
                document_frequencies.insert(std::make_pair(doc_id_set.first, 1));
            } else {
                //标记数加一
                it_doc_freq->second += 1;
            }
        }
    }
    //遍历DF，fre == query_words_nu, 就是相关网页
    for(const auto& pair : document_frequencies){
        if(pair.second == query_words_num){
            relative_pages.push_back(pair.first);
        }
    }
    return relative_pages;
}

DocsVector WebPageQuery::CaculateRelativePagesVectors(const unordered_map<string, uint64_t>& query_words){
    auto relative_pages = GetRelativePages(query_words);
    DocsVector relative_pages_vectors;
    relative_pages_vectors.reserve(relative_pages.size());

    for(const auto& doc_id : relative_pages){
        auto page = _pre._indexed_page_lib_list.find(doc_id)->second;

        auto page_all_vectors = CaculateValue(page._term_frequencies, _pre._document_frequencies,
                                              _pre._page_lib_list.size());
        unordered_map<string, TF_IDF_Weight> page_query_vectors;
        page_query_vectors.reserve(query_words.size());
        for(const auto& pair : query_words){
            auto it = page_all_vectors.find(pair.first);
            if(it == page_all_vectors.end()){
                continue;
            }
            page_query_vectors.insert(std::make_pair(pair.first, it->second));
        }
        relative_pages_vectors.emplace_back(make_pair(doc_id, page_query_vectors));
    }
    return relative_pages_vectors;
}

Cosine WebPageQuery::CaculateCosSimilarity(
    const unordered_map<string, TF_IDF_Weight>& x,
    const unordered_map<string, TF_IDF_Weight>& y){
    // X * Y = (x1 * x2 + x2 * y2 + ...)
    double sum = 0.0;
    for(const auto& pair : x){
        auto it = y.find(pair.first);
        if(it == y.end()){
            throw std::runtime_error("CaculateCosSimilarity error");
        }
        sum += pair.second * it->second;
    }

    // |X|
    double x_norm = 0.0;
    for(const auto& pair : x){
        x_norm += pair.second * pair.second;
    }
    x_norm = sqrt(x_norm);

    // |Y|
    double y_norm = 0.0;
    for(const auto& pair : y){
        y_norm += pair.second * pair.second;
    }
    y_norm = sqrt(y_norm);

    // Cosine = (X * Y) / (|X| * |Y|)
    return sum / (x_norm * y_norm);
}

vector<DocId> WebPageQuery::GetPagesOrder(
        const unordered_map<string, TF_IDF_Weight>& base_vec, const DocsVector& relative_pages_vectors){
    map<double, DocId, std::greater<>> ordered_pages;
    for(const auto& pair : relative_pages_vectors){
        auto cos_similarity = CaculateCosSimilarity(base_vec, pair.second);
        ordered_pages.insert(std::make_pair(cos_similarity, pair.first));
    }
    vector<DocId> result;
    result.reserve(ordered_pages.size());
    for(const auto& pair : ordered_pages){
        result.push_back(pair.second);
    }
    return result;
}

string WebPageQuery::CreateJson(vector<DocId>& doc_ids){
    string result;
    for(const auto& doc_id : doc_ids){
        result.append("------------------------\n");
        result.append("<title> ");
        result.append(_pre._indexed_page_lib_list.find(doc_id)->second._rss_item.title);
        result.append("\n");
        result.append("<abstract> ");
        result.append(_pre._indexed_page_lib_list.find(doc_id)->second._doc_abstract);
        result.append("\n");
        result.append("<link> ");
        result.append(_pre._indexed_page_lib_list.find(doc_id)->second._rss_item.link);
        result.append("\n");
        result.append("\n");
    }
    return result;
}

string WebPageQuery::NoAnswer(){
    return "No answer\n";
}

unordered_map<string, uint64_t> WebPageQuery::Cut(const string& sought){
    unordered_map<string, uint64_t> result;
    _split_tool.Cut(sought, result, true);
    return result;
}
}
