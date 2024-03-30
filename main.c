#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

struct PASSENGR { //creating the passenger structure to use it later.
    int ID;
    char travelling_date[9];
    char travelling_time[6];
    char from[35];
    char to[35];
    int matched; //to divide the passenger in the initial load array to matched and unmatched and putting them into unmatched linked list, or a bus passenger linked list if matched.
    struct PASSENGR *next;
};

typedef struct PASSENGR *Linked_List; //this will be used as a type to initial all linked list used.
typedef struct PASSENGR *ptr; //will be used to make nodes, or temporary nodes.
typedef ptr pos; //same as ptr

struct BUS { //creating the bus structure to use later, does not have next as i will store them in an array.
    int Bus_num;
    char dep_date[9];
    char dep_time[6];
    char from[35];
    char to[35];
    double price;
    int capacity;
    int emptySeats; //as to see if the bus's capacity still can get more passengers to add to the linked list of passengers. if not empty it will search for another bus
    Linked_List PassengersList; //the linked list of passenger matching the specific bus initialized.
};

typedef struct BUS *busptr; //will be used to make bus nodes, and the array of buses.
Linked_List unmatched; //a linked list to put all unmatched passengers.

void menu();
char *trimwhitespace(char *str);
int countlines(FILE *filename);
void loadBus(busptr[]);
void loadPassenger(ptr[]);
void printBus(busptr);
void printPassengerList(Linked_List);
busptr createBusNode(int num, char date[9], char time[6], char from[35], char to[35], double price, int capacity);
int isEmpty(Linked_List l);
int getSize(Linked_List l);
ptr newNode(int id, char date[9], char time[6], char from[35], char to[35]);
void insert(Linked_List l, ptr newnode);
void printPassengerUnmatchedList(Linked_List l);
void deleteNode(Linked_List L, ptr node);
ptr findPrevious(Linked_List l, ptr node);
void assignPassengers(busptr[], int, ptr[], int);
void deleteBus(busptr bus[], int *busSize);
ptr search(Linked_List l, int id);
void printSpecificBus(busptr bus[], int busSize);
void addPassenger(busptr bus[], int busSize);
void insert2LL(Linked_List a, Linked_List b);
void deletePassenger(busptr bus[], int busSize);

int main(){

    //--------------------------------------------
    FILE *bus_lines; //this will be used to get it the buses file
    bus_lines = fopen("busses.txt", "r");
     if(bus_lines == NULL){ //if file not found
        printf("file busses.txt cannot be found");
        exit(1);
    }
    int numOfLines_b = countlines(bus_lines); //if found count the lines of it (as number of buses).
    busptr bus_arr[numOfLines_b]; //initialing the array of buses
    //---------------------------------------------

    FILE *pas_lines; //this will be used to open the passengers file
    pas_lines = fopen("passengers.txt", "r");
     if(pas_lines == NULL){ //if not found
        printf("file passengers.txt cannot be found");
        exit(1);
    }
    int numOfLines_p = countlines(pas_lines); //if found, count the lines (as numbers of passengers)
    ptr pasArr[numOfLines_p]; //initialing the initial array,used to only store passengers initially before putting them into their right linked lists. for loading purpose only.
    //----------------------------------------------
    unmatched = (Linked_List)malloc(sizeof(struct PASSENGR)); //initiazing the linked list and making sure it has space to be made.
    if(unmatched==NULL) //if there is no memory spasce
    {
        printf("could not initiate list. Out of memory\n");
        exit (1);
    }
    unmatched->next = NULL; //if there is memory space make the next null, so linked list can be used in the right way (first node always empty)
    //------------------------------------------------
    printf("\t\t\t\t    Welcome to my program\n");
    printf("\t   This program reads Busses and Passengers' info and analyze them. Check it out!\n\n");
    int selection = 0; //selection to know which thing from the menu it wants to do
    menu(); //printing menu on the screen for the user
    scanf("%d", &selection); //reading the selection

    while(selection != 9){ //while the user didn't choose exit
        switch(selection){
        case 1: //if first choice
            loadBus(bus_arr); //load the buses info into array
            for(int i=0; i < numOfLines_b; i++){ //initialize all the linked lists of passengers in the buses.
                bus_arr[i]->PassengersList = (Linked_List)malloc(sizeof(struct PASSENGR));
                if(bus_arr[i]->PassengersList==NULL){
                    printf("could not initiate list. Out of memory\n");
                    exit (1);
                }
                bus_arr[i]->PassengersList->next = NULL;
            }
            printf("Loding is Complete!\n\n");
            break;
        case 2: //if second choice
            loadPassenger(pasArr); //load all passengers info from file
            printf("Loding is Complete!\n\n");
            break;
        case 3: // if choice 3
            assignPassengers(bus_arr, numOfLines_b, pasArr, numOfLines_p); //get all passengers to the right buses or put them into unmatched
            break;
        case 4: //if choice 4
            printSpecificBus(bus_arr, numOfLines_b); //printing only one bus's info
            break;
        case 5:
            printPassengerUnmatchedList(unmatched); //printing all the unmatched passengers
            break;
        case 6:
            addPassenger(bus_arr, numOfLines_b); //adding new passenger and assigning it to the right bus or unmatched
            break;
        case 7:
            deletePassenger(bus_arr, numOfLines_b); //delete a passenger from buses or unmatched
            break;
        case 8:
            deleteBus(bus_arr, &numOfLines_b); //deleting a bus and putting it's passengers into the unmatched
            break;
        }
        menu(); //putting the menu again on screen
        scanf("%d", &selection); //reading the selection for the while loop
    }
    return 0;
}

