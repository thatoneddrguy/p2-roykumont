// process defintion
struct process
{
    int processNum;
    int arrivalTime;
    int endTime;  // not initially known; depends on when MMU begins processing this process (not always arrivalTime!)
    int burstTime;
    int memoryNeed;
};
