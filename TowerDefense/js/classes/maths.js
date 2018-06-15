const square = function(x) {
  return x * x;
};

const Absolute = function(a) {
  if (a < 0) {
    a = -a;
  }
  return a;
};

const EuclideanDistance = function(startcorner, goalcorner) {
  return Math.sqrt((square(startcorner.x - goalcorner.x)) + (square(startcorner.y - goalcorner.y)));
};

const ManhattanDistance = function(startCorner, goalCorner) {
  return Absolute(goalCorner.y - startCorner.y) + Absolute(goalCorner.x - startCorner.x);
};

const OctileDistance = function(startCorner, goalCorner) {
  var dx, dy;
  dx = Absolute(startCorner.x - goalCorner.x);
  dy = Absolute(startCorner.y - goalCorner.y);
  return dx + dy + (1 - Math.sqrt(2)) * (Math.min(dx, dy));
};

const CornerAtDiagonalIntersection = function(x, y) {
  if ($scope.cornersMap[y][x][0].cornerType == kCornerNormal)
    return false;
  return true;
};
/*
const IsTraversable = function(x, y) {
  if (myGrid[y][x].isObstacle) {
    return false;
  }
  return true;
};
*/
/*
//Line of Sight
const LineOfSight = function(startCorner, goalCorner) {
  var dx, dy, f, sx, sy, x1, x2, x_offset, y1, y2, y_offset;
  x1 = startCorner.x;
  x2 = goalCorner.x;
  y1 = startCorner.y;
  y2 = goalCorner.y;

  dy = y2 - y1;
  dx = x2 - x1;
  f = 0;
  sy = 0;
  sx = 0;
  if(dy < 0){
    dy = -dy;
    sy = -1;
  }
  else{
    sy = 1
  }

  if(dx < 0){
    dx = -dx;
    sx = -1;
  }
  else{
    sx = 1;
  }

  if(dx >= dy){
    while(x1 != x2){
      f = f + dy;
      if(f >= dx){
        if(!IsTraversable(x1 + ((sx - 1)/2), y1 + (sy - 1)/2)){
          return false;
        }

        y1 = y1 + sy;
        f = f - dx;
      }
      if(f != 0 && !IsTraversable(x1 + ((sx - 1)/2), y1 + (sy - 1)/2)){
        return false;
      }
      if(dy == 0 && !IsTraversable(x1 + ((sx - 1)/2), y1)  && !IsTraversable(x1 + ((sx - 1)/2), y1 - 1)){
        return false;
      }
      x1 = x1 + sx;
    }
  }
  else{
    while(y1 != y2){
      f = f + dx;
      if(f >= dy){
         if(!IsTraversable(x1 + ((sx - 1)/2), y1 + (sy - 1)/2)){
          return false;
        }
        x1 = x1 + sx;
        f = f - dy;
      }
      if(f != 0 && !IsTraversable(x1 + ((sx - 1)/2), y1 + (sy - 1)/2)){
        return false;
      }
      if(dy == 0 && !IsTraversable(x1, y1 + ((sy - 1)/2))  && !IsTraversable(x1 - 1, y1 + ((sy - 1)/2))){
        return false;
      }
      y1 = y1 +sy;
    }
  }
  return true;
};
*/
