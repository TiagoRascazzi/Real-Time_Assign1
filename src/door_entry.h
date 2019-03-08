/*
 * assign1_display.h
 *
 *  Created on: Oct 11, 2018
 *  Author: Tiago Rascazzi, Armando Arenas, hurdleg
 */

#ifndef DOOR_ENTRY_H_
#define DOOR_ENTRY_H_


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

const char *inMessageDesc[NUM_INPUTS] = {
	"Left Scan",
	"Right Scan",
	"Guard Left Unlock",
	"Guard Right Unlock",
	"Left Open",
	"Right Open",
	"Weight Scale",
	"Left Close",
	"Right Close",
	"Guard Left Lock",
	"Guard Right Lock",
	"Exit Program"
};




#define NUM_OUTPUTS 12

typedef enum {
	SCAN_ACK,
	UNLOCKED_LEFT_DOOR,
	UNLOCKED_RIGHT_DOOR,
	OPENED_LEFT_DOOR,
	OPENED_RIGHT_DOOR,
	WEIGHT_ACK,
	CLOSED_LEFT_DOOR,
	CLOSED_RIGHT_DOOR,
	LOCKED_LEFT_DOOR,
	LOCKED_RIGHT_DOOR,
	INVALID_INPUT,
	EXIT_MSG
} Output;

const char *outMessage[NUM_OUTPUTS] = {
	"Person scanned ID, ID =",
	"Left door unlocked by Guard",
	"Right door unlocked by Guard",
	"Left door opened",
	"Right door opened",
	"Person weighed, Weight =",
	"Left door closed",
	"Right door closed",
	"Left door locked by Guard",
	"Right door locked by Guard",
	"Invalid input, for current state",
	"Exiting Display"
};




typedef enum{
	INBOUND,
	OUTBOUND
} Dirrection;

typedef enum{
	LEFT_DOOR,
	RIGHT_DOOR
} Door;

// inputs client sends a Person struct to its server, the controller
typedef struct {
	int personID;
	int weight;
	Dirrection direction;
	Door door;
	Input input;
} Person;

// controller client sends a Display struct to its server, the display
typedef struct {
	Output msgIndex; //index to outMessages
	Person person;
} Display;

#endif
