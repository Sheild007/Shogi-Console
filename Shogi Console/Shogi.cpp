#include<iostream>
#include<conio.h>
#include<windows.h>
#include<fstream>
#include<math.h>
#include<string>

using namespace std;


#define LightGrey 7
#define Grey 8
#define WHITE 15
#define BLACK 0
#define Red 12
#define Blue 9

enum Color { White, Black };
struct Position
{
	int rpos;
	int cpos;

};

//utelities
void getRowColbyLeftClick(int& rpos, int& cpos)
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD Events;
	INPUT_RECORD InputRecord;
	SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
	do
	{
		ReadConsoleInput(hInput, &InputRecord, 1, &Events);
		if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
			rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
			break;
		}
	} while (true);
}
void gotoRowCol(int rpos, int cpos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = cpos;
	scrn.Y = rpos;
	SetConsoleCursorPosition(hOuput, scrn);
}
void SetClr(int tcl, int bcl)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (tcl + (bcl * 16)));
}
void L2R(int r, int cs, int ce, char sym, int clr)
{
	for (int c = cs; c <= ce; c++)
	{
		SetClr(clr, BLACK);
		gotoRowCol(r, c);
		cout << sym;

	}
}
void T2B(int c, int rs, int re, char sym, int clr)
{
	for (int r = rs; r <= re; r++)
	{
		SetClr(clr, BLACK);
		gotoRowCol(r, c);
		cout << sym;

	}
}


//Board
void drawBox(int r, int c, int brows, int bcols, int clr)
{
	for (int i = 0; i < brows; i++)
	{
		for (int j = 0; j < bcols; j++)
		{
			SetClr(clr, BLACK);

			gotoRowCol(r + i, j + c);

			cout << char(-37);



		}


	}
}
void DrawBoard(int dim, int rows, int cols)
{
	for (int i = 0; i < dim; i++)
	{
		for (int j = 0; j < dim; j++)
		{

			if ((i + j) % 2 == 0)
			{
				drawBox(i * rows, j * cols, rows, cols, LightGrey);
			}
			else
			{
				drawBox(i * rows, j * cols, rows, cols, Grey);
			}


		}

	}


}
void DrawCapBoard(int dim, int rows, int cols, string Names[])
{
	for (int i = 1; i <= 4; i += 2)
	{
		for (int j = 0; j < 7; j++)
		{

			if ((i + j) % 2 == 0)
			{
				drawBox(i * rows, j * cols + 10 * cols, rows, cols, LightGrey);
			}
			else
			{
				drawBox(i * rows, j * cols + 10 * cols, rows, cols, Grey);
			}


		}

	}

	gotoRowCol(rows - 2, 12 * cols);
	cout << Names[0] << "'s Captured Pieces";
	gotoRowCol(3 * rows - 2, 12 * cols);
	cout << Names[1] << "'s Captured Pieces";




}
void PrintSpecificCapBoard(int dim, int rows, int cols, int** Freq)
{
	for (int i = 1, f = 0; i <= 4; i += 2, f++)
	{
		for (int j = 0; j < 7; j++)
		{

			if (Freq[f][j] == 0)
			{
				if ((i + j) % 2 == 0)
				{
					drawBox(i * rows, j * cols + 10 * cols, rows, cols, LightGrey);
				}
				else
				{
					drawBox(i * rows, j * cols + 10 * cols, rows, cols, Grey);
				}

			}
		}

	}
}
bool validCapRange(Position source)
{

	if ((source.rpos == 1 || source.rpos == 3) && (source.cpos >= 10 && source.cpos <= 16))
		return true;
	return false;
}
void RemovePieces(int boxdim, Position Sc, Position Dc, int** Freq)
{

	//Remove Source
	if (validCapRange(Sc))
	{
		int f;
		if (Sc.rpos == 1)
		{
			f = Freq[0][Sc.cpos - 10];
		}
		if (Sc.rpos == 3)
		{
			f = Freq[1][Sc.cpos - 10];
		}
		if (f == 0)
		{
			if ((Sc.rpos + Sc.cpos) % 2 == 0)
			{
				drawBox(Sc.rpos * boxdim, Sc.cpos * boxdim, boxdim, boxdim, LightGrey);
			}
			else
			{
				drawBox(Sc.rpos * boxdim, Sc.cpos * boxdim, boxdim, boxdim, Grey);
			}
		}
	}
	else
	{
		if ((Sc.rpos + Sc.cpos) % 2 == 0)
		{
			drawBox(Sc.rpos * boxdim, Sc.cpos * boxdim, boxdim, boxdim, LightGrey);
		}
		else
		{
			drawBox(Sc.rpos * boxdim, Sc.cpos * boxdim, boxdim, boxdim, Grey);
		}
	}
	//Remove Destination

	if ((Dc.rpos + Dc.cpos) % 2 == 0)
	{
		drawBox(Dc.rpos * boxdim, Dc.cpos * boxdim, boxdim, boxdim, LightGrey);
	}
	else
	{
		drawBox(Dc.rpos * boxdim, Dc.cpos * boxdim, boxdim, boxdim, Grey);
	}

}

//Main Flow
void init(char**& Board, int& dim, int turn, string Names[], char**& TB, char**& TB2, char**& Captured, int cap_index[], char**& CapturedD, int**& CapCount, char**& TCaptured, int boxDim)
{
	ifstream rdr("New.txt");
	if (!rdr)
		cout << "Loading fail....", exit(1);
	rdr >> dim;


	Board = new char* [dim];
	TB = new char* [dim];
	TB2 = new char* [dim];
	Captured = new char* [2];
	TCaptured = new char* [2];
	CapturedD = new char* [2];
	CapCount = new int* [2];
	for (int i = 0; i < dim; i++)
	{
		Board[i] = new char[dim] {};
		TB[i] = new char[dim] {};
		TB2[i] = new char[dim] {};

	}
	for (int i = 0; i < 2; i++)
	{
		Captured[i] = new char[20] {};
		TCaptured[i] = new char[20] {};
		CapturedD[i] = new char[7] {};
		CapCount[i] = new int[7] {};
		for (int j = 0; j < 20; j++)
		{
			Captured[i][j] = '-';
			TCaptured[i][j] = '-';
		}
		for (int j = 0; j < 7; j++)
		{
			CapturedD[i][j] = '-';
			CapCount[i][j] = 0;
		}


	}
	for (int i = 0; i < dim; i++)
	{
		for (int j = 0; j < dim; j++)
			rdr >> Board[i][j];

	}
	for (int i = 0; i < dim; i++)
	{
		for (int j = 0; j < dim; j++)
		{
			TB[i][j] = Board[i][j];
			TB2[i][j] = Board[i][j];
		}

	}


	gotoRowCol(5 * boxDim + 6, 10 * boxDim);
	cout << "Enter Players Names:";


	for (int i = 0; i < 2; i++)
	{
		gotoRowCol(5 * boxDim + 6 + i + 1, 10 * boxDim);
		cout << "Player " << i + 1 << " Name:";
		cin >> Names[i];
		cap_index[i] = 0;
	}
	turn = White;

}
void initLoad(const char* fName, char**& Board, int& dim, int& turn, string Names[], char**& TB, char**& TB2, char**& Captured, int cap_index[], char**& CapturedD, int**& CapCount, char**& TCaptured)
{
	ifstream rdr(fName);
	if (!rdr)
		cout << "Loading fail....", exit(1);
	rdr >> dim;


	Board = new char* [dim];
	TB = new char* [dim];
	TB2 = new char* [dim];
	Captured = new char* [2];
	TCaptured = new char* [2];
	CapturedD = new char* [2];
	CapCount = new int* [2];
	for (int i = 0; i < dim; i++)
	{
		Board[i] = new char[dim] {};
		TB[i] = new char[dim] {};
		TB2[i] = new char[dim] {};

	}
	for (int i = 0; i < 2; i++)
	{
		Captured[i] = new char[20] {};
		TCaptured[i] = new char[20] {};
		CapturedD[i] = new char[7] {};
		CapCount[i] = new int[7] {};
		for (int j = 0; j < 20; j++)
		{
			Captured[i][j] = '-';
			TCaptured[i][j] = '-';
		}
		for (int j = 0; j < 7; j++)
		{
			CapturedD[i][j] = '-';
			CapCount[i][j] = 0;
		}


	}


	for (int i = 0; i < dim; i++)
	{
		for (int j = 0; j < dim; j++)
			rdr >> Board[i][j];

	}
	for (int i = 0; i < dim; i++)
	{
		for (int j = 0; j < dim; j++)
		{
			TB[i][j] = Board[i][j];
			TB2[i][j] = Board[i][j];
		}

	}


	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			rdr >> Captured[i][j];
			TCaptured[i][j] = Captured[i][j];
		}
	}

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			rdr >> CapturedD[i][j];

		}
	}
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			rdr >> CapCount[i][j];

		}
	}

	rdr >> turn;
	for (int i = 0; i < 2; i++)
	{

		rdr >> Names[i];

	}

}

