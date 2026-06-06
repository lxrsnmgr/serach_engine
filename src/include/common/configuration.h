#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include "file.h"
#include "logger.h"
#include "nlohmann/json.hpp"
#include <map>
#include <set>
#include <string>
#include <utility>

namespace simple{
using std::map;
using std::set;
using std::string;

class IConfiguration{
public:
    virtual ~IConfiguration() = default;
    virtual void Load() = 0;
    virtual void LoadConfig() = 0;
    virtual void LoadStopWordList() = 0;
    virtual map<string, string>& GetConfig() = 0;
    virtual set<string> & GetStopWordList() = 0;
};

class Configuration : public IConfiguration{
public:
    explicit Configuration(string file_path, Logger& logger);
    ~Configuration() override = default;
    Configuration(const Configuration&) = delete;
    Configuration(Configuration&&) = default;
    Configuration& operator=(Configuration&&) = delete;

    void Load() override;
    map<string, string>& GetConfig() override;
    set<string>& GetStopWordList() override;

private:
    void LoadConfig() override;
    void LoadStopWordList() override;
    map<string, string> _config;
    set<string> _stop_word_list;
    string _file_path;
    [[maybe_unused]] Logger& _logger;
};
}
#endif