void menu(){ //this function is used to print the menu.
    printf("--------------------------------------------Menu--------------------------------------------------------\n");
    printf("\t.....................................................................................\n");
    printf("\t:\t1. Load the bus information file.\t\t\t\t\t    :\n");
    printf("\t:\t2. Load the passenger information file.\t\t\t\t\t    :\n");
    printf("\t:\t3. Assign passengers and print assignment information of all busses.\t    :\n");
    printf("\t:\t4. Print a specific bus information along with its passengers information.  :\n");
    printf("\t:\t5. Print unmatched passengers\t\t\t\t\t\t    :\n");
    printf("\t:\t6. Add new passenger.\t\t\t\t\t\t\t    :\n");
    printf("\t:\t7. Delete passenger.\t\t\t\t\t\t\t    :\n");
    printf("\t:\t8. Delete bus number.\t\t\t\t\t\t\t    :\n");
    printf("\t:\t9. Exit.\t\t\t\t\t\t\t\t    :\n");
    printf("\t.....................................................................................\n\n");
    printf("What is your Choice?\n");
}

char *trimwhitespace(char *str){ //this function used to trim all spaces from the words to that strcmp work properly
    char *end;

    // Trim leading space
    while(isspace((unsigned char)*str)) str++;

    if(*str == 0)  // All spaces?
        return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;

    // Write new null terminator character
    end[1] = '\0';

    return str;
}

int countlines(FILE *filename){
    // count the number of lines in the file called filename
    int ch=0;
    int lines=0;

    if (filename == NULL){
        return 0;
    }

    while ((ch = fgetc(filename)) != EOF){ //read till the end of file
        if (ch == '\n') //if newline lines++
            lines++;
    }
    fclose(filename);
    return lines;
}

busptr createBusNode(int num, char date[9], char time[6], char from[35], char to[35], double price, int capacity){
    //this function is used to create a new bus node
    busptr b = (busptr) malloc(sizeof (struct BUS)); //initializing space and the node
    b->Bus_num = num; //putting all needed info in it
    strcpy(b->dep_date, date);
    strcpy(b->dep_time, time);
    strcpy(b->from, from);
    strcpy(b->to, to);
    b->price = price;
    b->capacity = capacity;
    b->emptySeats = capacity;
    return b;
}

