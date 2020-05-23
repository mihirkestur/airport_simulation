#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

// This is a structure that acts as a node for both the landing and takeoff queue
typedef struct queue{
    int plane_number;
    struct queue *next_plane;
} Queue;
// two struct queue pointer for landing queue and two struct queue pointers for takeoff queue
Queue *front_of_landing_queue,*rear_of_landing_queue,*front_of_takeoff_queue,*rear_of_takeoff_queue;

// This is a structure that acts as a counter for the number of planes and the idle time
typedef struct Count{
    int number_of_planes;
    int idle_time;
} count;
//counters for landing queue, takeoff queue, landed planes, departed planes, planes redirected, planes denied takeoff.
count in_landing_queue, in_takeoff_queue, landed, departed, redirect, denied_takeoff;

int max_planes_takeoff, max_planes_land;

void initialize(); //initalize all variables
int check_if_int(char *); //checks if input is int (handles input error)
int plane_requests_to_land(int ); //enqueue to landing queue
void plane_lands();   //dequeue to landing queue
int plane_requests_to_takeoff(int ); //enqueue to takeoff queue
void plane_takesoff(); //dequeue to takeoff queue
int random_number();   //generate random number 
int randomize_traffic_index(int ); //generate random number based on input
int simulate(int ,int ); //simulation of airport
void summary(); //summary of the simulation

void main()
{   int simulation_time, traffic_index;
    char check[1000];   // dummy variable to accept user input, used to handle any errors
    srand(time(0));     // setting the seed for the random number generation
    initialize();       // initializes all the necessary variables
    printf("\n#########################################################################\n");
    printf("##################### Welcome to airport simulation #####################");
    printf("\n#########################################################################\n");
    // atoi is inbuilt function in stdlib, converts string to int
    printf("\nPlease enter simulation time (number of units the simulation should run for)\n");
    scanf("%s", check); if(check_if_int(check)){simulation_time = atoi(check);} else{main(); return;}    //checks if it is +ve int, assigns if true, else call main function again and exit

    printf("Enter the maximum amount of planes that can wait on the ground (takeoff queue can hold)\n");
    scanf("%s", check); if(check_if_int(check)){max_planes_takeoff = atoi(check);} else{main(); return;} //checks if it is +ve int, assigns if true, else call main function again and exit

    printf("Enter the maximum amount of planes that wait in the air (landing queue can hold)\n");
    scanf("%s", check); if(check_if_int(check)){max_planes_land = atoi(check);} else{main(); return;}    //checks if it is +ve int, assigns if true, else call main function again and exit

    printf("Enter the traffic index of the airport (enter any postive integer)\n");
    scanf("%s", check); if(check_if_int(check)){traffic_index = atoi(check);} else{main(); return;}      //checks if it is +ve int, assigns if true, else call main function again and exit

    simulate(simulation_time, traffic_index); //simulate the airport 
    summary();                                //summarize the simulation
    printf("\n\nMade by MIHIR Mk\n\n");
}

//This function checks if the command line input is a positive integer or not
int check_if_int(char *text){
    int length; 
    length = strlen(text);
    for(int i=0; i<length; i++)
    {   
        if (isdigit(*(text + i)) == 0){
            printf("\nInvalid input, for all the inputs please enter any positive integer only\n");
            return 0;
            }
    }
    return 1;
    }
/*  initializes the two queues to NULL, 
    set the number of planes in each queue to 0, 
    set the number of planes landed/departed to 0,
    set the number of planes redirected/denied departure to 0 and 
    set the idle time to 0*/
void initialize(){
    front_of_landing_queue = rear_of_landing_queue = NULL;
    front_of_takeoff_queue = rear_of_takeoff_queue = NULL;
    in_landing_queue.number_of_planes = in_takeoff_queue.number_of_planes = landed.number_of_planes = departed.number_of_planes = 0;
    redirect.number_of_planes = denied_takeoff.number_of_planes = 0;
    in_landing_queue.idle_time = in_takeoff_queue.idle_time = landed.idle_time = departed.idle_time = 0;
}
 
// plane number gets added to the landing queue (behaves as enqueue function at the rear of the queue following FIFO)
int plane_requests_to_land(int number){   
    Queue *temp; // local temporary variable
    if(in_landing_queue.number_of_planes <= max_planes_land){
        //enqueue at the rear of the queue
        if (rear_of_landing_queue == NULL){
            rear_of_landing_queue = (Queue *)malloc(sizeof(Queue));
            rear_of_landing_queue->next_plane = NULL;
            rear_of_landing_queue->plane_number = number;
            front_of_landing_queue = rear_of_landing_queue;
        }
        else{
            temp=(Queue *)malloc(sizeof(Queue));
            rear_of_landing_queue->next_plane = temp;
            temp->plane_number = number;
            temp->next_plane = NULL;
            rear_of_landing_queue = temp;
        }
        ++in_landing_queue.number_of_planes;
        printf("Plane number %d requests to land at the airport and has been added to the landing queue\n", number);
        }
    else{ // if queue is full
        printf("Plane number %d requests to land at the airport and has been redirected to a different airport since landing queue is full\n",number);
        ++redirect.number_of_planes;
    }
    return 0;
}
 
