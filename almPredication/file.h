#ifndef FILE_H_
#define FILE_H_
#include <string>

template <typename T>
class FileGuardian {
public:
	explicit FileGuardian(const std::string& name) : file(name) {}
	~FileGuardian() { file.close(); }
	operator bool() { return static_cast<bool>(file); }
	
	FileGuardian(const FileGuardian&) = delete;
	FileGuardian& operator=(const FileGuardian&) = delete;
	FileGuardian(FileGuardian&&) = delete;
	FileGuardian& operator=(const FileGuardian&&) = delete;

protected:
	T file;
};

template <typename T>
class InputFile : public FileGuardian<T> {

public:
	explicit InputFile(const std::string& name) : FileGuardian(name) {}

	bool eof() const { return file.eof(); }

	template <typename U>
	T& operator>>(U& val) {
		file >> val;
		return file;
	}
};

template <typename T>
class OutputFile : public FileGuardian<T> {

public:
	explicit OutputFile(const std::string& name) : FileGuardian(name) {}

	template <typename U>
	T& operator<<(U& val) {
		file << val;
		return file;
	}
};

#endif
