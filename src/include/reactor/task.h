#ifndef __TASK_H__
#define __TASK_H__

#include  <functional>

namespace simple{
using std::function;

using Task = function<void()>;
}
#endif
