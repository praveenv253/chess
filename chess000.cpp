/*FILE DETAILS******************************************************************

 * File name: ai93.cpp

 * Author: Praveen Venkatesh

 * Changes made relative to ai92:
   - new option: 1p or 2p?
   - new copy_board function
   - changing move_piece function: removed equation of individual values and
     instead just directly equated the board[][] structures
   - inclusion of make_square_empty() function in order to reinitialise
     structure elements of the old square to zero
   - inclusion of update_board function comment inside commence_game()
   - inclusion of #include "gr0.cpp" comment
   - removed 'friend class chessboard' from class action
   - !! -
   - Replaced occurrences of piece_move_possible(...).val by:
     ret_val piece_mobility = piece_move_possible(...);
     if(piece_mobility.val) then and so on
   - also implemented the error messages sent back by the piece_move_possible
     functions so they now show in the output
   - !!! -
   - Placed an important continue statement after ai() call in commence_game()
   - In pawn_move_possible,

 * NO COMPILE ERRORS
 * NOT CHECKED WITH TEST FILES

 * Changes relative to ai93:
   - pawn_move_possible() has some very big holes in it like the use of the opp
     function, which can only lead to trouble. overloaded the opp function to
     either be relative to the current player or relative to the current piece

 * Changes relative to ai94:
	- Trying to reconfigure the whole of enpassant to work better
   - En passant reconfigured. Big errors in ai94 largely removed.
   - Regression testing for en passant required

 * Changes relative to ai95:
   - Created rough draft of the evaluate function
   - No compile errors
   - Seems to make wierd moves most of the time
   - Move side-effects/indirect results not done yet
   
NOTE:
   PERFORM DEFENSE BY PUTTING YOURSELF INTO THE OPPONENT'S SHOES.
   MAKE THE AI PLAY AS THE OPPONENT PLAYER AND FIGURE OUT THE FEW BEST MOVES.
   THEN COUNTER THE MOVES ACCORDINGLY. THIS CAN HELP WITH AVOIDING FORKS
   
   ALSO, WHEN CHECKING THE IMORTANCE OF CASTLING, CASTLE IF THE NUMBER OF PIECES
   ATTACKING E2 IS GREATER THAN THE NUMBER ATTACKING G2 SAY. ALSO, THE GAME
   SHOULD NOT BE TIPPING IN FAVOUR OF ANY ONE PLAYER AT SUCH A POINT IN TIME.

*******************************************************************************/

/*Inclusion of header files****************************************************/

#include <iostream>
#include <cstring>
#include <cstdlib>                                        //For exit() function
#include <cmath>                                  //For absolute value function

/*End of header file inclusions************************************************/

using namespace std;

/*Declaration of global variables**********************************************/

/*Structure to contain the x- and y-coordinates of a piece*/
struct position {
	int x;
	int y;
};

struct ret_val {
	int val;
	char error[100];
};

/*Structure which makes up the chessboard -
  Every square has two values: 1.A unique number which gives the piece type
										 2.A second number which gives the player
  Both these values are zero for an empty square
  The first number is not unique only in the case of a pawn promotion         */
struct chessboard {
   int num;
	int player;
	
	int enpassant_able;
	int ep_move_no;
	position pawn_pos;
	
	int wht_king_castle;
	int blk_king_castle;
	int wht_queen_castle;
	int blk_queen_castle;
};

int under_attack(int, int, int, chessboard [9][9]);
int opp(int);
int opp(int, int);
ret_val piece_move_possible(int, int, int, int, chessboard [9][9]);
ret_val pawn_move_possible(int, int, int, int, chessboard [9][9]);
ret_val rook_move_possible(int, int, int, int, chessboard [9][9]);
ret_val bishop_move_possible(int, int, int, int, chessboard [9][9]);
ret_val knight_move_possible(int, int, int, int, chessboard [9][9]);
ret_val queen_move_possible(int, int, int, int, chessboard [9][9]);
ret_val king_move_possible(int, int, int, int, chessboard [9][9]);
int checkmate(chessboard [9][9]);
ret_val castle_possible(int, int, int, int, chessboard [9][9]);
int king_in_check_after_move(int, int, int, int, chessboard [9][9]);
int end_of_game(int, int, int, int, chessboard [9][9]);
void move_piece(int, int, int, int, chessboard [9][9]);
void make_square_empty(chessboard &);
void move_castle(int, int, int, int, chessboard [9][9]);
void accept_input(int &, int &, int &, int &, chessboard [9][9]);
void display(chessboard [9][9]);
void msg_box(char [], chessboard [9][9]);
char pawn_promotion_msg(void);
position find_king(int, chessboard [9][9]);
/*void ai(chessboard [9][9]);

void draw_piece(int, int, int);
void draw_pawn(int, int, int);
void draw_queen(int, int, int);
void draw_king(int, int, int);
void draw_bishop(int, int, int);
void draw_knight(int, int, int);
void draw_rook(int, int, int);
void draw_cursor(int, int, int);
void exit_msg(chessboard board[9][9]);
class action;
*/

void commence_game(chessboard [9][9]);

enum {OP, MID, END};
int state=OP;                                                                           //!!! STATE NOT YET IMPLEMENTED !!!

/*Global variables which give the move number and current player*/
int k;
int curr_player;
int HS, AI;

/*An enumeration used in conjunction with the player variable to enhance
  identification of the player:
  White - 1 - WHT
  Black - 2 - BLK                                                             */
enum {WHT=1, BLK};

/*Global variables for kryboard interface
  * union REGS. Contains the key value entered from the keyboard
  * An enumuration to enhance identification of keyboard buttons
  * int x128. One hundred and twentieth of the maximum x-coordinate
  * int y128. One hundred and twentieth of the maximum y-coordinate
  * int x8. One eighth of the maximum x-coordinate
  * int y8. One eighth of the maximum y-coordinate                            */
//union REGS in, out;
//enum {ESC=1, ENTER=28, LEFT=75, RIGHT=77, UP=72, DOWN=80};
//int x128, y128, x8, y8;

/*End of declaration of global variables***************************************/

/*FUNCTION MAIN*****************************************************************
  * Function:
	 - Entry point to the program
	 - Contains initialisation of the board
	 - Contains the game loop which has calls to all other functions
	 - Displays final end-game result
  * Parameters:
	 - int argc. number of command line parameters
	 - char **argv. command line parameters in a pointer array
  * Return values:
	 - 0 on success
	 - non-zero on failure
*******************************************************************************/
int main(int argc, char **argv)
{
	/*Local variables specifying the graphics driver and graphics mode for the
	  interface*/
//	int gd=DETECT, gm;
	/*Initiation of graphics using initgraph() function*/
//	initgraph(&gd, &gm, "c:\\tc\\bgi");
	/*Initialising the global variables used to position the board and pieces*/
//	x128 = getmaxx()/128;
//	y128 = getmaxy()/128;
//	x8 = getmaxx()/8;
//	y8 = getmaxy()/8;

	chessboard main_board[9][9];
	commence_game(main_board);
	return 0;
}

