#include "Floor.h"

Floor::Floor(){

}
Floor::Floor( std::string _typeName, std::string _img_url, double _speed, bool _buildable ){
  typeName = _typeName;
  img_url = _img_url;
  speed = _speed;
  buildable = _buildable;
}
Floor::Floor( int _id, std::string _typeName, std::string _img_url, double _speed, bool _buildable ){
  typeName = _typeName;
  img_url = _img_url;
  speed = _speed;
  id = _id;
  buildable = _buildable;
}
int Floor::getId(){
  return id;
}
double Floor::getSpeed(){
  return speed;
}
std::string Floor::getImg_url(){
  return img_url;
}
std::string Floor::getTypeName(){
  return typeName;
}
void Floor::setSpeed( double _speed ){
  speed = _speed;
}
void Floor::setImg_url( std::string _img_url ){
  img_url = _img_url;
}
void Floor::setTypeName( std::string _typeName ){
  typeName = _typeName;
}
bool Floor::isBuildable(){
  return buildable;
}
