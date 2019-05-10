#include <iostream>
#include <fstream>
#include <queue>
#include "LLNode.h"
using namespace std;

int main()
{
  int memSize;
  int pageSize;
  ifstream inFile = ifstream("in1.txt");
  ofstream outFile = ofstream("out.txt");
  queue <int> arrivalTimes;
  queue <int> burstTimes;
  queue <int> memoryNeeds;
  queue <int> processQueue;
  LLNode* memoryMapHead = NULL;
  double avgTurnaroundTime = 0.0;

  memoryMapHead = new LLNode();

  cout << "Memory size>";
  cin >> memSize;
  cout << "\n";

  cout << "Page Size (1: 100, 2: 200, 3: 400)>";
  cin >> pageSize;

  outFile << memSize << " " << pageSize;
  return 0;
}