void printBus(busptr bus){ //this fuction is to print one bus's info and it's passengers list
    printf("BUS INFO ==>\n\n");
    printf("Bus Number       |  %d\nDeparture date   |  %s\nDeparture time   |  %s\nDeparture city   |  %s\nDestination      |  %s\nTicket price     |  %.2f\nBus capacity     |  %d\nBus empty seats  |  %d", bus->Bus_num, bus->dep_date, bus->dep_time, bus->from, bus->to, bus->price, bus->capacity, bus->emptySeats);
    printPassengerList(bus->PassengersList); //printing the passengers list
}

void printPassengerList(Linked_List l){ //to print all the passengers in the list elegently
    Linked_List r = l; //new one bcz it gives an error if same linked list used in get size and printing
    int num = getSize(r); //get the size to print it
    if(!isEmpty(l)){ //if the linked list is not empty
        ptr t = l->next;
        printf("\n\nThis Bus's passengers are -> %d \n\n",num);
        printf("\t============================================================================================================\n");
        printf("\tPassenger's ID\t    Travelling date\t    Travelling Time\t    Departure City\t    Destination City\n");
        printf("\t------------------------------------------------------------------------------------------------------------\n");
        while(t!=NULL){
            printf("\t   %-20d%-25s%-21s%-25s%-25s", t->ID, t->travelling_date, t->travelling_time, t->from, t->to);
            t = t->next;
        }
        printf("\t============================================================================================================\n\n");
    }else{
        printf("\n\nThis Bus has no passengers!!!\n\n");
    }
}

void printPassengerUnmatchedList(Linked_List l){ //to print all the unmatched passengers in the list elegently
    Linked_List r = l; //new one bcz it gives an error if same linked list used in get size and printing
    int num = getSize(r); //get the size to print it
    if(!isEmpty(l)){ //if the linked list is not empty
        ptr t = l->next;
        printf("\n\n There are %d unmatched passengers =>\n\n",num);
        printf("\t============================================================================================================\n");
        printf("\tPassenger's ID\t    Travelling date\t    Travelling Time\t    Departure City\t    Destination City\n");
        printf("\t------------------------------------------------------------------------------------------------------------\n");
        while(t!=NULL){
            printf("\t   %-20d%-25s%-21s%-25s%-25s\n", t->ID, t->travelling_date, t->travelling_time, t->from, t->to);
            t = t->next;
        }
        printf("\t============================================================================================================\n\n");
    }else{
        printf("\n\nThere are no unmatched passengers!!!\n\n");
    }
}

void loadBus(busptr bus_arr[]){ //this fuction used to load info from file and put it into array of struct(pointer to struct)
    FILE *bus;
    bus = fopen("busses.txt", "r"); //the file to read from
    char info[60]; //to store the line in it

    if(bus == NULL){
        printf("file busses.txt cannot be found");
        exit(1);
    }

    int i=0;//for the array and inputs
    char *token; //to divide the string and store it in it
    int bus_num;
    char dep_date[9];
    char *ptrdate; //to use the trim
    char dep_time[6];
    char *ptrtime;//to use the trim
    char from[35];
    char *ptrfrom;//to use the trim
    char to[35];
    char *ptrto;//to use the trim
    double price;
    int capacity;

    while(fgets(info, 60, bus)){ //while not end of file and still readung
        token = strtok(info, "#"); //read the first part of the line
        bus_num = atoi(token); //to convert it to int

        token = strtok(NULL, "#"); //read the second part
        ptrdate = trimwhitespace(token); //to trim it and store it
        strcpy(dep_date, ptrdate);

        token = strtok(NULL, "#");
        ptrtime = trimwhitespace(token);
        strcpy(dep_time, ptrtime);

        token = strtok(NULL, "#");
        ptrfrom = trimwhitespace(token);
        strcpy(from, ptrfrom);

        token = strtok(NULL, "#");
        ptrto = trimwhitespace(token);
        strcpy(to, ptrto);

        token = strtok(NULL, "#");
        price = atof(token); //converting string to double

        token = strtok(NULL, "#");
        capacity = atoi(token); //converting string to int

        bus_arr[i] = createBusNode(bus_num, dep_date, dep_time, from, to, price, capacity); //creating new node of bus and storing it into the array
        i++; //next array element
    }
}

