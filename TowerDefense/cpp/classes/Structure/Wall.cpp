#include "Wall.h"

Wall::Wall():Structure( 1, 1 ){

}
Wall::Wall( int _x, int _y ):Structure( _x, _y, 1, 1 ){

}
std::vector<int> Wall::grid = { 1 };

std::string Wall::img_url = "wall.jpg";
