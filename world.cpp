#include "world.h"




//default constructor
world::world()
{
//create the cells
  for (int i = 0; i < DOMAIN_DIM_1; i++)
    {
      for(int j = 0; j < DOMAIN_DIM_2; j++) 
        {
	  cellList[i][j] = new cell;
        }
    }
  

  //tell the cells who their neighbors are
  for (int i = 0; i < DOMAIN_DIM_1; i++)
    {
        for(int j = 0; j < DOMAIN_DIM_2; j++) 
        {

            vector<cell*> v(8);
            v[0] = cellList[i-1][j-1];
            v[1] = cellList[i][j-1];
            v[2] = cellList[i+1][j-1];

            v[3] = cellList[i-1][j];
            v[4] = cellList[i+1][j];

            v[5] = cellList[i-1][j+1];
            v[6] = cellList[i][j+1];
            v[7] = cellList[i+1][j+1];

            if(i == 0)
            {
                v[0]=v[3]=v[5]=NULL;
            }
             if((i+1) == DOMAIN_DIM_1)
            {
                v[2]=v[4]=v[7]=NULL;
            }
            if(j == 0)
            {
                v[0]=v[1]=v[2]=NULL;
            }
             if((j+1) == DOMAIN_DIM_2)
            {
                v[5]=v[6]=v[7]=NULL;
            }
            
            cellList[i][j]->set_neighbors(v);
        }
    }
	theLonelyIsland = new cell;
}

world::~world(){
    
    for(int i = 0; i < DOMAIN_DIM_1; i++){
        for(int j = 0; j < DOMAIN_DIM_2; j++){
            delete cellList[i][j];
        }
    }
    
	delete theLonelyIsland;

}




