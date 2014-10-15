qSim, Andrew Han
qSim simulates customer and teller actions at a bank. It takes as arguments:
customers tellers simulationTime averageServiceTime seed(optional)
These arguments represent the number of customers, number of tellers, length of the simulation in minutes, the average service time by tellers in minutes, and the seed for the random number generator.

Test cases:
Arguments:
100 4 60 2.3 257

One common line was used.
Customers served: 100
Time to serve all customers: 63.75 minutes
Number of tellers: 4
Average time spent in bank: 5.118 minutes
Standard deviation: 1.71 minutes
Maximum line wait time: 5.796 minutes
Total teller service time: 221.2 minutes
Total teller idle time: 26.47 minutes
A line for each teller was used.
Customers served: 100
Time to serve all customers: 66.66 minutes
Number of tellers: 4
Average time spent in bank: 5.583 minutes
Standard deviation: 2.643 minutes
Maximum line wait time: 9.145 minutes
Total teller service time: 244.5 minutes
Total teller idle time: 20.06 minutes

Arguments:
50 10 120 1 23

One common line was used.
Customers served: 50
Time to serve all customers: 121.2 minutes
Number of tellers: 10
Average time spent in bank: 1.225 minutes
Standard deviation: 0.5572 minutes
Maximum line wait time: 1.648 minutes
Total teller service time: 53.56 minutes
Total teller idle time: 1152 minutes
A line for each teller was used.
Customers served: 50
Time to serve all customers: 121.3 minutes
Number of tellers: 10
Average time spent in bank: 1.11 minutes
Standard deviation: 0.5167 minutes
Maximum line wait time: 0.7233 minutes
Total teller service time: 48.12 minutes
Total teller idle time: 1158 minutes

Arguments:
200 3 60 5 91

One common line was used.
Customers served: 200
Time to serve all customers: 353.8 minutes
Number of tellers: 3
Average time spent in bank: 157.1 minutes
Standard deviation: 80.92 minutes
Maximum line wait time: 286.3 minutes
Total teller service time: 1037 minutes
Total teller idle time: 18.13 minutes
A line for each teller was used.
Customers served: 200
Time to serve all customers: 370.1 minutes
Number of tellers: 3
Average time spent in bank: 158.9 minutes
Standard deviation: 85.53 minutes
Maximum line wait time: 304.2 minutes
Total teller service time: 1075 minutes
Total teller idle time: 21.56 minutes

From these test cases I notice that in terms of the time spent in the bank, having multiple lines or just one doesn't make much of a difference. This is likely due to the fact that we didn't account for how sluggish some customers might be moving from a central line to individual teller windows.

My program consists of 3 .cpp files and 2 .h files.

qSim.cpp contains the main() function and does the work.

Event.cpp/Event.h declare my Event class and contain all the information about my Event class, which manages the linkedlists that are used for the queue and the lines.

Stats.cpp/Stats.h declare my Stats class and contain the information about the class, which manages all the statistics I need to store as the sim is running.