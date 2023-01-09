// This file contains the functions that would be called and the "main" function
#include"handler.h"
bool analyse(std::string posn, bool analyse_prnt = true, bool mode_play = false, char* FEN = nullptr, int playouts_primary=10, int playouts_secondary=10)
{
	int count1 = 0;
	char marker;
	bool no_victor=true;
	for (int i = 0; i < 9; i++)
	{
		if (posn[i] == '_')
		{
			count1++;
		}
	}
	srand(time(0));
	Position testboard[9];
	Position* historytmp[10];
	int expl, depth, count = 0, best;
	std::unordered_map<std::string, Position> all_positions_yet;
	for (int i = 0; i < 9 - count1; i++)
	{
		historytmp[i] = &testboard[i];
	}
	testboard[9 - count1].FEN = posn;
	historytmp[9 - count1] = &testboard[9 - count1];
	all_positions_yet[testboard[9 - count1].FEN] = testboard[9 - count1];
	fd_child_nodes(historytmp[9 - count1], all_positions_yet);
	//std::cout << "Been there done that " << count1 << std::endl;
	if (analyse_prnt)
	{
		std::cout << "Best Continuation: ";
	}
	for (int mvpld = 9 - count1; mvpld < 9; mvpld++)
	{
		depth = mvpld;
		while (depth < 9 && count < 1000)
		{
			count++;
			expl = explore_aor_best_continuation(historytmp, depth, playouts_primary, true);
			depth++;
			historytmp[depth] = historytmp[depth - 1]->child[expl];
			if (win(historytmp[depth]->FEN) != 2)
			{
				depth = mvpld;
				continue;
			}
			if (historytmp[depth - 1]->child[expl]->chd_fd)
			{
				continue;
			}
			fd_child_nodes(historytmp[depth], all_positions_yet);
			explore_aor_best_continuation(historytmp, depth, playouts_secondary, false);
			depth = mvpld;
		}
		best = fd_best_play(historytmp, mvpld, playouts_primary);
		if (analyse_prnt)
		{
			std::cout << historytmp[mvpld]->child[best]->FEN << "   ";
		}
		if (mode_play && mvpld == 9 - count1)
		{
			for (int i = 0; i < 9; i++)
			{
				FEN[i] = historytmp[mvpld]->child[best]->FEN[i];
			}
		}
		historytmp[mvpld + 1] = historytmp[mvpld]->child[best];
		if (win(historytmp[mvpld]->child[best]->FEN) != 2 && win(historytmp[mvpld]->child[best]->FEN) != 0)
		{
			if (analyse_prnt)
			{
				std::cout << std::endl;
				std::cout << "The final outcome considering, perfect players, is inescapable ";
				if (win(historytmp[mvpld]->child[best]->FEN) == -1)
				{
					marker = 'X';
				}
				else
				{
					marker = 'O';
				}
				std::cout << marker << " should win" << std::endl;
				std::cout << "semi-random playout gives the weighted probability as : " << historytmp[9 - count1]->w / historytmp[9 - count1]->N;
				std::cout << " For the losing side " << std::endl;
			}
			no_victor = false;
			break;
		}
		if (analyse_prnt)
		{
			if (win(historytmp[mvpld]->child[best]->FEN) == 0)
			{
				if (count1 % 2 == 0)
				{
					marker = 'X';
				}
				else
				{
					marker = 'O';
				}
				std::cout << std::endl;
				std::cout << "The final outcome considering, perfect players is DRAW " << std::endl;
				std::cout << "semi-random playout gives the weighted probability as : " << historytmp[9 - count1]->w / historytmp[9 - count1]->N;
				std::cout << " For " << marker << std::endl;
			}
			count = 0;
			no_victor = false;
		}
		else
		{
			break;
		}
	}
	return no_victor;

}
int play_against(char FEN[], bool analyse_prnt, int playouts_primary, int playouts_secondary)
{
	char marker;
	int playmv, mv = 10;
	std::string board;
	for (int i = 0; i < 9; i++)
	{
		if (FEN[i] == '_')
		{
			mv--;
		}
	}
	board = FEN;
	std::cout << "Marker of the opponent: >> ";
	std::cin >> marker;
	if (marker != 'O' && marker != 'X')
	{
		std::cout << "Error 002: Invalid marker (markers are supposed to be uppercase):" << std::endl;
		return 0;
	}
	if ( (marker == 'X' && mv%2 == 1) || (marker == 'O' && mv%2 == 0))
	{
		std::cout << board << std::endl;
	}
	else
	{
		analyse(board, analyse_prnt, true, FEN, playouts_primary, playouts_secondary);
		board = FEN;
		std::cout << board << std::endl<<std::endl;
		mv++;
	}
	while (true)
	{
		std::cout << ">> ";
		std::cin >> playmv;
		play(playmv, FEN, mv);
		board = FEN;
		std::cout << board << std::endl<<std::endl;
		if (win(board) != 2)
		{
			if (win(board) == -1)
			{
				std::cout << "X has won" << std::endl;
			}
			if (win(board) == 1)
			{
				std::cout << "O has won" << std::endl;
			}
			if (win(board) == 0)
			{
				std::cout << "DRAW" << std::endl;
			}
			break;
		}
		analyse(board, analyse_prnt, true, FEN, playouts_primary, playouts_secondary);
		board = FEN;
		std::cout << board << std::endl;
		if (win(board) != 2)
		{
			if (win(board) == -1)
			{
				std::cout << "X has won" << std::endl;
			}
			if (win(board) == 1)
			{
				std::cout << "O has won" << std::endl;
			}
			if (win(board) == 0)
			{
				std::cout << "DRAW" << std::endl;
			}
			break;
		}
		mv += 2;
	}
}
int help()
{
	std::cout << "play_start().ON/OFF    -> Allows to play against the engine from start" << std::endl;
	std::cout << "                          Toggle to ON if you want to see analysis side by side" << std::endl;
	std::cout << "play_from().ON/OFF     -> Allows you to play against the engine from a position you'll enter" << std::endl;
	std::cout << "                          Toggle to ON if you want to see analysis side by side" << std::endl;
	std::cout << "analyse()              -> Allows you to see engine continuations from present move" << std::endl;
	std::cout << "help()                 -> Shows the list of engine commands" << std::endl;
	std::cout << "set playouts_primary   -> set number of primary playouts" << std::endl;
	std::cout << "set playouts_secondary -> set number of secondary playouts" << std::endl;
	std::cout << "terminate()            -> close application" << std::endl;
	return 0;
}
bool text_recognizer(std::string command, int *playouts_primary, int *playouts_secondary)
{
	if (command == "help()")
	{
		help();
		return true;
	}
	if (command == "play_start().ON")
	{
		char start[10] = "_________";
		play_against(start, true, *playouts_primary, *playouts_secondary);
		return true;
	}
	if (command == "play_start().OFF")
	{
		char start[10] = "_________";
		play_against(start, false, *playouts_primary, *playouts_secondary);
		return true;
	}
	if (command == "play_from().ON")
	{
		char start[10];
		std::cout << ">> ";
		std::cin >> start;
		play_against(start, true, *playouts_primary, *playouts_secondary);
		return true;
	}
	if (command == "play_from().OFF")
	{
		char start[10];
		std::cout << ">> ";
		std::cin >> start;
		play_against(start, false, *playouts_primary, *playouts_secondary);
		return true;
	}
	if (command == "analyse()")
	{
		std::string start;
		std::cout << ">> ";
		std::cin >> start;
		analyse(start, true, false, nullptr , *playouts_primary, *playouts_primary);
		return true;
	}
	if (command == "set playouts_primary")
	{
		std::cout << ">> ";
		std::cin >> *playouts_primary;
		return true;
	}
	if (command == "set playouts_secondary")
	{
		std::cout << ">> ";
		std::cin >> *playouts_secondary;
		return true;
	}
	if (command == "terminate()")
	{
		return false;
	}
	std::cout << "Unknown command " << "\"" << command << "\"" << std::endl;
	return true;
}
int main()
{
	std::string command;
	int playouts_primary=10, playouts_secondary=10;
	bool ready = true;
	std::cout << "Tic-Tac-Toe engine (2022) version 1.0.0 by N-dimensions" << std::endl;
	std::cout << "=================================================" << std::endl;
	while (ready)
	{
		std::cout << ">> ";
		std::getline(std::cin, command);
		if (command == "")
		{
			std::getline(std::cin, command);
		}
		ready = text_recognizer(command, &playouts_primary, &playouts_secondary);
	}
	return 0;
}