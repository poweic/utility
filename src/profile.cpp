#include <profile.h>

void Profile::tic() {
  time(&_time); 
}

void Profile::toc() {
  time_t t_end;
  time(&t_end);
  printf("\n[Done] Elapsed time: %s %f %s sec\n", GREEN, difftime(t_end, _time), COLOREND);
}
