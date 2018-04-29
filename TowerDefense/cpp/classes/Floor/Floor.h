#ifndef DEF_Floor
#define DEF_Floor

#include <string>


class Floor{

  int id;
  double speed;
  std::string img_url;
  std::string typeName;

  public:

    Floor();
    Floor( std::string _typeName, std::string _img_url, double _speed );
    Floor( int _id, std::string _typeName, std::string _img_url, double _speed );
    int getId();
    double getSpeed();
    std::string getImg_url();
    std::string getTypeName();
    void setSpeed( double _speed );
    void setImg_url( std::string _img_url );
    void setTypeName( std::string _typeName );

};

#endif
