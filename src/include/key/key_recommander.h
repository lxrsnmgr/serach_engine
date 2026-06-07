#ifndef __KEY_RECOMMANDER_H__
#define __KEY_RECOMMANDER_H__
#include "dictionary.h"
#include "candidate_result.h"
#include <queue>

namespace simple{
using std::priority_queue;

struct CrCompare{
    bool operator()(CandidateResult &lhs, CandidateResult &rhs){
        bool flag;
        if(lhs._dist == rhs._dist){
            flag = (lhs._freq < rhs._freq);//词频大的放前边
        } else if(lhs._dist < rhs._dist){//距离大的放后边
            flag = false;
        } else {
            flag = true;
        }
        return flag;
    }
};

class KeyRecommander{
public:
    explicit  KeyRecommander(Dictionary &dictionary);
    void Query(const std::string &word);
    void Print();
    string GetString();

private:
    priority_queue<CandidateResult, vector<CandidateResult>, CrCompare> _prique;
    Dictionary &_dictionary;
    string _sought;
    vector<string> _resultvec;
    int _word_num;
    int MinimumEditDist(const std::string &rhs);
};
}
#endif
