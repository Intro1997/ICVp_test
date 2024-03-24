#pragma once

#include <fstream>
#include <string>

class File {
public:
  File(const std::string &path);
  File(const File &file);
  ~File();

  bool LoadFile(const std::string &path);
  bool TryOpen();
  std::string GetFileContent();
  void set_current_path(const std::string &path);
  std::string current_path() const;

private:
  std::ifstream file_reader_;
  std::string current_path_;
  bool need_read_;
  std::string file_content_;
};
