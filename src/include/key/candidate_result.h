#ifndef __CANDIDATE_RESULT_H__
#define __CANDIDATE_RESULT_H__

#include <string>
namespace simple{
using std::string;

class CandidateResult{
public:
    CandidateResult();
    ~CandidateResult();
    string _word;
    int _freq;
    int _dist;
};
}
#endif
