#include <iostream>
#include <fstream>
#include <queue>
#include <algorithm>
#include <iomanip>
#include "process.h"
#include "memblock.h"
using namespace std;

void printInputQueue(vector<process> q);
void printMMU(vector<memblock> &m, int pageSz);
void addUniqueCriticalPoint(vector<long> &points, long val);

int main()
{
    int freeMemory;
    int pageSize = 0;  // 0 == not set
    int numProcesses = 0;
    int initialNumProcesses = 0;
    ifstream inFile = ifstream("in1.txt");
    //ofstream outFile = ofstream("out.txt");
    queue<process> processQueue;  // add processes as they are read in from input file
    vector<process> inputQueue;  // processes in input queue (actually a vector) are processed front-to-back, removing from this "queue" and adding to MMU if MMU has enough space for that process
    vector<long> criticalPoints;  // keep track of points where we have to write to output
    vector<memblock> mmu;  // MMU, composed of vector of memblocks
    vector<process> processesInMemory;  // keep track of which processes are currently in memory map
    double totalTurnaroundTime = 0.0;

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
    initialNumProcesses = numProcesses;

    while(!inFile.eof() && numProcesses != 0)
    {
        int numMemoryPieces = 0;
        process p1;
        inFile >> p1.processNum;
        inFile >> p1.arrivalTime;
        inFile >> p1.burstTime;
        inFile >> numMemoryPieces;
        p1.memoryNeed = 0;
        for(int i = 0; i < numMemoryPieces; i++)
        {
            int memoryPiece;
            inFile >> memoryPiece;
            p1.memoryNeed += memoryPiece;
        }

        processQueue.push(p1);

        // add arrival time to criticalPoints if arrival time not already in criticalPoints
        addUniqueCriticalPoint(criticalPoints, p1.arrivalTime);

        /*
        cout << "processNum: " << p1.processNum << "\n";
        cout << "arrivalTime: " << p1.arrivalTime << "\n";
        cout << "burstTime: " << p1.burstTime << "\n";
        cout << "memoryNeed: " << p1.memoryNeed << "\n";
        */

        numProcesses--;
    }

    // go through criticalPoints and process arrival/completion events
    while(!criticalPoints.empty())
    {
        bool tLine = true;  // boolean to keep track of whether or not this is a "t = " line for formatting purposes (no \t on tLine)
        long currentCriticalPoint = criticalPoints.front();
        cout << "t = " << currentCriticalPoint << ": ";

        // move process from processQueue to inputQueue if criticalPoint matches arrival time, else it must be a completion time
        process currentProcess = processQueue.front();
        while(currentProcess.arrivalTime == currentCriticalPoint)
        {
            processQueue.pop();
            //inputQueue.push(currentProcess);
            inputQueue.push_back(currentProcess);
            if(tLine)
            {
                tLine = false;
            }
            else
            {
                cout << "\t";
            }
            cout << "Process " << currentProcess.processNum << " arrives" << endl;
            printInputQueue(inputQueue);
            currentProcess = processQueue.front();
        }

        // check if there is enough memory in MMU for any process in inputQueue
        for(auto i = inputQueue.begin(); i != inputQueue.end(); i++)
        {
            currentProcess = *i;
            if(freeMemory >= currentProcess.memoryNeed)
            {
                // move process from inputQueue to processesInMemory
                inputQueue.erase(i);
                i--;  // move iterator back after erasing front element...
                currentProcess.endTime = currentCriticalPoint + currentProcess.burstTime;
                processesInMemory.push_back(currentProcess);
                addUniqueCriticalPoint(criticalPoints, currentProcess.endTime);
                cout << "\tMM moves Process " << currentProcess.processNum << " to memory" << endl;

                // decrease currentProcess.memoryNeed until all needed pages are used
                int currentPage = 1;
                while(currentProcess.memoryNeed > 0)
                {
                    bool pageInserted = false;
                    currentProcess.memoryNeed -= pageSize;
                    freeMemory -= pageSize;
                    for(auto j = mmu.begin(); j != mmu.end(); j++)
                    {
                        if(j->processNum == 0 && !pageInserted)
                        {
                            j->processNum = currentProcess.processNum;
                            j->pageNum = currentPage;
                            currentPage++;
                            pageInserted = true;
                        }
                    }
                }
                printInputQueue(inputQueue);
                printMMU(mmu, pageSize);
            }
        }

        // check if any processes in MMU completed processing
        for(auto i = processesInMemory.begin(); i != processesInMemory.end(); i++)
        {
            currentProcess = *i;
            if(currentProcess.endTime == currentCriticalPoint)
            {
                for(auto j = mmu.begin(); j != mmu.end(); j++)
                {
                    //cout << "j->processNum:" << j->processNum << " ?= " << currentProcess.processNum << endl;
                    if(j->processNum == currentProcess.processNum)  // free up memory from MMU
                    {
                        j->processNum = 0;
                        j->pageNum = 0;
                        freeMemory += pageSize;
                    }
                }

                processesInMemory.erase(i);
                i--;
                totalTurnaroundTime += (currentProcess.endTime - currentProcess.arrivalTime);
                cout << "Process " << currentProcess.processNum << " completes" << endl;
                printMMU(mmu, pageSize);
            }
        }

        criticalPoints.erase(criticalPoints.begin());

        // keep criticalPoints sorted in ascending order
        sort(criticalPoints.begin(), criticalPoints.end());
        cout << endl;
    }  // end of checking criticalPoints

    /*
    for(auto i = criticalPoints.begin(); i != criticalPoints.end(); i++)
    {
        cout << *i << endl;
    }
    */

    cout << std::fixed;
    cout << std::setprecision(2);
    //cout << "Total Turnaround Time: " <<  totalTurnaroundTime << endl;
    cout << "\nAverage Turnaround Time: " <<  (totalTurnaroundTime / initialNumProcesses) << endl;

    cout << "Press ENTER to quit program.";
    cin.ignore().get();
    return 0;
}

