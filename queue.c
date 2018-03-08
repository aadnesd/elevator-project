#include "elev.h"
#include "io.h"
#include "channels.h"
#include <stdio.h>	


static int buttons[3][4]={{0},{0},{0}};
static int last_floor;
static int curr_floor;
static int last_dir;




void set_in_buttons(elev_button_type_t button, int floor, int value){  // sets the button to ordered 
	buttons[button][floor] = value;
}


void button_check_and_set_lamp(void){
	
	for(int i = 0; i<3; i++){
			for(int j = 0; j<4; j++){
				if((i==1 && j==0) || (i==0 && j==3)){
					continue;
				}
				if(elev_get_button_signal(i,j)==1){  
					set_in_buttons(i,j,1);
					elev_set_button_lamp(i,j,1);
				}
				
			}
	}
}


void remove_light_and_order_for_floor(int floor){
	
	
	
	if(floor>=0){
		for(int i = 0; i<3; i++){
			if((i==1 && floor==0) || (i==0 && floor==3)){  // avoids checking down in first and up in fourth
					continue;
			}
			set_in_buttons(i, floor, 0);
			elev_set_button_lamp(i,floor,0);
		}
	}
}


void set_last_floor(void){
	if(elev_get_floor_sensor_signal()>=0){
		last_floor = elev_get_floor_sensor_signal();
	}
}

int get_last_dir(void){
	return last_dir;
}

int get_last_floor(void){
	return last_floor;
}


void set_curr_floor(void){			//include -1 between floors, so that it doesn't stop for new order at prev floor
	curr_floor = elev_get_floor_sensor_signal();
}

int get_curr_floor(void){
	return curr_floor;
}

int should_stop(int last_floor, int direction){
	int sum = 0;
	for(int i = 0; i<3;i++){
		for(int j=0; j<4; j++){
		 sum += buttons[i][j];
		}
	}
			
	if(get_curr_floor()==-1){  //don't stop in between floors
		return 0;
	}

	else if(buttons[2][last_floor]==1 ){
		return 1;
	}
	//else if( ( (sum==1) || (sum==2) || (sum ==3) ) && ( (buttons[0][last_floor]==1) || (buttons[1][last_floor]==1) ) ) {
	//	return 1;
	//} 
	else if((is_order_above(last_floor) != 1) && (direction ==1)){
		return 1;
	}
	
	else if((is_order_below(last_floor) != 1) && (direction ==-1)){
		return 1;
	}
	
	


	else if(direction == 1 && buttons[0][last_floor]==1){
				return 1;
	}


	else if(direction == -1 && buttons[1][last_floor]==1){
				return 1;
	}
		
	return 0;
}


int choose_direction(int last_floor){
	
	for(int j = 0; j<4;j++){
		if(buttons[2][j]==1){
			if(j>last_floor){
				//elev_set_motor_direction(DIRN_UP);    //enten returnere 0 eller direction
				last_dir = DIRN_UP;
				return DIRN_UP;
			}
			else{
				//elev_set_motor_direction(DIRN_DOWN);
				last_dir = DIRN_DOWN;
				return DIRN_DOWN;
			}
		}
	}
	
	for(int i=last_floor+1; i<4 ; i++){
			if(buttons[0][i]==1 || buttons[1][i]==1){
				//elev_set_motor_direction(DIRN_UP);
				last_dir = DIRN_UP;
				return DIRN_UP;
				
			}
	}
	
	for(int i=last_floor-1; i>=0 ; i--){
			if(buttons[0][i]==1 || buttons[1][i]==1){
				last_dir = DIRN_DOWN;
				return DIRN_DOWN;
			}
	}

	
	return DIRN_STOP;

}


int choose_direction_after_stop(int last_floor){
	printf("After stop");
	for(int j = 0; j<4;j++){
		if(buttons[0][j]==1 || buttons[1][j]==1 || buttons[2][j]==1){
			if(j == last_floor){ //if ordering last floor
				if(last_dir == DIRN_UP){
					printf("Jepp");
					last_dir = DIRN_DOWN;
					return DIRN_DOWN;
				}
				else if (last_dir == DIRN_DOWN){
					printf("Jepp2");
					last_dir = DIRN_UP;
					return DIRN_UP;
				}
			}
		}
	}
	choose_direction(last_floor);
	return 0;
}


int is_order_above(int floor){	
	for(int i = 0; i < 3; i++){
		for(int j = floor + 1; j < 4; j++){
			if(buttons[i][j] == 1){
				return 1;
			}
		}
	}
	return 0;
}

int is_order_below(int floor){
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < floor; j++){
			if(buttons[i][j] == 1){
				return 1;
			}
		}
	}
	return 0;
}








