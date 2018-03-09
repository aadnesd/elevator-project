
//static int buttons[3][4]={{0},{0},{0}};


void set_in_buttons(elev_button_type_t button, int floor, int value);
void printut(void);
void remove_light_and_order_for_floor(int floor);
int choose_direction(int last_floor);
void set_curr_floor(void);
int get_curr_floor(void);
int get_last_floor(void);
int get_last_dir(void);
int is_order_above(int floor);
int is_order_below(int floor);
