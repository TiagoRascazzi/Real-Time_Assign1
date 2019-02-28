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

void startState();
void scanState();
void unlockLeftDoorState();
void unlockRightDoorState();
void openLeftState();
void openRightState();
void weightState();
void closeLeftState();
void closeRightState();
void lockLeftDoorState();
void lockRightDoorState();
void exitState();

int  displayCOID;
Display display; //display sent to the display


int main(int argc, const char* argv[]) {


	void (*stateFunctions[12])(void) = {startState, scanState, unlockLeftDoorState, unlockRightDoorState, openLeftState, openRightState, weightState, closeLeftState, closeRightState, lockLeftDoorState, lockRightDoorState, exitState};

	int inputID;	// indicates who we should reply to
	int chid;		// the channel ID
	pid_t displayPID;

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

	while (1) {

		inputID = MsgReceive(chid, &person, sizeof(person), NULL);
        MsgReply (inputID, EOK, NULL, 0);

		display.person = person;
		stateFunctions[person.state]();

	}

	ChannelDestroy(chid);
	ConnectDetach(displayCOID);
	return EXIT_SUCCESS;
}


void startState(){

}

void scanState(){

	int response;

	// send the message
	if (MsgSend(displayCOID, &display, sizeof(display), &response, sizeof(response)) == -1) {
		fprintf (stderr, "Error during MsgSend\n");
		perror (NULL);
		exit (EXIT_FAILURE);
	}
}

void unlockLeftDoorState(){

}

void unlockRightDoorState(){

}

void openLeftState(){

}

void openRightState(){

}

void weightState(){

}

void closeLeftState(){

}

void closeRightState(){

}

void lockLeftDoorState(){

}

void lockRightDoorState(){

}

void exitState(){

	int response;

	// send the message
	if (MsgSend(displayCOID, &display, sizeof(display), &response, sizeof(response)) == -1) {
		fprintf (stderr, "Error during MsgSend\n");
		perror (NULL);
		exit (EXIT_FAILURE);
	}

	printf("Exiting Controller\n");

	exit (EXIT_SUCCESS);
}
