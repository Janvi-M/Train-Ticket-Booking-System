#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_NODES 8
#define MAX_PASSWORD_LENGTH 50
#define PASSWORD_FILE "password.txt" //this should be replaced by absolute path of the file
#define GRAPH_FILE "graph.txt" //this should be replaced by absolute path of the file

int graph[MAX_NODES][MAX_NODES];
int main();

//function to read values from the external file into the adjacency matrix
void readGraphFromFile(int graph[MAX_NODES][MAX_NODES]) {
    FILE *file = fopen(GRAPH_FILE, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) {
            fscanf(file, "%d", &graph[i][j]);
        }
    }

    fclose(file);
}

// Function to write the updated matrix to the file
void writeGraphToFile() {
    FILE *file = fopen(GRAPH_FILE, "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) {
            fprintf(file, "%d ", graph[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

void printAdjacencyMatrix(int matrix[][8], int vertices) {
    printf("Adjacency Matrix:\n");

    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void printPath(char* cities[], int path[], int length) {
    printf("%s", cities[path[0]]);
    for (int i = 1; i < length; i++) {
        printf(" -> %s", cities[path[i]]);
    }
    printf("\n");
}

void printSolution(char* cities[], int path[], int length, int distance[], int time[], int price[], int l) {
    if (price[length - 1] > 0) {
        printf("Path %d: ", l);
        printPath(cities, path, length);
        printf("Distance: %d kms\nTime: %d hrs \nPrice: Rs. %d\n", distance[length - 1], time[length - 1], price[length - 1]);
        printf("\n");
    }
}

void dfs(int graph[MAX_NODES][MAX_NODES], int src, int dest, int visited[], int path[], int pathIndex,
         int distance[], int time[], int price[], char* cities[], int bill[][30], int* pathsCount) {
    visited[src] = 1;
    path[pathIndex] = src;
    pathIndex++;

    if (src == dest && distance[pathIndex - 1] != 0) {
        bill[0][*pathsCount] = distance[pathIndex - 1];
        bill[1][*pathsCount] = time[pathIndex - 1];
        bill[2][*pathsCount] = price[pathIndex - 1];
        printSolution(cities, path, pathIndex, distance, time, price, *pathsCount + 1);
        (*pathsCount)++;
    } else {
        for (int i = 0; i < MAX_NODES; i++) {
            if (!visited[i] && graph[src][i] != 0) {
                distance[i] = distance[src] + graph[src][i];
                time[i] = time[src] + 1;
                price[i] = price[src] + graph[src][i] + 50;
                dfs(graph, i, dest, visited, path, pathIndex, distance, time, price, cities, bill, pathsCount);
            }
        }
    }
    visited[src] = 0; // Backtrack
    pathIndex--;
}

int checkPassword(const char *enteredPassword) {
    FILE *file = fopen(PASSWORD_FILE, "r");
    if (file == NULL) {
        printf("Error: Unable to open password file.\n");
        exit(EXIT_FAILURE);
    }

    char correctPassword[MAX_PASSWORD_LENGTH];
    fscanf(file, "%s", correctPassword);
    fclose(file);

    return strcmp(enteredPassword, correctPassword) == 0;
}

void changePassword(const char *newPassword) {
    FILE *file = fopen(PASSWORD_FILE, "w");
    if (file == NULL) {
        printf("Error: Unable to open password file.\n");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%s", newPassword);
    fclose(file);

    printf("Password changed successfully.\n");
}

void AdminTerminal(){
    char enteredPassword[MAX_PASSWORD_LENGTH];
    char newPassword[MAX_PASSWORD_LENGTH];

    // Get the entered password from the user
    printf("Enter password: ");
    scanf("%s", enteredPassword);

    // Check if the entered password is correct
    if (checkPassword(enteredPassword)) {
        printf("Password correct.\n\nChoose: \n 1. Change password\n 2. Change matrix \n");
        int choice;
        scanf(" %d", &choice);

        switch(choice){
            case 1:
            //Updating password.txt 
            // Get the new password from the user
            printf("Enter new password: ");
            scanf("%s", newPassword);

            // Change the password and update the file
            changePassword(newPassword);

            break;
            case 2:
                // Edit matrix option
                printf("This is the current adjacency matrix: \n");
                printAdjacencyMatrix(graph,8 );

                printf("\n");
                printf("Enter the new matrix values (%dx%d):\n", MAX_NODES, MAX_NODES);
                for (int i = 0; i < MAX_NODES; i++) {
                    for (int j = 0; j < MAX_NODES; j++) {
                        scanf("%d", &graph[i][j]);
                    }
                }
                
                writeGraphToFile();  // Save the updated matrix to the file
                printf("Matrix updated successfully.\n");
                break;
            default:
            break;
        }    
    } 
    // if incorrect password is entered
    else {
        int ch;
        printf("Incorrect password! Access denied.\n Enter 1 to reenter password or 0 to restart.");
        scanf("%d",&ch);
        switch(ch){
            case 0:
            main();
            break;
            case 1:
            AdminTerminal();
            break;
        }
    }
}

void processPayment(int totalPrice) {
    printf("\nPayment Confirmation:\n");
    printf("Total Amount: Rs. %d\n", totalPrice);

    // payment processing
    printf("Processing payment... Please wait.\n");
    sleep(5);
    // Add any additional payment processing logic here

    // Payment success message
    printf("Payment successful! Your ticket is confirmed.\n");
}

void CustomerTerminal(){
    char name[20];
    char gen;
    int age;
    int bill[3][30] = {0}; // Initializing bill array to avoid garbage values
    int pathsCount = 0;

    printf("Enter name\n");
    scanf("%s", name);

    printf("Enter gender - M/F/O\n");
    scanf(" %c", &gen);

    if (gen != 'M' && gen != 'F' && gen != 'O' && gen != 'm' && gen != 'f' && gen != 'o') {
        printf("Invalid gender entered\n");
        exit(0);
    }

    printf("Enter age\n");
    scanf("%d", &age);
    printf("\n");

    char* cities[MAX_NODES] = {"Mumbai", "Chennai", "Bangalore", "Delhi", "Hyderabad", "Jaipur", "Shimla", "Ahmedabad"};

    
    readGraphFromFile(graph);    //integrating the external text file with the program

    int src;

    printf("START STATION\n");
    printf(" 1. Mumbai \n 2. Chennai \n 3. Bangalore \n 4. Delhi \n 5. Hyderabad \n 6. Jaipur \n 7. Shimla \n 8. Ahmedabad \n");
    scanf("%d", &src); // Source city
    src = src - 1;

    printf("\nDESTINATION\n");
    printf(" 1. Mumbai \n 2. Chennai \n 3. Bangalore \n 4. Delhi \n 5. Hyderabad \n 6. Jaipur \n 7. Shimla \n 8. Ahmedabad \n");
    int dest; // Destination city
    scanf("%d", &dest);
    dest = dest - 1;

    if (src == dest) {
        printf("No paths available\n");
        exit(0);
    }
    printf("\n\n");

    int visited[MAX_NODES] = {0};
    int path[MAX_NODES];
    int distance[MAX_NODES] = {0};
    int time[MAX_NODES] = {0};
    int price[MAX_NODES] = {0};

    printf("All Paths from %s to %s:\n", cities[src], cities[dest]);
    dfs(graph, src, dest, visited, path, 0, distance, time, price, cities, bill, &pathsCount);

    int selectedPath;
    printf("Enter path number\n");
    scanf("%d", &selectedPath);
    selectedPath--;

    if (bill[0][selectedPath] == 0 && bill[1][selectedPath] == 0 && bill[2][selectedPath] == 0) {
        printf("Wrong path\n");
        exit(0);
    }

    printf("Choose Option\n");
    int ch;
    
     printf("\t\tThe Following Trains "
           "Are Available.....\n");
    printf("\t\t1. Rajdhani Express.."
           ".......10:00 "
           "a.m........Sealdah Station\n");
    printf("\t\t2. Satabdi Express..."
           ".......05:00 "
           "p.m........Howrah Station\n");
    printf("\t\t3. Humsafar Express..."
           ".......11:00 "
           "p.m........Kolkata Chitpur"
           " Station\n");
    printf("\t\t4. Garib-Rath Express"
           ".........05:00 "
           "p.m........Sealdah Station\n");
    printf("\t\t5. Duronto Express..."
           ".........07:00 "
           "a.m.........Santraganchi"
           "Station\n");
           
           scanf("%d", &ch);

    int totalPrice=bill[2][selectedPath];
    printf("Total Amount to be paid: %d",totalPrice);
    printf("\nPayment Options:\n");
    printf("1. Credit Card\n");
    printf("2. Debit Card\n");
    printf("3. Net Banking\n");

    int paymentOption;
    printf("Select a payment option (1-3): ");
    scanf("%d", &paymentOption);

    switch (paymentOption) {
        case 1:
            printf("You selected Credit Card payment.\n");
            break;
        case 2:
            printf("You selected Debit Card payment.\n");
            break;
        case 3:
            printf("You selected Net Banking payment.\n");
            break;
        default:
            printf("Invalid payment option selected.\n");
            break;
    }

    // Process the payment
    processPayment(totalPrice);


    printf("\n");
    printf("-------Final Bill ------\n");
    printf("\nName: %s", name);
    printf("\nGen: %c", gen);
    printf("\nAge: %d\n", age);

    printf("Train booked from %s to %s\n", cities[src], cities[dest]);
    printf("Train Booked for-\n");
    printf("Distance: %d kms\nTime: %d hrs \nPrice: Rs. %d\n", bill[0][selectedPath], bill[1][selectedPath], bill[2][selectedPath]);

    switch (ch) {
        case 1:
            printf("Train Name: Rajdhani Express\nTime: 10:00 a.m\nStation: Sealdah Station\n");
            break;
        case 2:
            printf("Train Name: Satabdi Express\nTime: 05:00 p.m\nStation: Howrah Station\n");
            break;
        case 3:
            printf("Train Name: Humsafar Express\nTime: 11:00 p.m\nStation: Kolkata Chitpur Station\n");
            break;
        case 4:
            printf("Train Name: Garib-Rath Express\nTime: 05:00 p.m\nStation: Sealdah Station\n");
            break;
        case 5:
            printf("Train Name: Duronto Express\nTime: 07:00 a.m.\nStation: Santraganchi Station\n");
            break;
        default:
            printf("Wrong station entered");
    }

    printf("\nTHANK YOU.........");
}

int main() {

    int choice;
    printf("\n");
    printf("***********************************\n");
    printf("WELCOME!!\n");
    printf("***********************************\n");
    // printf("\n");
    printf("Choose 1 for admin and 2 for customer: ");
    scanf("%d",&choice);

    switch(choice){
        case 1:
            printf("***********************************\n");
            AdminTerminal();
            break;
        case 2:
            printf("***********************************\n");
            CustomerTerminal();
            break;
        default:
         printf("Wrong choice!");
         main();
         break;
    }

    return 0;
}