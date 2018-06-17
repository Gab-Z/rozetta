#ifndef DEF_DestinationPt
#define DEF_DestinationPt

#include <vector>

class DestinationPt {
  public:
    int pos1d;
    std::vector<int> pathPoints;
    DestinationPt();
    DestinationPt( int _pos1d );
    DestinationPt( int _pos1d, int _l );
    void init( int _l );
    void setPointTarget( int _pos, int _targetPos );
    int getPointTarget( int _pos );

};

#endif