void printInputQueue(vector<process> q)  // argument passed by value, so original inputQueue not changed
{
    cout << "\tInput Queue: [";
    while(!q.empty())
    {
        cout << q.front().processNum;
        q.erase(q.begin());
        if(!q.empty())
        {
            cout << " ";
        }
    }
    cout << "]" << endl;
}

void printMMU(vector<memblock> &m, int pageSz)
{
    /*
    m.at(1).processNum = 1;
    m.at(1).pageNum = 1;
    */
    bool inFreeFrames = false;
    cout << "\tMemory map:" << endl;
    for(int i = 0; i < m.size(); i++)
    {
        memblock currentMemBlock = m.at(i);
        if(currentMemBlock.processNum == 0)
        {
            if(!inFreeFrames)
            {
                cout << "\t\t" << (i*pageSz) << "-";
                inFreeFrames = true;
            }
            // else move on to the next memblock

            if(i == m.size() - 1) // reached last memblock
            {
                cout << (((i+1)*pageSz) - 1) << ": Free frame(s)" << endl;
            }
        }
        else
        {
            if(inFreeFrames) // close out previous free frame output
            {
                cout << ((i*pageSz) - 1) << ": Free frame(s)" << endl;
                inFreeFrames = false;
            }
            cout << "\t\t" << (i*pageSz) << "-" << (((i+1)*pageSz) - 1) << ": Process " << currentMemBlock.processNum << ", Page " << currentMemBlock.pageNum << endl;
        }
    }
}

void addUniqueCriticalPoint(vector<long> &points, long val)
{
    vector<long>::iterator checkDuplicate = find(points.begin(), points.end(), val);
    if(checkDuplicate == points.end())  // iterator == end means not found in criticalPoints
    {
        //cout << "new critical point found" << endl;
        points.push_back(val);
    }
}