void commence_game(chessboard board[9][9])
{
	/*Declaration of local variables*/
		//Counter variables for the loop
	int i, j;
		//Contains initial x- and y-coordinates of piece to be moved
	position initial={0,0};
		//Contains the final x- and y-coordinates of the piece to be moved
	position final={0,0};
		/*mate accepts the return value of the checkmate function
		  mate is zero when the game is in progress
		  mate is non-zero in case of checkmate or stalemate*/
	int mate=0;
	/*End of declarations*/

	/*Initialisation of the board*/
		//Declaring all structure elements to be zero
	for(i=0 ; i<=8 ; i++) {
		for(j=0 ; j<=8 ; j++) {
			make_square_empty(board[i][j]);
		}
	}
		//Initialising the pieces
	for(i=1 ; i<=8 ; i++) {
		board[i][1].num = i;
		board[i][8].num = i+8;
		board[i][2].num = i+16;
		board[i][7].num = i+24;
		board[i][1].player = 1;
		board[i][2].player = 1;
		board[i][7].player = 2;
		board[i][8].player = 2;
	}

   board[0][8].wht_king_castle = 1;
   board[0][8].blk_king_castle = 1;
	board[0][8].wht_queen_castle = 1;
	board[0][8].blk_queen_castle = 1;

	/*End of initialisation*/

/*	char choice;
	msg_box("1 player or 2 player? (1/2)", board);
	choice = getch();
	if(choice == '1') {
		msg_box("Do you want to be Black or White? (B/W)", board);
		choice = getch();
	if(choice=='W' || choice=='w') {
		HS=1;
		AI=2;
	}
	else if(choice=='B' || choice=='b') {
		HS=2;
		AI=1;
	}
	}
	else {
		AI=0;
	}
*/
	/*Start of main loop - the game*/
	for(k=1 ; ; k++) {

		/*White plays on odd turns, black on even turns*/
		curr_player = k%2;
		if(curr_player==0) curr_player+=2;

		mate = checkmate(board);
			//Program execution jumps out of the loop if the game is over
		if(mate) {
			break;
		}

		/*Function update board - to update all stored values for ai
		//update_board(board);

		/*Refresh label - all invalid moves regroup here*/
		refresh:

		/*Accepting input*/
		accept_input(initial.x, initial.y, final.x, final.y, board);
/*		if(AI) {
			if(curr_player==HS) {
//         cout<<"Move number: "<<((k-1)/2)+1;
				accept_input(initial.x, initial.y, final.x, final.y, board);
			}
			else {
				ai(board);
				display(board);
				continue;
			}
		}
		else {
			accept_input(initial.x, initial.y, final.x, final.y, board);
		}
*/
		/*If the current coordinates represent a pawn*/
		if(board[initial.x][initial.y].num >= 17) {
			ret_val pawn_mobility = pawn_move_possible(initial.x, initial.y, final.x, final.y, board);
				//For an ordinary pawn movement
			if(pawn_mobility.val==1) {
				move_piece(initial.x, initial.y, final.x, final.y, board);
					//In the case of promotion
				if(final.y==8 || final.y==1) {
					char choice;
						//Accepting user choice for promoting pawn
					choice = pawn_promotion_msg();
						//Re-nitialising the num-variable to the new piece
					switch(choice) {
						case 'Q':
						case 'q':board[final.x][final.y].num = (curr_player-1)*8 + 4;
									break;
						case 'K':
						case 'k':board[final.x][final.y].num = (curr_player-1)*8 + 2;
									break;
						case 'R':
						case 'r':board[final.x][final.y].num = (curr_player-1)*8 + 1;
									break;
						case 'B':
						case 'b':board[final.x][final.y].num = (curr_player-1)*8 + 3;
									break;
					}
				} //End of promotion
			}
				//For en passant
			else if(pawn_mobility.val==2) {
				move_piece(initial.x, initial.y, final.x, final.y, board);
				int u;
				if(curr_player==WHT) {
					u=1;
				}
				else {
					u=-1;
				}
				move_piece(final.x, final.y-u, 0, 0, board);
			}
			else {
				msg_box(pawn_mobility.error, board);
				goto refresh;
			}
		}
		/*End pawn*/

		switch(board[initial.x][initial.y].num) {

			/*If the current coordinates represent a rook*/
			case 1 :
			case 8 :
			case 9 :
			case 16: {	//Checking if the rook can move to the new location
                  ret_val rook_mobility = rook_move_possible(initial.x, initial.y, final.x, final.y, board);
                  if(rook_mobility.val) {
							move_piece(initial.x, initial.y, final.x, final.y, board);
						}
						else {
							msg_box(rook_mobility.error, board);
							goto refresh;
						}}
						break;
			/*End rook*/

			/*If the current coordinates represent a bishop*/
			case 3 :
			case 6 :
			case 11:
			case 14: {	//Checking if the bishop can move to the new location
                  ret_val bishop_mobility = bishop_move_possible(initial.x, initial.y, final.x, final.y, board);
                  if(bishop_mobility.val) {
							move_piece(initial.x, initial.y, final.x, final.y, board);
						}
						else {
							msg_box(bishop_mobility.error, board);
							goto refresh;
						}}
						break;
			/*End bishop*/

			/*If the current coordinates represent a knight*/
			case 2 :
			case 7 :
			case 10:
			case 15: {	//Checking if the knight can move to the new location
                  ret_val knight_mobility = knight_move_possible(initial.x, initial.y, final.x,	final.y, board);
						if(knight_mobility.val) {
							move_piece(initial.x, initial.y, final.x, final.y, board);
						}
						else {
							msg_box(knight_mobility.error, board);
							goto refresh;
						}}
						break;
			/*End knight*/

			/*If the current coordinates represents a queen*/
			case 4 :
			case 12: {	//Checking if the queen can move to the new location
                  ret_val queen_mobility = queen_move_possible(initial.x, initial.y, final.x, final.y, board);
                  if(queen_mobility.val) {
							move_piece(initial.x, initial.y, final.x, final.y, board);
						}
						else {
							msg_box(queen_mobility.error, board);
							goto refresh;
						}}
						break;
			/*End queen*/

			/*If the current coordinates represents a king*/
			case 5 :
			case 13: {	//Checking if the king can move to the new location
                  ret_val king_mobility = king_move_possible(initial.x, initial.y, final.x, final.y, board);
						if(king_mobility.val) {
								//In the case of castling
							if(king_mobility.val==2) {
								move_castle(initial.x, initial.y, final.x, final.y, board);
								break;
							}
							else {
								//For an ordinary king move
								move_piece(initial.x, initial.y, final.x, final.y, board);
							}
						}
						else {
							msg_box(king_mobility.error, board);
							goto refresh;
						}}
						break;
			/*End king*/
		}

	}
	/*End of game loop*/

	display(board);

	/*Displaying result after the game is over*/
	if(mate==1) {
		msg_box("Checkmate - White wins!", board);
	}
	else if(mate==2) {
		msg_box("Checkmate - Black wins!", board);
	}
	else if(mate==3) {
		msg_box("Stalemate", board);
	}

	/*Displaying the final checkmate position*/
	display(board);

	/*Function main returns zero on success*/
	return;
}
/*End of function commence_game************************************************/

ret_val piece_move_possible(int x1, int y1, int x2, int y2, chessboard board[9][9])
{
	/*Checking pawns*/
	if(board[x1][y1].num >= 17) {
		return pawn_move_possible(x1, y1, x2, y2, board);
	}

	switch(board[x1][y1].num) {
		/*Checking rooks*/
		case 1 :
		case 8 :
		case 9 :
		case 16: return rook_move_possible(x1, y1, x2, y2, board);
		/*Checking knights*/
		case 2 :
		case 7 :
		case 10:
		case 15: return knight_move_possible(x1, y1, x2, y2, board);
		/*Checking bishops*/
		case 3 :
		case 6 :
		case 11:
		case 14: return bishop_move_possible(x1, y1, x2, y2, board);
		/*Checking queens*/
		case 4 :
		case 12: return queen_move_possible(x1, y1, x2, y2, board);
		/*Checking king*/
		case 5 :
		case 13: return king_move_possible(x1, y1, x2, y2, board);
	} //End of switch statement
}

/*FUNCTION UNDER_ATTACK*********************************************************
  * Function:
	 - Checks if a square (x1,y1) is under attack by the opponent of the
		specified player
  * Parameters:
	 - int x1. contains the x-coordinate of the square to check
	 - int y1. contains the y-coordinate of the square to check
	 - int player. the player who is being attacked
  * Returns:
	 - 0 if the square is not under attack
	 - 1 if the square is under attack
  * Logic:
	 -	Checks all opponent pieces to see if any one of them can move to the
		specified position
*******************************************************************************/
int under_attack(int x1, int y1, int player, chessboard board[9][9])
{
	/*Counter variables for the loop*/
	int i, j;

	/*For loop to check each opponent player*/
	for(i=1 ; i<=8 ; i++) {
		for(j=1 ; j<=8 ; j++) {
			int flag=0;
				//If the square is empty
			if(board[i][j].player==0) {
				continue;
			}
				//If the player is an opponent
			if(board[i][j].player != player) {
					//Checking opponent pieces
				//pawn
				if(board[i][j].num >= 17) {
					if(board[i][j].player==1) {
						if( (y1==j+1) && ((x1==i-1)||(x1==i+1)) ) {
							return 1;
						}
						flag = 1;
					}
					else {
						if( (y1==j-1) && ((x1==i-1)||(x1==i+1)) ) {
							return 1;
						}
						flag = 1;
					}
				}
				//king
				if(board[i][j].num==5 || board[i][j].num==13) {
					if( (abs(i-x1)<=1) && (abs(y1-j)<=1) ) {
						return 1;
					}
					else {
						flag = 1;
					}
				}

				if(!flag) {
					if(piece_move_possible(i, j, x1, y1, board).val) {
						return 1;
					}
				}

			}
		}
	}
	/*End of for loop*/

	/*Return zero if the square is not under attack*/
	return 0;
}
/*End of function under_attack*************************************************/

