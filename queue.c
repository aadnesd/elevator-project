#include "elev.h"
#include "io.h"
#include "channels.h"
#include <stdio.h>	
#include "queue.h"

/*  Matrise med dei tre forskjellige knappane for alle fire etasjar, lagrar bestillingane  */
static int buttons[3][4] = { { 0 },{ 0 },{ 0 } };

/*  Siste etasje heisen har vore i, gjer ingenting om heisen er mellom to etasjar  */
static int last_floor;

/*  Nåværande etasje (inkludert -1 dersom heisen er mellom to etasjar)  */
static int curr_floor;

/*  Blir endra kvar gong heisen endrar retning (dersom heisen stoppar blir den ikkje endra, så den held på førre retning)  */
static int last_dir;


/*  Set ein knapp inn i matrisa forbestillingar  */
void set_in_buttons (elev_button_type_t button, int floor, int value) {  
	buttons[button][floor] = value;
}

/*  Sjekkar om nokon knappar er trykt, set dei inn i matrisa og skrur på bestillingsljoset om dei er det.  */
void button_check_and_set_lamp (void) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			if ((i == 1 && j == 0) || (i == 0 && j == 3)) {  /*  Unngår å iterere gjennom knappane som ikkje finnast, opp i 4. og ned i 3. etg  */
				continue;
			}
			if (elev_get_button_signal (i, j) == 1) {
				set_in_buttons (i, j, 1);
				elev_set_button_lamp (i, j, 1);
			}

		}
	}
}

/*  Fjernar ljos og bestilling for etasjen  */
void remove_light_and_order_for_floor (int floor) {
	if (floor >= 0) {
		for (int i = 0; i < 3; i++) {
			if ((i == 1 && floor == 0) || (i == 0 && floor == 3)) {  /*  Unngår å iterere gjennom knappane som ikkje finnast  */
				continue;
			}
			set_in_buttons (i, floor, 0);
			elev_set_button_lamp (i, floor, 0);
		}
	}
}


/*  Set siste etasje heisen har vore i, gjer ingenting om heisen er mellom to etasjar  */
void set_last_floor (void) {
	if (elev_get_floor_sensor_signal () >= 0) {
		last_floor = elev_get_floor_sensor_signal ();
	}
}


int get_last_dir (void) {
	return last_dir;
}


int get_last_floor (void) {
	return last_floor;
}


/*  Set nåværande etasje (inkludert til -1 dersom heisen er mellom to etasjar)  */
void set_curr_floor (void) {			
	curr_floor = elev_get_floor_sensor_signal ();
}

int get_curr_floor (void) {
	return curr_floor;
}


/*  Sjekkar om heisen skal stoppe der han er  */
int should_stop (int last_floor, int direction) {

	/*  Sjekkar antal bestillingar  */
	int sum = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			sum += buttons[i][j];
		}
	}

	/*  Ikkje stopp mellom etasjar  */
	if (get_curr_floor () == -1) {  
		return 0;
	}

	/*  Stopp dersom kommandoknappen er trykt der han er */
	else if (buttons[2][last_floor] == 1) {
		return 1;
	}
	
	/*  Stopp dersom det ikkje er fleire bestillingar over der han er og han er på veg opp  */
	else if ((is_order_above(last_floor) != 1) && (direction == 1)) {
		return 1;
	}

	/*  Stopp dersom det ikkje er fleire bestillingar under der han er og han er på veg ned  */
	else if ((is_order_below(last_floor) != 1) && (direction == -1)) {
		return 1;
	}

	/*  Stopp dersom han er på veg opp og det er ein bestilling opp der han er  */
	else if (direction == 1 && buttons[0][last_floor] == 1) {
		return 1;
	}

	/*  Stopp dersom han er på veg ned og det er ein bestilling ned der han er  */
	else if (direction == -1 && buttons[1][last_floor] == 1) {
		return 1;
	}

	return 0;
}

/*  Vel retning han skal gå  */
int choose_direction (int last_floor) {

	/*  Sjekkar om kommandoknapp bestilt over eller under, returnerer retninga han skal  */
	for (int j = 0; j < 4; j++) {
		if (buttons[2][j] == 1) {
			if (j > last_floor) {
				last_dir = DIRN_UP;
				return DIRN_UP;
			}
			else {
				last_dir = DIRN_DOWN;
				return DIRN_DOWN;
			}
		}
	}

	/*  Sjekkar om bestilt over der han er, returnerer retninga han skal  */
	for (int i = last_floor + 1; i < 4; i++) {
		if (buttons[0][i] == 1 || buttons[1][i] == 1) {
			last_dir = DIRN_UP;
			return DIRN_UP;

		}
	}

	/*  Sjekkar om bestilt over der han er, returnerer retninga han skal  */
	for (int i = last_floor - 1; i >= 0; i--) {
		if (buttons[0][i] == 1 || buttons[1][i] == 1) {
			last_dir = DIRN_DOWN;
			return DIRN_DOWN;
		}
	}

	return DIRN_STOP;
}

/*  Vel retning han skal etter at stoppknappen har vore trykt, må ta hensyn til retning før han blei stoppa  */
int choose_direction_after_stop (int last_floor) {
	for (int j = 0; j < 4; j++) {
		if (buttons[0][j] == 1 || buttons[1][j] == 1 || buttons[2][j] == 1) {
			if (j == last_floor) {  /*  Dersom den fyrste bestillinga etter stopp er til den siste etasjen han var i før stopp, må heisen snu  */
				if (last_dir == DIRN_UP) {
					last_dir = DIRN_DOWN;
					return DIRN_DOWN;
				}
				else if (last_dir == DIRN_DOWN) {
					last_dir = DIRN_UP;
					return DIRN_UP;
				}
			}
		}
	}
	choose_direction (last_floor);  /*  Kan bruke den vanlege funksjonen så lenge ikkje nye bestillingen er til siste etasje før stopp  */
	return 0;
}


/*  Sjekkar om det er bestillingar over der han er  */
int is_order_above(int floor) {
	e
		for (int i = 0; i < 3; i++) {
			for (int j = floor + 1; j < 4; j++) {
				if (buttons[i][j] == 1) {
					return 1;
				}
			}
		}
	return 0;
}


/*  Sjekkar om det er bestillingar under der han er  */
int is_order_below (int floor) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < floor; j++) {
			if (buttons[i][j] == 1) {
				return 1;
			}
		}
	}
	return 0;
}









