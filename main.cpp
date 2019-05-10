#include <iostream>
#include <fstream>
using namespace std;

int main()
{
  int memSize;
  int pageSize;
  ofstream outFile = ofstream("out.txt");

  cout << "Memory size>";
  cin >> memSize;
  cout << "\n";

  cout << "Page Size (1: 100, 2: 200, 3: 400)>";
  cin >> pageSize;

  outFile << memSize << " " << pageSize;
  return 0;
}
