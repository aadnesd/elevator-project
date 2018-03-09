#include "elev.h"
#include "state_machine.h"
#include "orders.h"
#include <stdio.h>
#include "queue.h"
#include "timer.h"


int main()
{
	// Initialize hardware
	if (!elev_init ()) {
		printf("Unable to initialize elevator hardware!\n");
		return 1;
	}

	printf("Press STOP button to stop elevator and exit program.\n");




	/*  Precondition (heisen går til definert tilstand)  */

	sm_init ();


	while (1) {


		button_check_and_set_lamp ();

		/*  Set på indikatorljos når heisen kjem til ny etasje  */
		if (elev_get_floor_sensor_signal () >= 0) {
			elev_set_floor_indicator (elev_get_floor_sensor_signal ());
		}

		/*  Set forrige etasje (gjer ingenting om heisen er mellom 2 etasjar)  */
		set_last_floor ();

		/*  Set nåværande etasje (blir sett til -1 om heiser er i mellom 2 etasjarar  */
		set_curr_floor ();

		//Sjekkar om stoppknappen er trykt
		if (elev_get_stop_signal ()) {
			sm_stop_button_pressed ();
			timer_start ();
		}

		//Sjekkar om heisen skal stoppe der den er
		if (should_stop (get_last_floor (), get_directionm ())) {
			sm_open_door_and_enter ();
		}


		if (timer_out () == 1) {
			sm_idle();
		}

		if (timer_running () == 0) {
			if (choose_direction (get_last_floor ()) != DIRN_STOP) {   //Bare dersom noko er bestilt
				sm_move ();
			}
		}
	}
	return 0;
}