void TurnMessage(string Names[], int turn, int boxdim)
{
	gotoRowCol(5 * boxdim, 10 * boxdim);
	cout << "                              ";
	gotoRowCol(5 * boxdim, 10 * boxdim);
	cout << Names[turn] << "'s turn" << endl;

}
void selectPosition(Position& P, int dim, int divide)
{
	int y, x;
	//do 
	{
		getRowColbyLeftClick(y, x);
		P.rpos = y / divide;
		P.cpos = x / divide;
	}// while (y > 44 && x > 44);

	/*cout << "Select Your Move"<<flush << endl;
	cout << "Select(a1:h8):"<<flush;
	char c;
	int d;
	cin >> c >> d;
	c = toupper(c);
	P.rpos = (d - 1);
	P.cpos = (c - 'A') ;*/

}
void selectPositionRemove()
{
	cout << "                 " << endl;
	cout << "                 ";


}

Position FindaPieceAtlastIndex(char** array, int r, int c, char sym)
{
	Position R;
	for (int i = 0; i < r; i++)
	{
		for (int j = 0; j < c; j++)
		{
			char sym2 = array[i][j];
			if (array[i][j] == sym)
			{
				R.rpos = i;
				R.cpos = j;

			}
		}
	}
	return R;
	R.rpos = -1;
	R.cpos = -1;
	return R;
}
void PrintBoard(char** Board, int dim)
{
	for (int i = 0; i < dim; i++)
	{
		for (int j = 0; j < dim; j++)
		{
			if (i != dim - 1)
				cout << Board[i][j] << " ";
			else
			{
				if (j == 0)
					cout << "  ";
				cout << Board[i][j] << " ";
			}

		}
		cout << endl;
	}
}
void PrintBoard2(char** Board, int dim, int Bdim)
{
	int fixR = 0;
	int fixC = 0;
	for (int i = 0; i < dim; i++)
	{
		for (int j = 0; j < dim; j++)
		{
			gotoRowCol((i + Bdim / 2) + fixR, (j + Bdim / 2) + fixC);
			if (Board[i][j] != '-')
			{
				if (j == dim - 1 || i == dim - 1)
					cout << Board[i][j];

			}
			fixC += (Bdim - 1);

		}
		fixC = 0;
		fixR += (Bdim - 1);

	}
}
void PrintBoardRemove(char** Board, int dim, int Bdim)
{
	int fixR = 0;
	int fixC = 0;
	for (int i = 0; i < dim; i++)
	{
		for (int j = 0; j < dim; j++)
		{

			gotoRowCol((i + Bdim / 2) + fixR, (j + Bdim / 2) + fixC);
			cout << " ";
			fixC += (Bdim - 1);

		}
		fixC = 0;
		fixR += (Bdim - 1);

	}
}
bool isWhitePiece(char sym)
{
	return sym >= 'a' && sym <= 'z';
}
bool isBlackPiece(char sym)
{
	return sym >= 'A' && sym <= 'Z';
}
bool isMyPiece(char sym, int turn)
{
	if (turn == White)
		return isWhitePiece(sym);
	return isBlackPiece(sym);

}
bool validRange(int dim, Position source)
{

	if (source.rpos<0 || source.rpos>dim - 1 || source.cpos<0 || source.cpos>dim - 1)
		return false;
	return true;
}
void ShiftData(char** array, int r, int cs, int ce)
{
	for (int i = cs; i < ce - 1; i++)
	{
		swap(array[r][i], array[r][i + 1]);
	}
}
bool isUndoPosition(Position P)
{
	return(P.rpos == 8 && P.cpos == 17);
}
bool isValidSource(char** Board, int dim, int turn, Position source, char** CapD, Position UndoArray[])
{
	if (isUndoPosition(source) && UndoArray[0].rpos != -1)
	{
		return true;
	}
	char sym = 'a';
	if (validCapRange(source))
	{
		if (source.rpos == 1)
		{
			sym = CapD[0][source.cpos - 10];
		}
		if (source.rpos == 3)
		{
			sym = CapD[1][source.cpos - 10];
		}
		if (sym == '-')
			return false;

		return isMyPiece(sym, turn);

	}
	else
	{
		if (Board[source.rpos][source.cpos] == '-')
			return false;

		if (validRange(dim, source) == 0)
			return false;

		return isMyPiece(Board[source.rpos][source.cpos], turn);
	}

}
bool isValidDestination(char** Board, int dim, int turn, Position source)
{
	if (validRange(dim, source) == 0)
		return false;
	return !(isMyPiece(Board[source.rpos][source.cpos], turn));

}

void updadteBoard(char** Board, Position source, Position destination, char** Captured, char** CapD, int** Freq)
{
	char sym = '-';
	int f = 0;
	Position Temp;
	if (validCapRange(source))
	{
		if (source.rpos == 1)
		{
			sym = CapD[0][source.cpos - 10];
			f = Freq[0][source.cpos - 10];


		}
		if (source.rpos == 3)
		{
			sym = CapD[1][source.cpos - 10];
			f = Freq[1][source.cpos - 10];


		}

		Board[destination.rpos][destination.cpos] = sym;

		Temp = FindaPieceAtlastIndex(Captured, 2, 20, sym);
		Captured[Temp.rpos][Temp.cpos] = '-';
		if (f > 1)
		{
			ShiftData(Captured, Temp.rpos, Temp.cpos, 20);

		}


	}
	else
	{
		char temp = Board[source.rpos][source.cpos];
		Board[destination.rpos][destination.cpos] = temp;
		Board[source.rpos][source.cpos] = '-';
	}
}
void ReplaceBoard(char** Board, Position pre, Position destination, char sym)
{
	//char temp = Board2[so;
	Board[destination.rpos][destination.cpos] = sym;
	Board[pre.rpos][pre.cpos] = '-';

}
void TurnChnage(int& Turn)
{

	Turn = (Turn + 1) % 2;
}

