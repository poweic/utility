#ifndef _PROGRESS_BAR_H_
#define _PROGRESS_BAR_H_

#include <util.h>
#include <cstdio>
#include <iostream>

class ProgressBar {
public:
    ProgressBar(string status = "");
    void refresh(size_t i, size_t N);
    void refresh(size_t i, size_t N, string status);
    void refresh(double percentage);
    void refresh(double percentage, string status);
private:
    const static int MAX_BAR_LENGTH = 64;
    string _status;
};

#endif // _PROGRESS_BAR_H
