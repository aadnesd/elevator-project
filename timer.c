#include <stdio.h>
#include <sys/time.h>

static double timer_end_time;
static int running;



double get_wall_time (void) {
    struct timeval time;
    gettimeofday (&time, NULL);
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}


void timer_start (void) {
    timer_end_time=get_wall_time () + 3;
    running = 1;
}

int timer_running (void) {
    return running;
}

int timer_out (void) {
    if (timer_end_time == 0) {
        return 0;
    }
    if (timer_end_time <= get_wall_time ()) {
        timer_end_time = 0;
        running = 0;
	return 1;
    }
    else {
        return 0;
    } 
}






