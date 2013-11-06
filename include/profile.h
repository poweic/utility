#ifndef _PROFILE_H_
#define _PROFILE_H_

#include <ctime>
#include <cstdio>
#include <color.h>

class Profile {
public:
    void tic();
    double toc();
private:
    time_t _time;
};

#endif // _PROFILE_H_
