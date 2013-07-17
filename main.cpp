#include "world.h"
#include "helper_fcns.h"
#include <fstream>
#include <ctime>
#include <string>
#include <iostream>


int main()
{
	//Set file path for output
	std::ofstream strm;
	std::string filepath = "C:\\Users\\dougyd\\Desktop\\simple.txt";
	std::cout << "Data printed to " << filepath;
	strm.open(filepath.c_str());
	
	//setup
	time_t t = time(NULL);
	srand (t);
	
	//initialize world
	world w = world();
	w.populate_rand(5, swarm1);
	//w.populate_finches_rand(45, finch1);
	//w.populate_predator_rand(1, predator_2012);

	w.run(strm, 100, 1000);

	/*
    int jump_val = 4;
    int print_val = 200;
	*/
	//give some emotion!
	/*(w.agents_master[5])->set_q_mag(1.0);
	(w.agents_master[15])->set_q_mag(1.0);
	(w.agents_master[25])->set_q_mag(1.0);
	(w.agents_master[10])->set_q_mag(1.0);
	(w.agents_master[20])->set_q_mag(1.0);
	(w.agents_master[12])->set_q_mag(1.0);
	(w.agents_master[74])->set_q_mag(1.0);
	(w.agents_master[22])->set_q_mag(1.0);
	(w.agents_master[30])->set_q_mag(1.0);
	(w.agents_master[69])->set_q_mag(1.0);
	(w.agents_master[1])->set_q_mag(1.0);
	(w.agents_master[2])->set_q_mag(1.0);
	*/

	//solve initial data with euler's method

	/*
    for(int i = 1; i <= jump_val; i++)
	{
        w.update_forward_velocs();
		w.update_agent_pos_euler();
		w.move_to_cell();
		if(i % print_val == 0){
			std::cout << i << " " << time(NULL) - t << " total seconds elapsed" << "\n";
			w.print(str);
		}
		//w.print(str);
		//w.print(std::cout);
    }

	//solve using AB4
	for(int i = jump_val + 1; i <=100000; i++)
	{
		w.update_forward_velocs();
		w.update_agent_pos_ab4();
		w.move_to_cell();
		if(i % print_val == 0){
			std::cout << i << " " << time(NULL) - t << " total seconds elapsed" << "\n";
			w.print(str);
		}
		//w.print(str);
		//w.print(std::cout);
	}

	*/

	//cleanup
	
	strm.close(); // cleanup
	std::cout << "Data printed to " << filepath << "\n";
	system("PAUSE");
	return 0;
}
