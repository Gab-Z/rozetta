#include "Floor.h"

Floor::Floor(){
  Floor_id_counter++;
  id = Floor_id_counter;
}
Floor::Floor( std::string _typeName, std::string _img_url, double _speed ){
  typeName = _typeName;
  img_url = _img_url;
  speed = _speed;
  Floor_id_counter++;
  id = Floor_id_counter;
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
