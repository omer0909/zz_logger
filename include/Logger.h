#include <memory>
#include <sstream>

namespace logger {

const std::string allLogDirs{"/var/log/"};

enum LogTypes : int {
	Error,
	Info,
	Fatal,
	Warning,
};

class Logger {
   public:
	Logger(LogTypes logType, std::string const &fileName, int lineNumber, std::string const &functionName);
	~Logger();

	static void SetProjectName(std::string const &projectName);
	static std::string GetLogs(unsigned int maxLineNumber = 0xFFFFFFFF);

	template <typename T>
	Logger &operator<<(const T &value) {
		content_ << value;
		return *this;
	}

   private:
	LogTypes logType_;
	std::string const &fileName_;
	int lineNumber_;
	std::string const &functionName_;
	std::ostringstream content_;
	class File;
	static std::unique_ptr<File> pimpl_;
};
}  // namespace logger

#define LOG_E logger::Logger(logger::LogTypes::Error, __FILE__, __LINE__, __func__)

#define LOG_I logger::Logger(logger::LogTypes::Info, __FILE__, __LINE__, __func__)

#define LOG_F logger::Logger(logger::LogTypes::Fatal, __FILE__, __LINE__, __func__)

#define LOG_W logger::Logger(logger::LogTypes::Warning, __FILE__, __LINE__, __func__)

#define SET_PROJECT_NAME(NAME) logger::Logger::SetProjectName(NAME);