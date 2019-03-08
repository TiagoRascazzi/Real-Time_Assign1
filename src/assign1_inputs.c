/*
 * assign1_display.h
 *
 *  Created on: Feb 25, 2019
 *  Author: Tiago Rascazzi, Armando Arenas
 */

#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <string.h>

#include "../../Real-Time_Assign1_Controller/src/door_entry.h"

void printInputList();
void getInput(char* input);
int requestPersonID();
int requestPersonWeight();

int main(int argc, const char* argv[]) {

	int  coid;
	pid_t controllerPID;

	char input[64];

	Person person; //person to sent to/received from the controller

	if( argc != 2 ){
		perror("Invalid argument");
		return EXIT_FAILURE;
	}
	controllerPID = atoi(argv[1]);


	//Connect the server
	coid = ConnectAttach (ND_LOCAL_NODE, controllerPID, 1, _NTO_SIDE_CHANNEL, 0);
	if (coid == -1) {
		fprintf (stderr, "Couldn't ConnectAttach\n");
		perror (NULL);
		exit (EXIT_FAILURE);
	}

	while(1){

		getInput(input);

		for(int i=0; i<NUM_INPUTS; i++){
			if(strcmp(input, inMessage[i]) == 0){

				if(strcmp(input, "ls") == 0 || strcmp(input, "rs") == 0){

					person.personID = requestPersonID();

				}else if(strcmp(input, "ws") == 0){

					person.weight = requestPersonWeight();

				}

				person.input = i;
				break;
			}
		}

		// send the message
		if (MsgSend(coid, &person, sizeof(person), &person, sizeof(person)) == -1) {
			fprintf (stderr, "Error during MsgSend\n");
			perror (NULL);
			exit (EXIT_FAILURE);
		}

		if (strcmp(input, "exit") == 0){
			break;
		}
	}

	ConnectDetach(coid);
	return EXIT_SUCCESS;
}


void getInput(char* input){

	int isValid = 0;

	printInputList();

	while(!isValid){

		scanf("%s", input);

		for(int i=0; i<NUM_INPUTS; i++){
			if(strcmp(input ,inMessage[i]) == 0){
				isValid = 1;
				break;
			}
		}

		if(!isValid){
			printf("Invalid input, try again \n");
			printInputList();
		}
	}

}

int requestPersonID(){
	int personID = -1;
	while (personID < 0) {
		printf("Enter the Person's ID: ");
		fflush(stdout);
		scanf("%d", &personID);
		printf("\n");

		if(personID < 0){
			char c;
			while ((c = getchar()) != '\n' && c != EOF) {
			}
			printf("Invalid input: Person's ID must be a number greater then 0\n\n");
		}
	}
	return personID;
}

int requestPersonWeight(){
	int personID = -1;
	while (personID < 0) {
		printf("Enter the person's Weight: ");
		fflush(stdout);
		scanf("%d", &personID);
		printf("\n");

		if(personID < 0){
			char c;
			while ((c = getchar()) != '\n' && c != EOF) {
			}
			printf("Invalid input: Person's weight must be a number greater then 0\n\n");
		}
	}
	return personID;
}


void printInputList(){
	printf("\nInput list:\n");
	for(int i=0; i<NUM_INPUTS; i++){
		printf("\t%s = %s,\n", inMessage[i], inMessageDesc[i]);
	}
	printf("Enter the event type: ");
	fflush(stdout);
}






















