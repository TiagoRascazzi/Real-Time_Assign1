/*
 * assign1_display.h
 *
 *  Created on: Feb 25, 2019
 *  Author: Tiago Rascazzi, Armando Arenas
 */

#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <string.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>

#include "door_entry.h"


typedef void *(*StateFunc)(Person, Display, int);

void* scanState(Person person, Display display, int  displayCOID);
void* unlockState(Person person, Display display, int  displayCOID);
void* openState(Person person, Display display, int  displayCOID);
void* weightState(Person person, Display display, int  displayCOID);
void* closeState(Person person, Display display, int  displayCOID);
void* lockState(Person person, Display display, int  displayCOID);
void* exitState(Person person, Display display, int  displayCOID);


int main(int argc, const char* argv[]) {

	int inputID;	// indicates who we should reply to
	int chid;		// the channel ID
	pid_t displayPID;
	int  displayCOID;
	Display display; //display sent to the display

	Person person; //person received from inputs

	if( argc != 2 ){
		perror("Invalid argument");
		return EXIT_FAILURE;
	}
	displayPID = atoi(argv[1]);

	//Create the server channel
	chid = ChannelCreate(0);
	if (chid == -1) {
		perror("failed to create the channel.");
		exit(EXIT_FAILURE);
	}

	printf("The controller is running as PID : %d\n", getpid());
	printf("Waiting for Person...\n");

	//Connect the display
	displayCOID = ConnectAttach (ND_LOCAL_NODE, displayPID, 1, _NTO_SIDE_CHANNEL, 0);
	if (displayCOID == -1) {
		fprintf (stderr, "Couldn't ConnectAttach\n");
		perror (NULL);
		exit (EXIT_FAILURE);
	}


	StateFunc statefunc = scanState;

	while (1) {

		inputID = MsgReceive(chid, &person, sizeof(person), NULL);

		if(person.input == EXIT){
			display.person = person;
			display.msgIndex = EXIT_OUTPUT;

			// send the message
			if (MsgSend(displayCOID, &display, sizeof(display), NULL, 0) == -1) {
				fprintf (stderr, "Error during MsgSend\n");
				perror (NULL);
				exit (EXIT_FAILURE);
			}

			printf("Exiting Controller\n");
			break;
		}


	    statefunc = (StateFunc)(*statefunc)(person, display, displayCOID);

        MsgReply (inputID, EOK, &person, sizeof(person));

	}

	ChannelDestroy(chid);
	ConnectDetach(displayCOID);
	return EXIT_SUCCESS;
}



void* scanState(Person person, Display display, int  displayCOID){


	if(person.input == EXIT){
		return exitState;
	}
	return scanState;
}

void* unlockState(Person person, Display display, int  displayCOID){


	if(person.input == EXIT){
		return exitState;
	}

	return unlockState;
}

void* openState(Person person, Display display, int  displayCOID){


	if(person.input == EXIT){
		return exitState;
	}

	return openState;
}

void* weightState(Person person, Display display, int  displayCOID){


	if(person.input == EXIT){
		return exitState;
	}

	return weightState;
}

void* closeState(Person person, Display display, int  displayCOID){


	if(person.input == EXIT){
		return exitState;
	}

	return closeState;
}

void* lockState(Person person, Display display, int  displayCOID){


	if(person.input == EXIT){
		return exitState;
	}

	return lockState;
}























//	int response;
//
//	// send the message
//	display.person = input;
//	display.msgIndex = SCAN_ACK;
//	if (MsgSend(displayCOID, &display, sizeof(display), &response, sizeof(response)) == -1) {
//		fprintf (stderr, "Error during MsgSend\n");
//		perror (NULL);
//		exit (EXIT_FAILURE);
//	}
