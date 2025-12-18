#include "nanobind/nanobind.h"
#include "nanobind/stl/string.h"
#include "nanobind/stl/vector.h"
#include "nanobind/stl/shared_ptr.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/sink.h"
#include "spdlog/sinks/base_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/null_sink.h"
#include "spdlog/async.h"
#include "spdlog/async_logger.h"
#include "spdlog/common.h"
#include <chrono>

namespace nb = nanobind;
using namespace nb::literals;

NB_MODULE(spydlog, m) {
    // Log level enum
    nb::enum_<spdlog::level::level_enum>(m, "level")
        .value("trace", spdlog::level::trace)
        .value("debug", spdlog::level::debug)
        .value("info", spdlog::level::info)
        .value("warn", spdlog::level::warn)
        .value("err", spdlog::level::err)
        .value("critical", spdlog::level::critical)
        .value("off", spdlog::level::off);

    // Color mode enum
    nb::enum_<spdlog::color_mode>(m, "color_mode")
        .value("always", spdlog::color_mode::always)
        .value("automatic", spdlog::color_mode::automatic)
        .value("never", spdlog::color_mode::never);

    // Sink base class
    nb::class_<spdlog::sinks::sink>(m, "sink")
        .def("log", [](spdlog::sinks::sink& self, spdlog::level::level_enum lvl, const std::string& msg) {
            spdlog::details::log_msg log_msg(spdlog::source_loc{}, "", lvl, msg);
            self.log(log_msg);
        })
        .def("set_level", &spdlog::sinks::sink::set_level)
        .def("level", &spdlog::sinks::sink::level)
        .def("set_pattern", &spdlog::sinks::sink::set_pattern);

    // Console sinks
    nb::class_<spdlog::sinks::stdout_color_sink_mt, spdlog::sinks::sink>(m, "stdout_color_sink_mt")
        .def(nb::init<>())
        .def(nb::init<spdlog::color_mode>(), "mode"_a);

    nb::class_<spdlog::sinks::stdout_color_sink_st, spdlog::sinks::sink>(m, "stdout_color_sink_st")
        .def(nb::init<>())
        .def(nb::init<spdlog::color_mode>(), "mode"_a);

    nb::class_<spdlog::sinks::stderr_color_sink_mt, spdlog::sinks::sink>(m, "stderr_color_sink_mt")
        .def(nb::init<>())
        .def(nb::init<spdlog::color_mode>(), "mode"_a);

    nb::class_<spdlog::sinks::stderr_color_sink_st, spdlog::sinks::sink>(m, "stderr_color_sink_st")
        .def(nb::init<>())
        .def(nb::init<spdlog::color_mode>(), "mode"_a);

    nb::class_<spdlog::sinks::stdout_sink_mt, spdlog::sinks::sink>(m, "stdout_sink_mt")
        .def(nb::init<>());

    nb::class_<spdlog::sinks::stdout_sink_st, spdlog::sinks::sink>(m, "stdout_sink_st")
        .def(nb::init<>());

    nb::class_<spdlog::sinks::stderr_sink_mt, spdlog::sinks::sink>(m, "stderr_sink_mt")
        .def(nb::init<>());

    nb::class_<spdlog::sinks::stderr_sink_st, spdlog::sinks::sink>(m, "stderr_sink_st")
        .def(nb::init<>());

    // Basic file sink
    nb::class_<spdlog::sinks::basic_file_sink_mt, spdlog::sinks::sink>(m, "basic_file_sink_mt")
        .def(nb::init<const std::string&, bool>(),
             "filename"_a, "truncate"_a = false);

    nb::class_<spdlog::sinks::basic_file_sink_st, spdlog::sinks::sink>(m, "basic_file_sink_st")
        .def(nb::init<const std::string&, bool>(),
             "filename"_a, "truncate"_a = false);

    // Rotating file sink
    nb::class_<spdlog::sinks::rotating_file_sink_mt, spdlog::sinks::sink>(m, "rotating_file_sink_mt")
        .def(nb::init<const std::string&, size_t, size_t>(),
             "filename"_a, "max_size"_a, "max_files"_a);

    nb::class_<spdlog::sinks::rotating_file_sink_st, spdlog::sinks::sink>(m, "rotating_file_sink_st")
        .def(nb::init<const std::string&, size_t, size_t>(),
             "filename"_a, "max_size"_a, "max_files"_a);

    // Daily file sink
    nb::class_<spdlog::sinks::daily_file_sink_mt, spdlog::sinks::sink>(m, "daily_file_sink_mt")
        .def(nb::init<const std::string&, int, int>(),
             "filename"_a, "hour"_a = 0, "minute"_a = 0);

    nb::class_<spdlog::sinks::daily_file_sink_st, spdlog::sinks::sink>(m, "daily_file_sink_st")
        .def(nb::init<const std::string&, int, int>(),
             "filename"_a, "hour"_a = 0, "minute"_a = 0);

    // Null sink (no need to register null_sink_mt since they are the same sink)
    nb::class_<spdlog::sinks::null_sink_st, spdlog::sinks::sink>(m, "null_sink_st")
        .def(nb::init<>());

    // Logger class
    nb::class_<spdlog::logger>(m, "logger")
        .def(nb::init<const std::string&>())
        .def(nb::init<const std::string&, spdlog::sink_ptr>(),
             "name"_a, "sink"_a)
        .def("__init__", [](spdlog::logger* logger, const std::string& name, const std::vector<spdlog::sink_ptr>& sinks) {
            new (logger) spdlog::logger(name, sinks.begin(), sinks.end());
        }, "name"_a, "sinks"_a)
        .def("trace", [](spdlog::logger& self, const std::string& msg) { self.trace(msg); })
        .def("debug", [](spdlog::logger& self, const std::string& msg) { self.debug(msg); })
        .def("info", [](spdlog::logger& self, const std::string& msg) { self.info(msg); })
        .def("warn", [](spdlog::logger& self, const std::string& msg) { self.warn(msg); })
        .def("error", [](spdlog::logger& self, const std::string& msg) { self.error(msg); })
        .def("critical", [](spdlog::logger& self, const std::string& msg) { self.critical(msg); })
        .def("log", [](spdlog::logger& self, spdlog::level::level_enum lvl, const std::string& msg) {
            self.log(lvl, msg);
        })
        .def("set_level", &spdlog::logger::set_level)
        .def("level", &spdlog::logger::level)
        .def("name", &spdlog::logger::name)
        .def("set_pattern", &spdlog::logger::set_pattern)
        .def("flush", &spdlog::logger::flush)
        .def("flush_on", &spdlog::logger::flush_on)
        .def("sinks", [](spdlog::logger& self) { return self.sinks(); })
        .def("should_log", &spdlog::logger::should_log);

    // Thread-Pool and Async initialization
    nb::class_<spdlog::details::thread_pool>(m, "_thread_pool")
        .def(nb::init<size_t, size_t>(), "q_max_items"_a, "threads_n"_a);

    m.def("init_thread_pool", [](size_t queue_size, size_t thread_count) { spdlog::init_thread_pool(queue_size, thread_count, []{}, []{}); },
            "queue_size"_a, "thread_count"_a = 1);
    m.def("thread_pool", &spdlog::thread_pool);

    // Async logger
    nb::class_<spdlog::async_logger, spdlog::logger>(m, "async_logger")
        .def(nb::init<const std::string&, spdlog::sink_ptr, std::shared_ptr<spdlog::details::thread_pool>>(),
             "name"_a, "sink"_a, "thread_pool"_a)
        .def("__init__", [](spdlog::async_logger* logger, const std::string& name,
                            const std::vector<spdlog::sink_ptr>& sinks,
                            std::shared_ptr<spdlog::details::thread_pool> tp) {
            new (logger) spdlog::async_logger(name, sinks.begin(), sinks.end(), tp, spdlog::async_overflow_policy::block);
        }, "name"_a, "sinks"_a, "thread_pool"_a);

    // Global logger functions
    m.def("set_level", &spdlog::set_level);
    m.def("get_level", &spdlog::get_level);
    m.def("flush_on", &spdlog::flush_on);
    m.def("flush_every", [](int milliseconds) {
        spdlog::flush_every(std::chrono::milliseconds(milliseconds));
    }, "milliseconds"_a);
    m.def("set_pattern", [](const std::string& pattern) { spdlog::set_pattern(pattern); });

    // Global logging functions
    m.def("trace", [](const std::string& msg) { spdlog::trace(msg); });
    m.def("debug", [](const std::string& msg) { spdlog::debug(msg); });
    m.def("info", [](const std::string& msg) { spdlog::info(msg); });
    m.def("warn", [](const std::string& msg) { spdlog::warn(msg); });
    m.def("error", [](const std::string& msg) { spdlog::error(msg); });
    m.def("critical", [](const std::string& msg) { spdlog::critical(msg); });

    // Logger registry
    m.def("set_default_logger", &spdlog::set_default_logger);
    m.def("default_logger", &spdlog::default_logger);
    m.def("get", &spdlog::get, "name"_a);
    m.def("drop", &spdlog::drop, "name"_a);
    m.def("drop_all", &spdlog::drop_all);
    m.def("register_logger", &spdlog::register_logger);
    m.def("apply_all", [](const std::function<void(std::shared_ptr<spdlog::logger>)>& fun) {
        spdlog::apply_all(fun);
    });

    // Async overflow policy enum
    nb::enum_<spdlog::async_overflow_policy>(m, "async_overflow_policy")
        .value("block", spdlog::async_overflow_policy::block)
        .value("overrun_oldest", spdlog::async_overflow_policy::overrun_oldest);

    // Factory functions for common logger types
    m.def("stdout_color_mt", [](const std::string& logger_name, spdlog::color_mode mode) {
        return spdlog::stdout_color_mt(logger_name, mode);
    }, "logger_name"_a, "mode"_a = spdlog::color_mode::automatic);

    m.def("stdout_color_st", [](const std::string& logger_name, spdlog::color_mode mode) {
        return spdlog::stdout_color_st(logger_name, mode);
    }, "logger_name"_a, "mode"_a = spdlog::color_mode::automatic);

    m.def("stderr_color_mt", [](const std::string& logger_name, spdlog::color_mode mode) {
        return spdlog::stderr_color_mt(logger_name, mode);
    }, "logger_name"_a, "mode"_a = spdlog::color_mode::automatic);

    m.def("stderr_color_st", [](const std::string& logger_name, spdlog::color_mode mode) {
        return spdlog::stderr_color_st(logger_name, mode);
    }, "logger_name"_a, "mode"_a = spdlog::color_mode::automatic);

    m.def("stdout_logger_mt", [](const std::string& logger_name) { return spdlog::stdout_logger_mt(logger_name); });
    m.def("stdout_logger_st", [](const std::string& logger_name) { return spdlog::stdout_logger_st(logger_name); });
    m.def("stderr_logger_mt", [](const std::string& logger_name) { return spdlog::stderr_logger_mt(logger_name); });
    m.def("stderr_logger_st", [](const std::string& logger_name) { return spdlog::stderr_logger_st(logger_name); });

    m.def("basic_logger_mt", [](const std::string& logger_name, const std::string& filename, bool truncate) {
        return spdlog::basic_logger_mt(logger_name, filename, truncate);
    }, "logger_name"_a, "filename"_a, "truncate"_a = false);

    m.def("basic_logger_st", [](const std::string& logger_name, const std::string& filename, bool truncate) {
        return spdlog::basic_logger_st(logger_name, filename, truncate);
    }, "logger_name"_a, "filename"_a, "truncate"_a = false);

    m.def("rotating_logger_mt", [](const std::string& logger_name, const std::string& filename,
                                    size_t max_size, size_t max_files) {
        return spdlog::rotating_logger_mt(logger_name, filename, max_size, max_files);
    }, "logger_name"_a, "filename"_a, "max_size"_a, "max_files"_a);

    m.def("rotating_logger_st", [](const std::string& logger_name, const std::string& filename,
                                    size_t max_size, size_t max_files) {
        return spdlog::rotating_logger_st(logger_name, filename, max_size, max_files);
    }, "logger_name"_a, "filename"_a, "max_size"_a, "max_files"_a);

    m.def("daily_logger_mt", [](const std::string& logger_name, const std::string& filename,
                                 int hour, int minute) {
        return spdlog::daily_logger_mt(logger_name, filename, hour, minute);
    }, "logger_name"_a, "filename"_a, "hour"_a = 0, "minute"_a = 0);

    m.def("daily_logger_st", [](const std::string& logger_name, const std::string& filename,
                                 int hour, int minute) {
        return spdlog::daily_logger_st(logger_name, filename, hour, minute);
    }, "logger_name"_a, "filename"_a, "hour"_a = 0, "minute"_a = 0);
}
