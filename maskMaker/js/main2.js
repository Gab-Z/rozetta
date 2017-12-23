var edges = {};
var triangles = [];
var points = [];
var renderList = [];
var triangleIndex = 0;
var compareVec3 = function(pta,ptb){
  if( pta.x == ptb.x && pta.y == ptb.y && pta.z == ptb.z ) return true;
  return false;
}
window.onload = function(){
  var nbPoints = positions.length / 3;
  for( var i0 = 0; i0 < nbPoints; i0++ ){
    var id = i0 * 3;
    points.push( { x: positions[ id ], y: positions[ id + 1 ], z: positions[ id + 2 ] })
  }
  var nbTriangles = indices.length / 3;
  for( var i = 0; i < nbTriangles; i++ ){
    var id = i * 3,
        pta = points[ indices[ id ] ],
        ptb = points[ indices[ id + 1 ] ],
        ptc = points[ indices[ id + 2 ] ],
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
        triangle =  { ptA: pta,
                      ptB: ptb,
                      ptC: ptc,
                      AtoB: AtoB,
                      BtoC: BtoC,
                      CtoA: CtoA,
                      angleA: angleA,
                      angleB: angleB,
                      angleC: angleC,
                      neighbours: {ab:"none",bc:"none",ca:"none"},
                      idx: i
                    };
    triangles.push(  triangle )
  }
  for( var i2 = 0; i2 < nbTriangles; i2++ ){
    var triangle = triangles[ i2 ];
    for( var j = 0; j < nbTriangles; j++ ){
      if( j === i2 ) continue;
      var testedTriangle = triangles[ j ],
          matchA = compareVec3( triangle.ptA, testedTriangle.ptA ) || compareVec3( triangle.ptA, testedTriangle.ptB ) || compareVec3( triangle.ptA, testedTriangle.ptC ) || false,
          matchB = compareVec3( triangle.ptB, testedTriangle.ptA ) || compareVec3( triangle.ptB, testedTriangle.ptB ) || compareVec3( triangle.ptB, testedTriangle.ptC ) || false,
          matchC = compareVec3( triangle.ptC, testedTriangle.ptA ) || compareVec3( triangle.ptC, testedTriangle.ptB ) || compareVec3( triangle.ptC, testedTriangle.ptC ) || false;
      if( matchA && matchB ){
        triangles[ i2 ].neighbours.ab = j;
      }else if( matchB && matchC ){
        triangles[ i2 ].neighbours.bc = j;
      }else if( matchC && matchA ){
        triangles[ i2 ].neighbours.ca = j;
      }

    }
  }
  init();
  /*

  var firstTri = triangles[ 0 ],
      point0 = points[ firstTri.indices[ 0 ] ],
      point1 = points[ firstTri.indices[ 1 ] ],
      point2 = points[ firstTri.indices[ 2 ] ],
      dist0 = distanceVector( point0, point1 ),
      dist1 = distanceVector( point1, point2 ),
      dist2 = distanceVector( point2, point0 ),
      p0 = { x: 0, y: 0 },
      p1 = { x: dist0, y: 0 },
      intersec = intersection(p0.x, p0.y, dist2, p1.x, p1.y, dist1),
      p2 = { x: intersec[ 0 ], y: intersec[ 2 ] };
  point0.p2d = p0;
  point1.p2d = p1;
  point2.p2d = p2;

  renderList = [];
  for( var i = 0; i < nbTriangles; i++ ){
    if(renderList.indexOf( i ) < 0 ) renderList.push( i );
    listTriangles( triangles[ i ] )
  }
  var rl = renderList.length;
  for( var i = 0; i < rl; i++ ){
    var tr = triangles[ renderList[ i ] ],
        tIndices = tr.indices,
        p0 = points[ tIndices[ 0 ] ],
        p1 = points[ tIndices[ 1 ] ],
        p2 = points[ tIndices[ 2 ] ],
        dist0 = distanceVector( p0, p1 ),
        dist1 = distanceVector( p1, p2 ),
        dist2 = distanceVector( p2, p0 ),
        hasp2d0 = p0.hasOwnProperty( "p2d" ),
        hasp2d1 = p1.hasOwnProperty( "p2d" ),
        hasp2d2 = p2.hasOwnProperty( "p2d" );
      if( hasp2d0 && hasp2d1 && hasp2d2 ) continue;
      if( hasp2d0 && hasp2d1 && !hasp2d2){
        var intersec = intersection(p0.x, p0.y, dist2, p1.x, p1.y, dist1)
        p2.p2d = { x: intersec[ 0 ], y: intersec[ 2 ] };
      }else if( hasp2d1 && hasp2d2 && !hasp2d0){
        var intersec = intersection(p1.x, p1.y, dist0, p2.x, p2.y, dist2)
        p0.p2d = { x: intersec[ 0 ], y: intersec[ 2 ] };
      }else if( hasp2d2 && hasp2d0 && !hasp2d1){
        var intersec = intersection(p0.x, p0.y, dist0, p2.x, p2.y, dist1)
        p1.p2d = { x: intersec[ 0 ], y: intersec[ 2 ] };
      }else{
        p0.p2d = {x: 0, y: 0};
        p1.p2d = {x: dist0, y: 0};
        var intersect = intersection(p0.p2d.x, p0.p2d.y, dist2, p1.p2d.x, p1.p2d.x, dist1);
        p2.p2d = {x: intersect[ 0 ], y: intersect[ 2 ] };
      }
  }
  var canvas = document.body.appendChild( document.createElement( "canvas" ) ),
      ctx = canvas.getContext( "2d" );
  canvas.width = window.innerWidth;
  canvas.height = window.innerHeight;
  ctx.translate(canvas.width/2, canvas.height / 2);
  for( var i = 0; i < rl; i++ ){
    var tria = triangles[ i ],
        indcs = tria.indices,
        p0 = points[ indcs[ 0 ] ].p2d,
        p1 = points[ indcs[ 1 ] ].p2d,
        p2 = points[ indcs[ 2 ] ].p2d;
    ctx.beginPath();
    ctx.moveTo(p0.x,p0.y);
    ctx.lineTo(p1.x,p1.y);
    ctx.lineTo(p2.x,p2.y);
    ctx.closePath();
    ctx.stroke();
  }
  alert(renderList.length+", "+nbTriangles)
  */
}
var init = function(){
  var canvas = document.body.appendChild( document.createElement( "canvas" ) ),
      ctx = canvas.getContext( "2d" ),
      toolBox = document.body.appendChild( document.createElement( "div" ) ),
      prev = toolBox.appendChild( document.createElement( "button" ) ),
      next = toolBox.appendChild( document.createElement( "button" ) ),
      valid = toolBox.appendChild( document.createElement( "button" ) );
  prev.textContent = "<";
  next.textContent = ">"
  valid.textContent = "ok";
  toolBox.id = "toolBox";
  canvas.id = "canvas";
  canvas.width = window.innerWidth;
  canvas.height = window.innerHeight;
  prev.addEventListener( "click", clicPrev, false );
  next.addEventListener( "click", clicNext, false );
  valid.addEventListener( "click", clicValid, false );

}
var listTriangles = function( triangle ){
  var neighb = triangle.neighbours;
  for(var n=0; n<3;n++){
    var ngbIdx = neighb[ n ];
    if( ngbIdx === "" ) continue;
    if(renderList.indexOf( ngbIdx ) < 0 ){
      renderList.push( ngbIdx );
      listTriangles( triangles[ ngbIdx ] )
    }
  }
}
var clicPrev = function(e){
  triangleIndex += -1;
  if( triangleIndex < 0 ) triangleIndex = triangles.length - 1;
  update();
};
var clicNext = function(e){
  triangleIndex++;
  if( triangleIndex > triangles.length - 1 ) triangleIndex = 0;
  update();
};
var clicValid = function(e){
  drawAll();
};
var drawAll = function(){
  renderList = [];
  var canvas = document.getElementById( "canvas" ),
      ctx = canvas.getContext( "2d" );
  ctx.restore();
  ctx.save();
  ctx.clearRect( 0,0, canvas.width, canvas.height );
  ctx.translate( canvas.width / 2, canvas.height / 2 );
  var l = triangles.length;

  for( var i = 0; i < 1; i++ ){
  prepareForRender( triangles[ i ] );
  }
  for( var i = 0; i < l; i++ ){
    draw( triangles[ i ] );
  }
};
var draw = function( triangle ){
  var canvas = document.getElementById( "canvas" ),
      ctx = canvas.getContext( "2d" ),
      a2d = triangle.a2d,
      b2d = triangle.b2d,
      c2d = triangle.c2d;

  ctx.beginPath();
  ctx.moveTo( a2d.x, a2d.y );
  ctx.lineTo( b2d.x, b2d.y );
  ctx.lineTo( c2d.x, c2d.y );
  ctx.closePath();
  ctx.stroke();
}

