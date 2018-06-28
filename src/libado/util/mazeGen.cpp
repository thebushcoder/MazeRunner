/*
 * mazeGen.cpp
 *
 *  Created on: 14 Mar 2018
 *      Author: linbob
 */

#include "mazeGen.hpp"


void MazeGen::generateMaze(TileMap* map){
	gridSize = 6;
	mW = map->getWidth() / gridSize;
	mH = map->getHeight() / gridSize;

	initMaze();

	recursiveBackTrace(0, 0);

	for(int y = 0; y < mH; ++y){
		for(int x = 0; x < mW; ++x){
			printf("%s", (
					mazeGrid[x][y][Direction::S] ? " " : "_"
			));
			printf("%s", (
					mazeGrid[x][y][Direction::E] ? " " : "!"
			));
		}
		printf("\n");
	}

	for(int y = 0; y <= mH; ++y){
		for(int x = 0; x <= mW; ++x){
			int mapX = x * gridSize;
			int mapY = y * gridSize;

//			if(markBorder(x, y, mapX, mapY, map)) continue;
			markBorder(x, y, mapX, mapY, map);

			if(x >= mW || y >= mH) continue;

			if(!mazeGrid[x][y][Direction::S]){
				for(int i = 0; i < gridSize; ++i){
					if(mapX + i >= map->getWidth()) continue;

					if(mapY + gridSize - 1 >= map->getHeight()){
						map->getTileLayer().setTile(mapX + i, map->getHeight() - 1,
								map->getFactory()->createTile(0));
					}else{
						map->getTileLayer().setTile(mapX + i, mapY + gridSize - 1,
								map->getFactory()->createTile(0));
					}

					if(mapY + gridSize < map->getHeight()){
						map->getTileLayer().setTile(mapX + i, mapY + gridSize,
								map->getFactory()->createTile(0));
					}
				}
			}

			if(!mazeGrid[x][y][Direction::E]){
				for(int i = -1; i < gridSize + 1; ++i){
					if(mapY + i < 0 || mapY + i >= map->getHeight()) continue;

					if(mapX + gridSize - 1 >= map->getWidth()){
						map->getTileLayer().setTile(map->getWidth() - 1, mapY + i,
								map->getFactory()->createTile(0));
					}else{
						map->getTileLayer().setTile(mapX + gridSize - 1,
								mapY + i, map->getFactory()->createTile(0));
					}

					if(mapX + gridSize < map->getWidth()){
						map->getTileLayer().setTile(mapX + gridSize, mapY + i,
								map->getFactory()->createTile(0));
					}
				}
			}

		}
	}
}
bool MazeGen::markBorder(int x, int y, int mapX, int mapY, TileMap* map){
	bool flag = false;
	if(x == 0){
		for(int i = 0; i < gridSize; ++i){
			int tileY = mapY + i;
			if(tileY >= map->getHeight()){
				tileY = map->getHeight() - 1;
			}
			map->getTileLayer().setTile(x, tileY,
					map->getFactory()->createTile(1));
		}
		flag = true;
	}else if(x == mW){
		for(int i = 0; i < gridSize; ++i){
			int tileY = mapY + i;
			if(tileY >= map->getHeight()){
				tileY = map->getHeight() - 1;
			}
			map->getTileLayer().setTile(x * gridSize - 1, tileY,
					map->getFactory()->createTile(0));
		}
		flag = true;
	}
	if(y == 0){
		for(int i = 0; i < gridSize; ++i){
			int tileX = mapX + i;
			if(tileX >= map->getWidth()){
				tileX = map->getWidth() - 1;
			}
			map->getTileLayer().setTile(tileX, y,
					map->getFactory()->createTile(1));
		}
		flag = true;
	}else if(y == mH){
		for(int i = 0; i < gridSize; ++i){
			int tileX = mapX + i;
			if(tileX >= map->getWidth()){
				tileX = map->getWidth() - 1;
			}
			map->getTileLayer().setTile(tileX, y * gridSize - 1,
					map->getFactory()->createTile(0));
		}
		flag = true;
	}
	return flag;
}
void MazeGen::initMaze(){
	dirs = std::vector<Direction>{N, E, S, W};

	mazeGrid.clear();

	mazeGrid.resize(mW);
	for (int i = 0; i < mW; ++i){
		mazeGrid[i].resize(mH);
		for (int j = 0; j < mH; ++j){
			mazeGrid[i][j].resize(dirs.size());
		}
	}

	for(int x = 0; x < mazeGrid.size(); ++x){
		for(int y = 0; y < mazeGrid[x].size(); ++y){
			for(int k = 0; k < mazeGrid[x][y].size(); ++k){
				mazeGrid[x][y][k] = false;
			}
		}
	}

	gridVisited.resize(mW);
	for (int i = 0; i < mW; ++i){
		gridVisited[i].resize(mH);
	}

	for(int x = 0; x < gridVisited.size(); ++x){
		for(int y = 0; y < gridVisited[x].size(); ++y){
			gridVisited[x][y] = false;
		}
	}

	std::random_device rd;
	gen = std::mt19937_64(rd());

	directionDist = std::uniform_int_distribution<>(Direction::N, Direction::W);
}
void MazeGen::recursiveBackTrace(int x, int y){
	std::shuffle(dirs.begin(), dirs.end(), gen);

	for(auto d = dirs.begin(); d != dirs.end(); ++d){
		int nX = x + getDirX(*d);
		int nY = y + getDirY(*d);

		if((nX >= 0 && nX < mazeGrid.size()) &&
				(nY >= 0 && nY < mazeGrid[nX].size())){
			if(gridVisited[nX][nY]) continue;

			mazeGrid[x][y][*d] = true;
			mazeGrid[nX][nY][getOpposite(*d)] = true;
			gridVisited[nX][nY] = true;
			recursiveBackTrace(nX, nY);
		}
	}

}

MazeGen* MazeGen::getInstance(){
	static MazeGen instance;
	return &instance;
}
