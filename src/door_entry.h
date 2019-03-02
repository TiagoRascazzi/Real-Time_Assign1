/*
 * assign1_display.h
 *
 *  Created on: Oct 11, 2018
 *  Author: Tiago Rascazzi, Armando Arenas, hurdleg
 */

#ifndef DOOR_ENTRY_H_
#define DOOR_ENTRY_H_


#define NUM_STATES 12

typedef enum {
	START_STATE,
	SCAN_STATE,
	UNLOCK_LEFT_DOOR_STATE,
	UNLOCK_RIGHT_DOOR_STATE,
	OPEN_LEFT_STATE,
	OPEN_RIGHT_STATE,
	WEIGHT_STATE,
	CLOSE_LEFT_STATE,
	CLOSE_RIGHT_STATE,
	LOCK_LEFT_DOOR_STATE,
	LOCK_RIGHT_DOOR_STATE,
	EXIT_STATE
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




#define NUM_OUTPUTS 12

typedef enum {
	SCAN_REQUEST,
	SCAN_ACK,
	UNLOCK_LEFT_DOOR,
	UNLOCK_RIGHT_DOOR,
	OPEN_LEFT_DOOR,
	OPEN_RIGHT_DOOR,
	WEIGHT_REQUEST,
	WEIGHT_ACK,
	CLOSE_LEFT_DOOR,
	CLOSE_RIGHT_DOOR,
	LOCK_LEFT_DOOR,
	LOCK_RIGHT_DOOR
} Output;

const char *outMessage[NUM_OUTPUTS] = {
	"Enter the Person's ID: ",
	"Person scanned ID, ID = %d",
	"Left door unlocked by Guard",
	"Right door unlocked by Guard",
	"Left door opened",
	"Right door opened",
	"Enter the person's Weight: ",
	"Person weighed, Weight = %d",
	"Left door closed",
	"Right door closed",
	"Left door locked by Guard",
	"Right door locked by Guard"
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
	//TODO maybe add State state;
} Person;

// controller client sends a Display struct to its server, the display
typedef struct {
	Output msgIndex; //index to outMessages
	Person person;
} Display;

#endif
