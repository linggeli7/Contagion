#include "helper_fcns.h"
#include <cmath>

void swarm1(agent* me, agent* you)
{
	//get x,y,z coords
	double x1 = (*me).get_x_coord();
	double y1 = (*me).get_y_coord();
	double z1 = (*me).get_z_coord();
	double x2 = (*you).get_x_coord();
	double y2 = (*you).get_y_coord();
	double z2 = (*you).get_z_coord();

	//get component-wise velocity
	double vx1 = (*me).get_x_veloc_index(0);
	double vy1 = (*me).get_y_veloc_index(0);
	double vz1 = (*me).get_z_veloc_index(0);
	double vx2 = (*you).get_x_veloc_index(0);
	double vy2 = (*you).get_y_veloc_index(0);
	double vz2 = (*you).get_z_veloc_index(0);

	//calculate distances
	double dx= x2-x1;
	double dy= y2-y1;
	double dz= z2-z1;
	double r = sqrt(dx*dx + dy*dy + dz*dz);
	double sum = abs(dx)+abs(dy)+abs(dz);

	//calculate attraction/repulsion forces
	double u = -C_A * exp(-r / L_A) + C_R * exp(-r / L_R);

	//calculate alignment forces
	double h = KAPPA / pow((SIGMA*SIGMA + r*r), GAMMA);

	//update velocities
	double fx = u*dx/sum + h*dx;
	double fy = u*dy/sum + h*dy;
	double fz = u*dz/sum + h*dz;
	(*me).set_forward_v_x((*me).get_forward_v_x()+fx);
	(*me).set_forward_v_y((*me).get_forward_v_y()+fy);
	(*me).set_forward_v_z((*me).get_forward_v_z()+fz);
	(*you).set_forward_v_x((*you).get_forward_v_x()+fx);
	(*you).set_forward_v_y((*you).get_forward_v_y()+fy);
	(*you).set_forward_v_z((*you).get_forward_v_z()+fz);

}