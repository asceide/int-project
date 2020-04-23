#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h> //For the usage of boolean
#include <time.h>   //For the usage of computer time
#include <ctype.h>
#include "passenger.h" //Passenger struct, and all other structs, must come before the other headers.
#include "buying.h"
#include "using.h"


#define MENU "\nCharlotte Transportation Menu\n\n\n\n\nWhat would you like to do?\n1)Buy Pass\n2)Use Pass\n3)Check Balance\n4)Exit\n"
#define NOT_THERE "You have selected an incorrect option. Please try again."
#define PRESS_ENTER "Please Press Enter"

void Usage(int err);


int main(){
    char *u_input=NULL;
    struct Passenger only; 
    long choice;
    bool* pass_active;
    size_t single_input=0;
    bool mainFlag=true;
    pass_active=malloc(4*sizeof(bool));
    for(int i=0; i<4; i++){
        pass_active[i]=false;
    }
    only.passenger_type=0;
    only.passenger_id=-1;
    only.monthlyPasses=pass_active;
    only.isTWorker=false;
    only.pass_balance=0.0;
    system("clear");
    printf(PRESS_ENTER);
    getchar();
    do{
        system("clear");
        printf(MENU);
        getline(&u_input, &single_input, stdin); //Gets the user input, preferred over scanf.
        choice=atol(u_input); //Converts the user input to a long integer
        switch(choice){
            case 1:    
                    buy_pass(&only);
                    if(only.passenger_type=='t'){
                        only.isTWorker=true;
                    }else
                    {
                        only.isTWorker=false;
                    }
                    
                    break;
            case 2: 
                    use_pass(&only);
                    break;
            case 3: 
                    system("clear");
                    printf("Your pass balance is %.2f .\nPress Enter to Continue\n", only.pass_balance);
                    getchar();
                    break;
            case 4: 
                    printf("Goodbye");
                    mainFlag=false;
                    break;
            default: printf(NOT_THERE);
                     system("clear");
                     break;
        }

    }while(mainFlag==true);
    system("clear");
    free(u_input); //Freeing the memory allocated for the getline.
    free(pass_active);
    return 0;
}