//Legatities
//path checking
bool isHorizontal(Position Sc, Position Dc)
{
	return(Sc.rpos == Dc.rpos);

}
bool isVertical(Position Sc, Position Dc)
{
	return(Sc.cpos == Dc.cpos);
}
bool isDignol(Position Sc, Position Dc)
{
	return (abs(Sc.rpos - Dc.rpos) == abs(Sc.cpos - Dc.cpos));
}
bool isDignolMain(Position Sc, Position Dc)
{
	return ((Sc.rpos - Dc.rpos) == (Sc.cpos - Dc.cpos));
}
bool isDignolSec(Position Sc, Position Dc)
{
	return (abs(Sc.rpos - Dc.rpos) == abs(Sc.cpos - Dc.cpos) && (Sc.rpos - Dc.rpos) != (Sc.cpos - Dc.cpos));
}
// clear Path
bool isHorizonPathClear(char** B, Position Sc, Position Dc)
{
	int s, e;
	if (Dc.cpos > Sc.cpos)
	{
		s = Sc.cpos + 1;
		e = Dc.cpos;
	}
	else
	{
		s = Dc.cpos + 1;
		e = Sc.cpos;
	}

	for (int i = s; i < e; i++)
	{
		if (B[Sc.rpos][i] != '-')
			return false;

	}
	return true;

}
bool isVerticalPathClear(char** B, Position Sc, Position Dc)
{
	int s, e;
	if (Dc.rpos > Sc.rpos)
	{
		s = Sc.rpos + 1;
		e = Dc.rpos;
	}
	else
	{
		s = Dc.rpos + 1;
		e = Sc.rpos;
	}

	for (int i = s; i < e; i++)
	{
		if (B[i][Sc.cpos] != '-')
			return false;

	}
	return true;


}
bool isDiagonalPathClearL2R(char** B, Position Sc, Position Dc)
{
	int si, sj, ei, ej;
	if (Dc.rpos - Sc.rpos > 0 && Dc.cpos - Sc.cpos > 0)
	{
		si = Sc.rpos + 1;
		sj = Sc.cpos + 1;
		ei = Dc.rpos;
		ej = Dc.cpos;

	}
	else
	{

		si = Dc.rpos + 1;
		sj = Dc.cpos + 1;
		ei = Sc.rpos;
		ej = Sc.cpos;
	}

	for (int i = si, j = sj; i < ei, j < ej; i++, j++)
	{
		if (B[i][j] != '-')
			return false;
	}
	return true;

}
bool isDiagonalPathClearR2L(char** B, Position Sc, Position Dc)
{
	int si, sj, ei, ej;
	if (Dc.rpos - Sc.rpos > 0 && Dc.cpos - Sc.cpos < 0)
	{
		si = Sc.rpos + 1;
		sj = Sc.cpos - 1;
		ei = Dc.rpos - 1;
		ej = Dc.cpos + 1;
	}
	else
	{
		si = Dc.rpos + 1;
		sj = Dc.cpos - 1;
		ei = Sc.rpos - 1;
		ej = Sc.cpos + 1;

	}
	for (int i = si, j = sj; i <= ei && j >= ej; i++, j--)
	{
		if (B[i][j] != '-')
			return false;

	}

	return true;

}
//legal Moves
bool isKingLegal(char** B, Position S, Position D)
{
	if (validCapRange(S))
	{
		return B[D.rpos][D.cpos] == '-';
	}
	else
	{
		int temp = abs(D.rpos - S.rpos);
		int temp1 = abs(D.cpos - S.cpos);
		if (temp == 1 && isVertical(S, D))
		{
			return true;
		}
		else if (temp1 == 1 && isHorizontal(S, D))
		{
			return true;
		}
		else if ((abs(S.rpos - D.rpos) == 1) && (abs(S.cpos - D.cpos) == 1))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}
bool isLanceLegal(char** B, char sym, Position S, Position D)
{


	if (sym == toupper(sym))
	{
		if (validCapRange(S))
		{
			return B[D.rpos][D.cpos] == '-' && D.rpos != 8;
		}
		if (D.rpos > S.rpos)
		{
			return (isVertical(S, D) && isVerticalPathClear(B, S, D));
		}
	}
	else
	{
		if (validCapRange(S))
		{
			return B[D.rpos][D.cpos] == '-' && D.rpos != 0;
		}
		if (D.rpos < S.rpos)
		{
			return (isVertical(S, D) && isVerticalPathClear(B, S, D));
		}
	}

}
bool isRookLegal(char** B, Position S, Position D)
{
	if (validCapRange(S))
	{
		return B[D.rpos][D.cpos] == '-';
	}
	else
		return (isVertical(S, D) && isVerticalPathClear(B, S, D)) || (isHorizontal(S, D) && isHorizonPathClear(B, S, D));
}
bool isDragonLegal(char** B, Position S, Position D)
{
	if (validCapRange(S))
	{
		return B[D.rpos][D.cpos] == '-';
	}
	else
	{
		return (isRookLegal(B, S, D) || isKingLegal(B, S, D));
	}
}
bool isBishopLegal(char** B, Position S, Position D)
{
	if (validCapRange(S))
	{
		return B[D.rpos][D.cpos] == '-';
	}
	else
	{
		return (isDignolMain(S, D) && isDiagonalPathClearL2R(B, S, D)) || (isDignolSec(S, D) && isDiagonalPathClearR2L(B, S, D));
	}

}
bool isHorseLegal(char** B, Position S, Position D)
{

	return(isBishopLegal(B, S, D) || isKingLegal(B, S, D));

}
bool isKnightLegal(char** B, Position S, Position D, char sym)
{
	if (validCapRange(S))
	{
		if (sym == toupper(sym))
			return B[D.rpos][D.cpos] == '-' && D.rpos != 8 && D.rpos != 7;
		return B[D.rpos][D.cpos] == '-' && D.rpos != 0 && D.rpos != 1;
	}
	else
	{
		int check = abs(D.rpos - S.rpos);//DownandUp
		//int check1 = abs(D.cpos - S.cpos);//RightandLeft
		if (check == 2)
		{
			if (abs(D.cpos - S.cpos) == 1)
				return true;
		}
		/*	if (check1 == 2)
			{
				if (abs(D.rpos - S.rpos) == 1)
					return true;
			}*/
		return false;
	}
}
//bool isKnightLegal(char** B, Position S, Position D, char sym)
//{
//	
//	
//
//		int check = (D.rpos - S.rpos);
//		if (sym == toupper(sym))
//		{
//			if (validCapRange(S))
//			{
//				return B[D.rpos][D.cpos] == '-' && D.rpos!=8 && D.rpos!=7;
//			}
//			return check == 2 && (abs(D.cpos - S.cpos) == 1);
//		}
//		else 
//		{
//			if (validCapRange(S))
//			{
//				return B[D.rpos][D.cpos] == '-' && D.rpos != 0 && D.rpos != 1;
//			}
//			return check == -2 && (abs(D.cpos - S.cpos) == 1);
//		}
//		return false;
//	
//}
bool isSilverGeneralLegal(char** B, char sym, Position S, Position D)
{
	if (validCapRange(S))
	{
		return B[D.rpos][D.cpos] == '-';
	}
	else
	{
		if (sym == toupper(sym))
		{
			if (D.cpos - S.cpos > 0 && isVertical(S, D))
			{
				return true;
			}
			if ((abs(S.rpos - D.rpos) == 1) && (abs(S.cpos - D.cpos) == 1))
			{
				return true;
			}
		}
		else
		{
			if (D.cpos - S.cpos < 0 && isVertical(S, D))
			{
				return true;
			}
			if ((abs(S.rpos - D.rpos) == 1) && (abs(S.cpos - D.cpos) == 1))
			{
				return true;
			}
		}
		return false;
	}
}
bool isGoldenGeneralLegal(char** B, char sym, Position S, Position D)
{
	if (validCapRange(S))
	{
		return B[D.rpos][D.cpos] == '-';
	}
	else
	{
		int temp = abs(D.rpos - S.rpos);
		int temp1 = abs(D.cpos - S.cpos);
		if (temp == 1 && isVertical(S, D))
		{
			return true;
		}
		else if (temp1 == 1 && isHorizontal(S, D))
		{
			return true;
		}
		else {
			if (sym == toupper(sym))
			{
				if (((S.rpos - D.rpos == -1) && (S.cpos - D.cpos == 1)) || ((S.rpos - D.rpos == -1) && (S.cpos - D.cpos == -1)))
				{
					return true;
				}
			}
			else
			{
				if (((S.rpos - D.rpos == 1) && (S.cpos - D.cpos == 1)) || ((S.rpos - D.rpos == 1) && (S.cpos - D.cpos == -1)))
				{
					return true;
				}
			}
		}
		return false;
	}
}
bool PieceIsNotPresentInColoum(char** B, int dim, int c, char sym)
{
	for (int i = 0; i < dim - 1; i++)
	{
		if (B[i][c] == sym)
			return false;
	}
	return true;
}
bool isPawnLegal(char** B, char sym, Position S, Position D)
{
	if (validCapRange(S))
	{
		return PieceIsNotPresentInColoum(B, 10, D.cpos, sym) && B[D.rpos][D.cpos] == '-';

	}
	else
	{
		int check = (D.rpos - S.rpos);
		int check1 = (D.rpos - S.rpos);
		if (sym == toupper(sym))
		{
			if (check == 1 && isVertical(S, D))
				return true;
		}
		else
		{
			if (check1 == -1 && isVertical(S, D))
				return true;
		}
		return false;
	}
}
bool isLegalMove(char** B, Position S, Position D, char** CapD)
{
	char sym;
	if (validCapRange(S))
	{
		if (S.rpos == 1)
		{
			sym = CapD[0][S.cpos - 10];
		}
		if (S.rpos == 3)
		{
			sym = CapD[1][S.cpos - 10];
		}
	}
	else
	{
		sym = B[S.rpos][S.cpos];
	}
	switch (sym) {
	case 'l':
	case 'L':
		return isLanceLegal(B, sym, S, D);
	case 'r':
	case 'R':
		return isRookLegal(B, S, D);
	case 'D':
	case 'd':
		return isDragonLegal(B, S, D);
	case'b':
	case'B':
		return isBishopLegal(B, S, D);
	case'h':
	case'H':
		return isHorseLegal(B, S, D);
	case'n':
	case'N':
		return isKnightLegal(B, S, D, sym);
	case'k':
	case'K':
		return isKingLegal(B, S, D);
	case's':
	case'S':
		return isSilverGeneralLegal(B, sym, S, D);
	case 'g':
	case 'G':
	case 't':
	case 'T':
	case 'u':
	case 'U':
	case 'x':
	case 'X':
	case 'z':
	case 'Z':
		return isGoldenGeneralLegal(B, sym, S, D);
	case'p':
	case'P':
		return isPawnLegal(B, sym, S, D);
	}
}

//Higlight n Unhiglight
bool** HighlightLoc(char** board, int dim, Position Sc, int turn, char** CapD) {
	bool** boolB = new bool* [9] {};
	for (int i = 0; i < dim; i++)
	{
		boolB[i] = new bool[9] {};
	}
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			Position dc;
			dc.rpos = i; dc.cpos = j;
			if (isMyPiece(board[dc.rpos][dc.cpos], turn) == 0)
			{

				boolB[i][j] = isLegalMove(board, Sc, dc, CapD);
			}

		}
	}
	return boolB;
}
void UnHighlight(int dim, int rows, int cols, bool** Map)
{
	int clr;
	int shift = rows - 1;
	for (int i = 0; i < dim; i++)
	{
		for (int j = 0; j < dim; j++)
		{
			if (Map[i][j] == true)
			{
				if ((i + j) % 2 == 0)
				{
					clr = LightGrey;
				}
				else
				{
					clr = Grey;
				}

				L2R(i * rows, j * cols, ((j + 1) * (cols)) - 1, -37, clr);
				L2R(i * rows + shift, j * cols, ((j + 1) * (cols)) - 1, -37, clr);
				int c = j * cols;
				T2B(c, i * rows, ((i + 1) * rows) - 1, -37, clr);
				T2B(c + shift, i * rows, ((i + 1) * rows) - 1, -37, clr);
			}

		}

	}


}
void Highlight(int dim, int rows, int cols, bool** Map)
{
	int shift = rows - 1;
	for (int i = 0; i < dim; i++)
	{
		for (int j = 0; j < dim; j++)
		{
			if (Map[i][j] == true)
			{
				L2R(i * rows, j * cols, ((j + 1) * (cols)) - 1, -37, 2);
				L2R(i * rows + shift, j * cols, ((j + 1) * (cols)) - 1, -37, 2);
				int c = j * cols;
				T2B(c, i * rows, ((i + 1) * rows) - 1, -37, 2);
				T2B(c + shift, i * rows, ((i + 1) * rows) - 1, -37, 2);
			}
		}
	}
}

