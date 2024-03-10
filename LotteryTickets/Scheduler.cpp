#include "Scheduler.hpp"

Scheduler::Scheduler() : winningTicket(0), winningPID(-1), numBalls(4){
    std::string s = "";
    srand((unsigned int)time(0));
    for (int i = 0; i < numBalls; i++){
        // Selects n random numbers and appends each to string
        // Doing this so I don't get any zeros
        s += std::to_string(rand() % 9 + 1);
    }
    winningTicket = std::stoi(s);
    printf("Winning Ticket: %d\n", winningTicket);

    // Initialize mutex
    pthread_mutex_init(&winningTicket_mutex, NULL);
}
Scheduler::~Scheduler() {
    // Destroy mutex
    pthread_mutex_destroy(&winningTicket_mutex);
}

void Scheduler::start(){ // Starting function
    bool foundWinner = false;
    std::pair<bool,int> win;
    while (!foundWinner){ // Will run until the winning ticket is found
        purchaseTickets(processes);
        win = checkTickets(processes);
        if (win.first) {
            foundWinner = true;
            break;
        }
    }
    printf("\nWinner! Process %d WINS after purchasing %d tickets!\n", win.second, processes[win.second].getTickets());
    for (int i = 0; i < processes.size(); i++){
        if (win.second == processes[i].getPID()) continue;
        printf("Process %d purchased %d tickets total\n", processes[i].getPID() ,processes[i].getTickets());
    }
    printf("\nTotal tickets purchased: %d\n\n", (int)loserTickets.size());
}
// Struct to hold a process and if its a winner or not
struct CheckResult {
    Process* process;
    bool isWinner;  // Use bool for simplicity (can use atomic if needed)
    pthread_mutex_t mtx;                // Mutex for process access within the thread
    CheckResult() : process(nullptr), isWinner(false) {
      pthread_mutex_init(&mtx, NULL);  // Initialize mutex
    }
    ~CheckResult() {
      pthread_mutex_destroy(&mtx);  // Destroy mutex on object destruction
    }
};

// Struct that is used to pass through to our thread function
struct arg_struct {
    CheckResult check;
    int wTicket, wPID;
    pthread_mutex_t mutex;
    arg_struct() : wPID(-1){
        pthread_mutex_init(&mutex, NULL);
    }
    ~arg_struct(){
        pthread_mutex_destroy(&mutex);
    }
};

// Function to generate new tickets
int* Scheduler::generateTicket(int n){
    int* ticket = new int;
    std::string s = "";
    for (int i = 0; i < n; i++)
        s += std::to_string(rand() % 9 + 1); // Using a string to hold values 1-9 with no zero possible
    *ticket = (int)std::stoi(s);
    return ticket;
}

// Thread function
void* Scheduler::checkProcessTickets(void* arg){
    arg_struct *args = (arg_struct*) arg;
    pthread_mutex_lock(&args->mutex); // Lock the mutex

    std::vector<int> currTickets = args->check.process->getCurrentTickets();

    for (int i = 0; i < currTickets.size(); i++){ // Check all the tickets for the current process
        int ticket = currTickets[i];
        
        if (ticket == args->wTicket) { // The winning ticket is found!
            args->check.isWinner = true;
            args->wPID = args->check.process->getPID();
            pthread_mutex_unlock(&args->mutex); // Unlock mutex and exit
            pthread_exit(NULL);
        }
    }
    args->check.process->clearCurrentTickets(); // Clear the current tickets vector for next time
    pthread_mutex_unlock(&args->mutex); // Unlock mutex and exit
    return nullptr;
};

std::pair<bool, int> Scheduler::checkTickets(std::vector<Process>& processes) {
    // Use thread pool (thread count equal to number of processes)
    pthread_t checks [(int)processes.size()];
    
    // Create a vector to hold results from checkers
    std::vector<CheckResult> results(processes.size());
    for (int i = 0; i < processes.size(); i++) {
        results[i].process = &processes[i];
    }
    // Launch threads with processes to check
    for (int i = 0; i < (int)processes.size(); i++) {
        arg_struct* res = new arg_struct;
        res->check = results[i];
        res->wTicket = winningTicket;
        pthread_create(&checks[i], NULL, &Scheduler::checkProcessTickets, (void*) *&res);
        
        int status = pthread_join(checks[i], NULL); // join threads
        if (status == 0 && res->check.isWinner){ // When a we find a process with the winning ticket finish program
            int wPid = res->wPID;
            delete res; // Delete the allocated memory
            return {true, wPid};
        }
        delete res; // Delete the allocated memory
    }
    return {false, NULL};
}
void Scheduler::purchaseTickets(std::vector<Process>& processes){
    // Takes in the current process along with their total number of tickets
    for (int i = 0; i < processes.size(); i++){
        int ticketsBought = rand() % 4 + 1; // Player buys 1 - 4 tickets at a time
        processes[i].setPurchased(ticketsBought);
    }
    // creates threads that represent the tickets
    int* currTicket;
    for (int i = 0; i < processes.size(); i++) {
        for (int j = 0; j < processes[i].getPurchased(); j++){
            
            currTicket = generateTicket(numBalls); // num of balls to create
            if (loserTickets.contains(*currTicket)) {
                j--; // If the picked number has already been chosen choose again.
            }
            else {
                printf("Process %d buying %d ticket(s) ->", processes[i].getPID(), processes[i].getPurchased() - j );
                printf(" Picked: %d\n", *currTicket);
                processes[i].totalTickets(1); // Increase the total number of tickets the player has bought
                processes[i].addTicket(*currTicket); // Add the chosen ticket to the current vector of tickets
                loserTickets.insert(*currTicket); // Add this ticket to the set of chosen tickets
            }
            delete currTicket; // Delete the allocated memory
            if (loserTickets.size() == pow(9, numBalls)){ // all possible guesses have been made exit
                break;
            }
        }
    }
}
