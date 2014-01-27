#include <profile.h>

extern "C" void libprofile_is_present(void) {}

void Profile::tic() {
  time(&_time); 
}

double Profile::toc() {
  time_t t_end;
  time(&t_end);

  double diff = difftime(t_end, _time);
  printf("\n[Done] Elapsed time: %s %f %s sec\n", GREEN, diff , COLOREND);
  return diff;
}