/*FUNCTION FIND_KING************************************************************
  * Function:
	 - Finds the coordinates of the king of a given player
  * Parameters:
	 - int player. the player whose king is to be found
  * Return value:
	 - king_pos. a position structure which contains the coordinates of the king
  * Logic:
	 - Scans every square of the board using a for loop until the king is found
*******************************************************************************/
position find_king(int player, chessboard board[9][9])
{
	/*Counter variables for the loop*/
	int i, j;
	int king_num;

	/*Position structure to contain king's coordinates*/
	position king_pos={0,0};

	if(player==WHT) king_num=5;
	else king_num=13;

	/*Loop to find the king*/
	for(i=1 ; i<=8 ; i++) {
		for(j=1 ; j<=8 ; j++) {
			/*If the current square contains the king*/
			if( board[i][j].num == king_num ) {
				king_pos.x = i;
				king_pos.y = j;
				return king_pos;
			}
		}
	}
	/*End of for loop*/
}
/*End of function find_king****************************************************/

/*FUNCTION MOVE_PIECE***********************************************************
  * Function:
	 - Moves a piece from the current coordinates (x1,y1) to the destination
		coordinates (x2,y2)
	 - Invalidates castling in case a king- or rook-move is made
  * Parameters:
	 - int x1. current x-coordinate
	 - int y1. current y-coordinate
	 - int x2. new x-coordinate
	 - int y2. new y-coordinate
  * Return value:
	 - None
  * Logic:
	 - Changes the number and player of the destination structure to those of the
		initial structure and re-initialises the current structure to zero
*******************************************************************************/
void move_piece(int x1, int y1, int x2, int y2, chessboard board[9][9])
{
	/*Changing destination structure values to initial structure values*/
	board[x2][y2] = board[x1][y1];

	/*Re-initialising initial structure values to zero*/
   make_square_empty(board[x1][y1]);

	/*Invalidating castling if a king or rook has been moved*/
	if((board[x2][y2].num==1) || (board[x2][y2].num==5)) {
		board[0][8].wht_queen_castle = 0;
	}
	if((board[x2][y2].num==8) || (board[x2][y2].num==5)) {
		board[0][8].wht_king_castle = 0;
	}
	if((board[x2][y2].num==9) || (board[x2][y2].num==13)) {
		board[0][8].blk_queen_castle = 0;
	}
	if((board[x2][y2].num==16) || (board[x2][y2].num==13)) {
		board[0][8].blk_king_castle = 0;
	}
	/*End of invalidation*/
}
/*End of function move_piece***************************************************/

void make_square_empty(chessboard &square)
{
   square.num = 0;
   square.player = 0;
   square.enpassant_able = 0;
   square.ep_move_no = -1000;
}

/*FUNCTION OPP******************************************************************
  * Function:
	 - Checks if the given player is the opponent
  * Parameters:
	 - int player. the player whose identity is to be checked
  * Return value:
	 - 0 if 'player' is not the opponent (unoccupied or same player)
	 - 1 if 'player' is the opponent
  * Logic:
	 - The player is an opponent if the current player and 'player' are not the
		same
*******************************************************************************/
int opp(int player)
{
	if((curr_player==WHT) && (player==BLK))  {
		return 1;
	}
	else if((curr_player==BLK) && (player==WHT)) {
		return 1;
	}
	else {
		return 0;
	}
}
/*End of function opp**********************************************************/

int opp(int player1, int player2)
{
   if(player1==WHT && player2==BLK) return 1;
   if(player1==BLK && player2==WHT) return 1;

   return 0;
}

/*FUNCTION PAWN_MOVE_POSSIBLE***************************************************
  * Function:
	 - Checks whether a pawn can make a specified move
  * Parameters:
	 - int x1. current x-coordinate
	 - int y1. current y-coordinate
	 - int x2. new x-coordinate
	 - int y2. new y-coordinate
  * Return value:
	 - 0 if the pawn cannot move to (x2, y2)
	 - 1 if the pawn can move to (x2, y2)
	 - 2 if the pawn is trying to capture another pawn en passant
  * Logic:
	 - The pawn can move to a location one step in front of it
	 - The pawn can move two steps in front if it is in the first row
	 - The pawn can capture an opponent piece one step to the front and one to
		the left or right
	 - If an opponent pawn jumps two steps forward and right next to the current
		player's pawn, the current player can capture this pawn en passant
	 - The pawn cannot move if it is in any way pinned to the king - its removal
		should not cause the king to fall under attack
*******************************************************************************/
ret_val pawn_move_possible(int x1, int y1, int x2, int y2, chessboard board[9][9])
{
	ret_val return_val={0, ""};

	/*The pawn cannot move to a position outside the board*/
	if(x2<1 || y2<1 || x2>8 || y2>8) {
		strcpy(return_val.error, "The pawn cannot move outside the board");
		return return_val;
	}

	/*The pawn cannot move to a position occupied by the same player*/
	if(board[x2][y2].player==board[x1][y1].player) {
		strcpy(return_val.error, "You cannot take your own piece");
		return return_val;
	}

	/*u - increment variable*/
	int u;
	/*Initialising increment parameters*/
	if(board[x1][y1].player==WHT) {
		u = 1;             //White's pawns can only move forward in y-coordinate
	}
	else {
		u = -1;            //Black's pawns can only move backward in y-coordinate
	}

	/*If the pawn is trying to move one step forward*/
	if(y2==y1+u) {
			//If the pawn is trying to make an ordinary move
		if((x2==x1) && (board[x2][y2].player==0)) {
			return_val.val=1;
		}
			//If the pawn is trying to capture the opponent's piece
		else if( ((x2==x1+1) || (x2==x1-1)) && opp(board[x2][y2].player, board[x1][y1].player) ) {
			return_val.val=1;
		}
         //en passant //Piece on x1+1, y1 is assumed to be a pawn, for otherwise it will have enpassant_able as 0 !!!!!!!!!!
		else if(board[x1+1][y1].enpassant_able && board[x1+1][y1].ep_move_no==k-1 && opp(board[x1+1][y1].player, board[x1][y1].player) && x2==x1+1) {
		   return_val.val = 2;
		   board[0][8].pawn_pos.x = x2+1;
		   board[0][8].pawn_pos.y = y1;
      }
	   else if(board[x1-1][y1].enpassant_able && board[x1-1][y1].ep_move_no==k-1 && opp(board[x1-1][y1].player, board[x1][y1].player) && x2==x1-1) {
		   return_val.val = 2;
		   board[0][8].pawn_pos.x = x2-1;
		   board[0][8].pawn_pos.y = y1;
      }
      else strcpy(return_val.error, "The pawn can't move there");
	}
	/*If the pawn is trying to move two steps from its initial location*/
	else if((y2==y1+2*u) && (x1==x2)) {
		if( ((u==1)&&(y1==2)) || ((u==-1)&&(y1==7)) ) {
			if((board[x2][y2].player==0) && (board[x2][y1+u].player==0)) {
				return_val.val=1;
				board[x1][y1].enpassant_able=1;
				board[x1][y1].ep_move_no=k;
			}
			else strcpy(return_val.error, "The pawn can't move there");
		}
		else strcpy(return_val.error, "The pawn can't move there");
	}
	else strcpy(return_val.error, "The pawn can't move there");

	if(return_val.val == 0) {
		return return_val;
	}
	/*Checking if the king comes under attack on making the move*/
	else if(return_val.val == 2) {
      move_piece(board[0][8].pawn_pos.x, board[0][8].pawn_pos.y, x2, y2, board);
		if(king_in_check_after_move(x1, y1, x2, y2, board)) {
			return_val.val = 0;
			strcpy(return_val.error, "This move puts your king in check");
			move_piece(x2, y2, board[0][8].pawn_pos.x, board[0][8].pawn_pos.y, board);   //////////////////////////////////////
			return return_val;
		}
		move_piece(x2, y2, board[0][8].pawn_pos.x, board[0][8].pawn_pos.y, board);
	}
	else if(return_val.val == 1) {
		if(!end_of_game(x1, y1, x2, y2, board)) {
			if(king_in_check_after_move(x1, y1, x2, y2, board)) {
				return_val.val = 0;
				strcpy(return_val.error, "This move puts your king in check");
				return return_val;
			}
		}
	}
	return return_val;
}
/*End of function pawn_move_possible*******************************************/

