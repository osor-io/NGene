#pragma once
#include "Manager.h"
#include <string>
#include <vector>

#define MAX_OS_FILENAME_SIZE 512

class OSManager : public Manager<OSManager> {
	friend class CRSP<OSManager>;
private:
	OSManager();
	~OSManager();

public:

	void start_up() override;
	void shut_down() override;

	std::string user_open_file(const char* filters = nullptr); // Default value for filters was: "All\0*.*\0Text\0*.TXT\0"
	std::vector<std::string> user_open_files(const char* filters = nullptr); // Default value for filters was: "All\0*.*\0Text\0*.TXT\0"
	std::string user_save_file(const char* filters = nullptr); // Default value for filters was: "All\0*.*\0Text\0*.TXT\0"
	std::string executable_path();

private:
	std::string m_executable_path{};

};

