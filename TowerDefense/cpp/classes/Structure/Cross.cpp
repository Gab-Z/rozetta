#include "Cross.h"

Cross::Cross():Structure( 3, 3 ){

}
Cross::Cross( int _x, int _y ):Structure( _x, _y, 3, 3 ){

}
std::vector<int> Cross::grid = {
  0,  1,  0,
  1,  1,  1,
  0,  1,  0                                
 };
