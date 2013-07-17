#include "finch.h"


 finch::finch(double x, double y, void (* up)(agent*,agent*)) {
  //Assign Coordinates
	    x_coord = x;
	    for(int i = 0; i < HIST_LENGTH; i++)
	        x_veloc[i] = 0;
	    for(int i = 0; i < HIST_LENGTH; i++)
	        x_accel[i] = 0;
	    
	    y_coord = y;
	    for(int i = 0; i < HIST_LENGTH; i++)
	        y_veloc[i] = 0;
	    for(int i = 0; i < HIST_LENGTH; i++)
	        y_accel[i] = 0;
	    
	    z_coord = 0;
	    for(int i = 0; i < HIST_LENGTH; i++)
	        z_veloc[i] = 0;
	    for(int i = 0; i < HIST_LENGTH; i++)
	        z_accel[i] = 0;
	    
	    //Assign Contagion
	    q_mag = 0;
	    for(int i = 0; i < HIST_LENGTH; i++)
	        q_change[i] = 0;
	    
	    //Assign NN
	    NearestNeighbor_count = 0;
	    
	    agent_type = 1;
	    
	    update = up;
	    
	    alive = true;

		heading = 0;

	    
} 
void finch::iterate_NearestPred(){
    
    NearestPred_count++;
    
}

void finch::add_to_x_accel_pred(double add_to_accel){
    
    x_accel_pred += add_to_accel;
    
}
void finch::add_to_y_accel_pred(double add_to_accel){
    
    y_accel_pred += add_to_accel;
    
}

void finch::add_to_x_accel_prey(double add_to_accel){
    
    x_accel_prey += add_to_accel;
    
}
void finch::add_to_y_accel_prey(double add_to_accel){
    
    y_accel_prey += add_to_accel;
    
}


void finch::normalize_accel()
{
	if(NearestNeighbor_count != 0)
      {
        x_accel_prey =  (x_accel_prey / (double) NearestNeighbor_count);
		y_accel_prey =  (y_accel_prey / (double) NearestNeighbor_count);
		x_accel_pred =  (x_accel_pred / (double) NearestPred_count);
		y_accel_pred =  (y_accel_pred / (double) NearestPred_count);


		q_change_prey = (q_change_prey / (double) NearestNeighbor_count);


		x_accel[0] =  py_pd_kinetic * x_accel_prey + (1- py_pd_kinetic) * x_accel_pred;
		y_accel[0] =  py_pd_kinetic * y_accel_prey + (1- py_pd_kinetic) * y_accel_pred;
		
		q_change[0] = py_pd_emote * q_change_prey + (1 - py_pd_emote) *q_change_pred;
    }
}

void finch::drag()
{
    double veloc_mag = x_veloc[0]*x_veloc[0] + y_veloc[0]*y_veloc[0];
	double A = alpha_1*4*pow((q_mag - q_mag * scs + scs), 2);
	x_accel[0] += (A - veloc_mag)*x_veloc[0];
	y_accel[0] += (A - veloc_mag)*y_veloc[0];
	q_change[0] += -q_mag * decay;
    NearestNeighbor_count = 0;
} 

void finch::ab4_update()
{
    assert(HIST_LENGTH >= 4);
    
    double forward_v_x = x_veloc[0] +
                ( (STEP_SIZE * (1.0/24.0)) * 
                 ((55 * x_accel[0]) -
                  (59 * x_accel[1]) +
                  (37 * x_accel[2]) -
                  (9  * x_accel[3])));
    
    double forward_v_y = y_veloc[0] +
                ( (STEP_SIZE * (1.0/24.0)) * 
                 ((55 * y_accel[0]) -
                  (59 * y_accel[1]) +
                  (37 * y_accel[2]) -
                  (9  * y_accel[3])));

    if(abs(forward_v_x) > abs(forward_v_y))
	{
		if(forward_v_x > 0)
		{
			heading = 2;
		}
		else
		{
			heading = 4;
		}
	}
	else
	{
		if(forward_v_y > 0)
		{
			heading = 1;
		}
		else
		{
			heading = 3;
		}
	}

    x_coord = x_coord +
                ( (STEP_SIZE * (1.0/24.0)) * 
                  ((55 * forward_v_x) -
                   (59 * x_veloc[0]) +
                   (37 * x_veloc[1]) -
                   (9  * x_veloc[2])));
    
    y_coord = y_coord +
                ( (STEP_SIZE * (1.0/24.0)) *
                  ((55 * forward_v_y) -
                   (59 * y_veloc[0]) +
                   (37 * y_veloc[1]) -
                   (9  * y_veloc[2])));

	q_mag = q_mag +
                ( (STEP_SIZE * (1.0/24.0)) *
                  ((55 * q_change[0]) -
                   (59 * q_change[1]) +
                   (37 * q_change[2]) -
                   (9  * q_change[3])));
    
    //Move history forward.
    
    for(int i = HIST_LENGTH - 1; i > 0; --i){
        
        x_veloc[i] = x_veloc[i-1];
        x_accel[i] = x_accel[i-1];
        y_veloc[i] = y_veloc[i-1];
        y_accel[i] = y_accel[i-1];
		
        q_change[i] = q_change[i-1];

    }
    
    x_veloc[0] = forward_v_x;
    y_veloc[0] = forward_v_y;
    x_accel[0] = 0;
    y_accel[0] = 0;

	q_change[0] = 0;
    

	x_accel_prey =  0;
	y_accel_prey = 0;

	q_change_prey = 0;
	q_change_pred = 0;
}