void world::update_forward_velocs(){
	//Iterates through all cells and does pairwise comparisons
    
    cell* origin = cellList[0][0];
    cell* end = origin + (DOMAIN_DIM_1 * DOMAIN_DIM_3);
    

	//Same-cell comparison
    for(int i = 0; i < DOMAIN_DIM_1; i++){
      for(int j = 0; j < DOMAIN_DIM_2; j++){
	
	cell_node_iterator target = cellList[i][j]->get_iter();
	cell_node_iterator target2 = cellList[i][j]-> get_iter();
        
	for(target; target.current != NULL; target.next()){
	  
	  for(target2; target2.current != NULL; target2.next()){
	    if(target2.current == target.current)
	      break;
	    
	    target.current->get_agent()->update(target.current->get_agent(), target2.current->get_agent());
	    target2.current->get_agent()->update(target2.current->get_agent(), target.current->get_agent());
        //Do comparisons each way, since update rules might be different
	  }
          
	  target2.reset_current();
          
	}    
        
      }
    }
    
    /*
     THIS IS WHERE WE'D PUT OPEN MP
     */
    //UPDATE RIGHT
    /*
    for(cell* iter = origin; iter < end; iter++)
    {
        
        if(iter->get_neighbor(4) == NULL)
            continue;
        
        cell_node_iterator target = iter->get_iter();
        cell_node_iterator right = iter->get_neighbor(4) -> get_iter();
        
        for(target; target.current != NULL; target.next()){
            
            for(right; right.current != NULL; right.next()){
                
                target.current->get_agent()->update(target.current->get_agent(), right.current->get_agent());
                right.current->get_agent()->update(right.current->get_agent(), target.current->get_agent());

            }
            
            right.reset_current();
            
        }
        
    }*/
    

	
	//Compare to cell to the right
    for(int i = 0; i < DOMAIN_DIM_1; i++){
        for(int j = 0; j < DOMAIN_DIM_2; j++){
        
            if( cellList[i][j]->get_neighbor(4) == NULL )
                           continue;
        
            cell_node_iterator target = cellList[i][j]->get_iter();
            cell_node_iterator right = cellList[i][j]->get_neighbor(4) -> get_iter();
            
            for(target; target.current != NULL; target.next()){
                
                for(right; right.current != NULL; right.next()){
                    
                    target.current->get_agent()->update(target.current->get_agent(), right.current->get_agent());
                    right.current->get_agent()->update(right.current->get_agent(), target.current->get_agent());
                    
                }
                
                right.reset_current();
                
            }    
        
        }
    }
    
    //UPDATE UP
    /*
    end = origin + (DOMAIN_DIM_1);
    for(cell* iter = origin; iter < end; iter++)
    {
        
        for(cell* inner_iter = iter; inner_iter < end; inner_iter += DOMAIN_DIM_1){
            if(inner_iter->get_neighbor(6) == NULL)
                continue;
        
            cell_node_iterator target = inner_iter->get_iter();
            cell_node_iterator upwards = inner_iter->get_neighbor(6) -> get_iter();
            
            for(target; target.current != NULL; target.next()){
            
                for(upwards; upwards.current != NULL; upwards.next()){
                    
                    target.current->get_agent()->update(target.current->get_agent(), upwards.current->get_agent());
                    upwards.current->get_agent()->update(upwards.current->get_agent(), target.current->get_agent());

                
                }
            
                upwards.reset_current();
            
            }
        }
    }*/
    
	//Compare to cell below
    for(int i = 0; i < DOMAIN_DIM_1; i++){
        for(int j = 0; j < DOMAIN_DIM_2; j++){
            
            if(cellList[i][j]->get_neighbor(6) == NULL)
                continue;
            
            cell_node_iterator target = cellList[i][j]->get_iter();
            cell_node_iterator upwards = cellList[i][j]->get_neighbor(6) -> get_iter();
            
            for(target; target.current != NULL; target.next()){
                
                for(upwards; upwards.current != NULL; upwards.next()){
                    
                    target.current->get_agent()->update(target.current->get_agent(), upwards.current->get_agent());
                    upwards.current->get_agent()->update(upwards.current->get_agent(), target.current->get_agent());
                    
                    
                }
                
                upwards.reset_current();
                
            }
        }
    }
    
    
    /*
    //UPDATE Right DIAG
    //along first dimension
    for(int off_diag = 0; off_diag < DOMAIN_DIM_1; off_diag++)
    {
        cell* current = origin + off_diag;
        int runs = DOMAIN_DIM_1 - off_diag;
        for(int i = 0; i < runs; i++){
            if(current->get_neighbor(7) == NULL)
                continue;
            
            cell_node_iterator target = current->get_iter();
            cell_node_iterator upwards = current->get_neighbor(7) -> get_iter();
            
            for(target; target.current != NULL; target.next()){
                
                for(upwards; upwards.current != NULL; upwards.next()){
                    
                    target.current->get_agent()->update(target.current->get_agent(), upwards.current->get_agent());
                    upwards.current->get_agent()->update(upwards.current->get_agent(), target.current->get_agent());
                    
                }
                
                upwards.reset_current();
            }
            current += DOMAIN_DIM_1 + 1;
        }
    }
    //intermedius along second dimension
    for(int intermedius = 1; intermedius <= DOMAIN_DIM_2 - DOMAIN_DIM_1; intermedius++){
        cell* current = origin + (intermedius * DOMAIN_DIM_1);
        int runs = DOMAIN_DIM_1;
        for(int i = 0; i < runs; i++){
            if(current->get_neighbor(7) == NULL)
                continue;
            
            cell_node_iterator target = current->get_iter();
            cell_node_iterator upwards = current->get_neighbor(7) -> get_iter();
            
            for(target; target.current != NULL; target.next()){
                
                for(upwards; upwards.current != NULL; upwards.next()){
                    
                    target.current->get_agent()->update(target.current->get_agent(), upwards.current->get_agent());
                    upwards.current->get_agent()->update(upwards.current->get_agent(), target.current->get_agent());
                    
                }
                
                upwards.reset_current();
            }
            current += DOMAIN_DIM_1 + 1;
        }
    }
    //along second dimension
    for(int off_diag = DOMAIN_DIM_2 - DOMAIN_DIM_1 + 1; off_diag < DOMAIN_DIM_2; off_diag++)
    {
        cell* current = origin + (off_diag * DOMAIN_DIM_1);
        int runs = DOMAIN_DIM_2 - off_diag;
        for(int i = 0; i < runs; i++){
            if(current->get_neighbor(7) == NULL)
                continue;
            
            cell_node_iterator target = current->get_iter();
            cell_node_iterator upwards = current->get_neighbor(7) -> get_iter();
            
            for(target; target.current != NULL; target.next()){
                
                for(upwards; upwards.current != NULL; upwards.next()){
                    
                    target.current->get_agent()->update(target.current->get_agent(), upwards.current->get_agent());
                    upwards.current->get_agent()->update(upwards.current->get_agent(), target.current->get_agent());
                    
                }
                
                upwards.reset_current();
            }
            current += DOMAIN_DIM_1 + 1;
        }
    }
     */


	//Compare to cell to the bottomn right
    for(int i = 0; i < DOMAIN_DIM_1; i++){
        for(int j = 0; j < DOMAIN_DIM_2; j++){
            
            if(cellList[i][j]->get_neighbor(7) == NULL)
                continue;
            
            cell_node_iterator target = cellList[i][j]->get_iter();
            cell_node_iterator upwards = cellList[i][j]->get_neighbor(7) -> get_iter();
            
            for(target; target.current != NULL; target.next()){
                
                for(upwards; upwards.current != NULL; upwards.next()){
                    
                    target.current->get_agent()->update(target.current->get_agent(), upwards.current->get_agent());
                   upwards.current->get_agent()->update(upwards.current->get_agent(), target.current->get_agent());
                    
                }
                
                upwards.reset_current();
            }
        }
    }
    /*
    //UPDATE Left DIAG
    //Along Dim 1
    for(int off_diag = 0; off_diag < DOMAIN_DIM_1; off_diag++)
    {
        cell* current = origin + off_diag;
        int runs = off_diag;
        for(int i = 0; i < runs; i++){
            if(current->get_neighbor(5) == NULL)
                continue;
            
            cell_node_iterator target = current->get_iter();
            cell_node_iterator upwards = current->get_neighbor(5) -> get_iter();
            
            for(target; target.current != NULL; target.next()){
                
                for(upwards; upwards.current != NULL; upwards.next()){
                    
                    target.current->get_agent()->update(target.current->get_agent(), upwards.current->get_agent());
                    upwards.current->get_agent()->update(upwards.current->get_agent(), target.current->get_agent());
                    
                }
                
                upwards.reset_current();
                
            }
            current += DOMAIN_DIM_1 - 1;
        }
    }
    //intermedius along second dimension
    for(int intermedius = 1; intermedius <= DOMAIN_DIM_2 - DOMAIN_DIM_1; intermedius++){
        cell* current = end - 1 + (intermedius * DOMAIN_DIM_1);
        int runs = DOMAIN_DIM_1;
        for(int i = 0; i < runs; i++){
            if(current->get_neighbor(5) == NULL)
                continue;
            
            cell_node_iterator target = current->get_iter();
            cell_node_iterator upwards = current->get_neighbor(5) -> get_iter();
            
            for(target; target.current != NULL; target.next()){
                
                for(upwards; upwards.current != NULL; upwards.next()){
                    
                    target.current->get_agent()->update(target.current->get_agent(), upwards.current->get_agent());
                    upwards.current->get_agent()->update(upwards.current->get_agent(), target.current->get_agent());
                    
                }
                
                upwards.reset_current();
            }
            current += DOMAIN_DIM_1 - 1;
        }
    }
    
    for(int off_diag = DOMAIN_DIM_2 - DOMAIN_DIM_1 + 1; off_diag < DOMAIN_DIM_2; off_diag++)
    {
        cell* current = end - 1 + (off_diag * DOMAIN_DIM_1); //Remember the reassignment of end.
        int runs = DOMAIN_DIM_2 - off_diag;
        for(int i = 0; i < runs; i++){
            if(current->get_neighbor(5) == NULL)
                continue;
            
            cell_node_iterator target = current->get_iter();
            cell_node_iterator upwards = current->get_neighbor(5) -> get_iter();
            
            for(target; target.current != NULL; target.next()){
                
                for(upwards; upwards.current != NULL; upwards.next()){
                    
                    target.current->get_agent()->update(target.current->get_agent(), upwards.current->get_agent());
                    upwards.current->get_agent()->update(upwards.current->get_agent(), target.current->get_agent());
                    
                }
                
                upwards.reset_current();
                
            }
            current += DOMAIN_DIM_1 - 1;
        }
    }
     */


	//Compare to cell to the bottom left
    for(int i = 0; i < DOMAIN_DIM_1; i++){
        for(int j = 0; j < DOMAIN_DIM_2; j++){
            
            if(cellList[i][j]->get_neighbor(5) == NULL)
                continue;
            
            cell_node_iterator target = cellList[i][j]->get_iter();
            cell_node_iterator upwards = cellList[i][j]->get_neighbor(5) -> get_iter();
            
            for(target; target.current != NULL; target.next()){
                
                for(upwards; upwards.current != NULL; upwards.next()){
                    
                    target.current->get_agent()->update(target.current->get_agent(), upwards.current->get_agent());
                    upwards.current->get_agent()->update(upwards.current->get_agent(), target.current->get_agent());
                    
                }
                
                upwards.reset_current();
            }
        }
    }
    
    /*
    //DRAG TERM; Also should contain division by Nearest Neighbor Count.
    origin = cellList[0][0];
    end = origin + (DOMAIN_DIM_1 * DOMAIN_DIM_3);
    for(cell* iter = origin; iter < end; iter++){
        cell_node_iterator target = iter->get_iter();
        for(target; target.current != NULL; target.next()){
            
    //        target.current->get_agent()->drag();
            
        }
    }
    */
    
    
    
}

