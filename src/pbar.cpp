#include <pbar.h>

ProgressBar::ProgressBar(string status) {
  _status = status;
  std::cout << _status << "\t0 %\t[";
  for(int i=0; i<MAX_BAR_LENGTH+1; ++i)
    std::cout << " ";
  std::cout << "]";
  std::cout.flush();
}

void ProgressBar::refresh(double percentage) {
  printf("\r%s\t%.1f %%\t[", _status.c_str(), percentage*100);
  std::cout << BLUE;
  for(int i=0; i<percentage * MAX_BAR_LENGTH; ++i)
    std::cout << "="; 
  if(percentage != 1)
    std::cout << ">" << COLOREND;	
  else
    std::cout << "=" << COLOREND << "]\t" << GREEN << "v" << COLOREND << endl;
  std::cout.flush();
}