/*FUNCTION ROOK_MOVE_POOSIBLE***************************************************
  * Function:
	 - Checks whether a rook can make a specified move
  * Parameters:
	 - int x1. current x-coordinate
	 - int y1. current y-coordinate
	 - int x2. new x-coordinate
	 - int y2. new y-coordinate
  * Return value:
	 - 0 if the rook cannot move to (x2, y2)
	 - 1 if the rook can move to (x2, y2)
  * Logic:
	 - The rook can only move to a square on the same rank or the same file
	 - The rook cannot move to the specified position if it is occupied by a
		piece of the same player
	 - The rook cannot move to the position if there are any pieces (of either
		player) in the way
	 - The rook cannot move if it is in any way pinned to the king - its removal
		should not cause the king to fall under attack
*******************************************************************************/
ret_val rook_move_possible(int x1, int y1, int x2, int y2, chessboard board[9][9])
{
	ret_val return_val={0, ""};

	/*The rook cannot move to a position outside the board*/
	if(x2<1 || y2<1 || x2>8 || y2>8) {
		strcpy(return_val.error, "The rook cannot move outside the board");
		return return_val;
	}
	/*The rook cannot move to a square occupied by a piece of the same player*/
	if(board[x2][y2].player==board[x1][y1].player) {
		strcpy(return_val.error, "You cannot take your own piece");
		return return_val;
	}

	/*If the rook is trying to move to a spot which is neither on the same rank
	  nor the same file*/
	if((x1!=x2) && (y1!=y2)) {
		strcpy(return_val.error, "The rook can't move there");
		return return_val;
	}
	/*The rook cannot move to a position occupied by a piece of the same player*/
	if(board[x2][y2].player==board[x1][y1].player) {
		strcpy(return_val.error, "You cannot take your own piece");
		return return_val;
	}

	/*i,j - counter variables*/
	int i, j;
	/*u,v - increment variables*/
	int u, v;

	/*If the rook is travelling along the same rank, the files have to be checked
	  for other pieces*/
	if(x1==x2) {
		u = 0;
		v = (y2>y1)? (1):(-1);
	}
	/*If the rook is travelling along the same file, the ranks have to be checked
	  for other pieces*/
	else {
		v = 0;
		u = (x2>x1)? (1):(-1);
	}

	/*Checking intervening squares for pieces*/
	for(i=x1+u, j=y1+v ; ((i!=x2+u) || (j!=y2+v)) ; i+=u, j+=v) {
		if(board[i][j].player==board[x1][y1].player) {
			strcpy(return_val.error, "There is another piece in the way");
			return return_val;
		}
		if((i!=x2) || (j!=y2)) {
			if(board[i][j].player!=0) {
				strcpy(return_val.error, "There is another piece in the way");
				return return_val;
			}
		}
	}
	/*If the king falls in check after the rook has been moved*/
	if(!end_of_game(x1, y1, x2, y2, board)) {
		if(king_in_check_after_move(x1, y1, x2, y2, board)) {
			strcpy(return_val.error, "This move puts your king in check");
			return return_val;
		}
	}

	return_val.val = 1;
	return return_val;
}
/*End of function rook_move_possible*******************************************/

/*FUNCTION BISHOP_MOVE_POSSIBLE*************************************************
  * Function:
	 - Checks if a bishop can make a specified move
  * Parameters:
	 - int x1. current x-coordinate
	 - int y1. current y-coordinate
	 - int x2. new x-coordinate
	 - int y2. new y-coordinate
  * Return value:
	 - 0 if the bishop cannot move to (x2, y2)
	 - 1 if the bishop can move to (x2, y2)
  * Logic:
	 - The bishop can only move diagonally
	 - The bishop cannot move to a square occupied by a piece of the same player
	 - The bishop cannot move to a square if there are any intervening pieces
		(of either player) along the same diagonal
	 - The bishop cannot move if it is in any way pinned to the king - its move
		should not cause the king to fall under check
*******************************************************************************/
ret_val bishop_move_possible(int x1, int y1, int x2, int y2, chessboard board[9][9])
{
	ret_val return_val={0, ""};

	/*The bishop cannot move to a position outside the board*/
	if(x2<1 || y2<1 || x2>8 || y2>8) {
		strcpy(return_val.error, "The bishop cannot move outside the board");
		return return_val;
	}
	/*The bishop cannot move to a square occupied by a piece of the same player*/
	if(board[x2][y2].player==board[x1][y1].player) {
		strcpy(return_val.error, "You cannot take your own piece");
		return return_val;
	}

	/*If the bishop is trying to move outside of its diagonals*/
	if(abs(x2-x1) != abs(y2-y1)) {
		strcpy(return_val.error, "The bishop can't move there");
		return return_val;
	}
	/*If the bishop is trying to move to a spot occupied by a piece of the same
	  player*/
	if(board[x2][y2].player==board[x1][y1].player) {
		strcpy(return_val.error, "You can't take your own piece");
		return return_val;
	}

	/*i,j - counter variables*/
	int i, j;
	/*u,v - increment variables*/
	int u, v;
	/*Initialising increment variables along the required diagonal*/
	u = (x2>x1)? (1):(-1);
	v = (y2>y1)? (1):(-1);

	/*Checking for intervening pieces along the diagonal*/
	for(i=x1+u, j=y1+v ; (i!=x2+u && j!=y2+v) ; i+=u, j+=v) {
		if(board[i][j].player==board[x1][y1].player) {
			strcpy(return_val.error, "There is another piece in the way");
			return return_val;
		}
		if((i!=x2) && (j!=y2)) {
			if(board[i][j].player!=0) {
				strcpy(return_val.error, "There is another piece in the way");
				return return_val;
			}
		}
	}

	/*Checking if the king falls in check after the bishop's move*/
	if(!end_of_game(x1, y1, x2, y2, board)) {
		if(king_in_check_after_move(x1, y1, x2, y2, board)) {
			strcpy(return_val.error, "This move puts your king in check");
			return return_val;
		}
	}

	return_val.val = 1;
	return return_val;
}
/*End of function bishop_move_possible*****************************************/

/*FUNCTION KNIGHT_MOVE_POSSIBLE*************************************************
  * Function:
	 - Checks whether a knight can make a specified move
  * Parameters:
	 - int x1. current x-coordinate
	 - int y1. current y-coordinate
	 - int x2. new x-coordinate
	 - int y2. new y-coordinate
  * Return value:
	 - 0 if the bishop cannot move to (x2, y2)
	 - 1 if the bishop can move to (x2, y2)
  * Logic:
	 - The knight can move to a square that is one step on one of the
		co-ordinates and two steps on the other
	 - The knight cannot move to a square occupied by a piece of the same player
	 - The knight can move to a square regardless of the intervening pieces
	 - The knight cannot move if it is in any way pinned to the king - its move
		should not cause the king to fall under attack
*******************************************************************************/
ret_val knight_move_possible(int x1, int y1, int x2, int y2, chessboard board[9][9])
{
	ret_val return_val={0, ""};

	/*The knight cannot move to a position outside the board*/
	if(x2<1 || y2<1 || x2>8 || y2>8) {
		strcpy(return_val.error, "The knight cannot move outside the board");
		return return_val;
	}

	/*The knight cannot move to a square occupied by a piece of the same player*/
	if(board[x2][y2].player==board[x1][y1].player) {
		strcpy(return_val.error, "You cannot take your own piece");
		return return_val;
	}

	/*If the knight is trying to move to a square outside its limits*/
	if( (abs(x2-x1)!=2) && (abs(y2-y1)!=2) ) {
		strcpy(return_val.error, "The knight can't move there");
		return return_val;
	}
	if( (abs(x2-x1)!=1) && (abs(y2-y1)!=1) ) {
		strcpy(return_val.error, "The knight can't move there");
		return return_val;
	}
	/*The knight cannot move to a square occupied by a piece of the same player*/
	if(board[x2][y2].player==board[x1][y1].player) {
		strcpy(return_val.error, "You can't take your own piece");
		return return_val;
	}
	/*The knight cannot move if it causes the king to fall under attack*/
	if(!end_of_game(x1, y1, x2, y2, board)) {
		if(king_in_check_after_move(x1, y1, x2, y2, board)) {
			strcpy(return_val.error, "This move puts your king in check");
			return return_val;
		}
	}
	return_val.val = 1;
	return return_val;
}
/*End of function knight_move_possible*****************************************/

/*FUNCTION QUEEN_MOVE_POSSIBLE**************************************************
  * Function:
	 - Checks if a queen can make a specified move
  * Parameters:
	 - int x1. current x-coordinate
	 - int y1. current y-coordinate
	 - int x2. new x-coordinate
	 - int y2. new y-coordinate
  * Return value:
	 - 0 if the queen cannot move to (x2, y2)
	 - 1 if the queen can move to (x2, y2)
  * Logic:
	 - The queen can move both like a rook and a bishop
	 - If either a rook or a bishop can make the specified move, so can the queen
	 - The queen cannot move if it is in any way pinned to the king - its move
		should not cause the king to fall under attack
*******************************************************************************/
ret_val queen_move_possible(int x1, int y1, int x2, int y2, chessboard board[9][9])
{
	ret_val return_val={0, ""};

	return_val = rook_move_possible(x1, y1, x2, y2, board);
	if(return_val.val) return return_val;
	return_val = bishop_move_possible(x1, y1, x2, y2, board);
	if(return_val.val) return return_val;

	strcpy(return_val.error, "Invalid move");
	return return_val;
}
/*End of function queen_move_possible******************************************/