void world::update_agent_pos_ab4(){
  /*
    cell* origin = cellList[0][0];
    cell* end = origin + (DOMAIN_DIM_1 * DOMAIN_DIM_3);
    for(cell* iter = origin; iter < end; iter++){
        cell_node_iterator target = iter->get_iter();
        for(target; target.current != NULL; target.next()){
            
            target.current->get_agent()->drag();
			target.current->get_agent()->euler_update();
            //target.current->get_agent()->ab4_update();
            //target.current->get_agent()->
            // change to new cell if appropriate
            // change "switch flag"
        }
    }
  */
  for(int i = 0; i < DOMAIN_DIM_1; i++){
	for(int j = 0; j < DOMAIN_DIM_2; j++){
		cell_node_iterator target = cellList[i][j]->get_iter();
		for(target; target.current != NULL; target.next()){
			if (target.current->get_agent()->is_alive()) {
				target.current->get_agent()->normalize_accel();
				target.current->get_agent()->drag();
				target.current->get_agent()->ab4_update();
			} else {
				target.current->get_agent()->set_x_coord(-1);
				}
			}
		}
	}
  
}

void world::update_agent_pos_euler(){
    /*
     cell* origin = cellList[0][0];
     cell* end = origin + (DOMAIN_DIM_1 * DOMAIN_DIM_3);
     for(cell* iter = origin; iter < end; iter++){
     cell_node_iterator target = iter->get_iter();
     for(target; target.current != NULL; target.next()){
     
     target.current->get_agent()->drag();
     target.current->get_agent()->euler_update();
     //target.current->get_agent()->ab4_update();
     //target.current->get_agent()->
     // change to new cell if appropriate
     // change "switch flag"
     }
     }
     */
    for(int i = 0; i < DOMAIN_DIM_1; i++){
        for(int j = 0; j < DOMAIN_DIM_2; j++){
            cell_node_iterator target = cellList[i][j]->get_iter();
            for(target; target.current != NULL; target.next()){
                if (target.current->get_agent()->is_alive()) {
                    target.current->get_agent()->normalize_accel();
					target.current->get_agent()->drag();
                    target.current->get_agent()->euler_update();
                } else {
                    target.current->get_agent()->set_x_coord(-1);
				}
			}
		}
	}
    
}