// removes plane from landing queue as it has landed (behaves as dequeue function at the front of the queue following FIFO)
void plane_lands(){   
    Queue *temp; //local temporary variable
    temp = front_of_landing_queue; //dequeue from front of the queue

    if (temp == NULL){
        printf("There are no planes to land\n");
        ++in_landing_queue.idle_time;
        return;
    }
    else
        if (temp->next_plane != NULL){
            temp = temp->next_plane;
            printf("Request for landing approved, plane number %d has successfully landed\n", front_of_landing_queue->plane_number);
            free(front_of_landing_queue);
            front_of_landing_queue = temp;
        }
        else{
            printf("Request for landing approved, plane number %d has successfully landed\n", front_of_landing_queue->plane_number);
            free(front_of_landing_queue);
            front_of_landing_queue = rear_of_landing_queue = NULL;
        }
    --in_landing_queue.number_of_planes;
    ++landed.number_of_planes;
}

// plane number gets added to the takeoff queue (behaves as enqueue function at the rear of the queue following FIFO)
int plane_requests_to_takeoff(int number){   
    Queue *temp; //local temporary variable
    if(in_takeoff_queue.number_of_planes <= max_planes_takeoff){
        //enqueue at the rear of the queue
        if (rear_of_takeoff_queue == NULL){

            rear_of_takeoff_queue = (Queue *)malloc(sizeof(Queue));
            rear_of_takeoff_queue->next_plane = NULL;
            rear_of_takeoff_queue->plane_number = number;
            front_of_takeoff_queue = rear_of_takeoff_queue;
        }
        else{
            temp=(Queue *)malloc(sizeof(Queue));
            rear_of_takeoff_queue->next_plane = temp;
            temp->plane_number = number;
            temp->next_plane = NULL;
            rear_of_takeoff_queue = temp;
        }
        ++in_takeoff_queue.number_of_planes;
        printf("Plane number %d requests to depart from the airport and has been added to the takeoff queue\n", number);
        }
    else{   
        printf("Plane number %d has been denied permission for takeoff from the airport since takeoff queue is full\n",number);
        ++denied_takeoff.number_of_planes;
    }
    return 0;
}

// removes plane from takeoff queue as it has departed the airport (behaves as dequeue function at the front of the queue following FIFO)
void plane_takesoff(){   
    Queue *temp; //local temporary variable
    temp = front_of_takeoff_queue; //dequeue from front of the queue

    if (temp == NULL){
        printf("No plane is there to takeoff\n");
        ++in_takeoff_queue.idle_time;
        return;
    }
    else
        if (temp->next_plane != NULL){
            temp = temp->next_plane;
            printf("Request for take off approved, plane number %d has successfully departed the airport\n", front_of_takeoff_queue->plane_number);
            free(front_of_takeoff_queue);
            front_of_takeoff_queue = temp;
        }
        else{
            printf("Request for take off approved, plane number %d has successfully departed the airport\n", front_of_takeoff_queue->plane_number);
            free(front_of_takeoff_queue);
            front_of_takeoff_queue = rear_of_takeoff_queue = NULL;
        }
    --in_takeoff_queue.number_of_planes;
    ++departed.number_of_planes;
}

// generates a random number between 1 and 3
int random_number(){
    int randomnumber = (rand() % (3)) + 1;
    return randomnumber;
}

// generates a random number between 1 and traffic_index(entered by the user). This is simply to randomize the the process of generation of planes.
int randomize_traffic_index(int traffic_index){
    if(traffic_index == 0){
        return 0;
    }
    else {
    int random_traffic_index = (rand()%(traffic_index)) + 1;
    return random_traffic_index;
    }
}

// This function simulates the airport under the given conditions
int simulate(int simulation_time, int traffic_index){

  int begin, random_traffic_index, plane_number = 0;

  for(begin = 1; begin<=simulation_time; begin++){                          //one for loop represents 1 unit of time
        random_traffic_index = randomize_traffic_index(traffic_index);
        printf("\n----- Unit %d -----\n",begin);
        
            for(int i=0;i<random_traffic_index;i++){                        //to generate planes randomly, a for loop runs traffic_index number of times 
                int choice = random_number();                               //to increase the chances of either of the cases to occur
                
                switch(choice){                                             
                    case 1: ++plane_number; plane_requests_to_land(plane_number); break;    //plane gets added to landing queue if not full
                    case 2: ++plane_number; plane_requests_to_takeoff(plane_number); break; //plane gets added to takeoff queue if not full
                    case 3: break;                                                          //idle condition
                }//default case is not required since random number genereates number only between 1 and 3, hence choice is always between 1 and 3.
            }
        // As per the question only 1 plane can depart or land at each unit of time.
        if(in_landing_queue.number_of_planes == 0){
            plane_takesoff();
            ++in_landing_queue.idle_time;
        }
        else{
            plane_lands();
            ++in_takeoff_queue.idle_time;
        }
  }
  printf("\n_______________________________________________________________\n");
  printf("The simulation has concluded\nThe airport has dealt with %d planes in %d units of time",plane_number,simulation_time);
  printf("\n_______________________________________________________________\n");
  return 0;
}


// This function summarizes the simulation
void summary(){
    printf("\n########## Summary of the simulation ########## \n");
    printf("Number of planes still in landing queue : %d\n",in_landing_queue.number_of_planes);
    printf("Number of planes still in takeoff queue : %d\n",in_takeoff_queue.number_of_planes);
    printf("Number of planes that have successfully departed : %d\n",departed.number_of_planes);
    printf("Number of planes that have successfully landed : %d\n",landed.number_of_planes);
    printf("Number of planes that have been redirected to a different airport : %d\n",redirect.number_of_planes);
    printf("Number of times the planes have been denied permission for takeoff : %d\n",denied_takeoff.number_of_planes);
    printf("Number of units the landing runway has been idle : %d\n",in_landing_queue.idle_time);
    printf("Number of units the takeoff runway has been idle : %d\n",in_takeoff_queue.idle_time);
}