// check and checkmate
Position findKing(char** B, int dim, int turn)
{
	Position temp;
	char king;
	if (turn == 0)
		king = 'k';
	else
		king = 'K';

	for (int i = 0; i < dim - 1; i++)
		for (int j = 0; j < dim - 1; j++)
		{
			temp.rpos = i, temp.cpos = j;
			if (B[temp.rpos][temp.cpos] == king)
				return temp;

		}

}
bool check(char** B, int dim, int turn, char** CapD)
{
	Position Sc;
	TurnChnage(turn);
	Position Dc = findKing(B, dim, turn);
	TurnChnage(turn);
	for (int i = 0; i < dim - 1; i++)
	{
		for (int j = 0; j < dim - 1; j++)
		{
			Sc.rpos = i, Sc.cpos = j;
			if (isMyPiece(B[Sc.rpos][Sc.cpos], turn))
			{
				if (isLegalMove(B, Sc, Dc, CapD))
				{
					return true;
				}
			}
		}
	}


	return false;


}
bool SelfCheck(char** B, int dim, int turn, char** CapD)
{

	TurnChnage(turn);
	return check(B, dim, turn, CapD);

}
void CopyBoard(char** p1, char** p2, char** p3, int dim, char** Cap, char** TCap)
{
	for (int i = 0; i < dim - 1; i++)
	{
		for (int j = 0; j < dim - 1; j++)
		{
			p2[i][j] = p1[i][j];
			p3[i][j] = p1[i][j];
		}
	}

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			TCap[i][j] = Cap[i][j];

		}
	}

}
bool ChedkMate(char** B, int dim, int turn, char** B2, char** CapD)
{
	Position MyPiece, Dc, prev;
	bool checkmate = false;
	TurnChnage(turn);
	for (int i = 0; i < dim - 1; i++)
	{
		for (int j = 0; j < dim - 1; j++)
		{
			prev.rpos = i, prev.cpos = j;
			MyPiece.rpos = i, MyPiece.cpos = j;
			if (isMyPiece(B[i][j], turn))
			{

				for (int k = 0; k < dim - 1; k++)
				{
					for (int l = 0; l < dim - 1; l++)
					{
						Dc.rpos = k, Dc.cpos = l;
						if (isLegalMove(B, MyPiece, Dc, CapD))
						{
							ReplaceBoard(B2, prev, Dc, B[i][j]);

							if (SelfCheck(B2, dim, turn, CapD) == 0)
							{
								return false;
							}

							prev.rpos = k, prev.cpos = l;
						}



					}

				}


			}
		}
	}
	return true;
}


//Promotions
void PromotionMessage(char& a, int boxdim)
{
	gotoRowCol(5 * boxdim + 2, 10 * boxdim);
	cout << "Promote (Y/N):";

	cin >> a;
	gotoRowCol(5 * boxdim + 2, 10 * boxdim);
	cout << "                                   ";

}
void promotion(char** Board, char sym, Position pro_cod, int boxdim)
{
	char a = '0';
	if (pro_cod.rpos == 0 || pro_cod.rpos == 1 || pro_cod.rpos == 2)
	{


		switch (sym)
		{
		case 'p':
			PromotionMessage(a, boxdim);
			if (a == 'Y' || a == 'y')
				Board[pro_cod.rpos][pro_cod.cpos] = 't';
			break;
		case 'r':
			PromotionMessage(a, boxdim);
			if (a == 'Y' || a == 'y')
				Board[pro_cod.rpos][pro_cod.cpos] = 'd';
			break;
		case 'b':
			PromotionMessage(a, boxdim);
			if (a == 'Y' || a == 'y')
				Board[pro_cod.rpos][pro_cod.cpos] = 'h';
			break;
		case 'l':
			PromotionMessage(a, boxdim);
			if (a == 'Y' || a == 'y')
				Board[pro_cod.rpos][pro_cod.cpos] = 'u';
			break;
		case 's':
			PromotionMessage(a, boxdim);
			if (a == 'Y' || a == 'y')
				Board[pro_cod.rpos][pro_cod.cpos] = 'x';
			break;
		case 'n':
			PromotionMessage(a, boxdim);
			if (a == 'Y' || a == 'y')
				Board[pro_cod.rpos][pro_cod.cpos] = 'z';
			break;

		default:
			break;
		}



	}
	if (pro_cod.rpos == 8 || pro_cod.rpos == 7 || pro_cod.rpos == 6)
	{

		switch (sym)
		{
		case 'P':
			PromotionMessage(a, boxdim);
			if (a == 'Y' || a == 'y')
				Board[pro_cod.rpos][pro_cod.cpos] = 'T';
			break;
		case 'R':
			PromotionMessage(a, boxdim);
			if (a == 'Y' || a == 'y')
				Board[pro_cod.rpos][pro_cod.cpos] = 'D';
			break;
		case 'B':
			PromotionMessage(a, boxdim);
			if (a == 'Y' || a == 'y')
				Board[pro_cod.rpos][pro_cod.cpos] = 'H';
			break;
		case 'L':
			PromotionMessage(a, boxdim);
			if (a == 'Y' || a == 'y')
				Board[pro_cod.rpos][pro_cod.cpos] = 'U';
			break;
		case 'S':
			PromotionMessage(a, boxdim);
			if (a == 'Y' || a == 'y')
				Board[pro_cod.rpos][pro_cod.cpos] = 'X';
			break;
		case 'N':
			PromotionMessage(a, boxdim);
			if (a == 'Y' || a == 'y')
				Board[pro_cod.rpos][pro_cod.cpos] = 'Z';
			break;

		default:
			break;
		}
	}

}

