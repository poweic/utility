#ifndef _PROGRESS_BAR_H_
#define _PROGRESS_BAR_H_

#include <string>
#include <cstdio>
#include <iostream>

#include <color.h>

class ProgressBar {
public:
    ProgressBar(std::string status = "");
    void refresh(size_t i, size_t N);
    void refresh(size_t i, size_t N, std::string status);
    void refresh(double percentage);
    void refresh(double percentage, std::string status);
private:
    const static size_t MAX_BAR_LENGTH = 64;
    std::string _status;
};

#endif // _PROGRESS_BAR_H
