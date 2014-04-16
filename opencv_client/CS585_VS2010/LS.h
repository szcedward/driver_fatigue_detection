#pragma once
#include <windows.h>
#include <tchar.h>
#include "string"
#include "vector"

class LS {
private:
  WIN32_FIND_DATAA found;
  HANDLE hfind;
  std::string wildcard;
  std::vector<std::string> result;

  int getNextFile(std::string& fname) {
    int chk;
    chk = FindNextFileA(hfind, &found);

    if (chk) {
      fname = found.cFileName;
    }

    return chk;
  }

  void getAllFilename() {
    result.clear();
    std::string filename;

    while (getNextFile(filename) == 1) {
      result.push_back(filename);
    }
  }
public:
  LS(void) {}
  LS(const std::string& dir) {
    wildcard = dir + "\\*.*";
    hfind = FindFirstFileA(wildcard.c_str(), &found);
    FindNextFileA(hfind, &found);
  }

  void setDir(const std::string& dir) {
    wildcard = dir + "\\*.*";
    hfind = FindFirstFileA(wildcard.c_str(), &found);
    FindNextFileA(hfind, &found);
  }

  std::vector<std::string> getResult() {
    getAllFilename();
    return result;
  }

  int getSize() const {
    return result.size();
  }

};