var prepareForRender = function( triangle ){
  if( renderList.indexOf( triangle.idx ) > -1 ) return false;
  var a2d,
      b2d,
      c2d;
  if( !triangle.a2d && triangle.b2d && triangle.c2d ){
    b2d = triangle.b2d;
    c2d = triangle.c2d;
    var intersecToC = intersection( c2d.x, c2d.y, triangle.CtoA, b2d.x, b2d.y, triangle.AtoB );
    a2d = { x: intersecToC[ 0 ], y: intersecToC[ 2 ], xprime: intersecToC[ 1 ], yprime: intersecToC[ 3 ] };
  }else if( triangle.a2d && !triangle.b2d && triangle.c2d ){
    a2d = triangle.a2d;
    c2d = triangle.c2d;
    var intersecToC = intersection( a2d.x, a2d.y, triangle.AtoB, c2d.x, c2d.y, triangle.AtoB );
    b2d = { x: intersecToC[ 0 ], y: intersecToC[ 2 ], xprime: intersecToC[ 1 ], yprime: intersecToC[ 3 ] };
  }else if( triangle.a2d && triangle.b2d && !triangle.c2d ){
    a2d = triangle.a2d;
    b2d = triangle.b2d;
    var intersecToC = intersection( a2d.x, a2d.y, triangle.CtoA, b2d.x, b2d.y, triangle.BtoC );
    c2d = { x: intersecToC[ 0 ], y: intersecToC[ 2 ], xprime: intersecToC[ 1 ], yprime: intersecToC[ 3 ] };
  }else if( triangle.a2d && triangle.b2d && triangle.c2d ){
    renderList.push( triangle );
    return false;
  }else{
    a2d = { x: 0,y: 0 };
    b2d = { x: 0, y: triangle.AtoB };
    var intersecToC = intersection( a2d.x, a2d.y, triangle.CtoA, b2d.x, b2d.y, triangle.BtoC );
    c2d = { x: intersecToC[ 0 ], y: intersecToC[ 2 ], xprime: intersecToC[ 1 ], yprime: intersecToC[ 3 ] };
  }
  triangle.a2d = a2d;
  triangle.b2d = b2d;
  triangle.c2d = c2d;
  renderList.push( triangle );
  for( var k in triangle.neighbours ){
    var neighbourIdx = triangle.neighbours[ k ];
    if( neighbourIdx == "none" ) continue;
    var neighbour = triangles[ neighbourIdx ],
        thirdPoint = false,
        pts2d = k == "ab" ? {a2d:a2d,b2d:b2d,c2d:c2d} : k == "bc" ? {a2d:b2d,b2d:c2d,c2d:a2d} : {a2d:c2d,b2d:a2d,c2d:b2d};
    for( var l in neighbour.neighbours ){
      var neighbourIdx2 = neighbour.neighbours[ l ];
      if( neighbourIdx2 == "none" ) continue;
      var searchedNeighbour = triangles[ neighbourIdx2 ];
      if( searchedNeighbour == triangle ){
        var search;
        if( l == "ab" ){
          search = intersection(pts2d.a2d.x, pts2d.a2d.y, neighbour.CtoA, pts2d.b2d.x, pts2d.b2d.y, neighbour.BtoC );
          triangles[ neighbourIdx2 ].c2d = { x: search[ 0 ], xprime:search[ 1 ], y: search[ 2 ], yprime: search[ 3  ] };
        }else if( l == "bc" ){
          search = intersection(pts2d.a2d.x, pts2d.a2d.y, neighbour.AtoB, pts2d.b2d.x, pts2d.b2d.y, neighbour.CtoA );
          triangles[ neighbourIdx2 ].a2d = { x: search[ 0 ], xprime:search[ 1 ], y: search[ 2 ], yprime: search[ 3  ] };
        }else if( l == "ca" ){
          search = intersection(pts2d.a2d.x, pts2d.a2d.y, neighbour.BtoC, pts2d.b2d.x, pts2d.b2d.y, neighbour.AtoB );
          triangles[ neighbourIdx2 ].b2d = { x: search[ 0 ], xprime:search[ 1 ], y: search[ 2 ], yprime: search[ 3  ] };
        }
        thirdPoint = { x: search[ 0 ], xprime:search[ 1 ], y: search[ 2 ], yprime: search[ 3  ] }
        break;
      }

    }

      prepareForRender( neighbour );
  }
}
var update = function(){
  var canvas = document.getElementById( "canvas" ),
      ctx = canvas.getContext( "2d" );

  var triangle = triangles[ triangleIndex ],
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
  for( var k in triangle.neighbours ){
    var neighbourIdx = triangle.neighbours[ k ];
    if( neighbourIdx == "none" ) continue;
    var neighbour = triangles[ neighbourIdx ],
        thirdPoint = false,
        pts2d = k == "ab" ? {a2d:a2d,b2d:b2d,c2d:c2d} : k == "bc" ? {a2d:b2d,b2d:c2d,c2d:a2d} : {a2d:c2d,b2d:a2d,c2d:b2d};
    for( var l in neighbour.neighbours ){
      var neighbourIdx2 = neighbour.neighbours[ l ];
      if( neighbourIdx2 == "none" ) continue;
      var searchedNeighbour = triangles[ neighbourIdx2 ];
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

    ctx.beginPath();
    ctx.moveTo( pts2d.a2d.x, pts2d.a2d.y );
    ctx.lineTo( pts2d.b2d.x, pts2d.b2d.y );
    ctx.lineTo( thirdPoint.xprime, thirdPoint.yprime );
    ctx.closePath();
    ctx.stroke();
  //  break;
  }

}
var getTri = function ( i ) {
  var p = positions,
      idx = i * 3;
  return { x: p[ indices[ idx ] ], y: p[ indices[ idx + 1 ] ], z: p[ indices[ idx + 2] ] }
};
function distanceVector( v1, v2 ){
    var dx = v1.x - v2.x;
    var dy = v1.y - v2.y;
    var dz = v1.z - v2.z;

    return Math.sqrt( dx * dx + dy * dy + dz * dz );
};
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