//Captured Pieces
bool isCaptured(char** B, int turn, Position Sc, Position Dc)
{
	return isMyPiece(B[Sc.rpos][Sc.cpos], turn) && !isMyPiece(B[Dc.rpos][Dc.cpos], turn) && B[Dc.rpos][Dc.cpos] != '-';
}
void intilizeWithASymbol(char** array, int m, int n, char K)
{
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			array[i][j] = K;
		}
	}
}
void intilizeWithANum(int** array, int m, int n, int K)
{
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			array[i][j] = 0;
		}
	}

}
int Freq(char** array, int r, int c, char sym)
{
	int count = 0;

	for (int j = 0; j < c; j++)
	{
		if (array[r][j] == sym)
		{
			count++;
		}
	}

	return count;
}
void compute_distints(char** array, int r, int c, char** Darray, int dc1, int dc2)
{

	intilizeWithASymbol(Darray, 2, 7, '*');
	int dr;
	char sym;
	dc1 = 0, dc2 = 0;
	for (int i = 0; i < r; i++)
		for (int j = 0; j < c; j++)
		{
			{
				sym = array[i][j];



				if (Freq(Darray, i, 7, sym) == 0)
				{
					if (i == 1)
					{
						dr = 1;
						Darray[dr][dc1++] = array[i][j];
					}
					else
					{
						dr = 0;
						Darray[dr][dc2++] = array[i][j];
					}


				}
			}
		}

}
void compute_Freq(char** array, int r, int c, char** Darray, int** Farray)
{
	int dr, dc1 = 0, dc2 = 0;
	char sym;
	int freq = 0;
	intilizeWithANum(Farray, 2, 7, 0);

	for (int i = 0; i < r; i++)
		for (int j = 0; j < c; j++)
		{
			{
				sym = Darray[i][j];


				freq = Freq(array, i, 20, sym);
				if (sym == '-' || sym == '*')
					freq = 0;
				if (i == 1)
				{
					dr = 1;
					Farray[dr][dc1++] = freq;
				}
				else
				{
					dr = 0;
					Farray[dr][dc2++] = freq;
				}



			}
		}

}
int  indexTellercap(char** array, int turn, int si, int ei)
{
	for (int i = si; i < ei; i++)
	{
		if (array[turn][i] == '-')
			return i;

	}
}
char Depromorion(char sym)
{
	switch (sym)
	{
	case 'G':
		return 'G';
	case 'K':
		return 'K';
	case 'P':
	case 'T':
		return 'P';
		break;
	case 'R':
	case 'D':
		return 'R';
		break;
	case 'B':
	case 'H':
		return 'B';
		break;
	case 'L':
	case 'U':
		return 'L';
		break;
	case 'S':
	case'X':
		return 'S';
		break;
	case 'N':
	case 'Z':
		return 'N';
		break;
	case 'g':
		return 'g';
	case 'k':
		return 'k';
	case 'p':
	case 't':
		return 'p';
		break;
	case 'r':
	case 'd':
		return 'r';
		break;
	case 'b':
	case 'h':
		return 'b';
		break;
	case 'l':
	case 'u':
		return 'l';
		break;
	case 's':
	case'x':
		return 's';
		break;
	case 'n':
	case 'z':
		return 'n';
		break;
	default:
		break;
	}
}
void StoreCapturedPiece(char** CapArray, char** B, int turn, Position Dc, int** count, char** CapArrayD, int& dc1, int& dc2)
{
	char Piece;
	if (turn == White)
		Piece = tolower(B[Dc.rpos][Dc.cpos]);
	else
		Piece = toupper(B[Dc.rpos][Dc.cpos]);

	Piece = Depromorion(Piece);
	int index = indexTellercap(CapArray, turn, 0, 20);
	if (CapArray[turn][index - 1] == Piece && CapArray[turn][index + 1] != Piece && CapArray[turn][index + 1] != '-')
	{
		index = indexTellercap(CapArray, turn, index + 1, 20);
	}


	CapArray[turn][index] = Piece;

	compute_distints(CapArray, 2, 20, CapArrayD, dc1, dc2);

	compute_Freq(CapArray, 2, 7, CapArrayD, count);

}

