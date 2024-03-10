# LottaryTickets
_## HW for CS540 to demonstrate threads, processes, and scheduling_

_This program demonstrates how threads, processes, and schedulers can be applied in a lottery system._

The `main.cpp` is used to initialize the number of players and start the scheduler.

The `Process Class` is used to represent a "player" and each player has the following attributes:
- A process ID
- A vector that contains the tickets that the player has bought at the moment
- The total number of tickets the player bought overall.

The `Scheduler Class` is used to either have the process buy tickets or check the tickets for a ticket that matches the winning ticket.
Contains the following attributes:
- A vector to hold the all the players that are participating in the lottery.
- A set that contains all of the tickets that been previously picked.
- The winning ticket that the players are trying to find.
- The number of balls (or digits) that are in the winning ticket
- Then we have a helper functions that generate tickets and check tickets.

**Process:**
1. Each person buys a random number of tickets and for each ticket they've purchased, they pick their 'winning' ticket.
2. Once all of the players have picked their numbers they go to get them checked.
3. The checker function takes each player individually and runs each ticket as a thread and compares them to the winning ticket.
  - If the ticket matches exits thread and exits function
  - Else returns false and sends the player to go purchase new tickets
4. Once a player finds the winning ticket the while loop ends and it displays the winner, along with the other processes total number of tickets.
