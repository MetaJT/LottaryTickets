// Wichita State University CS540 HW2 Lottery System
// Jordan Trotter D362Y854
// Git: https://github.com/MetaJT/LottaryTickets.git

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <unordered_set>
#include <string>
#include "Process.hpp"
#include "Scheduler.hpp"

#define numPlayers 3

int main() {
    Scheduler scheduler; 

    for (int i = 0; i < numPlayers; i++){
        Process process(i);
        scheduler.addProcess(process);
    }
    
    srand ((unsigned int)time(0));
    scheduler.start();

    return 0;
}
