#ifndef _PROGRESS_BAR_H_
#define _PROGRESS_BAR_H_

#include <util.h>
#include <cstdio>
#include <iostream>

class ProgressBar {
public:
    ProgressBar(string status);
    void refresh(double percentage);
private:
    const static int MAX_BAR_LENGTH = 64;
    string _status;
};

#endif // _PROGRESS_BAR_H
