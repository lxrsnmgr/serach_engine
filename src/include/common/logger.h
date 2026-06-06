#ifndef __LOGGER_H__
#define __LOGGER_H__

#include "spdlog/sinks/basic_file_sink.h"
#include  "spdlog/spdlog.h"
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <spdlog/common.h>
#include <string>

#define LOG_PREFIX(msg) string("[")\
    .append(__FILE__).append(":")\
    .append(std::to_string(__LINE__))\
    .append(":")\
    .append(__FUNCTION__)\
    .append("]")\
    .append(msg).c_str()

//类内使用方法
#define LOG_ERROR(msg, ...) _logger.Error(LOG_PREFIX(msg), ##__VA_ARGS__)
#define LOG_WARN(msg, ...) _logger.Warn(LOG_PREFIX(msg), ##__VA_ARGS__)
#define LOG_INFO(msg, ...) _logger.Info(LOG_PREFIX(msg), ##__VA_ARGS__)
#define LOG_DEBUG(msg, ...) _logger.Debug(LOG_PREFIX(msg), ##__VA_ARGS__)

#pragma clang diagnostic pop
namespace simple{
using DocId = uint64_t;
using TF_IDF_WEIGHT = double;
using std::string;
class Logger{
public:
    explicit Logger(const std::string& filename){
        try{
            spdlog::set_pattern("[%D:%H:%M:%S %z] [%n] [%l] [thread %t] %v");
            _logger = spdlog::basic_logger_mt("simple", filename);
        } catch (const spdlog::spdlog_ex& ex){
            std::cerr << "Log init failed: " << ex.what() << std::endl;
            exit(1);
        }
    }

    template<typename... Args>
    void Info(const std::string& msg, Args... args){
        _logger->info(msg, args...);
    }

    template<typename... Args>
    void Debug(const std::string& msg, Args... args){
        _logger->debug(msg, args...);
    }

    template<typename... Args>
    void Warn(const std::string& msg, Args... args){
        _logger->warn(msg, args...);
    }

    template<typename... Args>
    void Error(const std::string& msg, Args... args){
        _logger->error(msg, args...);
    }

    void SetLogLevel(spdlog::level::level_enum level){
        _logger->set_level(level);
    }

private:
    std::shared_ptr<spdlog::logger> _logger;
};
}
#endif
