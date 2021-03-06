/*
 
 This is the World class header for the Contagion Project, UCLA REU 2013.
 
 062413 1150 Created. (DM)
 
 
 */


#ifndef WORLD_H
#define WORLD_H

#include "constants.h"
#include "agent.h"
#include "predator.h"
//#include "finch.h"
#include "cell.h"
#include <vector>
//#include "helper_fcns.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <cstddef>

void ab4_update(agent smith);
void euler_update(agent &smith);

using std::vector;

class world {
public:
    world();
    ~world();
	vector<agent*> agents_master;
    cell* cellList[DOMAIN_DIM_1][DOMAIN_DIM_2];
    cell* theLonelyIsland; //no agent is an island.
    
    void add_agent(double x, double y, void (* up)(agent*,agent*));
    void add_predator(double x, double y, void (* up)(agent*,agent*));
//	void add_finch_rand(double x, double y, void (* up)(agent*,agent*));

	void populate(int n, vector<double>& xpos, vector<double>& ypos, void (* up)(agent*,agent*));
    void populate_rand(int n, void (* up)(agent*,agent*));
    void populate_predator_rand(int n, void (* up)(agent*,agent*));
//	void populate_finches_rand(int n, void (* up)(agent*,agent*));

	void run(std::ostream& strm, int print_every, int iterations);

    cell* get_cell(int i, int j);
    int is_empty(int i, int j);
    

	void update_forward_velocs();
    void update_agent_pos_euler();
    void update_agent_pos_ab4();
	void move_to_cell();
	void print(std::ostream& strm);
	void print_csv(std::string filename);

 private:
    
};


#endif

