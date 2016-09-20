// ConsoleApplication9.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <map>
#include <string.h>
using namespace std;
/*************
* CONST
*************/
#define WALL '#'
#define WAY 'R'
#define START 'S'
#define FINISH 'F'
#define OTHER '.'

#define DRAWPATH

#define M 60
#define N 60

struct Point{
	int x;
	int y;
	Point(){
		x = 0;
		y = 0;
	}
	Point(int _x, int _y)
	{
		x = _x;
		y = _y;

	}
	void drawIn(char array[N][M + 1], char sym)
	{
		if (x < N && x >= 0 && y >= 0 && y < M)
			array[x][y] = sym;
	}
	bool operator==(Point in){
		if (in.x == x && y == in.y)
			return true;
		else
			return false;
	}
};

struct Line
{
	Point start;
	Point end;
	Line(int x1, int y1, int x2, int y2){
		start = Point(y1,x1);
		end = Point(y2, x2);
	}
	Line(Point p1, Point p2){
		start = p1;
		end = p2;
	}
	double length()
	{
		return	sqrt(pow((double)end.x - start.x, 2) + pow((double)end.y - start.y, 2));
	}
	double getA()
	{
		return start.y - end.y;
	}
	double getB()
	{
		return end.x - start.x;
	}
	double getC()
	{
		return start.x*end.y - end.x*start.y;
	}

	bool isMyPoint(Point in){
		return (abs(sqrt(pow((double)start.x - in.x, 2) + pow((double)start.y - in.y, 2)) + sqrt(pow((double)end.x - in.x, 2) + pow((double)end.y - in.y, 2)) - sqrt(pow((double)end.x - start.x, 2) + pow((double)end.y - start.y, 2))) < 0.2);
	}

	Point cross(Line in){
		int a1 = getA();
		int b1 = getB();
		int c1 = getC();

		int a2 = in.getA();
		int b2 = in.getB();
		int c2 = in.getC();
		if (b1*a2 - b2*a1 == 0)
			return Point(-1, -1);

		int x = (c1*b2 - c2*b1) / (b1*a2 - b2*a1);
		int y = (c2*a1 - c1*a2) / (b1*a2 - b2*a1);
		/* if (x == 0 || x > N || y == 0 || y > M)
		return Point(-1, -1);
		*/
		bool isMy1 = isMyPoint(Point(x, y));
		bool isMy2 = in.isMyPoint(Point(x, y));
		if (isMy1 && isMy2)
		{
			// printf("x=%d, y=%d", x, y);
			return Point(x, y);
		}
		else
		{
			//  printf("NO!!!");
			return Point(-1, -1);
		}
	}

	void drawIn(char array[N][M + 1], char sym)
	{
		if (start.x == end.x)
		{
			for (int y = start.y; y != end.y;)
			{
				if (start.x < N && start.x >= 0 && y >= 0 && y < M)
					array[start.x][y] = sym;
				if (start.y > end.y)	y--;
				else y++;
			}
			return;
		}
		if (start.y == end.y)
		{
			for (int x = start.x; x != end.x;)
			{
				if (x < N && x >= 0 && start.y >= 0 && start.y < M)
					array[x][start.y] = sym;
				if (start.x > end.x)	x--;
				else x++;
			}
			return;
		}

		for (int x = start.x; x != end.x;)
		{
			int y = -(getA()*x + getC()) / getB();
			if (x < N && x >= 0 && y >= 0 && y < M)
				array[x][y] = sym;
			if (x > end.x)	x--;
			else x++;

		}
	}

};


void show(char array[N][M + 1]){
	for (int i = 0; i < N; i++)
	{
		array[i][M] = '\0';
		printf("%s\n", array[i]);
	}
}

/************************
* VALUES
*************************/
Line wallH = Line(N / 8, M / 2, N, M / 2);
Line wallV = Line(N / 2, M / 8, N / 2, M / 8 * 7);
Line task = Line(0, 0, N / 2 + 2, M / 2 + 2);

struct Node{
	Point p;
	int g = INT_MAX;
	Node *parent = NULL;
	vector<Node*> neiborns;
	Node(Point in){
		p = in;
	}
	void checkForCross(Node *check){
		Point H = Line(p, check->p).cross(wallH);
		Point V = Line(p, check->p).cross(wallV);

		if ((V == Point(-1, -1) || ( (V == check->p || V == p))))
			if (H == Point(-1, -1) || ( (H == check->p || H == p)))
			{
				double nG = g + Line(p, check->p).length();
				if (check->g > nG)
				{
					check->g = nG;
					check->parent = this;
					neiborns.push_back(check);
				}
			}
	}
};

int main(int argc, char* argv[])
{
	char matrix[N][M + 1];
	memset(matrix, OTHER, N*(M + 1));
	//
	//
	Node* all[5] = { &Node(task.end), &Node(wallH.start), &Node(wallH.end), &Node(wallV.start), &Node(wallV.end) };
	vector<Node*> v(all, all + sizeof all / sizeof all[0]);
	vector<Node*> res;
	//.erase (myvector.begin()+5);
	Node start(task.start);
	res.push_back(&start);
	start.g = 0;

	Point H = Line(wallV.start, task.end).cross(wallH);

	while (res.size() > 0)
	{
		Node *temp = res.at(0);
		if (temp == NULL)
		{
			int l = 0;
		}
		for (int i = 0; i < v.size(); i++)
		{
			temp->checkForCross(v.at(i));
		}
		//res.reserve((res.size() + temp->neiborns.size())*2);
		for (int i = 0; i < temp->neiborns.size(); i++)
		{
			res.push_back(temp->neiborns.at(i));
		}
		/*if (temp->neiborns.size() > 0)
		{
		res.insert((vector<Node*>::iterator)res.end(), temp->neiborns.begin(), temp->neiborns.end());//VS2013 problem
		}*/
		res.erase(res.begin());
	}



#ifdef DRAWPATH
	task.drawIn(matrix, '@');
#endif // DRAWPATH

	Node *result = all[0];
	while (result->parent != NULL)
	{
		Line(result->p, result->parent->p).drawIn(matrix, WAY);
		result = result->parent;
	}
	wallV.drawIn(matrix, WALL);
	wallH.drawIn(matrix, WALL);
	task.start.drawIn(matrix, START);
	task.end.drawIn(matrix, FINISH);
	//
	show(matrix);
	getchar();
	return 0;
}

