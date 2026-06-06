#ifndef __WEB_PAGE_QUERY_H__
#define __WEB_PAGE_QUERY_H__
#include "page"
#include <map>
#include <algorithm>

namespace simple{
using Cosine = double;
using DocVector = vector<pair<DocId, unordered_map<string, TF_IDF_Weight>>>;
}
#endif
