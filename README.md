# Train-Ticket-Booking-System

This project is a train booking system implemented in C, featuring admin and customer functionalities with interactions through external files for passwords and train route data.

Key Components
External Files:

password.txt: Stores the admin password.
graph.txt: Contains the adjacency matrix for train routes.
Data Structures:

graph[MAX_NODES][MAX_NODES]: Adjacency matrix for train routes.
char* cities[MAX_NODES]: Array of city names.
Functionality
File Operations:

Reading and writing the adjacency matrix from/to graph.txt.
Admin Terminal:

Password Check: Validates the admin password.
Password Change: Allows the admin to change the password.
Matrix Update: Enables the admin to update the train routes adjacency matrix.
Customer Terminal:

User Input: Collects customer details.
Pathfinding: Uses DFS to find and display all paths from the source to the destination city.
Ticket Booking: Allows customers to select a path, view train options, and make a payment.
Payment Processing: Simulates payment and confirms the booking.
Utility Functions:

Printing the adjacency matrix and paths.
Simulating and confirming payment.
