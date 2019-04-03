// Author: Minrui(hustminrui@126.com)

#ifndef YCMD_HIGHLIGHT_H
#define YCMD_HIGHLIGHT_H

#include "Location.h"

#include <string>

struct Highlight {
  std::string text_;
  std::string type_;
  unsigned int line_;
  unsigned int col_;
};

#endif