void world::run(std::ostream& strm, int print_every, int iterations)
{
	
    time_t t = time(NULL);
	std::cout << 0 << " " << time(NULL) - t << " total seconds elapsed" << "\n";
	print(strm);
	for(int i = 1; i <= 4; i++)
	{
        update_forward_velocs();
		update_agent_pos_euler();
		move_to_cell();
		if(i % print_every == 0){
			std::cout << i << " " << time(NULL) - t << " total seconds elapsed" << "\n";
			print(strm);
		}
		//w.print(str);
		//w.print(std::cout);
    }

	//solve using AB4
	for(int i = 5; i <= iterations; i++)
	{
		update_forward_velocs();
		update_agent_pos_ab4();
		move_to_cell();
		if(i % print_every == 0){
			std::cout << i << " " << time(NULL) - t << " total seconds elapsed" << "\n";
			print(strm);
		}
		//w.print(str);
		//w.print(std::cout);
	}

}



////use ab4 to update the positions of all agents
//void world::refresh_ab4()
//{
//		for (int i = 0; i < NUM_OF_AGENTS; i ++)
//		{
//			(*agents_master[i]).ab4_update();
//		}
//    
//}
//
////use euler to update positions when not enough points for ab4
//void world::refresh_eul()
//{
//		for (int i = 0; i < NUM_OF_AGENTS; i ++)
//		{
//			(*agents_master[i]).euler_update();
//		}
//}




