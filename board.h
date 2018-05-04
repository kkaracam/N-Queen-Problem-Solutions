#include<vector>
#include<iostream>
#include <stdlib.h>
#include <chrono>
#include <ctime>
#include<algorithm>
#include<fstream>

using namespace std;

struct Cell {
	Cell(bool o = false, int xc = -1, int yc = -1, int heu = -1) : occupied(o), x(xc), y(yc), h(heu) {};
	bool occupied;
	int x, y, h;

	const bool & operator == (const Cell & rhs) {
		return (this->x == rhs.x) && (this->y == rhs.y);
	};
	const bool & operator != (const Cell & rhs) {
		return !(*this == rhs);
	};
	const Cell & operator = (const Cell & rhs) {
		this->h = rhs.h;
		this->occupied = rhs.occupied;
		this->x = rhs.x;
		this->y = rhs.y;

		return *this;
	};
};

class Board
{
public:
	Board(int s = 100) {
		size = s;
		occ = new Cell[size];
		fillTable();

	};
	~Board() {
		delete[] occ;
	};

	void fillTable() {
		for (int i = 0; i < size; i++)
		{
			int pos = rand() % size;
			Cell temp(true, pos, i);
			occ[i] = temp;
		}
	};

	bool canAttack(Cell queen, Cell target) {
		int xt = queen.x - target.x;
		int yt = queen.y - target.y;
		xt = xt < 0 ? -1 * xt : xt;
		yt = yt < 0 ? -1 * yt : yt;

		return ((xt == yt) || !(xt && yt)) && (queen != target);
	}

	int totalBoardH() {
		int res = 0;
		for (int i = 0; i < size; i++)
		{
			occ[i].h = 0;
			for (int j = 0; j < size; j++)
			{
				if (canAttack(occ[i], occ[j]))
					occ[i].h++;
			}
			res += occ[i].h;
		}
		return res;
	}

	Cell calcH(Cell queen, int index, bool isFC) {
		if (!queen.occupied) {
			cout << "There are no queens in the given cell.\n";
			return queen;
		}

		Cell max = new Cell;
		max = queen;
		max.h = isFC ? queen.h : INT_MAX;
		for (int k = 0; k < size; k++) {
			Cell target(false, k, queen.y, 0);

			for (int i = 0; i < size; i++)
			{
				if (canAttack(target, occ[i]) && i!=queen.y)
					target.h++;
			}
			if (target.h < max.h) {
				max = target;
				if (isFC)
					break;
			}
		}

		max.occupied = true;

		return max;
	};

	float move_decide(int old, int n, float tmp) {
		return n < old ? 1.0 : exp((old-n) / tmp *1.0);
	}

	float fRand(float fMin, float fMax)
	{
		double f = (double)rand() / RAND_MAX;
		return fMin + f * (fMax - fMin);
	}

	void runSA(float tmp, float coolF) {
		heuristic = totalBoardH();
		int it = 0;
		do
		{
			int col = rand() % size, row = rand() % size;
			Cell temp(false, row, col, 0);
			for (int i = 0; i < size; i++)
			{
				if (canAttack(temp, occ[i]) && i != temp.y)
					temp.h++;
			}
			if(move_decide(occ[col].h, temp.h, tmp) > fRand(0, 1))
				occ[col] = temp;
				
			tmp *= (1-coolF);

		} while (tmp > 0.00001);
		heuristic = totalBoardH();
	}

	void replace(bool isFC) {
		heuristic = totalBoardH();
		do
		{
			int pos_move = 0;
			Cell best_max(false, -1, -1, INT_MAX);
			for (int i = 0; i < size; i++) {
				Cell temp = calcH(occ[i], i, isFC);
				if (temp.h < best_max.h && temp != occ[i]) {
					best_max = temp;
					pos_move++;
					if (isFC)
						break;
				}
			}		

			if (pos_move)
				occ[best_max.y] = best_max;
			else
				break;

			heuristic = totalBoardH();
		} while (true);
		heuristic = totalBoardH();
	}

	bool isSuccess() {
		return !heuristic;
	}

	void run(bool isFC) {
		replace(isFC);
	};

private:
	int size, heuristic;
	Cell **board;
	Cell *occ;
};