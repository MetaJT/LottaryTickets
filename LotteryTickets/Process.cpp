#include "Process.hpp"

void Process::totalTickets(const int newTickets){
    ticketsPurchased += newTickets;
}
void Process::addTicket(int ticket){
    currentTickets.push_back(ticket);
}
