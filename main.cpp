#include <iostream>
#include <fstream>
#include <queue>
#include <algorithm>
#include "process.h"
#include "memblock.h"
using namespace std;

int main()
{
    int freeMemory;
    int pageSize = 0;  // 0 == not set
    int numProcesses = 0;
    //bool inFreeFrames = false;
    ifstream inFile = ifstream("in1.txt");
    //ofstream outFile = ofstream("out.txt");
    queue<process> processQueue;  // add processes as they are read in from input file
    queue<process> inputQueue;  // processes in input queue (actually a vector) are processed front-to-back, removing from this "queue" and adding to MMU if MMU has enough space for that process
    vector<long> criticalPoints;  // keep track of points where we have to write to output
    vector<memblock> mmu;  // MMU, composed of vector of memblocks
    double avgTurnaroundTime = 0.0;

    cout << "Memory size>";
    cin >> freeMemory;
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

    //cout << freeMemory << " " << pageSize << "\n";

    // at this point we have pageSize and MMU memory size, initialize MMU
    for(int i = 0; i < (freeMemory/pageSize); i++)
    {
        memblock m1;
        m1.processNum = 0;
        m1.pageNum = 0;
        mmu.push_back(m1);
    }

    /*
    for(auto i = mmu.begin(); i != mmu.end(); i++)
    {
        cout << i->processNum;
    }
    */

    inFile >> numProcesses;

    while(!inFile.eof() && numProcesses != 0)
    {
        int numMemoryPieces = 0;
        process p1;
        inFile >> p1.processNum;
        inFile >> p1.arrivalTime;
        inFile >> p1.burstTime;
        inFile >> numMemoryPieces;
        for(int i = 0; i < numMemoryPieces; i++)
        {
            int memoryPiece;
            inFile >> memoryPiece;
            p1.memoryNeed += memoryPiece;
        }

        processQueue.push(p1);

        /*
        cout << "processNum: " << p1.processNum << "\n";
        cout << "arrivalTime: " << p1.arrivalTime << "\n";
        cout << "burstTime: " << p1.burstTime << "\n";
        cout << "memoryNeed: " << p1.memoryNeed << "\n";
        */

        numProcesses--;
    }

    // processQueue contains process data from input file at this point, being processing by placing processes in inputQueue
    while(!processQueue.empty())
    {
        // move process from processQueue to inputQueue
        process currentProcess = processQueue.front();
        processQueue.pop();
        inputQueue.push(currentProcess);

        // add arrival time to criticalPoints if arrival time not already in criticalPoints
        vector<long>::iterator checkDuplicate = find(criticalPoints.begin(), criticalPoints.end(), currentProcess.arrivalTime);
        if(checkDuplicate == criticalPoints.end())  // iterator == end means not found in criticalPoints
        {
            //cout << "new critical point found" << endl;
            criticalPoints.push_back(currentProcess.arrivalTime);
        }

        //cout << "t = " << currentProcess.arrivalTime << " ";
    }

    for(auto i = criticalPoints.begin(); i != criticalPoints.end(); i++)
    {
        cout << *i << endl;
    }

    cout << "Press ENTER to quit program.";
    cin.ignore().get();
    return 0;
}
