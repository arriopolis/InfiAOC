#include <iostream>
#include <fstream>
#include <map>
#include <jsoncpp/json/json.h>
using namespace std;

int main (int argc, char * argv[]) {
  if (argc <= 1) {
    cout << "Please supply the input data file as a command line argument." << endl;
    return 0;
  }

  ifstream data_file(argv[1], fstream::binary);
  Json::Value data;
  data_file >> data;

  map<int,int> flats;
  for (auto f : data["flats"]) {
    flats[f[0].asInt()] = f[1].asInt();
  }

  auto pos = data["flats"][0];
  int x = pos[0].asInt(), y = pos[1].asInt();
  int ctr = 1;
  for (auto s : data["sprongen"]) {
    int dx = s[0].asInt() + 1, dy = s[1].asInt();
    if (flats.find(x + dx) == flats.end()) {
      cout << ctr << endl;
      return 0;
    }
    if (y + dy < flats[x + dx]) {
      cout << ctr << endl;
      return 0;
    }
    ctr++;
    x += dx;
    y = flats[x];
    // cout << x << ' ' << y << endl;
  }

  cout << 0 << endl;
  return 0;
}
