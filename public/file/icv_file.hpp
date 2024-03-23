#ifndef __TOOLS_FILE_HPP__
#define __TOOLS_FILE_HPP__

#include <fstream>

class File {
public:
  File(const char *path, const char *filename);
  const char *path() const;

  std::string GetContent() const;
  std::string filename() const;
  bool LoadContent();
  void Clear();

private:
  std::string path_;
  std::string filename_;
  std::fstream file_stream_;
  std::string content_;
};

#endif //__TOOLS_FILE_HPP__