/*FUNCTION KING_MOVE_POSSIBLE***************************************************
  * Function:
	 - Checks whether the king can make a specified move
  * Parameters:
	 - int x1. current x-coordinate
	 - int y1. current y-coordinate
	 - int x2. new x-coordinate
	 - int y2. new y-coordinate
  * Return value:
	 - 0 if the king cannot move to (x2, y2)
	 - 1 if the king can move to (x2, y2)
	 - 2 if the king castles
  * Logic:
	 - The king cannot move to a square occupied by a piece of the same player
	 - The king cannot move more than one step in any direction
	 - The king cannot move to a position if the move causes it to fall in check
*******************************************************************************/
ret_val king_move_possible(int x1, int y1, int x2, int y2, chessboard board[9][9])
{
	ret_val return_val={0, ""};

	/*The king cannot move to a position outside the board*/
	if(x2<1 || y2<1 || x2>8 || y2>8) {
		strcpy(return_val.error, "The king cannot move outside the board");
		return return_val;
	}
	/*The king cannot move to a square occupied by a piece of the same player*/
	if(board[x2][y2].player==board[x1][y1].player) {
		strcpy(return_val.error, "You cannot take your own piece");
		return return_val;
	}
	/*If the king is trying to castle*/
	if(abs(x2-x1)==2 && y1==y2) {					////////////////////////////////////////////////////////////////////////////////////
   	return_val = castle_possible(x1, y1, x2, y2, board);
		return return_val;
	}
	/*The king cannot move more than one step in any direction*/
	if( (abs(x2-x1)>1) || (abs(y2-y1)>1) ) {
		strcpy(return_val.error, "The king cannot move there");
		return return_val;
	}
	/*The king cannot move to a square that is under attack*/
	if(under_attack(x2, y2, board[x1][y1].player, board)) {
		strcpy(return_val.error, "That square is under attack");
		return return_val;
	}
	/*The king should not fall in check after moving*/
	if(king_in_check_after_move(x1, y1, x2, y2, board)) {
		strcpy(return_val.error, "That move puts your king in check");
		return return_val;
	}
	return_val.val = 1;
	return return_val;
}
/*End of function king_move_possible*******************************************/

/*FUNCTION CHECKMATE************************************************************
  * Function:
	 - Checks if either player has been checkmated
  * Parameters:
	 - None
  * Return value:
	 - 0 if neither player has been checkmated
	 - 1 if black has been checkmated
	 - 2 if white has been checkmated
	 - 3 if a stalemate condition has been reached
  * Logic:
	 - Finds the king's position and checks whether the king can move
	 - If this is not possible, the function proceeds to check if any of the
		current player's pieces can make a legal move
	 - If this is also not possible, stalemate condition has been reached
	 - Further, if the king is under attack by an opponent piece, then the
		current player has been checkmated and has lost
*******************************************************************************/
int checkmate(chessboard board[9][9])
{
	/*Declaring position structure to hold the co-ordinates of the king*/
	position king_pos;
	/*Finding the position of the current player's king*/
	king_pos = find_king(curr_player, board);

	/*i,j,k,l - counter variables*/
	int i, j, k, l;

	for(i=-1 ; i<=1 ; i++) {
		for(j=-1 ; j<=1 ; j++) {
			/*Checking if the king can make a legal move*/
			if(king_move_possible(king_pos.x, king_pos.y, king_pos.x+i,
 																	  king_pos.y+j, board).val) {
				return 0;
			}
		}
	}

	/*For loop to find each of the current player's pieces*/
	for(i=1 ; i<=8 ; i++) {
		for(j=1 ; j<=8 ; j++) {

			if(board[i][j].player==curr_player) {
				/*For loop to find a position to which this piece can be moved*/
				for(k=1 ; k<=8 ; k++) {
					for(l=1 ; l<=8 ; l++) {

							//Checking for pawn
						if(board[i][j].num>=17) {
							if(pawn_move_possible(i, j, k, l, board).val) {
								return 0;
							}
						}
						switch(board[i][j].num) {
							case 1 :
							case 8 :
							case 9 :
							case 16: 	//Checking for rook
										if(rook_move_possible(i, j, k, l, board).val) {
											return 0;
										}
										break;
							case 2 :
							case 7 :
							case 10:
							case 15: 	//Checking for knight
										if(knight_move_possible(i, j, k, l, board).val) {
											return 0;
										}
										break;
							case 3 :
							case 6 :
							case 11:
							case 14: 	//Checking for bishop
										if(bishop_move_possible(i, j, k, l, board).val) {
											return 0;
										}
										break;
							case 4 :
							case 12: 	//Checking for queen
										if(queen_move_possible(i, j, k, l, board).val) {
											return 0;
										}
										break;
						} /*End of switch statement*/

					}
				}
			} /*End of if statement*/

		}
	} /*End of outer loop*/

	/* *** If the program has reached this line, the game is over *** */

	/*If the king is under attack*/
	if(under_attack(king_pos.x, king_pos.y, curr_player, board)) {
		/*Return the number corresponding to the winning player*/
		return (3-curr_player);
	}
	else {
		/*Stalemate*/
		return 3;
	}
}
/*End of function checkmate****************************************************/

/*FUNCTION CASTLE_POSSIBLE******************************************************
  * Function:
	 - Checks if the king can castle
  * Parameters:
	 - int x1. current x-coordinate
	 - int y1. current y-coordinate
	 - int x2. new x-coordinate
	 - int y2. new y-coordinate
  * Return value:
	 - 0 if the king cannot castle
	 - 2 if the king can castle
  * Logic:
	 The king can castle if and only if the following conditions are satisfied
	 simultaneously
	 - The king must never have moved
	 - The chosen rook must never have moved
	 (These conditions hold good if the corresponding global variable for
	  castling holds a non-zero value)
	 - There must be no piece between the king and the chosen rook
	 - The king must not currently be in check
	 - The king must not pass over a square under attack by any enemy piece
	 - The king must not fall in check after the move has been made
	 - The chosen king and rook must be on the same rank
	 Castling is possible even if
	 - The king has been in check previously (but not currently)
	 - The rook involved is under attack
	 - The rook moves over a square under attack
*******************************************************************************/
ret_val castle_possible(int x1, int y1, int x2, int y2, chessboard board[9][9])
{
	ret_val return_val={0, ""};          ////////////////////////////////////////Converted all curr_player to board[x1][y1].player

	/*If the king tries to move to a different rank*/
	if(y2!=y1) {
		return return_val;
	}
	/*i,count - counter variable*/
	int i, count;
	/*u - increment variable*/
	int u;

	/*In case of king's side castle*/
	if(x2==x1+2) {
			//Increment is positive
		u = 1;
			//Castling is possible only if neither the king nor the rook has moved
		if((board[x1][y1].player==WHT) && (!board[0][8].wht_king_castle)) {
			strcpy(return_val.error, "Your king or rook has moved before");
			return return_val;
		}
		else if((board[x1][y1].player==BLK) && (!board[0][8].blk_king_castle)) {
			strcpy(return_val.error, "Your king or rook has moved before");
			return return_val;
		}
	}
	/*In case of queen's side castle*/
	else if(x2==x1-2) {
			//Increment is negative
		u = -1;
			//Castling is possible only if neither the king nor the rook has moved
		if((board[x1][y1].player==WHT) && (!board[0][8].wht_queen_castle)) {
			strcpy(return_val.error, "Your king or rook has moved before");
			return return_val;
		}
		else if((board[x1][y1].player==BLK) && (!board[0][8].blk_queen_castle)) {
			strcpy(return_val.error, "Your king or rook has moved before");
			return return_val;
		}
	}
	else {
		return return_val;
	}

	/*The king must not pass over a square under attack*/
	for(i=x1, count=0 ; count<=2 ; i+=u, count++) {
		if(under_attack(i, y1, curr_player, board)) {
			strcpy(return_val.error, "Your king is under attack or passes over a square under attack");
			return return_val;
		}
	}

	/*Checking if the rook can move to its new location - no intervening pieces*/
	if(u==1) {
		if(!rook_move_possible(8, y1, 6, y1, board).val) {
			strcpy(return_val.error, "There is a piece in the way");
			return return_val;
		}
	}
	else if(u==-1) {
		if(!rook_move_possible(1, y1, 4, y1, board).val) {
			strcpy(return_val.error, "There is a piece in the way");
			return return_val;
		}
	}

	/*Returns castle possible if all conditions are satisfied*/
	return_val.val = 2;
	return return_val;
}
/*End of function castle_possible**********************************************/

