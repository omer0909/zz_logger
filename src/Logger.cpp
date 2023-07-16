#include <zz_logger.h>

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <mutex>
#include <sstream>

using namespace std;

namespace COLOR {
static constexpr string_view RESET{"\033[39m"};
static constexpr string_view BLACK{"\033[30m"};
static constexpr string_view RED{"\033[31m"};
static constexpr string_view GREEN{"\033[;32m"};
static constexpr string_view YELLOW{"\033[;33m"};
static constexpr string_view BLUE{"\033[;34m"};
static constexpr string_view MAGENTA{"\033[;35m"};
static constexpr string_view CYAN{"\033[;36m"};
static constexpr string_view WHITE{"\033[;37m"};
}  // namespace COLOR

const map<logger::LogTypes, string_view> logTypeColors{
    {{logger::LogTypes::Error, COLOR::RED},
     {logger::LogTypes::Fatal, COLOR::MAGENTA},
     {logger::LogTypes::Info, COLOR::CYAN},
     {logger::LogTypes::Warning, COLOR::YELLOW}}};

const map<logger::LogTypes, string_view> logTypeNames{
    {{logger::LogTypes::Error, "[ERROR]"},
     {logger::LogTypes::Fatal, "[FATAL]"},
     {logger::LogTypes::Info, "[INFO] "},
     {logger::LogTypes::Warning, "[WARN] "}}};

class logger::Logger::File {
   public:
	File(std::string const &fileName) : file_(fileName, ios_base::app) {}
	fstream file_;
};

static string GetTimeText() {
	using namespace chrono;
	auto now = system_clock::now();
	auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
	auto timer = system_clock::to_time_t(now);
	tm bt = *localtime(&timer);
	ostringstream ss;
	ss << put_time(&bt, "%F %T");
	ss << '.' << setfill('0') << setw(3) << ms.count();
	return ss.str();
}

logger::Logger::Logger(logger::LogTypes logType, string const &fileName, int lineNumber, string const &functionName) : logType_(logType), fileName_(fileName), lineNumber_(lineNumber), functionName_(functionName) {}

std::unique_ptr<logger::Logger::File> logger::Logger::pimpl_ = nullptr;

logger::Logger::~Logger() {
	static std::mutex mtx;
	scoped_lock lock(mtx);

	if (pimpl_ == nullptr)
		cerr << COLOR::RED << "Please set project name with \"SET_PROJECT_NAME(\"project_name\")\"!" << COLOR::RESET << endl;

	ostringstream ss;
	ss << logTypeNames.at(logType_);
	ss << " [" << GetTimeText() << "]";
	ss << " [(" << fileName_ << ", " << lineNumber_ << ")"
	   << ", " << functionName_ << "] ";
	ss << content_.str() << endl;
	string message = ss.str();

	cerr << logTypeColors.at(logType_) << message << COLOR::RESET.data();

	if (pimpl_ == nullptr)
		return;

	pimpl_->file_ << message;
}

std::string logger::Logger::GetLogs(unsigned int maxLineNumber) {
	ostringstream ss;
	string line;

	pimpl_->file_.seekg(0, pimpl_->file_.beg);

	while (getline(pimpl_->file_, line) && maxLineNumber--) {
		ss << line;
	}

	return ss.str();
}

void logger::Logger::SetProjectName(std::string const &projectName) {
	string logDir = allLogDirs + projectName + "/";

	if (!filesystem::is_directory(logDir) || !filesystem::exists(logDir))
		filesystem::create_directory(logDir);

	pimpl_ = std::make_unique<logger::Logger::File>(logger::Logger::File(logDir + "console.log"));
	if (!pimpl_->file_.is_open()) {
		pimpl_.reset();
		cerr << COLOR::RED << "Log file cannot created! : Permission denied" << COLOR::RESET << endl;
	}
}
