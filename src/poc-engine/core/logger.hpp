#pragma once

#include <iostream>
#include <string>
#include <string_view>

namespace poc {

	namespace Logger {

		enum class Level {
			DEBUG = 0,
			INFO = 1,
			WARN = 2,
			ERROR = 3
		};

#ifdef NDEBUG

		inline void setLoggerLevel(const Level level) {}
		inline void debug(const std::string_view& tag, const std::string& msg) {}
		inline void info(const std::string_view& tag, const std::string& msg) {}
		inline void warn(const std::string_view& tag, const std::string& msg) {}
		inline void error(const std::string_view& tag, const std::string& msg) {}
		inline void log(const Level level, const std::string_view& tag, const std::string& msg) {}
#else

		static Level loggerLevel = Level::INFO;

		inline void setLoggerLevel(const Level level) {
			loggerLevel = level;
		}

		// Log format: LEVEL --- [TAG] Log message
		inline void log(const Level level, const std::string_view& tag, const std::string& msg) {
			if (loggerLevel <= level) {
				switch (level)
				{
				case Level::DEBUG:
					std::cout << " DEBUG --- " << "[" << tag << "] " << msg << std::endl;
					break;
				case Level::INFO:
					std::cout << " INFO  --- " << "[" << tag << "] " << msg << std::endl;
					break;
				case Level::WARN:
					std::cout << " WARN  --- " << "[" << tag << "] " << msg << std::endl;
					break;
				case Level::ERROR:
					std::cout << " ERROR --- " << "[" << tag << "] " << msg << std::endl;
					break;
				default:
					break;
				}
			}
		}

		inline void debug(const std::string_view& tag, const std::string& msg) {
			log(Level::DEBUG, tag, msg);
		}

		inline void info(const std::string_view& tag, const std::string& msg) {
			log(Level::INFO, tag, msg);
		}

		inline void warn(const std::string_view& tag, const std::string& msg) {
			log(Level::WARN, tag, msg);
		}

		inline void error(const std::string_view& tag, const std::string& msg) {
			log(Level::ERROR, tag, msg);
		}

#endif
	}

}