/*FUNCTION MOVE_CASTLE**********************************************************
  * Function:
	 - Performs castling by moving the king and the rook
  * Parameters:
	 - int x1. current x-coordinate of the king
	 - int y1. current y-coordinate of the king
	 - int x2. new x-coordinate of the king
	 - int y2. new y-coordinate of the king
  * Return value:
	 - None
  * Logic:
	 - Uses the move_piece function to first move the king, then the rook,
		depending on whether a king's side or queen's side castle is performed
	 - Move_piece automatically invalidates any further castling
*******************************************************************************/
void move_castle(int x1, int y1, int x2, int y2, chessboard board[9][9])
{
	move_piece(x1, y1, x2, y2, board);
	if(x2>x1) {
		move_piece(x2+1, y1, x1+1, y2, board);
	}
	else {
		move_piece(x2-2, y1, x1-1, y2, board);
	}
}
/*End of function move_castle**************************************************/

/*FUNCTION KING_IN_CHECK_AFTER_MOVE*********************************************
  * Function:
	 - Checks if the king is put in check after a move has been made, without
		actually performing the move
  * Parameters:
	 - int x1. current x-coordinate
	 - int y1. current y-coordinate
	 - int x2. new x-coordinate
	 - int y2. new y-coordinate
  * Return value:
	 - 0 if the king does not fall in check
	 - 1 if the king falls in check
  * Logic:
	 - The function stores a copy of the chessboard in a temporary location
	 - It then performs the move and then finds the king's co-ordinates
	 - If these co-ordinates are under attack by an enemy piece, the king is in
		check
	 - The board is then restored to its former state
*******************************************************************************/
int king_in_check_after_move(int x1, int y1, int x2, int y2, chessboard board[9][9])
{
	/*Creating the temporary chessboard*/
	chessboard temp_board[9][9];
	/*i,j - counter variables*/
	int i, j;
	/*Position structure for storing the king's co-ordinates*/
	position king_pos;
	int return_val;

	/*Initialising the temporary board*/
	for(i=0 ; i<=8 ; i++) {
		for(j=0 ; j<=8 ; j++) {
			temp_board[i][j]=board[i][j];
		}
	}

	/*Performing the move*/
	board[x2][y2].num = board[x1][y1].num;
	board[x2][y2].player = board[x1][y1].player;
	board[x1][y1].num = 0;
	board[x1][y1].player = 0;

	/*Finding the king's co-ordinates*/
	king_pos = find_king(board[x2][y2].player, board);

	/*Checking if the king has fallen under attack*/
	if(under_attack(king_pos.x, king_pos.y, board[x2][y2].player, board)) {
		return_val = 1;
	}
	else {
		return_val = 0;
	}

	/*Restoring the board to its former state*/
	for(i=0 ; i<=8 ; i++) {
		for(j=0 ; j<=8 ; j++) {
			board[i][j]=temp_board[i][j];
		}
	}

	return return_val;
}
/*End of function king_in_check_after_move*************************************/

/*FUNCTION END_OF_GAME**********************************************************
  * Function:
	 - Checks if the given move results in the end of the game, that is, if it
		captures the opponent's king
  * Parameters:
	 - int x1. current x-coordinate of capturing piece
	 - int y1. current y-coordinate of capturing piece
	 - int x2. new x-coordinate of capturing piece
	 - int y2. new y-coordinate of capturing piece
  * Return value:
	 - 0 if the move does not result in the end of the game
	 - 1 if the move results in the end of the game
  * Logic:
	 - The move results in the end of the game if it captures the opponent's king
	 - Therefore, the square given by (x2, y2) should contain the opponent's king
		for the function to return affirmative
*******************************************************************************/
int end_of_game(int x1, int y1, int x2, int y2, chessboard board[9][9])
{
	if(board[x1][y1].player==WHT && board[x2][y2].num==13) {
		return 1;
	}
	else if(board[x1][y1].player==BLK && board[x2][y2].num==5) {
		return 1;
	}
	else {
		return 0;
	}
}
/*End of function end_of_game**************************************************/




/*********************REPLACEMENTS FOR GRAPHICS FUNCTIONS**********************/

void accept_input(int &x1, int &y1, int &x2, int &y2, chessboard board[9][9])
{
	display(board);

	refresh:
	cout<<"\n\nEnter current coordinates: ";
	cin>>x1>>y1;

	if(x1<1 || x1>8 || y1<1 || y1>8) {
		msg_box("Invalid location", board);
		goto refresh;
	}
	if(board[x1][y1].player == 0) {
		msg_box("No piece in this position", board);
		goto refresh;
	}
	if(opp(board[x1][y1].player)) {
		msg_box("You cannot move the opponent's piece", board);
		goto refresh;
	}

	cout<<"Enter new coordinates: ";
	cin>>x2>>y2;

	if(x2<1 || x2>8 || y2<1 || y2>8) {
		msg_box("Invalid location", board);
		goto refresh;
	}
	if((x2==x1) && (y2==y1)) {
		goto refresh;
	}
	if(board[x2][y2].player == curr_player) {
		msg_box("You cannot take your own piece", board);
		goto refresh;
	}
}

void display(chessboard board[9][9])
{
	int i, j;
	for(j=8 ; j>=1 ; j--) {
		cout<<"\n\n\n";
		for(i=1 ; i<=8 ; i++) {
			cout<<"\t";
			if(board[i][j].player == WHT)
				cout<<"W";
			else if(board[i][j].player == BLK)
				cout<<"B";
			else
				cout<<0;
			if(board[i][j].num>=17) {
				cout<<"P";
			}
			else {
				switch(board[i][j].num) {
					case 1 :
					case 8 :
					case 9 :
					case 16: cout<<"R";
								break;
					case 2 :
					case 7 :
					case 10:
					case 15: cout<<"N";
								break;
					case 3 :
					case 6 :
					case 11:
					case 14:	cout<<"B";
								break;
					case 4 :
					case 12: cout<<"Q";
								break;
					case 5 :
					case 13: cout<<"K";
								break;
				}
			}
		}
	}
}

void msg_box(char msg[], chessboard board[9][9])
{
	cout<<msg<<"\n";
}

char pawn_promotion_msg(void)
{
	char ch;
	cout<<"Promote to Q/R/B/K";
	cin>>ch;
	return ch;
}



