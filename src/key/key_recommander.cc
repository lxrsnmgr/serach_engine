#include "../include/key/key_recommander.h"
#include <pthread.h>

namespace simple{
KeyRecommander::KeyRecommander(Dictionary &dictionary)
: _dictionary(dictionary)
, _sought()
, _word_num(200){
    
}

void KeyRecommander::Query(const string &word){
    _sought = std::move(word);

    unordered_map<int, int> word_map(_dictionary.DoQuery(_sought));
    
    CandidateResult result;
    if(simple::Dictionary::CharLen(_sought) == 3){
        vector<pair<string, int>> &dict = _dictionary.GetCnDict();//加载中文词典
        for(const auto &elem : word_map){
            result._word = dict[elem.first].first;
            result._freq = dict[elem.second].second;
            result._dist = MinimumEditDist(result._word);
            _prique.emplace(result);
        }
    } else {
        vector<pair<string, int>> &dict = _dictionary.GetEnDict();//加载英文词典
        for(const auto &elem : word_map){
            result._word = dict[elem.first].first;
            result._freq = dict[elem.first].second;
            result._dist = MinimumEditDist(result._word);
            _prique.emplace(result);
        }
    }
}

void KeyRecommander::Print(){
    //输出优先级队列中的数据
    int cnt = 1;
    for(size_t i = 0; i < _prique.size(); ++i){
        std::cout << _prique.top()._word << std::endl;
        _prique.pop();
        if(cnt == 5){
            break;;
        }
        ++cnt;
    }
    for(size_t i = 0; i < _prique.size(); ++i){
        _prique.pop();
    }
}

string KeyRecommander::GetString(){
    _resultvec.clear();
    //保存优先级队列中的前5个元素
    for(int i = 0; i < _prique.size(); ++i){
        _resultvec.push_back(_prique.top()._word);
        _prique.pop();
        if(i == 4){
            break;
        }
    }
    for(int i = 0; i < _prique.size(); ++i){
        _prique.pop();
    }
    string result;
    for(const auto &elem : _resultvec){
        result.append(elem).append("\n");
    }
    return result;
}

int KeyRecommander::MinimumEditDist(const string &rhs){
    const string &lhs = _sought;
    int char_len = simple::Dictionary::CharLen(rhs);
    int lhs_len = static_cast<int>(lhs.length()) / char_len;
    int rhs_len = static_cast<int>(rhs.length()) / char_len;

    if(0 == lhs_len * rhs_len){
        return lhs_len + rhs_len;
    }

    vector<vector<int>> distance(lhs_len + 1, vector<int> (rhs_len + 1));

    for(int i = 0; i < lhs_len; ++i){
        distance[i][0] = i;
    }
    for(int i = 0; i < rhs_len; ++i){
        distance[0][i] = i;
    }

    for(int i = 1; i < lhs_len + 1; ++i){
        for(int j = 0; j < rhs_len + 1; ++j){
            int left = distance[i - 1][j] + 1;
            int down = distance[i][j - 1] + 1;
            int left_down = distance[i - 1][j - 1];
            if(string(lhs, (i - 1) * char_len, char_len) != string(rhs, (j - 1) * char_len, char_len)){
                ++left_down;
            }
            distance[i][j] = std::min(left, std::min(down, left_down));
        }
    }
    return distance[lhs_len][rhs_len];
}
}
