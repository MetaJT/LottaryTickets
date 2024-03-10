#ifndef Process_hpp
#define Process_hpp

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <unordered_set>
#include <string>

class Process {
public:
    Process() : pID(0), ticketsPurchased(0) {}
    inline Process(int pID){ this->pID = pID; }
    void totalTickets( int newTickets ); // Stores the total tickets purchased for a process
    inline int getPID() { return pID; } // Grabs the Process ID
    inline int getTickets() { return ticketsPurchased; } // How many tickets the process purchased at that time
    void addTicket(int ticket); // Add ticket to the current tickets array
    inline std::vector<int> getCurrentTickets() { return currentTickets; } // Grab the current tickets vector
    inline void clearCurrentTickets() { currentTickets.clear(); } // Clear the current tickets vector
    inline void setPurchased(int p){ purchased = p; } // Set the current tickets purchased amount
    inline int getPurchased(){ return purchased; } 
    
private:
    int pID, ticketsPurchased, purchased;
    std::vector<int> currentTickets;
};
#endif /* Process_hpp */