void loadPassenger(ptr pas_arr[]){ //this fuction used to load info from file and put it into array of struct(pointer to struct)
    FILE *pass;
    pass = fopen("passengers.txt", "r"); //the file to read from
    char info[60]; //to store the line in it

    if(pass == NULL){
        printf("file passengers.txt cannot be found");
        exit(1);
    }

    int i=0; //for the array and inputs
    char *token; //to divide the string and store it in it
    int id;
    char travelling_date[9];
    char *ptrdate; //to use the trim
    char travelling_time[6];
    char *ptrtime; //to use the trim
    char from[35];
    char *ptrfrom; //to use the trim
    char to[35];
    char *ptrto; //to use the trim

    while(fgets(info, 60, pass)){ //while not end of file and still readung
        token = strtok(info, "#");//read the first part of the line
        id = atoi(token);//to convert it to int

        token = strtok(NULL, "#"); //read the second part
        ptrdate = trimwhitespace(token); //to trim it and store it
        strcpy(travelling_date, ptrdate);

        token = strtok(NULL, "#");
        ptrtime = trimwhitespace(token);
        strcpy(travelling_time, ptrtime);

        token = strtok(NULL, "#");
        ptrfrom = trimwhitespace(token);
        strcpy(from, ptrfrom);

        token = strtok(NULL, "#");
        ptrto = trimwhitespace(token);
        strcpy(to, ptrto);

        pas_arr[i] = newNode(id, travelling_date, travelling_time, from, to); //creating new node of bus and storing it into the array
        i++; //next array element
    }
}

int isEmpty(Linked_List l){ //to check if the array is empty
    if(l == NULL){ //if not initialized then empty
        return 1;
    }else{
        return (l->next == NULL); //if initialized and next node in empty then empty
    }
}

int getSize(Linked_List l){ //used to get the size of the linked list
    int count = 0; //counter of nodes
    if(!isEmpty(l)){ //if not empty
        pos t = l->next; //get to the first node
        while(t != NULL){ //until we get to the last node which connected to null
            ++count; //count the nodes
            t = t->next; //go to the next node
        }
    }
    return count;
}

ptr newNode(int id, char date[9], char time[6], char from[35], char to[35]){ //to make a new node of passenger
    ptr node = (ptr) malloc(sizeof (struct PASSENGR)); //initialing memory size
    node->ID = id;
    strcpy(node->travelling_date, date);
    strcpy(node->travelling_time, time);
    strcpy(node->from, from);
    strcpy(node->to, to);
    node->next = NULL; //because initially will be stored in a an array, then when assigned it will have a next, and put into linked list
    node->matched = 0; //used to know if should put in bus or unmatched, initially 0 -> unmatched till found a bus -> 1
    return node;
}

void insert(Linked_List l, ptr newnode){ //to insert a new node at the beginning of linked list
    if(l != NULL && newnode != NULL){ //if none of them null
        newnode->next = l->next; //new node -> next becomes the list
        l->next= newnode; //and the linked list first node becomes the node
    }else{
        printf("This linked list or new node needs initializing\n");
    }
}

