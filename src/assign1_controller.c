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


typedef void *(*StateFunc)(Person*, Display*);

void* startedState(Person* person, Display* display);
void* scannedState(Person* person, Display* display);
void* unlockedState(Person* person, Display* display);
void* openedState(Person* person, Display* display);
void* weightedState(Person* person, Display* display);
void* closedState(Person* person, Display* display);
void* lockedState(Person* person, Display* display);


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
	printf("Waiting for person...\n");
	fflush(stdout);

	//Connect the display
	displayCOID = ConnectAttach (ND_LOCAL_NODE, displayPID, 1, _NTO_SIDE_CHANNEL, 0);
	if (displayCOID == -1) {
		fprintf (stderr, "Couldn't ConnectAttach\n");
		perror (NULL);
		exit (EXIT_FAILURE);
	}

	StateFunc statefunc = startedState;

	while (1) {

		inputID = MsgReceive(chid, &person, sizeof(person), NULL);

		if(person.input == EXIT){
			display.person = person;
			display.msgIndex = EXIT_MSG;

			// send the message
			if (MsgSend(displayCOID, &display, sizeof(display), NULL, 0) == -1) {
				fprintf (stderr, "Error during MsgSend\n");
				perror (NULL);
				exit (EXIT_FAILURE);
			}

	        MsgReply (inputID, EOK, &person, sizeof(person));
			break;
		}

		display.msgIndex = INVALID_INPUT;
		display.person = person;
	    statefunc = (StateFunc)(*statefunc)(&person, &display);


		// send the message
		if (MsgSend(displayCOID, &display, sizeof(display), NULL, 0) == -1) {
			fprintf (stderr, "Error during MsgSend\n");
			perror (NULL);
			exit (EXIT_FAILURE);
		}

        MsgReply (inputID, EOK, &person, sizeof(person));

	}

	printf("Exiting Controller\n");
	ChannelDestroy(chid);
	ConnectDetach(displayCOID);
	return EXIT_SUCCESS;
}




void* startedState(Person* person, Display* display){

	if(person->input == LS){
		person->direction = INBOUND;
		person->door = LEFT_DOOR;
		display->msgIndex = SCAN_ACK;
		return scannedState; //Left Scan
	}

	if(person->input == RS){
		person->direction = OUTBOUND;
		person->door = RIGHT_DOOR;
		display->msgIndex = SCAN_ACK;
		return scannedState; //Right Scan
	}

	return startedState;
}

void* scannedState(Person* person, Display* display){

	if(person->direction == INBOUND && person->door == LEFT_DOOR && person->input == GLU){
		display->msgIndex = UNLOCKED_LEFT_DOOR;
		return unlockedState; //Guard Left Unlock
	}

	if(person->direction == OUTBOUND && person->door == RIGHT_DOOR && person->input == GRU){
		display->msgIndex = UNLOCKED_RIGHT_DOOR;
		return unlockedState; //Guard Right Unlock
	}

	return scannedState;
}

void* unlockedState(Person* person, Display* display){

	if(person->direction == INBOUND && person->door == LEFT_DOOR && person->input == LO){
		display->msgIndex = OPENED_LEFT_DOOR;
		return openedState; //Left Open
	}

	if(person->direction == OUTBOUND && person->door == LEFT_DOOR && person->input == LO){
		display->msgIndex = OPENED_LEFT_DOOR;
		return openedState; //Left Open
	}

	if(person->direction == INBOUND && person->door == RIGHT_DOOR && person->input == RO){
		display->msgIndex = OPENED_RIGHT_DOOR;
		return openedState; //Right Open
	}

	if(person->direction == OUTBOUND && person->door == RIGHT_DOOR && person->input == RO){
		display->msgIndex = OPENED_RIGHT_DOOR;
		return openedState; //Right Open
	}

	return unlockedState;
}

void* openedState(Person* person, Display* display){

	if(person->direction == INBOUND && person->door == LEFT_DOOR && person->input == WS){
		display->msgIndex = WEIGHT_ACK;
		return weightedState; //Weight
	}

	if(person->direction == OUTBOUND && person->door == LEFT_DOOR && person->input == LC){
		display->msgIndex = CLOSED_LEFT_DOOR;
		return closedState; //Left Close
	}

	if(person->direction == OUTBOUND && person->door == RIGHT_DOOR && person->input == WS){
		display->msgIndex = WEIGHT_ACK;
		return weightedState; //Weight
	}

	if(person->direction == INBOUND && person->door == RIGHT_DOOR && person->input == RC){
		display->msgIndex = CLOSED_RIGHT_DOOR;
		return closedState; //Right Close
	}

	return openedState;
}

void* weightedState(Person* person, Display* display){

	if(person->direction == INBOUND && person->door == LEFT_DOOR && person->input == LC){
		display->msgIndex = CLOSED_LEFT_DOOR;
		return closedState; //Left Close
	}

	if(person->direction == OUTBOUND && person->door == RIGHT_DOOR && person->input == RC){
		display->msgIndex = CLOSED_RIGHT_DOOR;
		return closedState; //Right Close
	}

	return weightedState;
}

void* closedState(Person* person, Display* display){

	if(person->direction == INBOUND && person->door == LEFT_DOOR && person->input == GLL){
		display->msgIndex = LOCKED_LEFT_DOOR;
		return lockedState; //Left Lock
	}

	if(person->direction == OUTBOUND && person->door == LEFT_DOOR && person->input == GLL){
		display->msgIndex = LOCKED_LEFT_DOOR;
		return lockedState; //Left Lock
	}

	if(person->direction == OUTBOUND && person->door == RIGHT_DOOR && person->input == GRL){
		display->msgIndex = LOCKED_RIGHT_DOOR;
		return lockedState; //Right Lock
	}

	if(person->direction == INBOUND && person->door == RIGHT_DOOR && person->input == GRL){
		display->msgIndex = LOCKED_RIGHT_DOOR;
		return lockedState; //Right lock
	}

	return closedState;
}

void* lockedState(Person* person, Display* display){

	if(person->direction == INBOUND && person->door == LEFT_DOOR && person->input == GRU){
		person->door = RIGHT_DOOR;
		display->msgIndex = UNLOCKED_RIGHT_DOOR;
		return unlockedState; //Guard Right Unlock
	}

	if(person->direction == OUTBOUND && person->door == RIGHT_DOOR && person->input == GLU){
		person->door = LEFT_DOOR;
		display->msgIndex = UNLOCKED_LEFT_DOOR;
		return unlockedState; //Guard Left Unlock
	}


	if(person->direction == INBOUND && person->door == RIGHT_DOOR && person->input == LS){
		person->door = LEFT_DOOR;
		person->direction = INBOUND;
		display->msgIndex = SCAN_ACK;
		return scannedState; //Left Scan
	}

	if(person->direction == INBOUND && person->door == RIGHT_DOOR && person->input == RS){
		person->door = RIGHT_DOOR;
		person->direction = OUTBOUND;
		display->msgIndex = SCAN_ACK;
		return scannedState; //Right Scan
	}

	if(person->direction == OUTBOUND && person->door == LEFT_DOOR && person->input == LS){
		person->door = LEFT_DOOR;
		person->direction = INBOUND;
		display->msgIndex = SCAN_ACK;
		return scannedState; //Left Scan
	}

	if(person->direction == OUTBOUND && person->door == LEFT_DOOR && person->input == RS){
		person->door = RIGHT_DOOR;
		person->direction = OUTBOUND;
		display->msgIndex = SCAN_ACK;
		return scannedState; //Right Scan
	}

	return lockedState;
}


