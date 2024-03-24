#include "icv_file.hpp"
#include <iostream>
#include <cstdio>

File::File(const std::string &path) : current_path_(path), need_read_(true) {}

File::File(const File &file)
    : current_path_(file.current_path()), need_read_(true) {}

File::~File() {
  if (file_reader_.is_open()) {
    file_reader_.close();
  }
}

std::string File::GetFileContent() {
  if (!need_read_) {
    return file_content_;
  }

  if (LoadFile(current_path_)) {
    return file_content_;
  }
  return "";
}

bool File::LoadFile(const std::string &path) {
  if (path == current_path_ && !need_read_) {
    return true;
  }

  current_path_ = path;

  file_reader_.open(path, std::ios::out);
  if (file_reader_.fail()) {
    std::cerr << "Open "
              << "\"" << path << "\" failed\n";
    return false;
  }

  std::string line;
  file_content_.clear();
  try {
    while (std::getline(file_reader_, line)) {
      file_content_ += line;
      file_content_ += "\n";
    }
    file_reader_.close();
  } catch (const std::exception &e) {
    std::cerr << "Read file \"" << path
              << "\" failed.\n\tErrorMsg: " << e.what() << std::endl;

    printf("Read file \"%s\" failed. ErrorMsg: %s\n", path.c_str(), e.what());
    file_content_.clear();
    return false;
  }
  need_read_ = false;
  return true;
}

bool File::TryOpen() {
  if (current_path_.empty()) {
    return false;
  }
  file_reader_.open(current_path_);
  if (file_reader_.fail()) {
    file_reader_.clear();
    file_reader_.close();
    return false;
  }
  file_reader_.clear();
  file_reader_.close();
  return true;
}

void File::set_current_path(const std::string &path) {
  if (path != current_path_) {
    current_path_ = path;
    need_read_ = true;
  }
}

std::string File::current_path() const { return current_path_; }