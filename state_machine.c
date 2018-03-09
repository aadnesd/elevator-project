#include <stdio.h>
#include "state_machine.h"
#include "elev.h"
#include "timer.h"

typedef enum {
	state_idle,
	state_move,
	state_stop,
	state_entering
} Elstate;

static Elstate curr_state;


void sm_init (void) {
	if (elev_get_floor_sensor_signal () == -1) {
		elev_set_motor_direction (DIRN_DOWN);
		while (elev_get_floor_sensor_signal () == -1) {
		}
	}
	elev_set_motor_direction (DIRN_STOP);
	curr_state = state_idle;
}

void sm_idle (void) {
	remove_light_and_order_for_floor (elev_get_floor_sensor_signal ()); 
	switch(curr_state) {
		case state_stop: {
		
		curr_state = state_idle;
		elev_set_stop_lamp (0);
		elev_set_door_open_lamp (0);
		
		elev_set_motor_direction (choose_direction_after_stop (get_last_floor ())); 
		timer_start ();
		break;
		}

		case state_entering: {
		
		curr_state = state_idle;
		elev_set_door_open_lamp (0);
		break;
		}
	}
}


void sm_move (void) {
	switch (curr_state) {
		case state_idle: {
		
		curr_state = state_move;
		elev_set_motor_direction (choose_direction (get_last_floor ())); 
		break;
		}
	}
}

void sm_stop_button_pressed (void) {
	elev_set_motor_direction (DIRN_STOP);  //does not change last_dir
	curr_state = state_stop;
	elev_set_stop_lamp (1);
	for (int i = 0; i < 4; i++) {
		remove_light_and_order_for_floor (i);
	}
	if (elev_get_floor_sensor_signal () != -1) {
		elev_set_door_open_lamp (1);
	}
	while (elev_get_stop_signal ()){
	}
	elev_set_stop_lamp (0);
}
	


void sm_open_door_and_enter (void) { 		//when stopped
	remove_light_and_order_for_floor (elev_get_floor_sensor_signal ()); //sjekk om fjerne denna eller på idle
	switch (curr_state) {
		case state_idle: {
		curr_state = state_entering;
		elev_set_motor_direction (0);
		elev_set_door_open_lamp (1);
		timer_start ();		
		break;
		}
		
		case state_move: {
		curr_state = state_entering;
		elev_set_motor_direction (0);
		elev_set_door_open_lamp (1);
		timer_start ();			
		break;
		}
	}
}








	
