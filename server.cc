#include <cstdlib>
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char *argv[]) {
  int port_number = -1;
  for (int i = 0; i < argc; ++i) {
    if (strcmp(argv[i], "-port") == 0 && i + 1 < argc) {
      port_number = atoi(argv[i+1]);
    }
  }

  if (port_number == -1) {
    cout << "The following argument format must be used: `./server -port "
         << "<portnumber>`" << endl;
    return EXIT_FAILURE;
  }

  cout << "Listening for requests at port " << port_number << endl;
  return EXIT_SUCCESS;
}