void world::print(std::ostream& strm)
{
	for (size_t  i = 0; i < agents_master.size(); i++)
		{
			if(agents_master[i]->alive)
			{
			strm<< agents_master[i]->heading << "," << (*agents_master[i]).get_x_coord() 
		      <<"," << (*agents_master[i]).get_y_coord() 
		      <<"," << agents_master[i]->agent_type 
		      << "," << (*agents_master[i]).get_q_mag() 
		      << ",";
			}
			else
			{
				strm << 0 << "," << -100
		      <<"," << -100
		      <<"," << 0
		      << "," << 0
		      << ",";
			}
		}
	strm << "\n";
}

void world::add_agent(double x, double y)
{
	if(x < 0)
		x = 0;
	if(x >= CELL_LENGTH*DOMAIN_DIM_1)
		x = CELL_LENGTH*DOMAIN_DIM_1 - 1;
	if(y < 0)
		y = 0;
	if(y >= CELL_LENGTH*DOMAIN_DIM_2)
		y = CELL_LENGTH*DOMAIN_DIM_2 - 1;

	agents_master.push_back(new agent(x,y));
	int i = (int) (x)/CELL_LENGTH;
	int j = (int) (y)/CELL_LENGTH;
	cellList[i][j]->add_top(new cell_node(agents_master.back()));
}

void world::add_agent(double x, double y, void (* up)(agent*,agent*))
{
	if(x < 0)
		x = 0;
	if(x >= CELL_LENGTH*DOMAIN_DIM_1)
		x = CELL_LENGTH*DOMAIN_DIM_1 - 1;
	if(y < 0)
		y = 0;
	if(y >= CELL_LENGTH*DOMAIN_DIM_2)
		y = CELL_LENGTH*DOMAIN_DIM_2 - 1;

	agents_master.push_back(new agent(x, y, up));
	int i = (int) (x)/CELL_LENGTH;
	int j = (int) (y)/CELL_LENGTH;
	cellList[i][j]->add_top(new cell_node(agents_master.back()));
}

//void world::add_predator(double x, double y, void (* up)(agent*,agent*))
//{
//	if(x < 0)
//		x = 0;
//	if(x >= CELL_LENGTH*DOMAIN_DIM_1)
//		x = CELL_LENGTH*DOMAIN_DIM_1 - 1;
//	if(y < 0)
//		y = 0;
//	if(y >= CELL_LENGTH*DOMAIN_DIM_2)
//		y = CELL_LENGTH*DOMAIN_DIM_2 - 1;
//
//	agents_master.push_back(new predator(x, y, up));
//	int i = (int) (x)/CELL_LENGTH;
//	int j = (int) (y)/CELL_LENGTH;
//	cellList[i][j]->add_top(new cell_node(agents_master.back()));
//}

/*
void world::add_finch_rand(double x, double y, void (* up)(agent*,agent*))
{
	if(x < 0)
		x = 0;
	if(x >= CELL_LENGTH*DOMAIN_DIM_1)
		x = CELL_LENGTH*DOMAIN_DIM_1 - 1;
	if(y < 0)
		y = 0;
	if(y >= CELL_LENGTH*DOMAIN_DIM_2)
		y = CELL_LENGTH*DOMAIN_DIM_2 - 1;

	agents_master.push_back(new finch(x, y, up));
	int i = (int) (x)/CELL_LENGTH;
	int j = (int) (y)/CELL_LENGTH;
	cellList[i][j]->add_top(new cell_node(agents_master.back()));
}

*/

void world::add_agent(double x, double y, double z, void (* up)(agent*,agent*))
{
	if(x < 0)
		x = 0;
	if(x >= CELL_LENGTH*DOMAIN_DIM_1)
		x = CELL_LENGTH*DOMAIN_DIM_1 - 1;
	if(y < 0)
		y = 0;
	if(y >= CELL_LENGTH*DOMAIN_DIM_2)
		y = CELL_LENGTH*DOMAIN_DIM_2 - 1;
	if(z < 0)
		z = 0;
	if(z >= CELL_LENGTH*DOMAIN_DIM_2)
		z = CELL_LENGTH*DOMAIN_DIM_2 - 1;
	
	agents_master.push_back(new agent(x, y, z, up)); //?!?!?!
	int i = (int) (x)/CELL_LENGTH;
	int j = (int) (y)/CELL_LENGTH;
	cellList[i][j]->add_top(new cell_node(agents_master[i]));
}

void world::populate_rand()
{
	for (int i = 0; i < NUM_OF_AGENTS; i ++)
		{
			double x = rand() % (CELL_LENGTH*DOMAIN_DIM_1 ) - 1;
			double y = rand() % (CELL_LENGTH*DOMAIN_DIM_2 ) - 1;
			//double z = rand() % (CELL_LENGTH*DOMAIN_DIM_3 ) - 1;
			add_agent(x, y);
		}
}

