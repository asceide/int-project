/*  Author: Patrick Alarcon
*   This is the main file for this Transportation program. It holds the main menu and the goes into different modules according to the user choice.
*   This program allows the user to buy a pass (either monthly, pay-per-ride, or both), and use it on three modes of transportation (4 if the user is elderly) and to check their pass balance. 
*   Transportation workers are given a special pass that gives them a free ride on any mode of transportation. Elderly and Students get a 50% discount on monthly and pay-per-ride passes. Monthly passes are prorated with a 50% discount if they are bought past the half way point of the month (usually the 15th)
*   Weekend pay-per-ride passes are 75% of the cost of the daily rides. For Subways and trams, in the future there maybe a charge per stop. All prices are subject to change.
*/

#include <stdlib.h>//Typical inclusion of C Libraries
#include <stdio.h>
#include <stdbool.h> //For the usage of boolean
#include "passenger.h" //Header file that will contain the passenger structure. It is including before all of the other self-made modules as it is used throughout the program 
#include "buying.h" //Header file that contains the buying.c module, used to give the user
#include "using.h"  //Header file that

#ifdef _WIN32 //In order to clear the screen instead of relying on system calls as that isn't compliant.
#include <conio.h>
#else
#define clrscr() printf("\e[1;1H\e[2J") //Using escapes and regex to clear screen on posix/Unix systems.
#endif

//Macros to hold Menus and errors
#define MENU "\nCharlotte Transportation Menu\n\n\n\n\nWhat would you like to do?\n1)Buy Pass\n2)Use Pass\n3)Check Balance\n4)Exit\n"
#define NOT_THERE "You have selected an incorrect option. Please try again."
#define PRESS_ENTER "Please Press Enter"

int main(){
    char *u_input=NULL; //For getline usage. getline is the best method for getting user input as it avoid several problems like buffer overflow problems with gets, waits for a newline instead of the first blank when reading a string and doesn't appended a null char like with scanf, amongst other
    struct Passenger only; //A struct that will hold the passenger information
    long choice;    //Since I am using numbers for the menu
    size_t single_input=1; //For getline, size of memory used by u_input
    bool mainFlag=true; //A bool flag to keep looping through the main menu
    only.monthlyPasses=malloc(4*(sizeof(bool)));
    for(int i=0; i<4; i++){
        only.monthlyPasses[i]=false;
    }
    //Initiallizing the struct 
    only.passenger_type=0;
    only.passenger_id=-1;
    only.isTWorker=false;
    only.pass_balance=0.0;
    clrscr();
    printf(PRESS_ENTER);
    getline(&u_input, &single_input, stdin);
    do{//Do while loop as I want to the program to run atleast once no matter what.
        clrscr();
        printf(MENU);
        getline(&u_input, &single_input, stdin); //Gets the user input, preferred over scanf.
        choice=atol(u_input); //Converts the user input to a long integer
        switch(choice){//The Menu
            case 1:    //If they want to buy a pass
                    buy_pass(&only); //Send the address of the Passenger struct as a parameter Passenger*, since we want to manipulate the struct
                    if(only.passenger_type=='t'){//If the user is a transportation worker, set a flag saying so in order for them to get a free ride everywhere.
                        only.isTWorker=true;
                    }else
                    {
                        only.isTWorker=false;
                    }
                    
                    break;
            case 2: 
                    use_pass(&only); //If they want to use the pass
                    break;
            case 3: 
                    clrscr(); //If they want to check their pass balance
                    printf("Your pass balance is %.2f .\nPress Enter to Continue\n", only.pass_balance);
                    getline(&u_input, &single_input, stdin);
                    break;
            case 4: //If they want to exit the program
                    printf("Goodbye");
                    mainFlag=false;
                    break;
            default: printf(NOT_THERE);//Error checking if none of the above options are selected.
                     break;
        }

    }while(mainFlag==true);
    clrscr();
    free(u_input); //Freeing allocated memories
    free(only.monthlyPasses);
    return 0;//Return a zero.
}

