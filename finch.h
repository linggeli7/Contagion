#ifndef FINCH_H
#define FINCH_H

#include "constants.h"
#include "agent.h"
#include <cmath>
#include <cassert>
#include "helper_fcns.h"
#include <cstdlib>

class finch : public agent {
private:
        
public:
    finch(double x, double y,void (* up)(agent*,agent*));
    
    
	double NearestPred_count;
	void iterate_NearestPred();

	double q_change_prey;
	double q_change_pred;
	double x_accel_pred;
	double y_accel_pred;
	
	
	double x_accel_prey;
	double y_accel_prey;

	void add_to_x_accel_prey(double add_to_accel);
    void add_to_y_accel_prey(double add_to_accel);
	void add_to_x_accel_pred(double add_to_accel);
    void add_to_y_accel_pred(double add_to_accel);

	virtual void normalize_accel();
	virtual void drag();
	virtual void ab4_update();
	virtual void euler_update();


	double gamma;
	double v_align_mag;
	double pred_repel;
	double py_pd_kinetic;
	double py_pd_emote;
	double scs;
	double decay;
};
#endif 