//Graphics
void Circle(int h, int k, float radius, char sym, int clr)
{

	for (int theta = 0; theta < 360; theta++)
	{
		SetClr(clr, BLACK);
		float rad = theta * 3.1415 / 180;
		float x = ceil(radius * cos(rad)) + k;
		float y = ceil(radius * sin(rad)) + h;
		gotoRowCol(y, x);
		cout << sym;
	}


}
void pawn(Position center, int clr, int bclr)
{

	Circle(center.rpos, center.cpos, 2.1, -37, clr);
	Circle(center.rpos, center.cpos, 1, -37, clr);
}
void king(Position center, int clr, int bclr)
{



	SetClr(clr, bclr);
	char sym = -37;
	gotoRowCol(center.rpos, center.cpos);
	cout << sym;
	gotoRowCol(center.rpos, center.cpos + 1);
	cout << sym;
	gotoRowCol(center.rpos + 1, center.cpos);
	cout << sym;
	gotoRowCol(center.rpos + 1, center.cpos + 1);
	cout << sym;
	L2R(center.rpos - 1, center.cpos - 1, center.cpos + 2, sym, clr);
	L2R(center.rpos + 2, center.cpos - 1, center.cpos + 2, sym, clr);
	L2R(center.rpos - 2, center.cpos - 2, center.cpos + 3, sym, clr);
	L2R(center.rpos + 3, center.cpos - 2, center.cpos + 3, sym, clr);
	SetClr(clr, bclr);
	gotoRowCol(center.rpos - 3, center.cpos);
	cout << sym;
	gotoRowCol(center.rpos - 3, center.cpos + 1);
	cout << sym;
	gotoRowCol(center.rpos - 3, center.cpos - 2);
	cout << sym;
	gotoRowCol(center.rpos - 3, center.cpos + 3);
	cout << sym;

	SetClr(clr, bclr);
	/*gotoRowCol(center.rpos - 4, center.cpos - 1);
	cout << char(221);
	gotoRowCol(center.rpos - 4, center.cpos + 2);
	cout << char(222);
	SetClr(clr,bclr);
	gotoRowCol(center.rpos - 3, center.cpos - 1);
	cout << char(221);
	gotoRowCol(center.rpos - 3, center.cpos + 2);
	cout << char(222);*/

	SetClr(clr, bclr);

	gotoRowCol(center.rpos - 4, center.cpos - 2);
	cout << char(221);
	gotoRowCol(center.rpos - 4, center.cpos + 3);
	cout << char(222);

	SetClr(WHITE, BLACK);

}
void GoldGen(Position center, int clr, int bclr)
{

	SetClr(clr, bclr);
	char sym = -37;
	gotoRowCol(center.rpos, center.cpos);
	cout << sym;
	gotoRowCol(center.rpos, center.cpos + 1);
	cout << sym;
	gotoRowCol(center.rpos + 1, center.cpos);
	cout << sym;
	gotoRowCol(center.rpos + 1, center.cpos + 1);
	cout << sym;
	L2R(center.rpos - 1, center.cpos - 2, center.cpos + 3, sym, clr);
	L2R(center.rpos + 2, center.cpos - 2, center.cpos + 3, sym, clr);
	L2R(center.rpos + 3, center.cpos - 3, center.cpos + 4, sym, clr);
	SetClr(clr, bclr);
	gotoRowCol(center.rpos - 2, center.cpos - 2);
	cout << sym;
	gotoRowCol(center.rpos - 2, center.cpos + 3);
	cout << sym;


	gotoRowCol(center.rpos - 2, center.cpos);
	cout << char(221);
	gotoRowCol(center.rpos - 2, center.cpos + 1);
	cout << char(222);

	SetClr(WHITE, BLACK);


}
void SilverGen(Position center, int clr, int bclr)
{

	SetClr(clr, bclr);
	char sym = -37;
	gotoRowCol(center.rpos, center.cpos);
	cout << sym;
	gotoRowCol(center.rpos, center.cpos + 1);
	cout << sym;
	gotoRowCol(center.rpos + 1, center.cpos);
	cout << sym;
	gotoRowCol(center.rpos + 1, center.cpos + 1);
	cout << sym;
	L2R(center.rpos - 1, center.cpos - 2, center.cpos + 3, sym, clr);
	L2R(center.rpos + 2, center.cpos - 2, center.cpos + 3, sym, clr);
	L2R(center.rpos + 3, center.cpos - 3, center.cpos + 4, sym, clr);
	SetClr(clr, bclr);
	gotoRowCol(center.rpos - 2, center.cpos);
	cout << char(220);
	gotoRowCol(center.rpos - 2, center.cpos + 1);
	cout << char(220);
	gotoRowCol(center.rpos - 2, center.cpos - 2);
	cout << char(221);
	gotoRowCol(center.rpos - 2, center.cpos + 3);
	cout << char(222);


	SetClr(WHITE, BLACK);

}
void Lance(Position center, int clr, int bclr)
{

	SetClr(clr, bclr);
	char sym = -37;
	gotoRowCol(center.rpos, center.cpos);
	cout << sym;
	gotoRowCol(center.rpos, center.cpos + 1);
	cout << sym;
	gotoRowCol(center.rpos + 1, center.cpos);
	cout << sym;
	gotoRowCol(center.rpos + 1, center.cpos + 1);
	cout << sym;

	L2R(center.rpos + 2, center.cpos - 1, center.cpos + 2, sym, clr);
	gotoRowCol(center.rpos - 1, center.cpos);
	cout << sym;
	gotoRowCol(center.rpos - 1, center.cpos + 1);
	cout << sym;



	SetClr(WHITE, BLACK);



}
void Horse(Position center, int clr, int bclr)
{

	SetClr(clr, bclr);
	char sym = -37;
	char sym2 = 222;
	gotoRowCol(center.rpos, center.cpos);
	cout << sym;
	gotoRowCol(center.rpos, center.cpos + 1);
	cout << sym;
	gotoRowCol(center.rpos + 1, center.cpos);
	cout << sym;
	gotoRowCol(center.rpos + 1, center.cpos + 1);
	cout << sym;

	L2R(center.rpos + 2, center.cpos - 2, center.cpos + 3, sym, clr);
	SetClr(clr, bclr);
	gotoRowCol(center.rpos - 1, center.cpos);
	cout << sym;
	gotoRowCol(center.rpos - 1, center.cpos + 1);
	cout << sym;
	gotoRowCol(center.rpos, center.cpos - 1);
	cout << sym;
	gotoRowCol(center.rpos - 1, center.cpos - 1);
	cout << sym;
	gotoRowCol(center.rpos - 1, center.cpos - 2);
	cout << sym;

	L2R(center.rpos - 2, center.cpos - 1, center.cpos + 2, sym, clr);
	SetClr(clr, bclr);
	gotoRowCol(center.rpos - 1, center.cpos + 2);
	cout << sym2;
	gotoRowCol(center.rpos - 1, center.cpos + 3);
	cout << sym;

	gotoRowCol(center.rpos - 3, center.cpos);
	cout << sym;
	gotoRowCol(center.rpos - 3, center.cpos + 1);
	cout << sym;

	SetClr(WHITE, BLACK);
}
void Bishop(Position center, int clr, int bclr)
{
	SetClr(clr, bclr);
	char sym = -37;
	char right = 221;
	char left = 222;
	gotoRowCol(center.rpos, center.cpos);
	cout << sym;
	gotoRowCol(center.rpos - 1, center.cpos - 1);
	cout << sym;
	gotoRowCol(center.rpos - 1, center.cpos + 1);
	cout << sym;
	SetClr(clr, bclr);
	gotoRowCol(center.rpos - 2, center.cpos - 1);
	cout << sym;
	gotoRowCol(center.rpos - 2, center.cpos + 1);
	cout << sym;
	gotoRowCol(center.rpos - 1, center.cpos - 2);
	cout << sym;
	gotoRowCol(center.rpos - 1, center.cpos + 2);
	cout << sym;
	gotoRowCol(center.rpos - 2, center.cpos - 2);
	cout << left;
	gotoRowCol(center.rpos - 2, center.cpos + 2);
	cout << right;
	gotoRowCol(center.rpos - 3, center.cpos - 1);
	cout << sym;
	SetClr(clr, bclr);
	gotoRowCol(center.rpos, center.cpos - 1);
	cout << sym;
	gotoRowCol(center.rpos, center.cpos + 1);
	cout << sym;
	SetClr(clr, bclr);
	L2R(center.rpos + 1, center.cpos - 2, center.cpos + 2, sym, clr);
	SetClr(clr, bclr);

	SetClr(WHITE, BLACK);

}
void Rook(Position center, int clr, int bclr)
{

	SetClr(clr, bclr);
	char sym = -37;
	gotoRowCol(center.rpos, center.cpos);
	cout << sym;
	gotoRowCol(center.rpos, center.cpos + 1);
	cout << sym;
	gotoRowCol(center.rpos + 1, center.cpos);
	cout << sym;
	gotoRowCol(center.rpos + 1, center.cpos + 1);
	cout << sym;


	L2R(center.rpos - 1, center.cpos - 2, center.cpos + 3, sym, clr);
	L2R(center.rpos + 2, center.cpos - 2, center.cpos + 3, sym, clr);
	L2R(center.rpos - 2, center.cpos - 3, center.cpos + 4, sym, clr);
	L2R(center.rpos + 3, center.cpos - 3, center.cpos + 4, sym, clr);

	SetClr(WHITE, BLACK);

}
void PrintPieces(char** Board, int dim, int Bdim)
{
	int fixR = 0;
	int fixC = 0;
	int bclr;
	Position center;
	char Piece;
	for (int i = 0; i < dim - 1; i++)
	{
		for (int j = 0; j < dim - 1; j++)
		{

			center.rpos = i + Bdim / 2 + fixR, center.cpos = j + Bdim / 2 + fixC;
			Piece = Board[i][j];

			if ((i + j) % 2 == 0)
				bclr = LightGrey;
			else
				bclr = Grey;
			switch (Piece)
			{
				//Small Letters
			case 't':
				pawn(center, Blue, bclr);
				break;
			case 'p':
				pawn(center, WHITE, bclr);
				break;
			case 'r':
				Rook(center, WHITE, bclr);
				break;
			case 'd':
				Rook(center, Blue, bclr);
				break;
			case 'b':
				Bishop(center, WHITE, bclr);;
				break;
			case 'h':
				Bishop(center, Blue, bclr);
				break;
			case 'l':
				Lance(center, WHITE, bclr);;
				break;
			case 'u':
				Lance(center, Blue, bclr);
				break;
			case 's':
				SilverGen(center, WHITE, bclr);;
				break;
			case 'x':
				SilverGen(center, Blue, bclr);
				break;
			case 'n':
				Horse(center, WHITE, bclr);;
				break;
			case 'z':
				Horse(center, Blue, bclr);
				break;
			case 'g':
				GoldGen(center, WHITE, bclr);;
				break;
			case 'k':
				king(center, WHITE, bclr);;
				break;

				//Capital Letters
			case 'T':
				pawn(center, Red, bclr);
				break;
			case 'P':
				pawn(center, BLACK, bclr);
				break;
			case 'R':
				Rook(center, BLACK, bclr);
				break;
			case 'D':
				Rook(center, BLACK, bclr);
				break;
			case 'B':
				Bishop(center, BLACK, bclr);
				break;
			case 'H':
				Bishop(center, Red, bclr);
				break;
			case 'L':
				Lance(center, BLACK, bclr);
				break;
			case 'U':
				Lance(center, Red, bclr);
				break;
			case 'S':
				SilverGen(center, BLACK, bclr);
				break;
			case 'X':
				SilverGen(center, Red, bclr);
				break;
			case 'N':
				Horse(center, BLACK, bclr);
				break;
			case 'Z':
				Horse(center, Red, bclr);
				break;
			case 'G':
				GoldGen(center, BLACK, bclr);
				break;
			case 'K':
				king(center, BLACK, bclr);
				break;
			default:
				break;

			}

			fixC += (Bdim - 1);

		}
		fixC = 0;
		fixR += (Bdim - 1);

	}
	SetClr(WHITE, BLACK);
}
void PrintOnePiece(char** Board, int Bdim, Position Dc)
{
	int fixR = Dc.rpos * (Bdim - 1);
	int fixC = Dc.cpos * (Bdim - 1);
	int bclr;
	Position center;
	char Piece;


	center.rpos = Dc.rpos + Bdim / 2 + fixR, center.cpos = Dc.cpos + Bdim / 2 + fixC;
	Piece = Board[Dc.rpos][Dc.cpos];

	if ((Dc.rpos + Dc.cpos) % 2 == 0)
		bclr = LightGrey;
	else
		bclr = Grey;
	switch (Piece)
	{
		//Small Letters
	case 't':
		pawn(center, Blue, bclr);
		break;
	case 'p':
		pawn(center, WHITE, bclr);
		break;
	case 'r':
		Rook(center, WHITE, bclr);
		break;
	case 'd':
		Rook(center, Blue, bclr);
		break;
	case 'b':
		Bishop(center, WHITE, bclr);;
		break;
	case 'h':
		Bishop(center, Blue, bclr);
		break;
	case 'l':
		Lance(center, WHITE, bclr);;
		break;
	case 'u':
		Lance(center, Blue, bclr);
		break;
	case 's':
		SilverGen(center, WHITE, bclr);;
		break;
	case 'x':
		SilverGen(center, Blue, bclr);
		break;
	case 'n':
		Horse(center, WHITE, bclr);;
		break;
	case 'z':
		Horse(center, Blue, bclr);
		break;
	case 'g':
		GoldGen(center, WHITE, bclr);;
		break;
	case 'k':
		king(center, WHITE, bclr);;
		break;

		//Capital Letters
	case 'T':
		pawn(center, Red, bclr);
		break;
	case 'P':
		pawn(center, BLACK, bclr);
		break;
	case 'R':
		Rook(center, BLACK, bclr);
		break;
	case 'D':
		Rook(center, BLACK, bclr);
		break;
	case 'B':
		Bishop(center, BLACK, bclr);
		break;
	case 'H':
		Bishop(center, Red, bclr);
		break;
	case 'L':
		Lance(center, BLACK, bclr);
		break;
	case 'U':
		Lance(center, Red, bclr);
		break;
	case 'S':
		SilverGen(center, BLACK, bclr);
		break;
	case 'X':
		SilverGen(center, Red, bclr);
		break;
	case 'N':
		Horse(center, BLACK, bclr);
		break;
	case 'Z':
		Horse(center, Red, bclr);
		break;
	case 'G':
		GoldGen(center, BLACK, bclr);
		break;
	case 'K':
		king(center, BLACK, bclr);
		break;
	default:
		break;

	}

	SetClr(WHITE, BLACK);
}
Position FindaPiece(char** array, int r, int c, char sym)
{
	Position R;
	for (int i = 0; i < r; i++)
	{
		for (int j = 0; j < c; j++)
		{
			char sym2 = array[i][j];
			if (array[i][j] == sym)
			{
				R.rpos = i;
				R.cpos = j;
				return R;
			}
		}
	}
	R.rpos = -1;
	R.cpos = -1;
	return R;
}
void PrintCapPiece(char** Board, int Bdim, int turn, int** count, char** Capt, char** B, Position Dest)
{
	char Piece;

	Position Dc, count_Cord;
	Piece = B[Dest.rpos][Dest.cpos];
	Piece = Depromorion(Piece);
	if (Piece == toupper(Piece))
		Piece = tolower(Piece);
	else
		Piece = toupper(Piece);
	Position prev = FindaPiece(Capt, 2, 7, Piece);
	if (prev.rpos != -1)
	{
		if (prev.rpos == 0)
			Dc.rpos = 1;
		else
			Dc.rpos = 3;
		Dc.cpos = prev.cpos;
	}


	int fixR = Dc.rpos * (Bdim - 1);
	int fixC = Dc.cpos * (Bdim - 1);
	int bclr;
	Position center;



	center.rpos = Dc.rpos + Bdim / 2 + fixR, center.cpos = Dc.cpos + Bdim / 2 + fixC + 10 * Bdim;


	if ((Dc.rpos + Dc.cpos) % 2 == 0)
		bclr = LightGrey;
	else
		bclr = Grey;
	switch (Piece)
	{
		//Small Letters
	case 't':
		pawn(center, Blue, bclr);
		break;
	case 'p':
		pawn(center, WHITE, bclr);
		break;
	case 'r':
		Rook(center, WHITE, bclr);
		break;
	case 'd':
		Rook(center, Blue, bclr);
		break;
	case 'b':
		Bishop(center, WHITE, bclr);;
		break;
	case 'h':
		Bishop(center, Blue, bclr);
		break;
	case 'l':
		Lance(center, WHITE, bclr);;
		break;
	case 'u':
		Lance(center, Blue, bclr);
		break;
	case 's':
		SilverGen(center, WHITE, bclr);;
		break;
	case 'x':
		SilverGen(center, Blue, bclr);
		break;
	case 'n':
		Horse(center, WHITE, bclr);;
		break;
	case 'z':
		Horse(center, Blue, bclr);
		break;
	case 'g':
		GoldGen(center, WHITE, bclr);;
		break;
	case 'k':
		king(center, WHITE, bclr);;
		break;

		//Capital Letters
	case 'T':
		pawn(center, Red, bclr);
		break;
	case 'P':
		pawn(center, BLACK, bclr);
		break;
	case 'R':
		Rook(center, BLACK, bclr);
		break;
	case 'D':
		Rook(center, BLACK, bclr);
		break;
	case 'B':
		Bishop(center, BLACK, bclr);
		break;
	case 'H':
		Bishop(center, Red, bclr);
		break;
	case 'L':
		Lance(center, BLACK, bclr);
		break;
	case 'U':
		Lance(center, Red, bclr);
		break;
	case 'S':
		SilverGen(center, BLACK, bclr);
		break;
	case 'X':
		SilverGen(center, Red, bclr);
		break;
	case 'N':
		Horse(center, BLACK, bclr);
		break;
	case 'Z':
		Horse(center, Red, bclr);
		break;
	case 'G':
		GoldGen(center, BLACK, bclr);
		break;
	case 'K':
		king(center, BLACK, bclr);
		break;
	default:
		break;

	}

	SetClr(WHITE, BLACK);
}
void PrintFreq(int** count, int bdim)
{
	for (int i = 1, r = 0; i <= 4; i += 2, r++)
	{
		for (int j = 0; j < 7; j++)
		{


			gotoRowCol(i * bdim, j * bdim + 10 * bdim + bdim - 1);
			if (count[r][j] > 0)
				cout << count[r][j];


		}

	}
}
void PrintAllCapPiece(char** Board, int Bdim, char** Capt)

