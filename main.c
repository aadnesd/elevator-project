#include "elev.h"
#include "state_machine.h"
#include "orders.h"
#include <stdio.h>
#include "queue.h"
#include "timer.h"


int main() {
    // Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }

    printf("Press STOP button to stop elevator and exit program.\n");

    //elev_set_motor_direction(DIRN_STOP); //endra frå up
	

	//precondition (definert tilstand)
	
	sm_init();
	//elev_set_motor_direction(DIRN_UP);
	
	//elev_set_motor_direction(DIRN_DOWN);
	
    	
    while (1) {
    	
    	
    	button_check_and_set_lamp();
    	if(elev_get_floor_sensor_signal()>=0){
			elev_set_floor_indicator(elev_get_floor_sensor_signal());
			
		}
    	print_state();
    	//printf("Last floor: %d \n", get_last_floor());
    	set_last_floor();
    	set_curr_floor();
    	//sm_move();
    	
    	if(elev_get_stop_signal()){
    		sm_stop_button_pressed();
    		timer_start();
    	}
    	
    	//if (timer_running==0){
    		if(should_stop(get_last_floor(),get_direction())){
    			sm_open_door_and_enter();
    		}
    	//}
    	
    	if(timer_out()==1){
    		sm_idle();
    	}
    	
    	if(timer_running()==0){
    		if(choose_direction(get_last_floor()) != DIRN_STOP){   //only if something ordered
    			sm_move();
    		}
    	}

		/*
		if(timer_out_2()==1){
			lifeboat();
		}    	
    	*/
    	
    		
    
    
    	
    	
    	
    	/**
    	
		//move_to_floor();
		
		button_check_and_set_lamp();
		remove_light_and_order_for_floor(elev_get_floor_sensor_signal());
		set_last_floor();
		
		elev_set_motor_direction(choose_direction(get_last_floor()));
			
		

		printf("Choose dir: %d \n", choose_direction(get_last_floor()));
		printf("Last floor: %d \n", get_last_floor());
		//printf("%d \n", get_last_floor());
		printf("%d \n",should_stop(get_last_floor(),get_direction()));

		//set floor light indicator
		if(elev_get_floor_sensor_signal()>=0){
			elev_set_floor_indicator(elev_get_floor_sensor_signal());
			
		}
		
		//choose_direction(int last_floor)
		
		
		

        // Change direction when we reach top/bottom floor
        if (elev_get_floor_sensor_signal() == N_FLOORS - 1) {
            elev_set_motor_direction(DIRN_DOWN);
        } else if (elev_get_floor_sensor_signal() == 0) {
            elev_set_motor_direction(DIRN_UP);
        }

        // Stop elevator and exit program if the stop button is pressed
        if (elev_get_stop_signal()) {
            elev_set_motor_direction(DIRN_STOP);
            break;
        }
        
    }
*/
    
    
    }
    return 0;
}