void world::populate(int n, vector<double>& xpos, vector<double>& ypos, void (* up)(agent*,agent*)) {
	for (int i = 0; i < n; i++) {
	  double x = xpos[i];
		double y = ypos[i];
		add_agent(x, y, up);                   
	}
}

void world::populate_rand(int n, void (* up)(agent*,agent*))
{
	for (int i = 0; i < n; i ++)
		{
			/*double x = 2*CELL_LENGTH + (double)rand() / RAND_MAX * (CELL_LENGTH*DOMAIN_DIM_1 - 4*CELL_LENGTH);
			double y = 2*CELL_LENGTH + (double)rand() / RAND_MAX * (CELL_LENGTH*DOMAIN_DIM_2 - 4*CELL_LENGTH);
			*/


			double x = (double)rand() / RAND_MAX * (CELL_LENGTH*DOMAIN_DIM_1);
			double y = (double)rand() / RAND_MAX * (CELL_LENGTH*DOMAIN_DIM_2);
			add_agent(x, y, up);
		}
}

//void world::populate_predator_rand(int n, void (* up)(agent*,agent*))
//{
//	for (int i = 0; i < n; i ++)
//		{
//			double x = (double)rand() / RAND_MAX * (CELL_LENGTH*DOMAIN_DIM_1);
//			double y = (double)rand() / RAND_MAX * (CELL_LENGTH*DOMAIN_DIM_2);
//			/*
//			double x = CELL_LENGTH + (double)rand() / RAND_MAX * (2*CELL_LENGTH);
//			double y = CELL_LENGTH + (double)rand() / RAND_MAX * (2*CELL_LENGTH);
//			if(x > 2*CELL_LENGTH)
//			{
//				x += (DOMAIN_DIM_1-4)*CELL_LENGTH;
//			}
//			if(y > 2*CELL_LENGTH)
//			{
//				y += (DOMAIN_DIM_2-4)*CELL_LENGTH;
//			}*/
//			//double z = rand() % (CELL_LENGTH*DOMAIN_DIM_3 ) - 1;
//			add_predator(x, y, up);
//		}
//}
/*
void world::populate_finches_rand(int n, void (* up)(agent*,agent*))
{
	for (int i = 0; i < n; i ++)
		{
			double x = (double)rand() / RAND_MAX * (CELL_LENGTH*DOMAIN_DIM_1);
			double y = (double)rand() / RAND_MAX * (CELL_LENGTH*DOMAIN_DIM_2);
			
			add_finch_rand(x, y, up);
		}
}
*/
void world::move_to_cell() {
	for (int i = 0; i < DOMAIN_DIM_1; i++) {
		for (int j = 0; j < DOMAIN_DIM_2; j++) {
			cell* current_cell = cellList[i][j];
			cell_node* current_node = current_cell->get_top();
			cell_node* prev = NULL;
			while (current_node != NULL) {
				agent* current_agent = current_node->get_agent();
				int x = current_agent->cell_num_dim1();
				int y = current_agent->cell_num_dim2();
				if ( x != i || y != j) {
					if (prev == NULL) {
						current_node = current_cell->remove_top();
					} else {
						prev->set_next(current_node->get_next());
						current_node->set_next(NULL);
					}
					if(x < 0 || x >= DOMAIN_DIM_1 || y < 0 || y >= DOMAIN_DIM_2){
						current_agent->kill();
						theLonelyIsland->add_top(current_node);
					}
					else{
						cellList[x][y]->add_top(current_node);
					}
					if(prev == NULL){
						current_node = current_cell->get_top();
					} else {
						current_node = prev->get_next();
					}
				} else {
					prev = current_node;
					current_node = current_node->get_next();
				}
			}
		}
	}
}

// Functions that are helpful for debugging 

// Get cell of index i,j 

cell* world::get_cell(int i, int j) {
	return(cellList[i][j]);
}



// Ask whether cell i,j is empty 
int world::is_empty(int i, int j) 
{
  cell* current_cell = cellList[i][j];
  //std::cout << current_cell << std::endl;
  
  cell_node* current_node = current_cell->get_top();
  
  if(current_node == NULL) 
    {
      //std::cout << "Current node was empty" << std::endl;
      return 1;
    }
  else 
    {
      //std::cout << "Current node not empty" << std::endl;
      return 0;
    }
}