/*******************************AI STARTS HERE*********************************/
/*
int control(int, int, int, int, chessboard [9][9]);
void copy_board(chessboard [9][9], chessboard [9][9]);
int piece_val(chessboard piece);
int checkmate_after_move(int, int, int, int, chessboard [9][9]);
int num_centre_sq_controlled(int, int, int, int, chessboard [9][9]);
int opp_king_in_check_after_move(int, int, int, int, chessboard [9][9]);
int under_control(int, int, int, chessboard [9][9]);
int assault(int, int, int, chessboard [9][9], int);
int attacker_exists(int, int, chessboard [9][9]);
int defender_exists(int, int, chessboard [9][9]);
position find_smallest_piece_attacking(int, int, int, chessboard [9][9]);
void compile_undefended_pieces(position [16], int &, int, chessboard [9][9]);
void compile_attacked_pieces(int, int, position [16], int &, chessboard [9][9]);
int update(int, int, chessboard [9][9]);
int num_sq_controlled(int , chessboard [9][9]);
int num_centre_sq_controlled(int, chessboard [9][9]);

class action {
	private:
		position init;
		position fin;
	public:
		int score;
		action(void);
		~action(void) {}
		inline void initialise(int, int, int, int);
		inline void perform(chessboard [9][9]);
		void evaluate(chessboard [9][9]);
};

action::action(void)
{
	score = -32768;
	init.x = 0;
	init.y = 0;
	fin.x = 0;
	fin.y = 0;
}

void ai(chessboard board[9][9])
{
	action move[2];
	int i, j, k, l;
	for(i=1 ; i<=8 ; i++) {
		for(j=1 ; j<=8 ; j++) {
			if(board[i][j].player == AI) {
				for(k=1 ; k<=8 ; k++) {
					for(l=1 ; l<=8 ; l++) {
						if(piece_move_possible(i, j, k, l, board).val) {    //NOTE: Due to the presence of this statement, evaluate does not check to see if the move is valid. evalute implicitly assumes the same
							move[1].initialise(i, j, k, l);
							move[1].evaluate(board);
							if(move[1].score > move[0].score) {
								move[0] = move[1];
							}
						}
					}
				}
			}
		}
	}
	move[0].perform(board);
}

void action::initialise(int x1, int y1, int x2, int y2)
{
	init.x = x1;
	init.y = y1;
	fin.x = x2;
	fin.y = y2;
}

void action::perform(chessboard board[9][9])
{
   if( (board[init.x][init.y].num == 5 || board[init.x][init.y].num == 13) && abs(init.x - fin.x) == 2 ) {
      move_castle(init.x, init.y, fin.x, fin.y, board);
   }
   else {
      move_piece(init.x, init.y, fin.x, fin.y, temp_board);
   }
}

int control(int x1, int y1, int x2, int y2, chessboard board[9][9])
{
	chessboard temp_board[9][9];
	int i, j;
   copy_board(temp_board, board);

	int opp_player;
	opp_player = (board[x1][y1].num+1) % 2;
	if(opp_player == 0) opp_player += 2;

	for(i=0 ; i<=8 ; i++) {
		for(j=0 ; j<=8 ; j++) {
			if(i!=x1 || j!=y1) {
				if(board[i][j].player) {
					temp_board[i][j].player = opp_player;
				}
			}
		}
	}

	if(piece_move_possible(x1, y1, x2, y2, temp_board).val)
		return 1;
	else
		return 0;
}

void copy_board(chessboard temp[9][9], chessboard board[9][9])
{
   int i, j;
   for(i=0 ; i<=8 ; i++) {
      for(j=0 ; j<=8 ; j++) {
         temp[i][j] = board[i][j];
      }
   }
}


/************** ROUGH DRAFT OF EVALUATE AND UPDATE FUNCTIONS ******************/
/*
#define king_val 0
#define queen_val 9000
#define rook_val 5000
#define knight_val 3000
#define bishop_val 3000
#define pawn_val 1000

#define centre_sq_inc 1
#define attack_multiplier 0.1
#define king_check_inc 30
#define king_proximity_inc 0

int operator!= (chessboard board1, chessboard board2)
{
   if(board1.num != board2.num || board1.player != board2.player) {
      return 1;
   }
	if(board1.wht_king_castle != board2.wht_king_castle) {
      return 1;
   }
   return 0;
}

void action::evaluate(chessboard board[9][9])
{
   score = 0;
   int s1 = update(AI, AI, board);
   chessboard temp_board[9][9];
   copy_board(temp_board, board);
   if( (board[init.x][init.y].num == 5 || board[init.x][init.y].num == 13) && abs(init.x - fin.x) == 2 ) {
      move_castle(init.x, init.y, fin.x, fin.y, board);
   }
   else {
      move_piece(init.x, init.y, fin.x, fin.y, temp_board);
   }
   int s2 = update(AI, HS, temp_board);
   score = s2 - s1;

/*
   int x1 = init.x;
   int y1 = init.y;
   int x2 = fin.x;
   int y2 = fin.y;

	if(checkmate_after_move(x1, y1, x2, y2, board)) {
      score = 32767;
      return;
   }

   score += centre_sq_inc * num_centre_sq_controlled(x1, y1, x2, y2, board);
   if(k==1 || k==2) {
      if((x1==4 || x1==5) && abs(y2-y1)==2)
         score += 2*centre_sq_inc;
   }

   /*AI attacking an opponent piece*/
/*   if(opp(board[x2][y2].player, board[x1][y1].player)) {
      score += piece_val(board[x2][y2]);
   }
   chessboard temp_board[9][9];
   copy_board(temp_board, board);
   move_piece(x1, y1, x2, y2, temp_board);
   if(under_attack(x2, y2, temp_board[x2][y2].player, temp_board)) {
      if(!under_control(x2, y2, temp_board[x2][y2].player, temp_board)) {
         score -= piece_val(temp_board[x2][y2]);
      }
      else {
         score += assault(x2, y2, board[x2][y2].player, temp_board);
      }
   }

   /*AI defending undefended pieces*/
/*   position undefended[16];
	int i, num;
   copy_board(temp_board, board);
   compile_undefended_pieces(undefended, num, temp_board[x1][y1].player, temp_board);
   for(i=0 ; i<num ; i++) {
		if(undefended[i].x != x1 && undefended[i].y != y1) {
      	move_piece(x1, y1, x2, y2, temp_board);
			if(under_control(x2, y2, temp_board[x2][y2].player, temp_board)) {
   	      score += assault(undefended[i].x, undefended[i].y, temp_board[x2][y2].player, temp_board);
         }
         copy_board(temp_board, board);
      }
   }

   /*AI moving piece out of attack*/
/*   for(i=0 ; i<num ; i++) {
      if(temp_board[x1][y1] != temp_board[undefended[i].x][undefended[i].y]) {
         score -= piece_val(temp_board[undefended[i].x][undefended[i].y]);
      }
	}

   /*AI moving piece into attack position*/
   //copy_board(temp_board, board);
   //move_piece(x1, y1, x2, y2, temp_board);
