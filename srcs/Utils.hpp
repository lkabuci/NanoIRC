#pragma once

#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <string>
#include <vector>

class Utils {
  public:
    static std::string join(const std::vector<std::string>& arr);
    static std::string toStr(int nbr);

  private:
    Utils();
};

#endif
