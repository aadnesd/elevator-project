#include "elev.h"
#include "io.h"
#include "channels.h"



/*  Matrise med dei tre forskjellige knappane for alle fire etasjar, lagrar bestillingane  */
static int buttons[3][4] = { { 0 },{ 0 },{ 0 } };


/*  Set ein knapp inn i matrisa forbestillingar  */
void set_in_buttons(elev_button_type_t button, int floor, int value) {
	buttons[button][floor] = value;
}


/*  Sjekkar om nokon knappar er trykt, set dei inn i matrisa og skrur på bestillingsljoset om dei er det.  */
void button_check_and_set_lamp(void) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			if ((i == 1 && j == 0) || (i == 0 && j == 3)) {  /*  Unngår å iterere gjennom knappane som ikkje finnast, opp i 4. og ned i 3. etg  */
				continue;
			}
			if (elev_get_button_signal(i, j) == 1) {
				set_in_buttons(i, j, 1);
				elev_set_button_lamp(i, j, 1);
			}

		}
	}
}


/*  Fjernar ljos og bestilling for etasjen  */
void remove_light_and_order_for_floor(int floor) {
	if (floor >= 0) {
		for (int i = 0; i < 3; i++) {
			if ((i == 1 && floor == 0) || (i == 0 && floor == 3)) {  /*  Unngår å iterere gjennom knappane som ikkje finnast  */
				continue;
			}
			set_in_buttons(i, floor, 0);
			elev_set_button_lamp(i, floor, 0);
		}
	}
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
int is_order_below(int floor) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < floor; j++) {
			if (buttons[i][j] == 1) {
				return 1;
			}
		}
	}
	return 0;
}