/*   position attacked[16];
   position opp_king_pos = find_king(3-temp_board[x2][y2].player, temp_board);
   compile_attacked_pieces(x2, y2, attacked, num, temp_board);
   for(i=0 ; i<num ; i++) {
      if(attacked[i].x == opp_king_pos.x && attacked[i].y == opp_king_pos.y) {
         score += king_check_inc;
         continue;
      }
      if(!defender_exists(attacked[i].x, attacked[i].y, temp_board)) {
         score += attack_multiplier * piece_val(temp_board[attacked[i].x][attacked[i].y]);
      }
      else if(piece_val(temp_board[attacked[i].x][attacked[i].y]) > piece_val(temp_board[x2][y2])) {
         score += attack_multiplier * (piece_val(temp_board[attacked[i].x][attacked[i].y]) - piece_val(temp_board[x2][y2]));
      }
      if(abs(attacked[i].x - opp_king_pos.x) <= 1 && abs(attacked[i].y - opp_king_pos.y) <=1) {
         score += king_proximity_inc;
      }
   }

   */ /*
}

int checkmate_after_move(int x1, int y1, int x2, int y2, chessboard board[9][9])
{
   chessboard temp_board[9][9];
   copy_board(temp_board, board);

   move_piece(x1, y1, x2, y2, temp_board);

   return checkmate(temp_board);
}

int num_centre_sq_controlled(int x1, int y1, int x2, int y2, chessboard board[9][9])
{
   int count=0;
   chessboard temp_board[9][9];
   copy_board(temp_board, board);

   move_piece(x1, y1, x2, y2, temp_board);
   if(control(x2, y2, 4, 4, temp_board)) count++;
   if(control(x2, y2, 4, 5, temp_board)) count++;
   if(control(x2, y2, 5, 4, temp_board)) count++;
   if(control(x2, y2, 5, 5, temp_board)) count++;

   return count;
}

int piece_val(chessboard piece)
{
   switch(piece.num) {
      case 0 : return 0;
      case 1 :
      case 8 :
		case 9 :
      case 16: return rook_val;
      case 2 :
      case 7 :
      case 10:
      case 15: return knight_val;
      case 3 :
      case 6 :
      case 11:
      case 14: return bishop_val;
      case 4 :
      case 12: return queen_val;
		case 5 :
      case 13: return king_val;
      default: return pawn_val;
   }
}

int opp_king_in_check_after_move(int x1, int y1, int x2, int y2, chessboard board[9][9])
{
   chessboard temp_board[9][9];
   copy_board(temp_board, board);
   position king_pos;
   int opp_player;

   if(board[x1][y1].player == WHT) opp_player = BLK;
   if(board[x1][y1].player == BLK) opp_player = WHT;

   move_piece(x1, y1, x2, y2, temp_board);
   king_pos = find_king(opp_player, temp_board);

   if(under_attack(king_pos.x, king_pos.y, opp_player, temp_board)) {
      return 1;
   }

	return 0;
}

/*checks if x1, y1 is under control by the given player*/
/*int under_control(int x1, int y1, int player, chessboard board[9][9])
{
	int i, j;

	for(i=1 ; i<=8 ; i++) {
		for(j=1 ; j<=8 ; j++) {
			int flag=0;
				//If the square is empty
			if(board[i][j].player==0) {
				continue;
			}
            //
			if(board[i][j].player == player) {
					//Checking opponent pieces
				//pawn
				if(board[i][j].num >= 17) {
					if(board[i][j].player==1) {
						if( (y1==j+1) && ((x1==i-1)||(x1==i+1)) ) {
							return 1;
						}
						flag = 1;
					}
					else {
						if( (y1==j-1) && ((x1==i-1)||(x1==i+1)) ) {
							return 1;
						}
						flag = 1;
					}
				}
				//king
				if(board[i][j].num==5 || board[i][j].num==13) {
					if( (abs(i-x1)<=1) && (abs(y1-j)<=1) ) {
						return 1;
					}
					else {
						flag = 1;
					}
				}

				if(!flag) {
					if(control(i, j, x1, y1, board)) {
						return 1;
					}
				}

			}
		}
	}


	return 0;
}

/*reqd_player is the player whose score is needed*/
/*
int assault(int x1, int y1, int reqd_player, chessboard board[9][9])
{
   int net_point_gain = 0, min_net_point_gain = 0;

   int player2 = board[x1][y1].player;
   int player1;
   if(player2 == BLK) player1 = WHT;
   else player1 = BLK;

   chessboard temp_board[9][9];
   copy_board(temp_board, board);

   position smallest_attacker, smallest_defender;
   while(attacker_exists(x1, y1, temp_board) && defender_exists(x1, y1, temp_board)) {
      smallest_attacker = find_smallest_piece_attacking(x1, y1, player2, temp_board);
      if(reqd_player == player2) {
         net_point_gain -= piece_val(temp_board[x1][y1]);
      }
      else {
         net_point_gain += piece_val(temp_board[x1][y1]);
			if(min_net_point_gain > abs(net_point_gain) && min_net_point_gain > 0) {
            min_net_point_gain = net_point_gain;
         }
         else if(abs(min_net_point_gain) > abs(net_point_gain) && net_point_gain < 0 && min_net_point_gain < 0) {
            min_net_point_gain = net_point_gain;
         }
      }
      move_piece(smallest_attacker.x, smallest_attacker.y, x1, y1, temp_board);

      smallest_defender = find_smallest_piece_attacking(x1, y1, player1, temp_board);
      if(reqd_player == player2) {
         net_point_gain += piece_val(temp_board[x1][y1]);
         if(min_net_point_gain > abs(net_point_gain) && min_net_point_gain > 0) {
            min_net_point_gain = net_point_gain;
         }
         else if(abs(min_net_point_gain) > abs(net_point_gain) && net_point_gain < 0 && min_net_point_gain < 0) {
            min_net_point_gain = net_point_gain;
         }
      }
      else {
         net_point_gain -= piece_val(temp_board[x1][y1]);
      }
      move_piece(smallest_defender.x, smallest_defender.y, x1, y1, temp_board);
	}

   if(attacker_exists(x1, y1, temp_board)) {
      smallest_attacker = find_smallest_piece_attacking(x1, y1, player2, temp_board);
      if(reqd_player == player2) {
         net_point_gain -= piece_val(temp_board[x1][y1]);
      }
      else {
         net_point_gain += piece_val(temp_board[x1][y1]);
         if(min_net_point_gain > abs(net_point_gain) && min_net_point_gain > 0) {
            min_net_point_gain = net_point_gain;
         }
			else if(abs(min_net_point_gain) > abs(net_point_gain) && net_point_gain < 0 && min_net_point_gain < 0) {
            min_net_point_gain = net_point_gain;
         }
      }
      move_piece(smallest_attacker.x, smallest_attacker.y, x1, y1, temp_board);
   }

   return min_net_point_gain;
}
*/
/*
int assault(int x1, int y1, int reqd_player, chessboard board[9][9], int score=0)
{

   //!! - The ai should NOT be negative on points after one of its own turns. It's okay to be negative on points after the opponent's turn.
   //Only return if score<0 AND opponent is to play - meaning you're about to lose even more points
   if(board[x1][y1].player == reqd_player && score < 0) {
      return score;
   }

   if(!under_attack(x1, y1, board[x1][y1].player, board)) {
      return score;
   }

   position sa = find_smallest_piece_attacking(x1, y1, board[x1][y1].player, board);
   chessboard temp_board[9][9];
   copy_board(temp_board, board);
   
   if(board[x1][y1].player == reqd_player) {
      score -= piece_val(temp_board[x1][y1]);
   }
   else {
      score += piece_val(temp_board[x1][y1]);
   }
   
	move_piece(sa.x, sa.y, x1, y1, temp_board);

   return assault(x1, y1, reqd_player, temp_board, score);
}

position find_smallest_piece_attacking(int x1, int y1, int player, chessboard board[9][9])
{
   int i, j, flag=0;
   position smallest_piece = {-1, -1};

   for(i=1 ; i<=8 ; i++) {
      for(j=1 ; j<=8 ; j++) {
         if(board[i][j].player) {
            if(opp(board[i][j].player, player)) {
               if(piece_move_possible(i, j, x1, y1, board).val) {
                  if(flag == 0) {
                     smallest_piece.x = i;
                     smallest_piece.y = j;
                     flag = 1;
                  }
                  else {
                     if(piece_val(board[smallest_piece.x][smallest_piece.y]) > piece_val(board[i][j])) {
                        smallest_piece.x = i;
								smallest_piece.y = j;
                     }
                  }
               }
            }
         }
      }
   }

   return smallest_piece;
}

int attacker_exists(int x1, int y1, chessboard board[9][9])
{
   int i, j;
   for(i=1 ; i<=8 ; i++) {
      for(j=1 ; j<=8 ; j++) {
         if(board[i][j].player) {
            if(opp(board[i][j].player, board[x1][y1].player)) {
               if(control(i, j, x1, y1, board)) {
                  return 1;
               }
            }
         }
      }
   }

   return 0;
}

int defender_exists(int x1, int y1, chessboard board[9][9])
{
   int i, j;
   for(i=1 ; i<=8 ; i++) {
      for(j=1 ; j<=8 ; j++) {
			if(board[i][j].player) {
            if(board[i][j].player == board[x1][y1].player) {
               if(control(i, j, x1, y1, board)) {
                  return 1;
               }
            }
         }
      }
   }

   return 0;
}

void compile_undefended_pieces(position undefended[16], int &num, int player, chessboard board[9][9])
{
   int i, j, count=0;
   for(i=1 ; i<=8 ; i++) {
      for(j=1 ; j<=8 ; j++) {
         if(board[i][j].player) {
            if(board[i][j].player == player) {
               if(under_attack(i, j, board[i][j].player, board)) {
                  if(!defender_exists(i, j, board)) {
                     undefended[count].x = i;
                     undefended[count].y = j;
                     count++;
                  }
                  else if(assault(i, j, board[i][j].player, board) < 0) {
                     undefended[count].x = i;
                     undefended[count].y = j;
                     count++;
                  }
               }
            }
         }
      }
	}
   num = count;
}

void compile_attacked_pieces(int x1, int y1, position attacked[16], int &num, chessboard board[9][9])
{
   int i, j, count=0;
   for(i=1 ; i<=8 ; i++) {
      for(j=1 ; j<=8 ; j++) {
         if(board[i][j].player) {
            if(opp(board[i][j].player, board[x1][y1].player)) {
               if(piece_move_possible(x1, y1, i, j, board).val) {
						attacked[count].x = i;
                  attacked[count].y = j;
                  count++;
               }
            }
         }
      }
   }
   num = count;
}

int update(int reqd_player, int to_play, chessboard board[9][9])
{
   int i, j, k;
   int score = 0;

   for(i=1 ; i<=8 ; i++) {
   	for(j=1 ; j<=8 ; j++) {

      	if(board[i][j].player == reqd_player) {
            score += piece_val(board[i][j]);

         	//AI has undefended pieces
				if(under_attack(i, j, board[i][j].player, board)) {
               int point_gain = assault(i, j, board[i][j].player, board);
               if(point_gain < 0) score += point_gain;
/*	         	if(!under_control(i, j, board[i][j].player, board)) {
                  score -= piece_val(board[i][j]);
	            }*/
/*	         	else {
	              	score -= assault(i, j, (3-board[i][j].player), board);
	            } */
/*	         }

	         //AI is attacking an opponent piece
				int num;
 	         position attacked[16];
  	         compile_attacked_pieces(i, j, attacked, num, board);
  	         for(k=0 ; k<num ; k++) {
               int point_gain = assault(attacked[k].x, attacked[k].y, board[i][j].player, board);
               if(to_play == reqd_player) {
                  score += point_gain;
               }
               else {
                  if(!under_attack(i, j, board[i][j].player, board)) {
                     score += attack_multiplier * point_gain;
                  }
               }
  	         }

				if(state == OP) {
	         	score += num_sq_controlled(board[i][j].player, board);
	            score += 2*num_centre_sq_controlled(board[i][j].player, board);
	         }
         }

         if(board[i][j].player && board[i][j].player!=reqd_player) {
         	score -= piece_val(board[i][j]);
			}
   	}
	}

   return score;
}

int num_sq_controlled(int player, chessboard board[9][9])
{
	int i, j, count=0;

   for(i=1 ; i<=8 ; i++) {
		for(j=1 ; j<=8 ; j++) {
      	if(!board[i][j].player && under_control(i, j, player, board)) {
         	count++;
         }
      }
   }

   return count;
}

int num_centre_sq_controlled(int player, chessboard board[9][9])
{
	int num=0;
   if(under_control(4, 4, player, board)) {
   	num++;
   }
	if(under_control(4, 5, player, board)) {
   	num++;
   }
   if(under_control(5, 4, player, board)) {
   	num++;
   }
   if(under_control(5, 5, player, board)) {
		num++;
   }

   return num;
}


*/




