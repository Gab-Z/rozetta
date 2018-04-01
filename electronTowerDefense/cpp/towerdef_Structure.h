#include <string>
class Structure{

  public:
    Structure( int x, int y );
    int getx();
    int gety();
    virtual std::string getTypeName(){ return "Structure"; };

  private:

    int x;
    int y;

};
