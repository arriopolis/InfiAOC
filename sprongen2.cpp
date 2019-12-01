#include <iostream>
#include <fstream>
#include <queue>
#include <map>
#include <jsoncpp/json/json.h>
#include <algorithm>
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

  // for (auto f : flats) {
  //  cout << f.first << ' ' << f.second << endl;
  // }
  // cout << "----" << endl;

  map<int,pair<int,int>> energie;
  priority_queue< pair<int,pair<int,int>>, vector<pair<int,pair<int,int>>>, greater<pair<int,pair<int,int>>> > q;
  q.push({0,{data["flats"][0][0].asInt(),-1}});
  while (!q.empty()) {
    auto el = q.top();
    q.pop();
    int e = el.first;
    int x = el.second.first;
    int prevx = el.second.second;
    // cout << e << ' ' << x << ' ' << prevx << endl;
    if (energie.find(x) == energie.end())
    {
      energie[x] = {e,prevx};
      int y = flats[x];
      for (int dx = 1; dx < 6; dx++) {
        if (flats.find(x + dx) != flats.end() && energie.find(x + dx) == energie.end()) {
          int dy = max(flats[x + dx] - y, 0);
          if (dx-1 + dy <= 4) {
            // cout << "Adding to queue: " << e + dx-1 + dy << ' ' << x + dx << ' ' << x << endl;
            q.push({e + dx-1 + dy, {x + dx, x}});
          }
        }
      }
    }
  }
  // cout << "----" << endl;

  // for (auto el : energie) {
  //   cout << el.first << " : " << el.second.first << " , " << el.second.second << endl;
  // }
  // cout << "----" << endl;

  int curx = data["flats"][data["flats"].size()-1][0].asInt();
  vector<pair<int,int>> sprongen;
  while (curx != data["flats"][0][0].asInt()) {
    int x = energie[curx].second;
    int dx = curx - x - 1;
    int dy = max(flats[curx] - flats[x], 0);
    sprongen.push_back({dx,dy});
    curx = x;
  }

  reverse(sprongen.begin(), sprongen.end());

  Json::Value json_sprongen(Json::arrayValue);
  int totale_energie = 0;
  for (auto s : sprongen) {
    int dx = s.first, dy = s.second;
    totale_energie += dx + dy;
    Json::Value json_sprong(Json::arrayValue);
    json_sprong.append(dx);
    json_sprong.append(dy);
    json_sprongen.append(json_sprong);
    // cout << dx << ' ' << dy << endl;
  }

  cout << json_sprongen << endl;
  cout << "Totale energie: " << totale_energie << endl;
}
