#include <iostream>
#include <fstream>
#include <queue>
#include "LLNode.h"
#include "Process.h"
using namespace std;

int main()
{
    int memSize;
    int pageSize = 0;  // 0 == not set
    int numProcesses = 0;
    bool inFreeFrames = false;
    ifstream inFile = ifstream("in1.txt");
    //ofstream outFile = ofstream("out.txt");
    // the 3 queues below can all be stored in Process struct
    /*queue <int> arrivalTimes;
    queue <int> burstTimes;
    queue <int> memoryNeeds;*/
    queue<Process> processQueue;  // add processes as they are read in from input file
    vector<Process> inputQueue;  // processes in input queue (actually a vector) are processed front-to-back, removing from this "queue" and adding to MMU if MMU has enough space for that process
    vector<long> criticalPoints;  // keep track of points where we have to write to output
    vector<Process> mmuInProgress;  // keep track of processes that are currently being processed by MMU
    LLNode* mmu = NULL;
    double avgTurnaroundTime = 0.0;

    mmu = new LLNode();

    cout << "Memory size>";
    cin >> memSize;
    cout << "\n";

    do
    {
        char pageIn;
        cout << "Page Size (1: 100, 2: 200, 3: 400)>";
        cin >> pageIn;
        if(pageIn == '1')
        {
            pageSize = 100;
        }
        else if(pageIn == '2')
        {
            pageSize = 200;
        }
        else if(pageIn == '3')
        {
            pageSize = 400;
        }
        else
        {
            pageSize = 0;
            cout << "ERROR: Please enter a valid value (1, 2, or 3).\n";
        }
    } while (pageSize == 0);

    //cout << memSize << " " << pageSize << "\n";

    inFile >> numProcesses;

    while(!inFile.eof())
    {
        int numMemoryPieces = 0;
        Process p1;
        inFile >> p1.processNum;
        inFile >> p1.arrivalTime;
        inFile >> p1.burstTime;
        p1.memoryNeed = 0;  // note: only one struct is actually being created, it seems...
        inFile >> numMemoryPieces;
        for(int i = 0; i < numMemoryPieces; i++)
        {
            int memoryPiece;
            inFile >> memoryPiece;
            p1.memoryNeed += memoryPiece;
        }
        cout << "processNum: " << p1.processNum << "\n";
        cout << "arrivalTime: " << p1.arrivalTime << "\n";
        cout << "burstTime: " << p1.burstTime << "\n";
        cout << "memoryNeed: " << p1.memoryNeed << "\n";
    }

    cout << "Press ENTER to quit program.";
    cin.ignore().get();
    return 0;
}
