"use strict"
var points,
    triangles,
    triangleSelectionIndex = 0;
window.onload = function(){
  testIntersection()
//  init( positions, indices );
};
var init = function( _positions, _indices){
  points = positionsToPoints( _positions );
  triangles = indicesToTriangles( _indices, points );
  selectFirstTriangle.start();
};
var selectFirstTriangle = {
  start: function(){
    toolBox.navSelect();
    triangleSelectionIndex = 0;
    var prev = document.getElementById( "prev" ),
        next = document.getElementById( "next" ),
        valid = document.getElementById( "valid" ),
        canvas = document.body.appendChild( document.createElement( "canvas" ) );
    prev.addEventListener( "click", selectFirstTriangle.move, false );
    next.addEventListener( "click", selectFirstTriangle.move, false );
    valid.addEventListener( "click", selectFirstTriangle.valid, false );
    //document.body.innerHTML = "";
    canvas.id = "canvas";
    canvas.width = window.innerWidth;
    canvas.height = window.innerHeight;
    selectFirstTriangle.draw();
  },
  move: function( e ){
    var id = e.currentTarget.id;
    if( id == "prev" ){
      triangleSelectionIndex += (0-1);
    }else if( id == "next" ){
      triangleSelectionIndex ++;
    }
    if( triangleSelectionIndex < 0 ){
      triangleSelectionIndex = triangles.triangles.length - 1;
    }else if( triangleSelectionIndex > triangles.length - 1 ){
      triangleSelectionIndex = 0;
    }
    selectFirstTriangle.draw();
  },
  draw: function(){
    var canvas = document.getElementById( "canvas" ),
        ctx = canvas.getContext( "2d" );
        var triangle = triangles.triangles[ triangleSelectionIndex ],
            a2d = { x: 0,y: 0 },
            b2d = { x: 0, y: triangle.AtoB },
            intersecToC = intersection( a2d.x, a2d.y, triangle.CtoA, b2d.x, b2d.y, triangle.BtoC ),
            c2d = { x: intersecToC[ 0 ], y: intersecToC[ 2 ], xprime: intersecToC[ 1 ], yprime: intersecToC[ 3 ] };
        ctx.restore();
        ctx.save();
        ctx.clearRect( 0,0, canvas.width, canvas.height );
        ctx.translate( canvas.width / 2, canvas.height / 2 );
        ctx.beginPath();
        ctx.moveTo( a2d.x, a2d.y );
        ctx.lineTo( b2d.x, b2d.y );
        ctx.lineTo( c2d.x, c2d.y );
        ctx.closePath();
        ctx.stroke();
          //alert(JSON.stringify(triangle.neighbours))
        for( var k in triangle.neighbours ){
          var neighbourIdx = triangle.neighbours[ k ];
          if( neighbourIdx == "none" ) continue;
          var neighbour = triangles.triangles[ neighbourIdx.id ],
              pts2d = getNeighbour2dPos( triangle, neighbourIdx, { a:a2d, b: b2d, c: c2d } ),
              a =  pts2d.a.xprime ? {x:pts2d.a.xprime,y:pts2d.a.yprime} : pts2d.a,
              b =  pts2d.b.xprime ? {x:pts2d.b.xprime,y:pts2d.b.yprime} : pts2d.b,
              c =  pts2d.c.xprime ? {x:pts2d.c.xprime,y:pts2d.c.yprime} : pts2d.c;
          ctx.strokeStyle = "#b87d3e";
          ctx.beginPath();
          ctx.moveTo( a.x, a.y );
          ctx.lineTo( b.x, b.y );
          ctx.lineTo( c.x, c.y );
          ctx.closePath();
          ctx.stroke();
          ctx.strokeStyle = "#000000";

        }


/*
        for( var k in triangle.neighbours ){
          var neighbourIdx = triangle.neighbours[ k ];
          if( neighbourIdx == "none" ) continue;
          var neighbour = triangles.triangles[ neighbourIdx.id ],
              thirdPoint = false,
              pts2d = k == "ab" ? {a2d:a2d,b2d:b2d,c2d:c2d} : k == "bc" ? {a2d:b2d,b2d:c2d,c2d:a2d} : {a2d:c2d,b2d:a2d,c2d:b2d};
          for( var l in neighbour.neighbours ){
            var neighbourIdx2 = neighbour.neighbours[ l ];
            if( neighbourIdx2 == "none" ) continue;
            var searchedNeighbour = triangles.triangles[ neighbourIdx2.id ];
            if( searchedNeighbour == triangle ){
              var search;
              if( l == "ab" ){
                search = intersection(pts2d.a2d.x, pts2d.a2d.y, neighbour.CtoA, pts2d.b2d.x, pts2d.b2d.y, neighbour.BtoC );
              }else if( l == "bc" ){
                search = intersection(pts2d.a2d.x, pts2d.a2d.y, neighbour.AtoB, pts2d.b2d.x, pts2d.b2d.y, neighbour.CtoA );
              }else if( l == "ca" ){
                search = intersection(pts2d.a2d.x, pts2d.a2d.y, neighbour.BtoC, pts2d.b2d.x, pts2d.b2d.y, neighbour.AtoB );
              }
              thirdPoint = { x: search[ 0 ], xprime:search[ 1 ], y: search[ 2 ], yprime: search[ 3  ] }
              break;
            }
          }
          ctx.strokeStyle = "#b87d3e";
          ctx.beginPath();
          ctx.moveTo( pts2d.a2d.x, pts2d.a2d.y );
          ctx.lineTo( pts2d.b2d.x, pts2d.b2d.y );
          ctx.lineTo( thirdPoint.xprime, thirdPoint.yprime );
          ctx.closePath();
          ctx.stroke();
          ctx.strokeStyle = "#000000";
        //  break;
        }
*/

  },
  valid: function( e ){
    var triangle = triangles.triangles[ triangleSelectionIndex ],
        a2d = { x: 0,y: 0 },
        b2d = { x: 0, y: triangle.AtoB },
        intersecToC = intersection( a2d.x, a2d.y, triangle.CtoA, b2d.x, b2d.y, triangle.BtoC ),
        c2d = { x: intersecToC[ 0 ], y: intersecToC[ 2 ], xprime: intersecToC[ 1 ], yprime: intersecToC[ 3 ] };
    triangle.pts2d.a = a2d;
    triangle.pts2d.b = b2d;
    triangle.pts2d.c = c2d;
    setTriangleNeighbours( triangle );
  }
};
function setNeighbours2dPos( triangle ){
  for( var k in triangle.neighbours ){
    var neighbourIdx = triangle.neighbours[ k ];
    if( neighbourIdx == "none" ) continue;
    var neighbour = triangles.triangles[ neighbourIdx.id ],
        matchs = neighbourIdx.matchs;
    neighbour.pts2d = getNeighbour2dPos( triangle, neighbourIdx );
  }
}
function getNeighbour2dPos( triangle, neighbourIdx, _pts2d ){
  var matchs = neighbourIdx.matchs,
      neighbour = triangles.triangles[ neighbourIdx.id ],
      pts2d = {},
      srcPts2d = _pts2d || triangle.pts2d;
  for( var l in neighbour.neighbours ){
    var subNeighbour = neighbour.neighbours[ l ];
    if( subNeighbour == "none" ) continue;
    var searchedNeighbour = triangles.triangles[ subNeighbour.id ];
    if( searchedNeighbour.idx == triangle.idx ){
      var matchNames = [];
      for( var n in matchs ){
        matchNames.push( matchs[ n ] );
      };
      var missingPtName;
      if( matchNames.indexOf( "a" ) < 0 ){
        missingPtName = "a";
      }else if( matchNames.indexOf( "b" ) < 0 ){
        missingPtName = "b";
      }else if( matchNames.indexOf( "c" ) < 0 ){
        missingPtName = "c";
      }
      //alert(JSON.stringify(matchs)+"\nmiss :  "+missingPtName+"\n"+matchNames)
    if( matchs.a && matchs.b ){
      pts2d[ matchs.a ] = srcPts2d.a;
      pts2d[ matchs.b ] = srcPts2d.b;
      var intersec = intersection(  srcPts2d.a.x, srcPts2d.a.y,
                                    neighbour[ ptsNamesToSizeName( matchs.a, missingPtName ) ],
                                    srcPts2d.b.x, srcPts2d.b.y,
                                    neighbour[ ptsNamesToSizeName( matchs.b, missingPtName ) ] ),

      xcenter = srcPts2d.a.x - ( (srcPts2d.a.x - srcPts2d.b.x) / 2 ),
      xprime =  xcenter + ( ( xcenter - intersec[0]) / 2 ),
      ycenter = srcPts2d.a.y - ( (srcPts2d.a.y - srcPts2d.b.y) / 2 ),
      yprime = ycenter + ( ( ycenter - intersec[2]) / 2 );
    pts2d[ missingPtName ] = { x: intersec[ 0 ], xprime: xprime, y: intersec[ 2 ], yprime: yprime };
//alert(JSON.stringify(pts2d));
      return pts2d;
    }else if( matchs.b && matchs.c ){
      pts2d[ matchs.b ] = srcPts2d.b;
      pts2d[ matchs.c ] = srcPts2d.c;
      var intersec = intersection(  srcPts2d.b.x, srcPts2d.b.y,
                                neighbour[ ptsNamesToSizeName( matchs.b, missingPtName ) ],
                                srcPts2d.c.x, srcPts2d.c.y,
                                neighbour[ ptsNamesToSizeName( matchs.c, missingPtName ) ] );

          xcenter = srcPts2d.b.x - ( (srcPts2d.b.x - srcPts2d.c.x) ),
          xprime =  xcenter + ( ( xcenter - intersec[0]) / 2 ),
          ycenter = srcPts2d.b.y - ( (srcPts2d.b.y - srcPts2d.c.y) ),
          yprime = ycenter + ( ( ycenter - intersec[2]) / 2 ),
          pts2d[ missingPtName ] = { x: intersec[ 0 ], xprime: xprime, y: intersec[ 2 ], yprime: yprime };
//alert(JSON.stringify(pts2d));
      return pts2d;
    }else if( matchs.c && matchs.a ){
      pts2d[ matchs.c ] = srcPts2d.c;
      pts2d[ matchs.a ] = srcPts2d.a;
      var intersec = intersection(  srcPts2d.c.x, srcPts2d.c.y,
                                    neighbour[ ptsNamesToSizeName( matchs.c, missingPtName ) ],
                                    srcPts2d.a.x, srcPts2d.a.y,
                                    neighbour[ ptsNamesToSizeName( matchs.a, missingPtName ) ] );
          xcenter = srcPts2d.c.x - ( (srcPts2d.c.x - srcPts2d.a.x) ),
          xprime =  xcenter + ( ( xcenter - intersec[0]) / 2 ),
          ycenter = srcPts2d.c.y - ( (srcPts2d.c.y - srcPts2d.a.y) ),
          yprime = ycenter + ( ( ycenter - intersec[2]) / 2 ),
          pts2d[ missingPtName ] = { x: intersec[ 0 ], xprime: xprime, y: intersec[ 2 ], yprime: yprime };
  //    alert(JSON.stringify(pts2d));
      return pts2d;
    }
  }
  }
}
var toolBox = {

  create: function(){
    var toolB = document.body.appendChild( document.createElement( "div" ) );
    toolB.id = "toolBox";
  },

  navSelect: function(){
    toolBox.empty();
    var toolB = document.getElementById( "toolBox" ),
        prev = toolB.appendChild( document.createElement( "button" ) ),
        next = toolB.appendChild( document.createElement( "button" ) ),
        valid = toolB.appendChild( document.createElement( "button" ) );
    prev.textContent = "<";
    prev.id = "prev";
    next.textContent = ">";
    next.id = "next";
    valid.textContent = "ok";
    valid.id = "valid";
  },

  empty: function(){
    toolBox.check();
    document.getElementById( "toolBox" ).innerHTML = "";
  },

  check: function(){
    if(! document.getElementById( "toolBox" ) )toolBox.create();
  }

}
var positionsToPoints = function( _positions ){
  var nbPoints = _positions.length / 3,
      ret = [];
  for( var i = 0; i < nbPoints; i++ ){
    var id = i * 3;
    ret.push( { x: _positions[ id ], y: _positions[ id + 1 ], z: _positions[ id + 2 ] })
  }
  return ret;
}
var indicesToTriangles = function( _indices, _points ){
  var nbTriangles = _indices.length / 3,
      ret = [];
  for( var i = 0; i < nbTriangles; i++ ){
    var id = i * 3,
        pta = _points[ _indices[ id ] ],
        ptb = _points[ _indices[ id + 1 ] ],
        ptc = _points[ _indices[ id + 2 ] ],
        AtoB = distanceVector( pta, ptb ),
        BtoC = distanceVector( ptb, ptc ),
        CtoA = distanceVector( ptc, pta ),
        a2d = {x:0,y:0},
        b2d = {x:0,y:AtoB},
        intersecToC = intersection( a2d.x, a2d.y, CtoA, b2d.x, b2d.y, BtoC ),
        c2d = {x:intersecToC[ 0 ],y: intersecToC[ 2 ], xprime: intersecToC[ 1 ], yprime: intersecToC[ 3 ]},
        angleA = getAngleBetweenTwoTouchingLineSegments(a2d, b2d, c2d),
        angleB = getAngleBetweenTwoTouchingLineSegments(b2d, c2d, a2d),
        angleC = getAngleBetweenTwoTouchingLineSegments(c2d, a2d, b2d),
        triangle =  { pts3d: {
                        a: pta,
                        b: ptb,
                        c: ptc
                      },
                      AtoB: AtoB,
                      BtoC: BtoC,
                      CtoA: CtoA,
                      angleA: angleA,
                      angleB: angleB,
                      angleC: angleC,
                      neighbours: {ab:"none",bc:"none",ca:"none"},
                      idx: i,
                      pts2d:{}
                    };
    ret.push( triangle )
  };
  for( var i2 = 0; i2 < nbTriangles; i2++ ){
    var triangle = ret[ i2 ];
    for( var j = 0; j < nbTriangles; j++ ){
      if( j === i2 ) continue;
      var testedTriangle = ret[ j ],
          matchs = {};
      if( compareVec3( triangle.pts3d.a, testedTriangle.pts3d.a ) ){
        matchs.a = "a";
      }else if( compareVec3( triangle.pts3d.a, testedTriangle.pts3d.b ) ){
        matchs.a = "b";
      }else if( compareVec3( triangle.pts3d.a, testedTriangle.pts3d.c ) ){
        matchs.a = "c";
      }
      if( compareVec3( triangle.pts3d.b, testedTriangle.pts3d.a ) ){
        matchs.b = "a";
      }else if( compareVec3( triangle.pts3d.b, testedTriangle.pts3d.b ) ){
        matchs.b = "b";
      }else if( compareVec3( triangle.pts3d.b, testedTriangle.pts3d.c ) ){
        matchs.b = "c";
      };
      if( compareVec3( triangle.pts3d.c, testedTriangle.pts3d.a ) ){
        matchs.c = "a";
      }else if( compareVec3( triangle.pts3d.c, testedTriangle.pts3d.b ) ){
        matchs.c = "b";
      }else if( compareVec3( triangle.pts3d.c, testedTriangle.pts3d.c ) ){
        matchs.c = "c";
      }
      if( matchs.a && matchs.b ){
        ret[ i2 ].neighbours.ab = { id: j, matchs: matchs };
      }else if( matchs.b && matchs.c ){
        ret[ i2 ].neighbours.bc = { id: j, matchs: matchs };
      }else if( matchs.c && matchs.a ){
        ret[ i2 ].neighbours.ca = { id: j, matchs: matchs };
      }
    }
  }
  //alert(JSON.stringify(ret))
  return {  indices: _indices,
            triangles: ret };
};
var edgesPtsNames = {
    ab : [ "a", "b" ],
    bc : [ "b", "c" ],
    ca : [ "c", "a" ]
};
function ptsNamesToSizeName( a, b ){
  var names = edgesPtsNames;
  if( names.ab.indexOf( a ) > -1 && names.ab.indexOf( b ) > -1 ){
    return "AtoB";
  }else if( names.bc.indexOf( a ) > -1 && names.bc.indexOf( b ) > -1 ){
    return "BtoC";
  }else if( names.ca.indexOf( a ) > -1 && names.ca.indexOf( b ) > -1 ){
    return "CtoA";
  }
};
function distanceVector( v1, v2 ){
    var dx = v1.x - v2.x;
    var dy = v1.y - v2.y;
    var dz = v1.z - v2.z;

    return Math.sqrt( dx * dx + dy * dy + dz * dz );
};
function testIntersection(){
  var canvas = document.body.appendChild( document.createElement( "canvas" ) ),
      ctx = canvas.getContext( "2d" ),
      p1 = {x:0,y:0},
      p2 = {x:150,y:200},
      l1 = 150,
      l2 = 180,
      intersec = intersection(p1.x, p1.y, l1, p2.x, p2.y, l2),
      p3 = {x: intersec[ 0 ], xprime: intersec[ 1 ], y: intersec[ 2 ], yprime: intersec[ 3 ] };
    canvas.width = window.innerWidth;
    canvas.height = window.innerHeight;
    ctx.restore();
    ctx.save();
    ctx.clearRect( 0,0, canvas.width, canvas.height );
    ctx.translate( canvas.width / 2, canvas.height / 2 );
    ctx.beginPath();
    ctx.lineWidth = 2;
    ctx.strokeStyle = "#000000";
    ctx.moveTo( p1.x, p1.y );
    ctx.lineTo( p2.x, p2.y );
    ctx.stroke();

    ctx.beginPath();
    ctx.strokeStyle = "#ab2323";
    ctx.moveTo( p1.x, p1.y );
    ctx.lineTo( p3.x, p3.y );
    ctx.lineTo( p2.x, p2.y );
    ctx.stroke();

    ctx.beginPath();
    ctx.strokeStyle = "#88bb1c";
    ctx.moveTo( p1.x, p1.y );
    ctx.lineTo( p3.xprime, p3.yprime );
    ctx.lineTo( p2.x, p2.y );
    ctx.stroke();


}
function intersection(x0, y0, r0, x1, y1, r1) {
        var a, dx, dy, d, h, rx, ry;
        var x2, y2;

        /* dx and dy are the vertical and horizontal distances between
         * the circle centers.
         */
        dx = x1 - x0;
        dy = y1 - y0;

        /* Determine the straight-line distance between the centers. */
        d = Math.sqrt((dy*dy) + (dx*dx));

        /* Check for solvability. */
        if (d > (r0 + r1)) {
            /* no solution. circles do not intersect. */
            return false;
        }
        if (d < Math.abs(r0 - r1)) {
            /* no solution. one circle is contained in the other */
            return false;
        }

        /* 'point 2' is the point where the line through the circle
         * intersection points crosses the line between the circle
         * centers.
         */

        /* Determine the distance from point 0 to point 2. */
        a = ((r0*r0) - (r1*r1) + (d*d)) / (2.0 * d) ;

        /* Determine the coordinates of point 2. */
        x2 = x0 + (dx * a/d);
        y2 = y0 + (dy * a/d);

        /* Determine the distance from point 2 to either of the
         * intersection points.
         */
        h = Math.sqrt((r0*r0) - (a*a));

        /* Now determine the offsets of the intersection points from
         * point 2.
         */
        rx = -dy * (h/d);
        ry = dx * (h/d);

        /* Determine the absolute intersection points. */
        var xi = x2 + rx;
        var xi_prime = x2 - rx;
        var yi = y2 + ry;
        var yi_prime = y2 - ry;

        return [xi, xi_prime, yi, yi_prime];
    }
    function pythagSquared(point1, point2) {
    	return Math.pow(point1.x - point2.x, 2) + Math.pow(point1.y - point2.y, 2);
    }

    function pythag(point1, point2) {
    	return Math.sqrt(pythagSquared(point1, point2));
    }

    function getAngleBetweenTwoTouchingLineSegments(touchPoint, line1EndPoint, line2EndPoint) {
    	var line2EndPointVector = {x: line2EndPoint.x - touchPoint.x, y: line2EndPoint.y - touchPoint.y},
    		line1EndPointVector = {x: line1EndPoint.x - touchPoint.x, y: line1EndPoint.y - touchPoint.y},
    		aSq = pythagSquared(line1EndPoint, line2EndPoint),
    		b = pythag(line2EndPoint, touchPoint),
    		c = pythag(line1EndPoint, touchPoint),
    		crossProduct = line2EndPointVector.x * line1EndPointVector.y - line1EndPointVector.x * line2EndPointVector.y;

    	return (crossProduct < 0 ? -1 : 1) * Math.acos((b * b + c * c  - aSq) / (2 * b * c)); // cosine rule
    }
var compareVec3 = function(pta,ptb){
  if( pta.x == ptb.x && pta.y == ptb.y && pta.z == ptb.z ) return true;
  return false;
}
