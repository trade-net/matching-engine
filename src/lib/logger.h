#ifndef INCLUDED_LOGGER
#define INCLUDED_LOGGER

#include <iostream>

namespace logging{

enum class Level{ Debug = 0, Info = 1, Warn = 2, Error = 3, Off = 4 };

inline Level& currentLevel()
{
	static Level level = Level::Warn;
	return level;
}

inline void setLevel(Level level)
{
	currentLevel() = level;
}

} // namespace logging

// expr is only evaluated (and streamed) when the level is enabled, so
// disabled log lines cost a single level comparison on the hot path.
#define LOG(level, expr) \
	do{ \
		if(level >= logging::currentLevel()){ \
			std::cout << expr << std::endl; \
		} \
	} while(0)

#define LOG_DEBUG(expr) LOG(logging::Level::Debug, expr)
#define LOG_INFO(expr)  LOG(logging::Level::Info, expr)
#define LOG_WARN(expr)  LOG(logging::Level::Warn, expr)
#define LOG_ERROR(expr) LOG(logging::Level::Error, expr)

#endif
