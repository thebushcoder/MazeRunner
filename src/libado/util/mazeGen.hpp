/*
 * MazeGen.hpp
 *
 *  Created on: 14 Mar 2018
 *      Author: linbob
 */

#ifndef LIBADO_UTIL_MAZEGEN_HPP_
#define LIBADO_UTIL_MAZEGEN_HPP_

#include <random>
#include <vector>

#include "../map/tileMap.hpp"

class MazeGen{
public:
	void generateMaze(TileMap* map);
	static MazeGen* getInstance();
private:
	enum Direction{
		N = 0,
		E = 1,
		S = 2,
		W = 3
	};

	void initMaze();
	bool markBorder(int x, int y, int mapX, int mapY, TileMap* map);
	void recursiveBackTrace(int x, int y);
	int getDirX(Direction d){
		switch(d){
		case N:{
			return 0;
			break;
		}
		case E:{
			return 1;
			break;
		}
		case S:{
			return 0;
			break;
		}
		case W:{
			return -1;
			break;
		}
		default:
			return 0;
		}
	}
	int getDirY(Direction d){
		switch(d){
		case N:{
			return -1;
			break;
		}
		case E:{
			return 0;
			break;
		}
		case S:{
			return 1;
			break;
		}
		case W:{
			return 0;
			break;
		}
		default:
			return 0;
		}
	}
	Direction getOpposite(Direction d){
		switch(d){
		case N:{
			return S;
			break;
		}
		case E:{
			return W;
			break;
		}
		case S:{
			return N;
			break;
		}
		case W:{
			return E;
			break;
		}
		}
	}

	int mW, mH, gridSize;

	std::mt19937_64 gen;
	std::uniform_int_distribution<> directionDist;
	std::vector< std::vector<std::vector<bool> > > mazeGrid;
	std::vector< std::vector<bool> > gridVisited;
	std::vector<Direction> dirs;
};

#endif /* LIBADO_UTIL_MAZEGEN_HPP_ */