{
	char Piece;
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 20; j++)
		{
			Piece = Board[i][j];
			Position Dc, count_Cord;
			Position prev = FindaPiece(Capt, 2, 7, Piece);
			if (prev.rpos != -1)
			{
				if (prev.rpos == 0)
					Dc.rpos = 1;
				else
					Dc.rpos = 3;
				Dc.cpos = prev.cpos;
			}
			int fixR = Dc.rpos * (Bdim - 1);
			int fixC = Dc.cpos * (Bdim - 1);
			int bclr;
			Position center;
			center.rpos = Dc.rpos + Bdim / 2 + fixR, center.cpos = Dc.cpos + Bdim / 2 + fixC + 10 * Bdim;
			if ((Dc.rpos + Dc.cpos) % 2 == 0)
				bclr = LightGrey;
			else
				bclr = Grey;
			switch (Piece)
			{
				//Small Letters
			case 't':
				pawn(center, Blue, bclr);
				break;
			case 'p':
				pawn(center, WHITE, bclr);
				break;
			case 'r':
				Rook(center, WHITE, bclr);
				break;
			case 'd':
				Rook(center, Blue, bclr);
				break;
			case 'b':
				Bishop(center, WHITE, bclr);;
				break;
			case 'h':
				Bishop(center, Blue, bclr);
				break;
			case 'l':
				Lance(center, WHITE, bclr);;
				break;
			case 'u':
				Lance(center, Blue, bclr);
				break;
			case 's':
				SilverGen(center, WHITE, bclr);;
				break;
			case 'x':
				SilverGen(center, Blue, bclr);
				break;
			case 'n':
				Horse(center, WHITE, bclr);;
				break;
			case 'z':
				Horse(center, Blue, bclr);
				break;
			case 'g':
				GoldGen(center, WHITE, bclr);;
				break;
			case 'k':
				king(center, WHITE, bclr);;
				break;

				//Capital Letters
			case 'T':
				pawn(center, Red, bclr);
				break;
			case 'P':
				pawn(center, BLACK, bclr);
				break;
			case 'R':
				Rook(center, BLACK, bclr);
				break;
			case 'D':
				Rook(center, BLACK, bclr);
				break;
			case 'B':
				Bishop(center, BLACK, bclr);
				break;
			case 'H':
				Bishop(center, Red, bclr);
				break;
			case 'L':
				Lance(center, BLACK, bclr);
				break;
			case 'U':
				Lance(center, Red, bclr);
				break;
			case 'S':
				SilverGen(center, BLACK, bclr);
				break;
			case 'X':
				SilverGen(center, Red, bclr);
				break;
			case 'N':
				Horse(center, BLACK, bclr);
				break;
			case 'Z':
				Horse(center, Red, bclr);
				break;
			case 'G':
				GoldGen(center, BLACK, bclr);
				break;
			case 'K':
				king(center, BLACK, bclr);
				break;
			default:
				break;

			}
		}
	SetClr(WHITE, BLACK);
}

