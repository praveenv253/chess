


/*FUNCTION ACCEPT_INPUT*********************************************************
  * Function:
	 - Accepts input from the user in terms of the initial and final positions of
		the piece to be moved using keyboard interface
	 - Displays any simple mistakes that the user may have made when trying to
		move the piece
  * Parameters:
	 - int x1. current x-coordinate
	 - int y1. current y-coordinate
	 - int x2. new x-coordinate
	 - int y2. new y-coordinate
  * Return value:
	 - None
*******************************************************************************/
void accept_input(int &x1, int &y1, int &x2, int &y2, chessboard board[9][9])
{
	/*c is the centring factor for displaying the cursor*/
	int c=(x8-y8)*4;
	/*Position structures to contain the coordinates of the cursor and the king*/
	position cursor_pos, king_pos;
	king_pos = find_king(curr_player, board);

	/*Refresh label - All simple errors made by the user regroup here*/
	refresh:

	/*Displaying the board and cursor. The cursor is initially over the king*/
	display(board);
	cursor_pos = king_pos;
	draw_cursor(cursor_pos.x, cursor_pos.y, curr_player);

	in.h.ah=0;
	out.h.ah=0;
	/*While loop to accept initial x- and y-coordinates*/
	while(out.h.ah!=ENTER) {
		/*The computer waits for input*/
		while(!kbhit());
		/*Input is received through keyboard interrupt*/
		int86(0x16, &in, &out);

		/*If the user wishes to exit*/
		if(out.h.ah==ESC) {
				//The program displays the exit message to confirm
			exit_msg(board);
				//If the user does not exit, the board and cursor are re-drawn
			draw_cursor(cursor_pos.x, cursor_pos.y, curr_player);
				//The program waits for further input
			while(!kbhit());
			int86(0x16, &in, &out);
		}

		/*Draw a rectangle to erase the earlier cursor*/
		setcolor(WHITE);
		if((cursor_pos.x+cursor_pos.y)%2==1) {
			setfillstyle(1, LIGHTGRAY);
		}
		else {
			setfillstyle(1, DARKGRAY);
		}
		int i;
		int rect[10]={ cursor_pos.x-1, 9-cursor_pos.y,
							cursor_pos.x, 9-cursor_pos.y,
							cursor_pos.x, 8-cursor_pos.y,
							cursor_pos.x-1, 8-cursor_pos.y,
							cursor_pos.x-1, 9-cursor_pos.y };
		for(i=0 ; i<10 ; i++) {
			rect[i]*=y8;
			if(i%2==0) {
				rect[i]+=c;
			}
			else {
				rect[i]+=1;
			}
		}
		fillpoly(4, rect);
		draw_piece(board[cursor_pos.x][cursor_pos.y].num, cursor_pos.x,
																						cursor_pos.y);

		/*Find the new position of the cursor depending on the keyboard input*/
		switch(out.h.ah) {
			case LEFT : if(cursor_pos.x>1) {
								cursor_pos.x--;
							}
							break;
			case RIGHT: if(cursor_pos.x<8) {
								cursor_pos.x++;
							}
							break;
			case UP   : if(cursor_pos.y<8) {
								cursor_pos.y++;
							}
							break;
			case DOWN : if(cursor_pos.y>1) {
								cursor_pos.y--;
							}
							break;
		}
		/*Draws a cursor at the new cursor position*/
		draw_cursor(cursor_pos.x, cursor_pos.y, curr_player);
	}
	/*When the user presses enter, the initial positions are accepted*/
	x1 = cursor_pos.x;
	y1 = cursor_pos.y;

	/*Displaying any errors in input of initial position*/
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

	out.h.ah = 0;
	/*While loop to accept the final position of the moving piece*/
	while(out.h.ah!=ENTER) {
		int i, j, flag;
		flag=0;
		/*While loop to produce a blinking effect until the cursor is moved*/
		while(!kbhit()) {
				//The piece is erased and re-drawn every 250 milliseconds
			delay(250);
			if(flag) flag=0;
			else flag=1;
			if(flag) {
					//Erase the piece
				setcolor(WHITE);
				if((cursor_pos.x+cursor_pos.y)%2==1) {
					setfillstyle(1, LIGHTGRAY);
				}
				else {
					setfillstyle(1, DARKGRAY);
				}
				int rect[10]={ cursor_pos.x-1, 9-cursor_pos.y,
									cursor_pos.x, 9-cursor_pos.y,
									cursor_pos.x, 8-cursor_pos.y,
									cursor_pos.x-1, 8-cursor_pos.y,
									cursor_pos.x-1, 9-cursor_pos.y };
				for(i=0 ; i<10 ; i++) {
					rect[i]*=y8;
					if(i%2==0) {
						rect[i]+=c;
					}
					else {
						rect[i]+=1;
					}
				}
				fillpoly(4, rect);
				draw_cursor(cursor_pos.x, cursor_pos.y, curr_player);
			}
			else {
					//Re-draw the piece
				draw_piece(board[x1][y1].num, cursor_pos.x, cursor_pos.y);
			}
		}
		/*Accept input from keyboard when key is pressed*/
		int86(0x16, &in, &out);

		/*Erase the position where the piece was previously to give the impression
		  of moving the piece*/
		setcolor(WHITE);
		if((cursor_pos.x+cursor_pos.y)%2==1) {
			setfillstyle(1, LIGHTGRAY);
		}
		else {
			setfillstyle(1, DARKGRAY);
		}
		int rect[10]={ cursor_pos.x-1, 9-cursor_pos.y,
							cursor_pos.x, 9-cursor_pos.y,
							cursor_pos.x, 8-cursor_pos.y,
							cursor_pos.x-1, 8-cursor_pos.y,
							cursor_pos.x-1, 9-cursor_pos.y };
		for(i=0 ; i<10 ; i++) {
			rect[i]*=y8;
			if(i%2==0) {
				rect[i]+=c;
			}
			else {
				rect[i]+=1;
			}
		}
		fillpoly(4, rect);
		if(!(cursor_pos.x==x1 && cursor_pos.y==y1)) {
			/*Draw the piece which was on the square previously*/
			draw_piece(board[cursor_pos.x][cursor_pos.y].num, cursor_pos.x,
																						cursor_pos.y);
		}

		/*Find the new position of the cursor depending on the keyboard input*/
		switch(out.h.ah) {
			case LEFT : if(cursor_pos.x>1) {
								cursor_pos.x--;
							}
							break;
			case RIGHT: if(cursor_pos.x<8) {
								cursor_pos.x++;
							}
							break;
			case UP   : if(cursor_pos.y<8) {
								cursor_pos.y++;
							}
							break;
			case DOWN : if(cursor_pos.y>1) {
								cursor_pos.y--;
							}
							break;
		}
	}
	/*When the user presses enter, the final positions are accepted*/
	x2 = cursor_pos.x;
	y2 = cursor_pos.y;

	/*Displaying any errors in input of final position*/
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
/*End of function accept_input*************************************************/

/*FUNCTION MSG_BOX**************************************************************
  * Function:
	 - Displays a message in a message box
	 - Terminates message on key-press
  * Parameters:
	 - char msg[]. The message to be displayed
  * Return value:
	 - None
*******************************************************************************/
void msg_box(char msg[], chessboard board[9][9])
{
	/*Local variables:
	  - i. counter variable for loop
	  - l. length of the message string*/
	int i, l;

	/*Draws a black rectangle with white border over the chessboard*/
	int rect[10]={x8*2, y8*2, x8*6, y8*2, x8*6, y8*6, x8*2, y8*6, x8*2, y8*2};
	setcolor(BLACK);
	setfillstyle(1, BLACK);
	fillpoly(4, rect);
	setcolor(WHITE);
	rectangle(x8*2+10, y8*2+10, x8*6-10, y8*6-10);

	/*Finds the length of the string to display the message centred*/
	for(l=0 ; msg[l]!='\0' ; l++);

	/*Displays the message and key-press prompt*/
	outtextxy(x8*4-l*4, y8*2+y8*4/3, msg);
	outtextxy(x8*4-96, y8*2+y8*8/3-6, "Press any key to continue");

	/*Accepts key-press and re-displays board*/
	getch();
	display(board);
}
/*End of function msg_box******************************************************/

/*FUNCTION PAWN_PROMOTION_MSG***************************************************
  * Function:
	 - Displays a message for pawn promotion
	 - Accepts choice of piece from the user
	 - Displays an error message if the user enters an invalid piece
  * Parameters:
	 - None
  * Return value:
	 - A character which gives the user's choice of piece
*******************************************************************************/
char pawn_promotion_msg(void)
{
	char choice;
	int flag=0;
	int rect[10]={x8*2, y8*2, x8*6, y8*2, x8*6, y8*6, x8*2, y8*6, x8*2, y8*2};

	/*Input label - errors in input regroup here*/
	input:

	/*Draws the message box at the centre of the screen*/
	setcolor(BLACK);
	setfillstyle(1, BLACK);
	fillpoly(4, rect);
	setcolor(WHITE);
	rectangle(x8*2+10, y8*2+10, x8*6-10, y8*6-10);

	if(flag) {
		/*Displays the error message*/
		outtextxy(x8*4-120, y8*4-6, "Invalid choice. Enter Q/K/R/B:");
	}
	else {
		/*Displays the initial menu*/
		outtextxy(x8*4-56, y8*2+y8*4/3, "Pawn promotion");
		outtextxy(x8*4-140, y8*2+y8*8/3-25,"Promote to Queen/Knight/Rook/Bishop");
		outtextxy(x8*4-36, y8*2+y8*8/3-7, "(Q/K/R/B)");
	}

	/*Accepts user input*/
	choice = getch();
	/*Checks for an invalid piece*/
	switch(choice) {
		case 'Q':
		case 'q':
		case 'K':
		case 'k':
		case 'R':
		case 'r':
		case 'B':
		case 'b':return choice;
		default :flag=1;
						//Display error and re-accept input
					goto input;
	}
}
/*End of function pawn_promotion_msg*******************************************/

/*FUNCTION EXIT_MSG*************************************************************
  * Function:
	 - Displays a message confirming exit
	 - Calls the exit function after confirmation
  * Parameters:
	 - None
  * Return value:
	 - None
*******************************************************************************/
void exit_msg(chessboard board[9][9])
{
	char choice;

	/*Draws the message box at the centre of the screen*/
	int rect[10]={x8*2, y8*2, x8*6, y8*2, x8*6, y8*6, x8*2, y8*6, x8*2, y8*2};
	setcolor(BLACK);
	setfillstyle(1, BLACK);
	fillpoly(4, rect);
	setcolor(WHITE);
	rectangle(x8*2+10, y8*2+10, x8*6-10, y8*6-10);

	/*Displaying the message*/
	outtextxy(x8*4-44, y8*4-6, "Exit? (Y/N)");
	/*Accepting user choice and exiting if 'yes'*/
	choice = getch();
	if(choice=='Y' || choice=='y') {
		exit(0);
	}
	/*Re-displaying the board if the user does not exit*/
	display(board);
}
/*End of function exit_msg*****************************************************/

/*FUNCTION DISPLAY**************************************************************
  * Function:
	 - Displays the chessboard by drawing the board and every piece
  * Parameters:
	 - None
  * Return value:
	 - None
*******************************************************************************/
void display(chessboard board[9][9])
{
	/*c is the centring factor to display the board on the centre of the screen*/
	int c;
	c=(x8-y8)*4;

	/*i,j - counter variables*/
	int i, j;
	/*Flag to switch color of the square*/
	int flag=0;

	/*Drawing the board by drawing squares of alternating colours*/
	for(i=0 ; i<8 ; i++) {
		for(j=0 ; j<8 ; j++) {
			int rect[10]={j*y8+c, i*y8+1, (j+1)*y8+c, i*y8+1, (j+1)*y8+c,
										  (i+1)*y8+1, j*y8+c, (i+1)*y8+1, j*y8+c, i*y8+1};
			if(flag==0) {
				setcolor(WHITE);
				setfillstyle(1, LIGHTGRAY);
				fillpoly(4, rect);
				flag=1;
			}
			else {
				setcolor(WHITE);
				setfillstyle(1, DARKGRAY);
				fillpoly(4, rect);
				flag=0;
			}
		}
		if(flag==1) flag=0;
		else flag=1;
	}

	/*Drawing the pieces on the board*/
	for(j=1 ; j<=8 ; j++) {
		for(i=1 ; i<=8 ; i++) {
			draw_piece(board[i][j].num, i, j);
		}
	}
}
/*End of function display******************************************************/

/*FUNCTION DRAW_PIECE***********************************************************
  * Function:
	 - Calls the required function to draw the piece of the given number
  * Parameters:
	 - int num. The number of the piece to draw
	 - int x. The x-coordiante of the piece to draw
	 - int y. The y-coordinate of the piece to draw
  * Return value:
	 - None
*******************************************************************************/
void draw_piece(int num, int x, int y)
{
	/*Finding the player represented by the piece from its number*/
	int player;
	if(num<=8)
		player=WHT;
	else if(num<=16)
		player=BLK;
	else if(num<=24)
		player=WHT;
	else
		player=BLK;

	/*c is the centring factor to draw the piece adjusted to the centre*/
	int c=(x8-y8)*4;

	/*Deciding which piece to draw from the number*/
	if(num>=17) {
		draw_pawn((x-1)*y8+c, (8-y)*y8+1, player);
	}
	switch(num) {
		case 1 :
		case 8 :
		case 9 :
		case 16: draw_rook((x-1)*y8+c, (8-y)*y8+1, player);
					break;
		case 2 :
		case 7 :
		case 10:
		case 15: draw_knight((x-1)*y8+c, (8-y)*y8+1, player);
					break;
		case 3 :
		case 6 :
		case 11:
		case 14:	draw_bishop((x-1)*y8+c, (8-y)*y8+1, player);
					break;
		case 4 :
		case 12: draw_queen((x-1)*y8+c, (8-y)*y8+1, player);
					break;
		case 5 :
		case 13: draw_king((x-1)*y8+c, (8-y)*y8+1, player);
					break;
	}
}
/*End of function draw_piece***************************************************/

/*FUNCTION DRAW_PAWN************************************************************
  * Function:
	 - Draws a pawn of the given colour at the given coordinates
  * Parameters:
	 - int x. The x-coordinate of the pawn
	 - int y. The y-coordinate of the pawn
	 - int color. The colour of the pawn - black or white
  * Return value:
	 - None
*******************************************************************************/
void draw_pawn(int x, int y, int color)
{
	if(color==WHT) setcolor(WHITE);
	else setcolor(BLACK);

	/*Postioning the pawn  at the centre of the square*/
	x+=2*y128;
	y+=2*y128;
	int y16=y128;

	/*Drawing the rectangle at the base of the pawn*/
	line(x+y16*4, y+y16*13, x+y16*3, y+y16*13);
	line(x+y16*3, y+y16*13, x+y16*3, y+y16*14);
	line(x+y16*3, y+y16*14, x+y16*13, y+y16*14);
	line(x+y16*13, y+y16*14, x+y16*13, y+y16*13);
	line(x+y16*13, y+y16*13, x+y16*12, y+y16*13);

	/*Drawing the arcs of the pawn*/
	arc(x+y16*8, y+y16*12, 350, 79, (int)(4.12*y16));
	arc(x+y16*8, y+y16*6, 294, 66, (int)(2.24*y16)+1);
	arc(x+y16*8, y+y16*3, 311, 228, (int)(1.14*y16));
	arc(x+y16*8, y+y16*6, 114, 246, (int)(2.24*y16)+1);
	arc(x+y16*8, y+y16*12, 101, 197, (int)(4.12*y16));

	/*Filling the pawn with the required colour*/
	if(color==1) {
		setfillstyle(1, WHITE);
		floodfill(x+y128*8, y+y128*12, WHITE);
	}
	else {
		setfillstyle(1, BLACK);
		floodfill(x+y128*8, y+y128*12, BLACK);
	}
}
/*End of function draw_pawn****************************************************/

/*FUNCTION DRAW_QUEEN************************************************************
  * Function:
	 - Draws a queen of the given colour at the given coordinates
  * Parameters:
	 - int x. The x-coordinate of the queen
	 - int y. The y-coordinate of the queen
	 - int color. The colour of the queen - black or white
  * Return value:
	 - None
*******************************************************************************/
void draw_queen(int x, int y, int color)
{
	/*Sets the outline colour as reqired*/
	if(color==1) setcolor(WHITE);
	else setcolor(BLACK);

	/*Setting the queen at the square's centre*/
	x+=2*y128;
	y+=2*y128;

	/*Array containing the points which make up the queen's outline*/
	int queen[36]={3, 14,
						4, 13,
						3, 12,
						4, 12,
						1, 6,
						5, 10,
						4, 3,
						7, 9,
						8, 1,
						9, 9,
						12, 3,
						11, 10,
						15, 6,
						12, 12,
						13, 12,
						12, 13,
						13, 14,
						3, 14  };

	/*Multiplying the points by the required factor*/
	for(int i=0 ; i<36 ; i+=2) {
		queen[i]=x+queen[i]*y128;
		queen[i+1]=y+queen[i+1]*y128;
	}

	/*Setting the fill colour as required*/
	if(color==1) {
		setfillstyle(1, WHITE);
	}
	else {
		setfillstyle(1, BLACK);
	}

	/*Drawing the queen*/
	fillpoly(18, queen);
}
/*End of function draw-queen***************************************************/

/*FUNCTION DRAW_KING************************************************************
  * Function:
	 - Draws a king of the given colour at the given coordinates
  * Parameters:
	 - int x. The x-coordinate of the king
	 - int y. The y-coordinate of the king
	 - int color. The colour of the king - black or white
  * Return value:
	 - None
*******************************************************************************/
void draw_king(int x, int y, int color)
{
	/*Sets the outline colour as required*/
	if(color==1) setcolor(WHITE);
	else setcolor(BLACK);

	/*Setting the king at the centre of the square*/
	x+=2*y128;
	y+=2*y128;

	/*Array of points which make up the king's outline*/
	int king[64]={8, 14,
					  9, 6,
					  11, 8,
					  9, 12,
					  12, 8,
					  11, 5,
					  14, 7,
					  12, 10,
					  15, 7,
					  12, 4,
					  13, 4,
					  12, 3,
					  13, 2,
					  3, 2,
					  4, 3,
					  3, 4,
					  4, 4,
					  1, 7,
					  4, 10,
					  2, 7,
					  5, 5,
					  4, 8,
					  7, 12,
					  5, 8,
					  7, 6,
					  8, 14,
					  8, 5,
					  6, 4,
					  8, 3,
					  10, 4,
					  8, 5,
					  8, 14 };

	/*Multiplying the points by the reuired factor*/
	for(int i=0 ; i<64 ; i+=2) {
		king[i]=x+king[i]*y128;
		king[i+1]=y+(16-king[i+1])*y128;
	}

	/*Setting the fill colour as required*/
	if(color==1) {
		setfillstyle(1, WHITE);
	}
	else {
		setfillstyle(1, BLACK);
	}

	/*Drawing the king*/
	fillpoly(32, king);

	/*Colouring the gem on the king's crown*/
	if(color==1) {
		setfillstyle(1, BLUE);
		floodfill(x+y128*8, y+y128*12, WHITE);
	}
	else {
		setfillstyle(1, RED);
		floodfill(x+y128*8, y+y128*12, BLACK);
	}
}
/*End of function draw_king****************************************************/

/*FUNCTION DRAW_BISHOP**********************************************************
  * Function:
	 - Draws a bishop of the given colour at the given coordinates
  * Parameters:
	 - int x. The x-coordinate of the bishop
	 - int y. The y-coordinate of the bishop
	 - int color. The colour of the pawn - black or white
  * Return value:
	 - None
*******************************************************************************/
void draw_bishop(int x, int y, int color)
{
	/*Setting the outline colour as required*/
	if(color==1) setcolor(WHITE);
	else setcolor(BLACK);

	/*Setting the bishop at the centre of the square*/
	x+=2*y128;
	y+=2*y128;

	/*Array containing the points which make up the bishop's outline*/
	int bishop[36]={3, 2,
						3, 3,
						5, 3,
						2, 5,
						6, 4,
						4, 7,
						5, 11,
						8, 7,
						6, 12,
						8, 15,
						10, 12,
						12, 7,
						10, 4,
						14, 5,
						11, 3,
						13, 3,
						13, 2,
						3, 2  };

	/*Multiplying the points by the required factor*/
	for(int i=0 ; i<36 ; i+=2) {
		bishop[i]=x+bishop[i]*y128;
		bishop[i+1]=y+(16-bishop[i+1])*y128;
	}
	bishop[12]+=y128/2;

	/*Setting the fill colour as required*/
	if(color==1) {
		setfillstyle(1, WHITE);
	}
	else {
		setfillstyle(1, BLACK);
	}

	/*Drawing the bishop*/
	fillpoly(18, bishop);
}
/*End of function draw_bishop**************************************************/

/*FUNCTION DRAW_NIGHT************************************************************
  * Function:
	 - Draws a knight of the given colour at the given coordinates
  * Parameters:
	 - int x. The x-coordinate of the knight
	 - int y. The y-coordinate of the knight
	 - int color. The colour of the knight - black or white
  * Return value:
	 - None
*******************************************************************************/
void draw_knight(int x, int y, int color)
{
	/*Setting the outline colour as required*/
	if(color==1) setcolor(WHITE);
	else setcolor(BLACK);

	/*Setting the knight at the centre of the square*/
	x+=2*y128;
	y+=2*y128;

	/*Array containing the points which make up the knight's outline*/
	int knight[44]={2, 8,
						 5, 11,
						 5, 10,
						 4, 9,
						 5, 9,
						 5, 10,
						 5, 11,
						 2, 15,
						 6, 12,
						 9, 13,
						 12, 12,
						 13, 10,
						 11, 6,
						 13, 3,
						 14, 3,
						 14, 2,
						 2, 2,
						 2, 3,
						 4, 3,
						 8, 9,
						 3, 7,
						 2, 8  };

	/*Multiplying the points by the required factor*/
	knight[10]+=y128/2;
	for(int i=0 ; i<44 ; i+=2) {
		knight[i]=x+knight[i]*y128;
		knight[i+1]=y+(16-knight[i+1])*y128;
	}

	/*Setting the fill colour as required*/
	if(color==1) {
		setfillstyle(1, WHITE);
	}
	else {
		setfillstyle(1, BLACK);
	}

	/*Drawing the knight*/
	fillpoly(22, knight);
}
/*End of function draw_knight**************************************************/

/*FUNCTION DRAW_ROOK************************************************************
  * Function:
	 - Draws a rook of the given colour at the given coordinates
  * Parameters:
	 - int x. The x-coordinate of the rook
	 - int y. The y-coordinate of the rook
	 - int color. The colour of the rook - black or white
  * Return value:
	 - None
*******************************************************************************/
void draw_rook(int x, int y, int color)
{
	/*Setting the outline colour as required*/
	if(color==1) setcolor(WHITE);
	else setcolor(BLACK);

	/*Setting the rook at the centre of the square*/
	x+=2*y128;
	y+=2*y128;

	/*Array containing the points which make up the outline of the rook*/
	int rook[66]={3, 2,
					  3, 3,
					  4, 3,
					  5, 4,
					  5, 10,
					  4, 11,
					  3, 11,
					  3, 14,
					  4, 14,
					  4, 12,
					  5, 12,
					  5, 14,
					  6, 14,
					  6, 12,
					  7, 12,
					  7, 14,
					  9, 14,
					  9, 12,
					  10, 12,
					  10, 14,
					  11, 14,
					  11, 12,
					  12, 12,
					  12, 14,
					  13, 14,
					  13, 11,
					  12, 11,
					  11, 10,
					  11, 4,
					  12, 3,
					  13, 3,
					  13, 2,
					  3, 2  };

	/*Multiplying the points by the required factor*/
	for(int i=0 ; i<66 ; i+=2) {
		rook[i]=x+rook[i]*y128;
		rook[i+1]=y+(16-rook[i+1])*y128;
	}

	/*Setting the fill colour as required*/
	if(color==1) {
		setfillstyle(1, WHITE);
	}
	else {
		setfillstyle(1, BLACK);
	}

	/*Drawing the rook*/
	fillpoly(33, rook);

	/*Drawing the rectangle in the centre of the rook*/
	if(color==1) {
		setcolor(BLACK);
	}
	else {
		setcolor(WHITE);
	}
	rectangle(x+y128*13/2, y+y128*6, x+y128*19/2, y+y128*12);
}
/*End of function draw_rook****************************************************/

/*FUNCTION DRAW_CURSOR**********************************************************
  * Function:
	 - Draws the cursor of the given colour at the given coordinates
  * Parameters:
	 - int x. The x-coordinate of the cursor
	 - int y. The y-coordinate of the cursor
	 - int color. The colour of the cursor - black or white
  * Return value:
	 - None
*******************************************************************************/
void draw_cursor(int x, int y, int color)
{
	/*Adjusting the x- and y-coordinates of the cursor*/
	y = 9-y;
	x--; y--;

	/*c is the centring factor to draw the cursor adjusted to the centre*/
	int c=(x8-y8)*4;

	/*Setting the colour of the cursor as required*/
	if(color==1) {
		setcolor(BLUE);
		setfillstyle(1, BLUE);
	}
	else {
		setcolor(RED);
		setfillstyle(1, RED);
	}

	/*Drawing the four bars that make up the cursor*/
	bar3d(x*y8+c, y*y8+1, x*y8+2+c, (y+1)*y8+1, 0, 0);
	bar3d(x*y8+c, y*y8+1, (x+1)*y8+c, y*y8+3, 0, 0);
	bar3d(x*y8+c, (y+1)*y8-1, (x+1)*y8+c, (y+1)*y8+1, 0, 0);
	bar3d((x+1)*y8-2+c, y*y8+1, (x+1)*y8+c, (y+1)*y8+1, 0, 0);
}
/*End of function draw_cursor**************************************************/




















/*
void update_board(chessboard board[9][9])
{
   /* Need to update:
      struct chessboard {
         int num;
      	int player;
      	int enpassant_able, ep_move_no;
      	position pawn_pos;
      	int wht_king_castle, blk_king_castle;
			int wht_queen_castle, blk_queen_castle;

         int num_attackers;
         int num_defenders;
         struct {
            int num;
            position pos;
         } attacked, defended;
         int functional_value;         */

 /*  int i, j, k, l;
   for(i=1 ; i<=8 ; i++) {
      for(j=1 ; j<=8 ; j++) {
         if(board[i][j].player) {

            num_attackers = 0;
            num_defenders = 0;
            for(k=1 ; k<=8 ; k++) {
               for(l=1 ; l<=8 ; l++) {

                  if(board[k][l].player) {
                     if(control(k, l, i, j, board)){
                        if(opp(board[k][l].player, board[i][j].player))
                           board[i][j].num_attackers++;
                        else
                           board[i][j].num_defenders++;
                     }

                     if(control(i, j, k, l, board)) {

                  }



}  */
