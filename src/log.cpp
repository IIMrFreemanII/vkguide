#include "log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

std::shared_ptr<spdlog::logger> log::_logger;

void log::init() {
    spdlog::set_pattern("%^[%T] %n: %v%$");

    _logger = spdlog::stdout_color_mt("app");
    _logger->set_level(spdlog::level::trace);
}
