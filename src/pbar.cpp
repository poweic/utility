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
  printf("\r%s\t%4.1f %%\t[", status.c_str(), percentage*100);
  cout << BLUE;

  size_t L = percentage * MAX_BAR_LENGTH;

  for(size_t i=0; i<L; ++i)
    cout << "="; 

  if (percentage < 1)
    cout << ">" << COLOREND;
  else
    cout << "=" << COLOREND;

  for (size_t i=L+1; i<MAX_BAR_LENGTH; ++i)
    cout << " ";

  cout << "]\t";

  if (percentage >= 1)
    cout << GREEN << "v" << COLOREND << endl;
    
  cout.flush();
}
