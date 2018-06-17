#ifndef DEF_Floor
#define DEF_Floor

#include <string>


class Floor{

  int id = 0;
  double speed = 0.0;
  std::string img_url = "";
  std::string typeName = "";
  bool buildable = false;

  public:

    Floor();
    Floor( std::string _typeName, std::string _img_url, double _speed, bool _buildable );
    Floor( int _id, std::string _typeName, std::string _img_url, double _speed, bool _buildable );
    int getId();
    double getSpeed();
    std::string getImg_url();
    std::string getTypeName();
    void setSpeed( double _speed );
    void setImg_url( std::string _img_url );
    void setTypeName( std::string _typeName );
    bool isBuildable();

};

#endif
