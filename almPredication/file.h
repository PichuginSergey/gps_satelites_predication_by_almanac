#ifndef FILE_H_
#define FILE_H_
#include <string>
#include <fstream>

template <typename T>
class FileGuardian {
public:
	explicit FileGuardian(const std::string& fileName) : file_(fileName) {}
	~FileGuardian() { file_.close(); }
	operator bool() { return static_cast<bool>(file_); }
	
	FileGuardian(const FileGuardian&) = delete;
	FileGuardian& operator=(const FileGuardian&) = delete;
	FileGuardian(FileGuardian&&) = delete;
	FileGuardian& operator=(const FileGuardian&&) = delete;

protected:
	T file_;
};

template <typename T>
class InputFile : public FileGuardian<T> {

public:
	explicit InputFile(const std::string& fileName) : FileGuardian<T>(fileName) {}

	bool eof() const { return file_.eof(); }

	template <typename U>
	T& operator>>(U& val) {
		file_ >> val;
		return file_;
	}
};

template <typename T>
class OutputFile : public FileGuardian<T> {

public:
	explicit OutputFile(const std::string& fileName) : FileGuardian<T>(fileName) {}

	template <typename U>
	T& operator<<(U& val) {
		file_ << val;
		return file_;
	}
};

#endif
