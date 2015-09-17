#include <cstdlib>
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char *argv[]) {
  int port_number = -1;
  string ip_address;

  for (int i = 0; i < argc; ++i) {
    if (strcmp(argv[i], "-port") == 0 && i + 1 < argc) {
      port_number = atoi(argv[i+1]);
    } else if (strcmp(argv[i], "-ip") == 0 && i + 1 < argc) {
      ip_address = argv[i+1];
    }
  }

  if (port_number == -1 || ip_address.empty()) {
    cout << "The following argument format must be used: `./client -port "
         << "<portnumber>` -ip <ipaddress>" << endl;
    return EXIT_FAILURE;
  }

  cout << "Connecting to " << ip_address << " on port " << port_number << endl;
  return EXIT_SUCCESS;
}
