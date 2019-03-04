/*
 * assign1_display.h
 *
 *  Created on: Oct 11, 2018
 *  Author: Tiago Rascazzi, Armando Arenas, hurdleg
 */

#ifndef DOOR_ENTRY_H_
#define DOOR_ENTRY_H_


#define NUM_STATES 6

typedef enum {
	SCAN_STATE,
	UNLOCK_DOOR_STATE,
	OPEN_DOOR_STATE,
	WEIGHT_STATE,
	CLOSE_DOOR_STATE,
	LOCK_DOOR_STATE
} State;




#define NUM_INPUTS 12

typedef enum {
	LS,
	RS,
	GLU,
	GRU,
	LO,
	RO,
	WS,
	LC,
	RC,
	GLL,
	GRL,
	EXIT
} Input;

const char *inMessage[NUM_INPUTS] = {
	"ls",
	"rs",
	"glu",
	"gru",
	"lo",
	"ro",
	"ws",
	"lc",
	"rc",
	"gll",
	"grl",
	"exit"
};




#define NUM_OUTPUTS 11

typedef enum {
	SCAN_ACK,
	UNLOCK_LEFT_DOOR,
	UNLOCK_RIGHT_DOOR,
	OPEN_LEFT_DOOR,
	OPEN_RIGHT_DOOR,
	WEIGHT_ACK,
	CLOSE_LEFT_DOOR,
	CLOSE_RIGHT_DOOR,
	LOCK_LEFT_DOOR,
	LOCK_RIGHT_DOOR,
	EXIT_OUTPUT
} Output;

const char *outMessage[NUM_OUTPUTS] = {
	"Person scanned ID, ID = %d",
	"Left door unlocked by Guard",
	"Right door unlocked by Guard",
	"Left door opened",
	"Right door opened",
	"Person weighed, Weight = %d",
	"Left door closed",
	"Right door closed",
	"Left door locked by Guard",
	"Right door locked by Guard"
	"Exiting Display"
};




typedef enum{
	INBOUND,
	OUTBOUND
} Dirrection;

// inputs client sends a Person struct to its server, the controller
typedef struct {
	int personID;
	int weight;
	Dirrection direction;
	Input input;
} Person;

// controller client sends a Display struct to its server, the display
typedef struct {
	Output msgIndex; //index to outMessages
	Person person;
} Display;

#endif