void assignPassengers(busptr bus[], int busSize, ptr pass[], int passSize){ //this function used to put all passengers to their right linked list
    for(int i=0; i < busSize; i++){ //to check all the buses in the array
        if(bus[i]->emptySeats == 0) //it bus is full go to the next bus to check
            continue;
        for(int j=0; j < passSize; j++){ //if has empty seats it will go and search the initial passengers list
            if(strcmp(bus[i]->dep_date, pass[j]->travelling_date) == 0 && strcmp(bus[i]->dep_time, pass[j]->travelling_time) == 0 &&strcmp(bus[i]->from, pass[j]->from) == 0 && strcmp(bus[i]->to, pass[j]->to) == 0){
                //if the info of the specific bus and the passenger match then
                bus[i]->emptySeats--; //first let the empty seats -1
                pass[j]->matched = 1; //then make the matched is the passenger 1
                insert(bus[i]->PassengersList, pass[j]); //inserting the passenger in the bus passengers linked list
            }
        }
    }

    for(int i=0; i < passSize; i++){ //will go through all the passengers in the initial passenger list, if matched 0 then it didn't match a bus
        if(pass[i]->matched == 0){
            insert(unmatched, pass[i]); //inserting the unmatched passenger in the unmatched linked list
        }
    }

    printPassengerUnmatchedList(unmatched); //printing unmatched linked list

    for(int i=0; i < busSize; i++){ //printing all buses info
        printBus(bus[i]);
    }
}

void printSpecificBus(busptr bus[], int busSize){ //here is the function to print a specific bus info
    int bus_number;
    int flag = 0; //to check if it's found in the buses array, if not then stays 0
    int index = -1; //to get the index of the bus in the array
    printf("Please enter the bus number to find bus info\n");
    scanf("%d", &bus_number); //getting bus number

    for(int i=0; i < busSize; i++){ //checking all buses in the array
        if(bus[i]->Bus_num == bus_number){
            flag = 1; //make the flag 1 means found in bus
            index =i; //getting the index of the bus
            break;
        }
    }

    if(flag == 0){ //if not found in bus
        printf("Could not find bus!\n");
    }else{
        printBus(bus[index]); //if found print it
    }
}

void addPassenger(busptr bus[], int busSize){ //function to add a passenger other than the file ones
    int id;
    char travelling_date[9];
    char travelling_time[6];
    char from[35];
    char to[35];

    ptr pas; //passenger to store info in it

    printf("Please enter the passenger's id:\n");
    scanf("%d", &id);

    printf("Please enter the traveling date:\n");
    scanf("%s", travelling_date);

    printf("Please enter traveling time:\n", travelling_time);
    scanf("%s", travelling_time);

    printf("Please enter departure city:\n");
    scanf("%s", from);

    printf("Please enter destination city:\n");
    scanf("%s", to);

    pas = newNode(id, travelling_date, travelling_time, from, to); //creating passenger node

    for(int i=0; i < busSize; i++){ //checking all buses
        if(bus[i]->emptySeats == 0) //if no empty seats go to the next bus
            continue;
        if(strcmp(bus[i]->dep_date, pas->travelling_date) == 0 && strcmp(bus[i]->dep_time, pas->travelling_time) == 0 &&strcmp(bus[i]->from, pas->from) == 0 && strcmp(bus[i]->to, pas->to) == 0){
            //if matches then
            bus[i]->emptySeats--; //empty seats in that bus -1
            pas->matched = 1; //it's matched to 1
            insert(bus[i]->PassengersList, pas); //add it to the linked list
        }
    }

    if(pas->matched == 0){ //if not found match
        printf("\nThe passenger didn't match any current buses!!\n");
        insert(unmatched, pas); //put it in unmatched
        printPassengerUnmatchedList(unmatched); //print umatched passengers
    }else{ //if found in bus print all buses info
        printf("\nThe passenger added matched a bus, and put to the list!\n");
        for(int i=0; i < busSize; i++){
            printBus(bus[i]);
        }
    }
}

void insert2LL(Linked_List a, Linked_List b){ //insert a linked list into a linked list, used to add the linked list of the deleted bus to the unmatched
    if(a != NULL && b != NULL){ //if none of the linked lists is null
        pos temp = a->next; //
        while(temp->next != NULL){
            temp = temp->next; //go to the last index of the first linked list
        }
        temp->next = b->next; //and make it's next the other linked list
    }
}

