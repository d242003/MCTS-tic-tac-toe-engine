#include<unordered_map>
#include<iostream>
#include<math.h>
#include<string>
#include<vector>
#include<algorithm>
/* FEN[10] --> would be like "XOX_OXX_O"
"w" denotes the number of wins/points from randomly generated games from this position,
N denotes the no. of randomly generated games.
node_exploration --> Tendency to explore given node.
if explored once or not... maybe we'll not need that.*/
struct Position
{
	std::string FEN="_________";
	float w=0, N=0;					
	float node_exploration = INFINITY;			
	bool explored=false;	
	bool chd_fd = false;
	Position* child[10];
};
bool new_posn_enc(std::unordered_map<std::string, Position>& all_positions_now, std::string positionFEN )
{
	if (all_positions_now.find(positionFEN) == all_positions_now.end())
	{
		return true;
	}
	return false;
}
//computes node exploration thing.
int find_node_exp(Position* parent, Position* child)
{
	child->node_exploration = child->w / child->N + sqrt(2 * log(parent->N) / child->N);		
	return 0;
}
//plays given move onto the given board.
int play(int move, char* FENtmpPar, int mvno)				
{
	if (FENtmpPar[move] == '_')
	{
		if (mvno % 2 == 1)
		{
			FENtmpPar[move] = 'X';
		}
		else
		{
			FENtmpPar[move] = 'O';
		}
	}
	else
	{
		std::cerr << "Error 001: The program inherited an invalid move"<<std::endl;
	}
	return 0;
}
//If X wins win()=-1, if O wins win()=1, if it's a dead draw win()=0.
int win(std::string posn)
{
	for (int i = 0; i < 3; i++)
	{
		if (posn[3*i] != '_')
		{
			if (posn[3 * i] == posn[3 * i + 1] && posn[3 * i] == posn[3 * i + 2])
			{
				if (posn[3 * i] == 'X')
				{
					return -1;
				}
				return 1;
			}
		}
		if (posn[i] != '_')
		{
			if (posn[i] == posn[3 + i] && posn[i] == posn[6 + i])
			{
				if (posn[i] == 'X')
				{
					return -1;
				}
				return 1;
			}
		}
	}
	if (posn[0] != '_')
	{
		if (posn[0] == posn[4] && posn[0] == posn[8])
		{
			if (posn[0] == 'X')
			{
				return -1;
			}
			return 1;
		}
	}
	if (posn[2] != '_')
	{
		if (posn[2] == posn[4] && posn[2] == posn[6])
		{
			if (posn[2] == 'X')
			{
				return -1;
			}
			return 1;
		}
	}
	for (int i = 0; i < 9; i++)
	{
		if (posn[i] == '_')
		{
			return 2;
		}
	}
	return 0;
}
//Plays some random games from "Present" position and updates accordingly
int play_rand_frm(Position** history, int mvpld, int nofgames)
{
	int move, Nog = nofgames;
	float wins = 0;
	bool to_be_played = true;
	if (win(history[mvpld]->FEN) != 2)
	{
		wins += float(nofgames)*float(win(history[mvpld]->FEN) + 1) / 2;
		//std::cout << wins << std::endl;
		to_be_played = true;
		//if (win(history[mvpld]->FEN) != 0)
		//{
		//	history[mvpld - 1]->N = INFINITY;
		//}
	}
	if (to_be_played)
	{
		std::vector<int> legal, inileg;
		char FENtmp1[9], FENtmp2[9];
		for (int i = 0; i < 9; i++)
		{
			FENtmp1[i] = history[mvpld]->FEN[i];
			FENtmp2[i] = FENtmp1[i];
			if (history[mvpld]->FEN[i] == '_')
			{
				inileg.push_back(i);
				legal.push_back(i);
			}
		}
		while (Nog--)
		{
			/*Testing code in this section------
												|
												|
												v  *//*
			std::cout << "Game number: " << nofgames - Nog << std::endl;
			std::cout << "===================================================" << std::endl;*/
			//Testing ends.
			while (win(FENtmp2) == 2)
			{
				move = rand() % legal.size();
				play(legal[move], FENtmp2, 10 - legal.size());
				/*Testing code in this section------
													|
													|
													v  *//*
				std::cout << "Continuation... ";
				for (int i = 0; i < 9; i++)
				{
					std::cout << FENtmp2[i];
				}
				std::cout << std::endl;*/
				//Testing ends.
				legal.erase(legal.begin() + move);
			}
			//std::cout << std::endl << std::endl;
			/*while none has won, a random move shall be chosen to be played.
			The chosen move is occupied so it can no longer be played hence erasing it.*/
			wins += float(win(FENtmp2) + 1) / 2;
			std::copy(FENtmp1, FENtmp1 + 10, FENtmp2);
			legal.clear();
			std::copy(inileg.begin(), inileg.end(), back_inserter(legal));
			//scores updated (internally), board position reset, legal moves reset.
		}
	}
	history[mvpld]->w += wins+(mvpld%2)*(nofgames-2*wins);
	history[mvpld]->N += nofgames;
	/*Testing code in this section------
										|
										|
										v  *//*
	std::cout << "Scores recorded: " << std::endl;
	std::cout << "w--> " << history[mvpld]->w<< std::endl;
	std::cout << "N--> " << history[mvpld]->N<< std::endl;*/
	//Testing ends.
	for (int i = mvpld-1; i>=0; i--)
	{
		history[i]->w += wins + (i % 2) * (nofgames - 2 * wins);
		history[i]->N += nofgames;
		find_node_exp(history[i], history[i+1]);
	}
	history[mvpld]->explored = true;
	//The results are updated into data structures.
	return 0;
}
//Finding possible child nodes and adding them to the vector containing all board positions.
int fd_child_nodes(Position* present, std::unordered_map<std::string, Position>& all_positions_now)
{
	Position next;
	//std::cout << "initial value of *next.FEN*: " << next.FEN << std::endl;
	int count=0, j=0;
	char marker;
	if (present->chd_fd)
	{
		return 0;
	}
	for (int i = 0; i < 9; i++)
	{
		//FENtmp1[i] = present->FEN[i];
		//std::cout << FENtmp1[i] << std::endl;
		if (present->FEN[i] == '_')
		{
			count++;
		}
	}
	//std::cout << "henno";
	next.FEN = present->FEN;
	//std::cout << "value of *next.FEN*: " << next.FEN <<std::endl;
	if (count % 2 == 1)
	{
		marker = 'X';
	}
	else
	{
		marker = 'O';
	}
	for (int i = 0; i < 9; i++)
	{
		if (next.FEN[i] == '_')
		{
			next.FEN[i] = marker;
			if (new_posn_enc(all_positions_now, next.FEN))
			{
				all_positions_now[next.FEN] = next;
				present->child[j] = &all_positions_now[next.FEN];
				/*Testing code in this section------
												|
												|
												v  *//*
				std::cout << "Child node no. " << j << " : ";
				std::cout << present->child[j]->FEN;
				std::cout << std::endl;*/
				//testing ends
			}
			else
			{
				present->child[j] = &all_positions_now[next.FEN];
				/*Testing code in this section------
												|
												|
												v  *//*
				std::cout << "Child node no. (child already exists): " << j << " : ";
				std::cout << present->child[j]->FEN;
				std::cout << std::endl;*/
				//testing ends
			}
			if (win(present->child[j]->FEN) != 2)
			{
				if (win(present->child[j]->FEN) != 0)
				{
					present->N = INFINITY;
				}
				else
				{
					present->child[j]->N = 200;
					present->child[j]->w = 100;
				}
			}
			j++;
			next.FEN[i] = '_';
		}
	}
	present->chd_fd = true;
	return 0;
}
//Finding best child node, you'll have to add them to history though
int explore_aor_best_continuation(Position** history, int mvpld, int mv_rand_play, bool fd_continuation)
{
	int i = 0, j = 0;
	int index_number=0;
	float exp_tendency=0;
	for (i = 0; i < 9-mvpld; i++)
	{
		if (!(history[mvpld]->child[i]->explored))
		{
			history[mvpld + 1] = history[mvpld]->child[i];
			play_rand_frm(history, mvpld + 1, mv_rand_play);
			history[mvpld]->child[i]->explored = true;
		}
		if (fd_continuation)
		{
			if (history[mvpld]->child[i]->node_exploration > exp_tendency)
			{
				exp_tendency = history[mvpld]->child[i]->node_exploration;
				index_number = i;
			}
		}
	}
	return index_number;
}
int fd_best_play(Position** history, int mvpld, int mv_rand_play)
{
	int i = 0, j = 0;
	int index_number = 0;
	float win_tendency = -1;
	for (i = 0; i < 9 - mvpld; i++)
	{
		//std::cout << "Value of i: " << i << std::endl;
		//if (!(history[mvpld]->child[i]->explored))
		//{
			//history[mvpld + 1] = history[mvpld]->child[i];
			//play_rand_frm(history, mvpld + 1, mv_rand_play);
			//history[mvpld]->child[i]->explored = true;
		//}
		/*std::cout << "The move being judged: " << history[mvpld]->child[i]->FEN << " -> " << history[mvpld]->child[i]->w << "/" << history[mvpld]->child[i]->N;
		std::cout << "-> Probability: " << history[mvpld]->child[i]->w / history[mvpld]->child[i]->N<<std::endl;
		*/
		if (history[mvpld]->child[i]->w / history[mvpld]->child[i]->N > win_tendency && history[mvpld]->child[i]->N<INFINITY)
		{
			win_tendency = history[mvpld]->child[i]->w / history[mvpld]->child[i]->N;
			index_number = i;
		}
	}
	return index_number;
}