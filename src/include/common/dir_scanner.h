#ifndef __DIR_SCANNER_H__
#define __DIR_SCANNER_H__

#include "logger.h"
#include "configuration.h"
#include <filesystem>
#include <string>
#include <vector>

namespace simple{
using std::string;
using std::vector;

class DirScanner{
public:
    explicit DirScanner(Logger& logger, IConfiguration& config);
    //调用Traverse()函数，扫描指定目录写的所有文件
    void operator()();
    vector<string>& GetFilesPath();
    //获取目标文件夹下的所有文件
    void Traverse(const string& dir_path);

private:
    //存放每个语种文件的绝对路径
    vector<string> _file_path_list;
    Logger& _logger;
    IConfiguration& _config;
};
}
#endif