void deleteBus(busptr bus[], int *busSize){ //function to delete a specific bus

    int size = *busSize; //getting the size as pointer so that the size of the array differ
    ptr un = unmatched->next; //getting into the linked list first node
    int bus_num;
    printf("Please enter a bus number to delete! -> ");
    scanf("%d", &bus_num);

    int index =-1; //to store the index of the bus
    for(int i=0; i < size; i++){ //chack all buses
        if(bus[i]->Bus_num == bus_num){ //if found
            index = i; //get the index
            insert2LL(unmatched, bus[i]->PassengersList); //and transfer it's passengers to the unmatched
        }
    }
    if(index != -1){ //if found bus
        for(int i = index; i < size; i++){  //shift the array back the from the index of the bus
            bus[i] = bus[i+1];
        }
        *busSize = *busSize - 1; //and set the array size to size -1
        printf("%d\n", *busSize);
    }else{
        printf("\n\nBus do not exist\n");
    }

    for(int i = 0; i < *busSize; i++){ //print all buses after delete
         printBus(bus[i]);
    }

    printPassengerUnmatchedList(unmatched); //print unmatched after transfering them
}

ptr search(Linked_List l, int id){ //to  search the linked list fore the needed node
    if(!isEmpty(l)){
        ptr t = l->next; //get to the first node
        while(t!=NULL){ //if not the last node
            if(t->ID == id){ //check the id match
                return t; //if found node return it
            }else{
                t = t->next; //if not go to the next node to check
            }
        }
        return t; //returns null usually as it didn't find the node
    }else{
        return NULL; //if the list empty or the first node null return null
    }
}

ptr findPrevious(Linked_List l, ptr node){ //function to find the previous to delete the node
    ptr a = l->next; //first node in list
    ptr b = l; //the list first always empty node

    while(a!=NULL && node->ID!=b->next->ID){ //if the node not null or the next node's id is not the same id
        //used to get to the previous node because it will stop at the node before the node with the id
        b = b->next; //next node
        a = a->next; //next node
    }

    if(a == NULL){ //if not found or empty
        return NULL;
    }else{
        if(b == l){ //if the linked list is back then it's empty from nodes, so return it so that it won't return null
            return l;
        }else{
            return b; //return the previous node found
        }
    }
}

void deleteNode(Linked_List l, ptr node){ //function to delete a specific node
    if(!isEmpty(l)){
        ptr p = findPrevious(l, node); //find previous
        if(p!= NULL){
            ptr d = p->next;
            p->next = d->next;
            free(d);
        }
    }else{
        printf("You can not delete from empty linked list!\n");
    }
}

void deletePassenger(busptr bus[], int busSize){ //function to delete a passenger
    int id;
    printf("Please enter the passenger id to delete -> ");
    scanf("%d", &id);
    int flag =0; //this flag is to check if found in bus
    int flag2 = 0; //this flag used to check if found in unmatched after not found in bus
    int index;

    for(int i=0; i < busSize; i++){ //go into all buses
        ptr a = search(bus[i]->PassengersList, id); //get the node
        if(a != NULL){ //if found not null
            index = i; //get bus index
            flag = 1; //flag found
            bus[i]->emptySeats++; //the bus's empty seats +1
            deleteNode(bus[i]->PassengersList, a); //delete that node from bus
        }
    }

    if(flag == 0){ //if not found in bus
        ptr b = search(unmatched, id); //search the unmatched linked list
        if(b!=NULL){ //if found in unmatched
            flag2=1; //flag found in unmatched
            deleteNode(unmatched, b); //delete from unmatched
        }
    }else{//if found in buses print that bus info after deletion
        printf("\nDeleted from bus number %d\n\n", bus[index]->Bus_num);
        printBus(bus[index]);
    }

    if(flag2 == 0){ //if not found in bus or unmatched
        printf("\nThere is no passenger with that id in busses or unmatched list\n");
    }else{ //then found in unmatched, print the unmatched after deletion
        printf("\nDeleted from unmatched!");
        printPassengerUnmatchedList(unmatched);
    }
}
