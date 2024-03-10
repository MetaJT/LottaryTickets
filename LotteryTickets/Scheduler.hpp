#ifndef Scheduler_hpp
#define Scheduler_hpp

#include <iostream>
#include <vector>
#include <pthread.h>
#include <unordered_set>
#include <string>
#include "Process.hpp"

class Scheduler {
public:
    Scheduler();
    ~Scheduler();
    void start();
    void purchaseTickets(std::vector<Process>& processes);
    void createThreads();
    inline void addProcess(const Process& process) { processes.push_back(process); }
    inline int getTotalTickets() { return int(loserTickets.size()); }
    std::pair<bool,int> checkTickets(std::vector<Process>& processes);
    pthread_mutex_t winningTicket_mutex;
    
private:
    std::vector<Process> processes;
    std::unordered_set<int> loserTickets;
    int winningTicket;
    int currentTicket;
    int winningPID;
    static void* checkProcessTickets(void* arg);
    int numBalls;
    int *generateTicket(int n);
};
#endif /* Scheduler_hpp */
