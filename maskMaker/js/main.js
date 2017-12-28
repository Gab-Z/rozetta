"use strict"
var points,
    triangles,
    meshParts =[],
    triangleSelectionIndex = 0;
window.onload = function(){
  init( positions, indices );
  start3dPreview( positions, indices );
};
var init = function( _positions, _indices){
  points = positionsToPoints( _positions );
  triangles = indicesToTriangles( _indices, points );

  var tl = triangles.triangles.length;
  for( var tr = 0; tr < tl; tr++ ){
    trianglesToParts( triangles.triangles[ tr ], triangles.triangles, meshParts )
  }

  alert(JSON.stringify(meshParts))
  var firstTri = triangles.triangles[ 0 ],
      intersct = intersection(0, 0, firstTri.CtoA, 0, firstTri.AtoB, firstTri.BtoC)
  firstTri.pts2d = {
    a: { x: 0, y: 0 },
    b: { x: 0, y: firstTri.AtoB },
    c: { x: intersct[ 0 ], y: intersct[ 2 ] }
  };
  var triList = [ firstTri.idx ],
      nbTri = triangles.triangles.length;
  for( var i = 0; i < nbTri; i++ ){
    if( i > triList.length - 1) break;
    triList = triList.concat( setNeighboursPos( triangles.triangles[ triList[ i ] ] ) ) ;
  }
  var edges = new Map();
  for( var t = 0; t < triList.length; t++ ){
    var triangle = triangles.triangles[ triList[ t ] ],
        p = triangle.pts2d,
        ab = getSmallerPoint( p.a, p.b ),
        bc = getSmallerPoint( p.b, p.c ),
        ca = getSmallerPoint( p.c, p.a );
    edges.set( ab, true );
    edges.set( bc, true );
    edges.set( ca, true );
  }
  var canvas = document.body.appendChild( document.createElement( "canvas" ) ),
      ctx = canvas.getContext("2d");
  canvas.id = "canvas";
  canvas.width = window.innerWidth;
  canvas.height = window.innerHeight;
  ctx.restore();
  ctx.save();
  ctx.clearRect( 0,0, canvas.width, canvas.height );
  ctx.translate( canvas.width / 2, canvas.height / 2 );
  edges.forEach(( val, pts, map ) => {
    ctx.beginPath();
    ctx.moveTo( pts[0].x, pts[0].y );
    ctx.lineTo( pts[1].x, pts[1].y );
    ctx.stroke();
  })

};
function getSmallerPoint(pt1,pt2){
  if( pt1.x < pt2.x || ( pt1.x == pt2.x && pt1.y <= pt2.y ) ){
    return [ pt1, pt2 ]
  }else {
    return [ pt2, pt1 ]
  }
}
function setNeighboursPos( triangle ){
  var neighbours = triangle.neighbours,
      ret = [];
  for( var k in neighbours ){
    if( neighbours [ k ] == "none" ) continue;
    var mainEdgeName = k,
        neighbour = triangles.triangles[ neighbours[ k ].id ],
        matchs = neighbours[ k ].matchs;
    if( neighbour.pts2d.a && neighbour.pts2d.b && neighbour.pts2d.c ) continue;
    var thirdPt;
    if(k=="ab"){
      neighbour.pts2d[ matchs.a ] = triangle.pts2d.a;
      neighbour.pts2d[ matchs.b ] = triangle.pts2d.b;
      thirdPt = triangle.pts2d.c
    }else if(k=="bc"){
      neighbour.pts2d[ matchs.b ] = triangle.pts2d.b;
      neighbour.pts2d[ matchs.c ] = triangle.pts2d.c;
      thirdPt = triangle.pts2d.a
    }else if(k=="ca"){
      neighbour.pts2d[ matchs.c ] = triangle.pts2d.c;
      neighbour.pts2d[ matchs.a ] = triangle.pts2d.a;
      thirdPt = triangle.pts2d.b
    }
    var n2d = neighbour.pts2d;
    if( !n2d.a){
      n2d.a = findNeighbour(n2d.c,n2d.b,thirdPt,neighbour.CtoA, neighbour.AtoB)
    }else if( !n2d.b){
      n2d.b  = findNeighbour(n2d.a,n2d.c,thirdPt,neighbour.AtoB, neighbour.BtoC)
    }else if( !n2d.c){
      n2d.c  = findNeighbour(n2d.a,n2d.b,thirdPt,neighbour.CtoA, neighbour.BtoC)
    }
    ret.push( neighbour.idx )
  }
  return ret;
}
function findNeighbour(edgeA,edgeB,mainC,sizeA, sizeB){
  var ax = edgeA.x,
      ay = edgeA.y,
      bx = edgeB.x,
      by = edgeB.y,
      cx = mainC.x,
      cy = mainC.y,
      intersec = intersection(ax, ay, sizeA, bx, by, sizeB),
      nx = intersec[ 0 ],
      ny = intersec[ 2 ];
  if(    is_in_triangle (nx,ny,ax,ay,bx,by,cx,cy)
      || is_in_triangle (cx,cy,ax,ay,bx,by,nx,ny)
      || testIntersect(ax, ay, nx, ny, bx, by, cx, cy)
      || testIntersect(bx, by, nx, ny, ax, ay, cx, cy)
  ){
    return {x:intersec[ 1 ],y:intersec[ 3 ]}
  }else{
    return  {x:intersec[ 0 ],y:intersec[ 2 ]}
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
function trianglesToParts( triangle, _triangles, _parts ){
  var firstTriangle = triangle,
      prtsL = _parts.length,
      prezens = false;
  for( var pn = 0; pn < prtsL; pn++ ){
    if( _parts[ pn ].indexOf( firstTriangle.idx ) > -1 ){
      prezens = true;
      break;
    }
  }
  if( prezens ) return false;
  var firstPart = [ firstTriangle.idx ];
  _parts.push( firstPart )
  for( var i = 0; i < firstPart.length; i++ ){
    var tri = firstPart[ i ],
        neighb = getTriNeighbours( _triangles[ tri ] ),
        nl = neighb.length,
        pl = _parts.length,
        presences = [];
    for( var n = 0; n < nl; n++ ){
      var neig = neighb[ n ],
          prez = false;
      for( var p = 0; p < pl; p++ ){
        if( _parts[ p ].indexOf( neig ) > -1 ){
          prez = true;
          break;
        }
      }
      if( !prez ) firstPart.push( neig );
    }
  }
  return _parts
}
function getTriNeighbours( tri ){
  var n = tri.neighbours,
      ret = [];
  if( n.ab != "none" ) ret.push( n.ab.id );
  if( n.bc != "none" ) ret.push( n.bc.id );
  if( n.ca != "none" ) ret.push( n.ca.id );
  return ret;
}
function trianglesToParts2( _triangles ){
  var parts = [],
      l = _triangles.length,
      part = [];
  findTriangleNeighbours( _triangles[ 0 ], _triangles, parts, part );
  /*
  for( var i = 0; i < l; i++ ){
    var tri = _triangles[ i ],
  }
  if( searchTri == "none" )return parts;
  var triangle =  _triangles[ searchTri ],
      neighbours = triangle.neighbours;
  for( var k in neighbours ){
    if( neighbours[ k ] == "none" ) continue;
  }
  */
}
function findTriangleNeighbours( triangle, _triangles, parts, part ){
  var neighbours = triangle.neighbours;
  for( var k in neighbours ){
    if( neighbours[ k ] == "none" ) continue;
    var neighbourId = neighbours[ k ].id,
        neighbour = _triangles[ neighbourId ],
        isInParts = false,
        pl = parts.length;
    for( var p = 0; p < pl; p++ ){
      if( parts[ p ].indexOf( neighbourId ) > -1 ){
        isInParts = true;
        break;
      }
    }
    if( isInParts ) continue;
    part.push( neighbourId );
    findTriangleNeighbours( neighbour, _triangles, parts, part )
  }
}
function trianglesToParts2( _triangles ){
  var parts = [],
      l = _triangles.length;
  for( var i = 0; i < l; i++ ){
    var tri = _triangles[ i ],
        isInParts = false,
        pl = parts.length;
    for( var p = 0; p < pl; p++ ){
      if( parts[ p ].indexOf( tri.idx ) > -1 ){
        isInParts = true;
        break;
      }
    }
    if( isInParts ) continue;
    var part = [ tri.idx ];
    parts.push( part );
    for( var n = 0; n < part.length; n++ ){
      var triangle = _triangles[ part[ n ] ],
          neighbours = triangle.neighbours;
      for( var k in neighbours ){
        if( neighbours[ k ] == "none" ) continue;
        var triId = neighbours[ k ].id,
            partsL = parts.length,
            inParts = false;
        for( var t = 0; t < partsL; t++ ){
          if( parts[ t ].indexOf( triId ) > -1 ){
            inParts = true;
            break;
          }
        }
        if( !inParts ) part.push( triId );
      }
    }
  }
  return parts;
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
  return {  indices: _indices,
            triangles: ret };
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
        dx = x1 - x0;
        dy = y1 - y0;
        d = Math.sqrt((dy*dy) + (dx*dx));
        if (d > (r0 + r1)) {
            return false;
        }
        if (d < Math.abs(r0 - r1)) {
            return false;
        }
        a = ((r0*r0) - (r1*r1) + (d*d)) / (2.0 * d) ;
        x2 = x0 + (dx * a/d);
        y2 = y0 + (dy * a/d);
        h = Math.sqrt((r0*r0) - (a*a));
        rx = -dy * (h/d);
        ry = dx * (h/d);
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
function is_in_triangle (px,py,ax,ay,bx,by,cx,cy){
  //credit: http://www.blackpawn.com/texts/pointinpoly/default.html
  var v0 = [cx-ax,cy-ay];
  var v1 = [bx-ax,by-ay];
  var v2 = [px-ax,py-ay];
  var dot00 = (v0[0]*v0[0]) + (v0[1]*v0[1]);
  var dot01 = (v0[0]*v1[0]) + (v0[1]*v1[1]);
  var dot02 = (v0[0]*v2[0]) + (v0[1]*v2[1]);
  var dot11 = (v1[0]*v1[0]) + (v1[1]*v1[1]);
  var dot12 = (v1[0]*v2[0]) + (v1[1]*v2[1]);
  var invDenom = 1/ (dot00 * dot11 - dot01 * dot01);
  var u = (dot11 * dot02 - dot01 * dot12) * invDenom;
  var v = (dot00 * dot12 - dot01 * dot02) * invDenom;
  return ((u >= 0) && (v >= 0) && (u + v < 1));
}
function testIntersect(x1, y1, x2, y2, x3, y3, x4, y4){
	var a1, a2, b1, b2, c1, c2;
	var r1, r2 , r3, r4;
	var denom, offset, num;
	a1 = y2 - y1;
	b1 = x1 - x2;
	c1 = (x2 * y1) - (x1 * y2);
	r3 = ((a1 * x3) + (b1 * y3) + c1);
	r4 = ((a1 * x4) + (b1 * y4) + c1);
	if ((r3 !== 0) && (r4 !== 0) && sameSign(r3, r4)){
		return 0;
	}
	a2 = y4 - y3;
	b2 = x3 - x4;
	c2 = (x4 * y3) - (x3 * y4);
	r1 = (a2 * x1) + (b2 * y1) + c2;
	r2 = (a2 * x2) + (b2 * y2) + c2;
	if ((r1 !== 0) && (r2 !== 0) && (sameSign(r1, r2))){
		return 0;
	}
	denom = (a1 * b2) - (a2 * b1);
	if (denom === 0) {
		return 1;
	}
	return 1;
}
const sameSign = (a, b) => (a * b) > 0;
var compareVec3 = function(pta,ptb){
  if( pta.x == ptb.x && pta.y == ptb.y && pta.z == ptb.z ) return true;
  return false;
};
function start3dPreview( _positions, _indices ){
  var babylonCanvas = document.body.appendChild( document.createElement( "canvas" ) );
  babylonCanvas.id = "babylonCanvas";
  var engine = new BABYLON.Engine(babylonCanvas, true);
  var scene = new BABYLON.Scene(engine);
  scene.clearColor =  new BABYLON.Color4(0,0,0,0);
  var camera = new BABYLON.ArcRotateCamera("Camera", Math.PI / 2, Math.PI / 2, 2, new BABYLON.Vector3(0, 30, 0), scene);
  camera.attachControl(babylonCanvas, true);
  // Add lights to the scene
  var light1 = new BABYLON.HemisphericLight("light1", new BABYLON.Vector3(1, -1, 0), scene);

  var customMesh = new BABYLON.Mesh("custom", scene);
  var vertexData = new BABYLON.VertexData();

  vertexData.positions = _positions;
  vertexData.indices = _indices;
  var normals = [];
  BABYLON.VertexData.ComputeNormals(_positions, _indices, normals);
  vertexData.normals = normals;

  vertexData.applyToMesh(customMesh);
  customMesh.position.x = 0;
  customMesh.position.y = 0;
  customMesh.position.z = 0;

  var myMaterial = new BABYLON.StandardMaterial("myMaterial", scene);
  myMaterial.diffuseColor = new BABYLON.Color3(0.5, 0.5, 0.5);
  myMaterial.specularColor = new BABYLON.Color3(0.1, 0.1, 0.1);
  myMaterial.emissiveColor = new BABYLON.Color3(0.2, 0.2, 0.2);
  myMaterial.ambientColor = new BABYLON.Color3(0.23, 0.98, 0.53);

  customMesh.material = myMaterial;
  customMesh.convertToFlatShadedMesh()

  babylonCanvas.addEventListener("click", function () {
     // We try to pick an object
     var pickResult = scene.pick(scene.pointerX, scene.pointerY);
     //alert(JSON.stringify(pickResult))
     if(pickResult.hit){
        var indices = pickResult.pickedMesh.getIndices();
        var index0 = indices[pickResult.faceId * 3];
        var index1 = indices[pickResult.faceId * 3 + 1];
        var index2 = indices[pickResult.faceId * 3 + 2];
        alert( index0 + ", " + index1 + ", " + index2 )
     }
  }),

  engine.runRenderLoop(function () { // Register a render loop to repeatedly render the scene
    scene.render();
  });
}
