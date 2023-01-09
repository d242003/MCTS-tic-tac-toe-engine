# Tic-Tac-Toe-engine
Consists of MCTS implementation on Tic-Tac-Toe <br/>
the engine has various commands which allows you to <br/>
play against it from given position, analyse different positions and also specify details. (Preffered Compiler - MSVC) <br/>
It uses MCTS (Monte Carlo Tree Search) where it conducts random playouts in a systematic manner. <br/>
commands work as follows <br/>
help()                 -> will give list of all commands (only 7 including help()...) <br/>
play_start().ON        -> to play against the engine from start (toggle to OFF if you don't want engine analysis) <br/>
play_from().ON         -> to play against the engine from specified position (same thing for toggling) <br/>
                          after passing the command you have to specify the position. <br/> <br/>
how to give position? <br/>
position has to be a 9 character long string, first 3 characters represent <br/>
the top row, next 3 represent the middle row, last 3 represent the last row <br/>
char "X" denotes X at the position, char "O" denotes O at the position, <br/>
char "_" denotes unoccupied position. <br/> <br/>
how to play against the engine? <br/>
just type the position of the char in the position string (which starts from 0) <br/>
example, input of 0 at _________ will make it X________. <br/> <br/>
analyse()              -> to get engine analysis for a particular position, after passing the command you have to specify the position.<br/>
set playouts_primary   -> setting primary playouts (see analyse function's working) default 10 (specify after passing command) <br/>
set playouts_secondary -> setting secondary playouts (see analyse function's working) default 10 (specify after passing command) <br/>
terminate()            -> exit the main function <br/>
![tic-tac-toe_output_1](https://github.com/N-dimensions/board-game-engines/blob/main/Tic-tac-toe-engine_output_1(2).PNG)
![tic-tac-toe_output_2](https://github.com/N-dimensions/board-game-engines/blob/main/Tic-tac-toe-engine_output_2(2).PNG)