//Undo
void UndoMessage(int boxDim, Position Undo)
{
	SetClr(Red, LightGrey);
	gotoRowCol(Undo.rpos * boxDim, Undo.cpos * boxDim);
	cout << "UNDO";
	gotoRowCol(Undo.rpos * boxDim - 1, Undo.cpos * boxDim);
	cout << "    ";
	gotoRowCol(Undo.rpos * boxDim + 1, Undo.cpos * boxDim);
	cout << "    ";
	SetClr(WHITE, BLACK);
}
void UndoArrayIntit(Position UndoArray[], int size)
{
	for (int i = 0; i < size; i++)
	{
		UndoArray[i].rpos = -1;
		UndoArray[i].cpos = -1;
	}
}
void UNDO(Position UndoArray[], Position& source, Position& dest)
{
	source.rpos = UndoArray[1].rpos;
	source.cpos = UndoArray[1].cpos;
	dest.rpos = UndoArray[0].rpos;
	dest.cpos = UndoArray[0].cpos;
}
void SafeUndo(Position UndoArray[], Position& source, Position& dest)
{
	UndoArray[0].rpos = source.rpos;
	UndoArray[0].cpos = source.cpos;
	UndoArray[1].rpos = dest.rpos;
	UndoArray[1].cpos = dest.cpos;


}


//Load n Safe
void Menu(int boxDim)
{
	gotoRowCol(5 * boxDim + 2, 10 * boxDim);
	cout << "1.New Game" << endl;
	gotoRowCol(5 * boxDim + 3, 10 * boxDim);
	cout << "2.Load Game" << endl;
	gotoRowCol(5 * boxDim + 4, 10 * boxDim);
	cout << "0.Exit" << endl;
}
void MenuRemove(int boxDim)
{
	for (int i = 2; i <= 10; i++)
	{
		gotoRowCol(5 * boxDim + i, 10 * boxDim);
		cout << "                               ";
	}
}
void Load(const char* fName, char** B, int dim, char** Cap, char** CapD, int** Freq, int turn, string Names[])
{
	ofstream writer(fName);
	writer << dim << endl;
	for (int i = 0; i < dim; i++)
	{
		for (int j = 0; j < dim; j++)
		{
			writer << B[i][j];
		}
		writer << endl;
	}

	writer << endl;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			writer << Cap[i][j];
		}
		writer << endl;
	}

	writer << endl;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			writer << CapD[i][j];
		}
		writer << endl;
	}

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			writer << Freq[i][j] << " ";
		}
		writer << endl;
	}
	writer << turn;
	for (int i = 0; i < 2; i++)
	{
		writer << Names[i] << " ";
	}

}


//Game
int main()
{

	char** B = nullptr, ** captured = nullptr, ** capturedD = nullptr, ** TempCaptured = nullptr;
	int** capCount = nullptr;
	char** TempB = nullptr;
	char** TempB2 = nullptr;
	string Names[2] = {};
	Position P, source, destination, capt, Undo, UndoArray[2];
	int turn = 0, dim = 0, cap_index[2], dc1 = 0, dc2 = 0;
	Undo.rpos = 8, Undo.cpos = 17;
	int boxDim = 9, choice = -1;


	DrawBoard(9, boxDim, boxDim);
	DrawCapBoard(9, boxDim, boxDim, Names);
	Menu(boxDim);
	gotoRowCol(5 * boxDim + 5, 10 * boxDim);
	cin >> choice;
	switch (choice)
	{
	case 0:
		cout << "Exit", exit(1);
	case 1:
		init(B, dim, turn, Names, TempB, TempB2, captured, cap_index, capturedD, capCount, TempCaptured, boxDim);
		break;
	case 2:
		initLoad("Load.txt", B, dim, turn, Names, TempB, TempB2, captured, cap_index, capturedD, capCount, TempCaptured);
		break;
	default:
		break;
	}
	MenuRemove(boxDim);

	UndoArrayIntit(UndoArray, 2);
	//DrawBoard(dim - 1, boxDim, boxDim);
	DrawCapBoard(dim - 1, boxDim, boxDim, Names);
	PrintBoard2(B, dim, boxDim);
	PrintPieces(B, dim, boxDim);
	PrintAllCapPiece(captured, boxDim, capturedD);
	PrintFreq(capCount, boxDim);
	UndoMessage(boxDim, Undo);


	while (true)
	{

		TurnMessage(Names, turn, boxDim);

		do
		{
			do
			{
				do
				{
					do
					{
						selectPosition(source, dim, boxDim);


					} while (isValidSource(B, dim, turn, source, capturedD, UndoArray) == 0);

					if (isUndoPosition(source))
						break;

					bool** Map = HighlightLoc(B, dim, source, turn, capturedD);
					Highlight(dim - 1, boxDim, boxDim, Map);
					selectPosition(destination, dim, boxDim);
					UnHighlight(dim - 1, boxDim, boxDim, Map);



				} while (isValidDestination(B, dim, turn, destination) == 0);
				if (isUndoPosition(source))
					break;
			} while ((isLegalMove(B, source, destination, capturedD) == 0));
			if (isUndoPosition(source))
				break;
			updadteBoard(TempB, source, destination, TempCaptured, capturedD, capCount);


		} while (SelfCheck(TempB, dim, turn, capturedD));



		if (isUndoPosition(source))
		{
			UNDO(UndoArray, source, destination);
			initLoad("Undo.txt", B, dim, turn, Names, TempB, TempB2, captured, cap_index, capturedD, capCount, TempCaptured);
			UndoArrayIntit(UndoArray, 2);
			RemovePieces(boxDim, source, destination, capCount);
			PrintOnePiece(B, boxDim, destination);
			PrintOnePiece(B, boxDim, source);
			PrintSpecificCapBoard(dim - 1, boxDim, boxDim, capCount);
			PrintAllCapPiece(captured, boxDim, capturedD);
			PrintFreq(capCount, boxDim);
			UndoMessage(boxDim, Undo);

		}

		else
		{
			SafeUndo(UndoArray, source, destination);
			Load("UNdo.txt", B, dim, captured, capturedD, capCount, turn, Names);

			if (isCaptured(B, turn, source, destination))
			{


				StoreCapturedPiece(captured, B, turn, destination, capCount, capturedD, dc1, dc2);
				PrintCapPiece(captured, boxDim, turn, capCount, capturedD, B, destination);
				PrintFreq(capCount, boxDim);


			}

			updadteBoard(B, source, destination, captured, capturedD, capCount);
			if (validCapRange(source))
			{

				compute_distints(captured, 2, 20, capturedD, dc1, dc2);
				compute_Freq(captured, 2, 7, capturedD, capCount);
				PrintFreq(capCount, boxDim);
			}
			promotion(B, B[destination.rpos][destination.cpos], destination, boxDim);
			CopyBoard(B, TempB, TempB2, dim, captured, TempCaptured);


			RemovePieces(boxDim, source, destination, capCount);
			PrintOnePiece(B, boxDim, destination);

			if (check(B, dim, turn, capturedD))
			{
				if (ChedkMate(B, dim, turn, TempB2, capturedD))
				{
					gotoRowCol(5 * boxDim + 4, 10 * boxDim);
					cout << "Checkmate";
					exit(1);
				}
				else
				{
					gotoRowCol(5 * boxDim + 3, 10 * boxDim);
					cout << "Check!!";
				}

			}
			else
			{
				gotoRowCol(5 * boxDim + 3, 10 * boxDim);
				cout << "                ";
			}

			TurnChnage(turn);

		}

		Load("Load.txt", B, dim, captured, capturedD, capCount, turn, Names);
	}



}




