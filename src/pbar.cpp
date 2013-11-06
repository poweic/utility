#include <pbar.h>
using namespace std;

extern "C" void libpbar_is_present(void) {}

ProgressBar::ProgressBar(string status) {
  _status = status;
  this->refresh(0);
}

void ProgressBar::refresh(double percentage) {
  this->refresh(percentage, _status);
}

void ProgressBar::refresh(size_t i, size_t N) {
  this->refresh((double) (i + 1) / N, _status);
}

void ProgressBar::refresh(size_t i, size_t N, string status) {
  this->refresh((double) (i + 1) / N, status);
}

void ProgressBar::refresh(double percentage, string status) {
  printf("\r%s\t%.1f %%\t[", status.c_str(), percentage*100);
  cout << BLUE;

  for(int i=0; i<percentage * MAX_BAR_LENGTH; ++i)
    cout << "="; 

  if(percentage != 1)
    cout << ">" << COLOREND;	
  else
    cout << "=" << COLOREND << "]\t" << GREEN << "v" << COLOREND << endl;

  cout.flush();
}
