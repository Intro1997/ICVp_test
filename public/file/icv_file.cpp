#include "icv_file.hpp"
#include <iostream>
#include <string>

File::File(const char *path, const char *filename) {
  path_.assign(path);
  filename_.assign(filename);
}

std::string File::GetContent() const { return content_; }

std::string File::filename() const { return filename_; }

bool File::LoadContent() {
  if (path_.empty()) {
    return false;
  }

  file_stream_.clear();
  file_stream_.open(path_);
  if (!file_stream_.is_open()) {
    file_stream_.clear();
    std::cerr << "open " << path_ << " failed\n";
    return false;
  }

  std::string line;

  while (std::getline(file_stream_, line)) {
    content_ += line;
    content_ += "\n";
  }

  file_stream_.clear();
  file_stream_.close();
  return true;
}

void File::Clear() {
  file_stream_.clear();
  content_.clear();
}