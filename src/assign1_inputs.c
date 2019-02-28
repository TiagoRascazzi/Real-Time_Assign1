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

int main(int argc, const char* argv[]) {

	int  coid;
	pid_t controllerPID;
	int response;

	char input[64];

	Person person; //person to sent to the controller

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

		printf("Enter the event type ");
		printf("(ls = Left Scan, rs = Right Scan, glu = Guard Left Unlock, gru = Guard Right Unlock, lo = Left Open, ro = Right Open, ws = Weight Scale, lc = Left Close, rc = Right Close, gll = Guard Left Lock, grl = Guard Right Lock, exit = Exit Program)\n\n");

		scanf("%s", &input);

		if (strcmp(input, "ls") == 0){

			printf("Enter the Person's ID: ");
			fflush(stdout);
			scanf("%d", &person.personID);
			printf("Person scanned ID, ID = %d\n", person.personID);

			person.direction = INBOUND;
			person.state = SCAN_STATE;

		} else if (strcmp(input, "rs") == 0){

			printf("Enter the Person's ID: ");
			fflush(stdout);
			scanf("%d", &person.personID);
			printf("Person scanned ID, ID = %d\n", person.personID);

			person.direction = OUTBOUND;
			person.state = SCAN_STATE;

		} else if (strcmp(input, "glu") == 0){
			person.state = UNLOCK_LEFT_DOOR_STATE;

		} else if (strcmp(input, "gru") == 0){
			person.state = UNLOCK_RIGHT_DOOR_STATE;

		} else if (strcmp(input, "lo") == 0){
			person.state = OPEN_LEFT_STATE;

		} else if (strcmp(input, "ro") == 0){
			person.state = OPEN_RIGHT_STATE;

		} else if (strcmp(input, "ws") == 0){

			printf("Enter the person's Weight: ");
			fflush(stdout);
			scanf("%d", &person.weight);
			printf("Person weighed, Weight = %d\n", person.weight);

			person.state = WEIGHT_STATE;

		} else if (strcmp(input, "lc") == 0){
			person.state = CLOSE_LEFT_STATE;

		} else if (strcmp(input, "rc") == 0){
			person.state = CLOSE_RIGHT_STATE;

		} else if (strcmp(input, "gll") == 0){
			person.state = LOCK_LEFT_DOOR_STATE;

		} else if (strcmp(input, "grl") == 0){
			person.state = LOCK_RIGHT_DOOR_STATE;

		} else if (strcmp(input, "exit") == 0){
			person.state = EXIT_STATE;

		}

		// send the message
		if (MsgSend(coid, &person, sizeof(person), &response, sizeof(response)) == -1) {
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

