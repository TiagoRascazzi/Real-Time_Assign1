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


typedef void *(*StateFunc)(Person input);

void* startState(Person input);
void* scanState(Person input);
void* unlockLeftDoorState(Person input);
void* unlockRightDoorState(Person input);
void* openLeftState(Person input);
void* openRightState(Person input);
void* weightState(Person input);
void* closeLeftState(Person input);
void* closeRightState(Person input);
void* lockLeftDoorState(Person input);
void* lockRightDoorState(Person input);
void* exitState(Person input);

int  displayCOID;
Display display; //display sent to the display


int main(int argc, const char* argv[]) {


	//void (*stateFunctions[12])(void) = {startState, scanState, unlockLeftDoorState, unlockRightDoorState, openLeftState, openRightState, weightState, closeLeftState, closeRightState, lockLeftDoorState, lockRightDoorState, exitState};

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

	StateFunc statefunc = startState;

	while (1) {

		inputID = MsgReceive(chid, &person, sizeof(person), NULL);
        MsgReply (inputID, EOK, NULL, 0);

		//display.person = person;

	    statefunc = (StateFunc)(*statefunc)(person);

	}

	ChannelDestroy(chid);
	ConnectDetach(displayCOID);
	return EXIT_SUCCESS;
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


void* startState(Person input){
	if(input.input == LS){
		display.person.direction = INBOUND;
		return scanState;
	}

	if(input.input == RS){
		display.person.direction = OUTBOUND;
		return scanState;
	}

	if(input.input == EXIT){
		return exitState;
	}

	return startState;
}

void* scanState(Person input){

	if(input.direction == INBOUND && input.input == GLU){
		return unlockLeftDoorState;
	}

	if(input.direction == OUTBOUND && input.input == GRU){
		return unlockRightDoorState;
	}

	if(input.input == EXIT){
		return exitState;
	}

	return scanState;
}

void* unlockLeftDoorState(Person input){

	if(input.direction == INBOUND && input.input == LO){
		return openLeftState;
	}

	if(input.direction == OUTBOUND && input.input == LO){
		return openLeftState;
	}

	if(input.input == EXIT){
		return exitState;
	}

	return unlockLeftDoorState;
}

void* unlockRightDoorState(Person input){
	if(input.direction == INBOUND && input.input == RO){
		return openRightState;
	}

	if(input.direction == OUTBOUND && input.input == RO){
		return openRightState;
	}

	if(input.input == EXIT){
		return exitState;
	}

	return unlockRightDoorState;
}

void* openLeftState(Person input){
	if(input.direction == INBOUND && input.input == WS){
		return weightState;
	}

	if(input.direction == OUTBOUND && input.input == LC){
		return closeLeftState;
	}

	if(input.input == EXIT){
		return exitState;
	}

	return openLeftState;
}

void* openRightState(Person input){
	if(input.direction == INBOUND && input.input == RC){
		return closeRightState;
	}

	if(input.direction == OUTBOUND && input.input == WS){
		return weightState;
	}

	if(input.input == EXIT){
		return exitState;
	}

	return openRightState;
}

void* weightState(Person input){
	if(input.direction == INBOUND && input.input == LC){
		return closeLeftState;
	}

	if(input.direction == OUTBOUND && input.input == RC){
		return closeRightState;
	}

	if(input.input == EXIT){
		return exitState;
	}

	return weightState;
}

void* closeLeftState(Person input){
	if(input.direction == INBOUND && input.input == GLL){
		return lockLeftDoorState;
	}

	if(input.direction == OUTBOUND && input.input == GLL){
		return lockLeftDoorState;
	}

	if(input.input == EXIT){
		return exitState;
	}

	return closeLeftState;
}

void* closeRightState(Person input){
	if(input.direction == INBOUND && input.input == GRL){
		return lockRightDoorState;
	}

	if(input.direction == OUTBOUND && input.input == GRL){
		return lockRightDoorState;
	}

	if(input.input == EXIT){
		return exitState;
	}

	return closeRightState;
}

void* lockLeftDoorState(Person input){
	if(input.direction == INBOUND && input.input == GRU){
		return unlockRightDoorState;
	}

	if(input.direction == OUTBOUND && (input.input == LS || input.input == RS) ){
		return scanState;
	}

	if(input.input == EXIT){
		return exitState;
	}

	return lockLeftDoorState;
}

void* lockRightDoorState(Person input){
	if(input.direction == INBOUND && (input.input == LS || input.input == RS)){
		return scanState;
	}

	if(input.direction == OUTBOUND && input.input == GLU){
		return unlockLeftDoorState;
	}

	if(input.input == EXIT){
		return exitState;
	}

	return lockRightDoorState;
}

void* exitState(Person input){

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
