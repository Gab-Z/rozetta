class ThreeDToCraftPattern_mesh{
  constructor(  name, positions, indices, scene ){
    this.BABYLON = {};
    this.BABYLON.mesh = new BABYLON.Mesh( name, scene );
    let vertexData = new BABYLON.VertexData(),
        nbTriangles = indices.length / 3,
        nbPoints = indices.length,
        separatePositions = new Array( indices.length * 3 ),
        separateIndices = new Array( indices.length ),
        generated_uvs =  new Array( indices.length * 2 );
    this.texGridSize = Math.floor( Math.sqrt( nbTriangles ) ) + 1;
    for( let h = 0; h < nbPoints; h++ ){
      separatePositions[ h * 3 ] = positions[ indices[ h ] * 3 ];
      separatePositions[ h * 3 + 1 ] = positions[ indices[ h ] * 3 + 1 ];
      separatePositions[ h * 3 + 2 ] = positions[ indices[ h ] * 3 + 2 ];

      separateIndices[ h ] = h;
    }
    let uvStore = [];
    for( var u = 0; u < nbTriangles; u++ ){
      let i0 = separateIndices[ u * 3 ],
          i1 = separateIndices[ u * 3 + 1 ],
          i2 = separateIndices[ u * 3 + 2],
          p3d0x = separatePositions[ i0 * 3 ],
          p3d0y = separatePositions[ i0 * 3 + 1 ],
          p3d0z = separatePositions[ i0 * 3 + 2 ],
          p3d1x = separatePositions[ i1 * 3 + 2 ],
          p3d1y = separatePositions[ i1 * 3 + 1 ],
          p3d1z = separatePositions[ i1 * 3 + 2 ],
          p3d2x = separatePositions[ i2 * 3 ],
          p3d2y = separatePositions[ i2 * 3 + 1 ],
          p3d2z = separatePositions[ i2 * 3 + 2 ],
          d0to1 = Math.sqrt( Math.pow( p3d0x - p3d1x, 2 ) + Math.pow( p3d0y - p3d1y, 2 ) + Math.pow( p3d0z - p3d1z, 2 ) ),
          d1to2 = Math.sqrt( Math.pow( p3d1x - p3d2x, 2 ) + Math.pow( p3d1y - p3d2y, 2 ) + Math.pow( p3d1z - p3d2z, 2 ) ),
          d2to0 = Math.sqrt( Math.pow( p3d2x - p3d0x, 2 ) + Math.pow( p3d2y - p3d0y, 2 ) + Math.pow( p3d2z - p3d0z, 2 ) ),
          p2d0x, p2d0y,
          p2d1x, p2d1y,
          p2d2x, p2d2y,
          centerx, centery;
      if( d0to1 >= d1to2 && d0to1 >= d2to0 ){
          p2d0x = 0;
          p2d0y = 0;
          p2d1x = Math.cos( Math.PI * 0.25 ) * d0to1;
          p2d1y = Math.sin( Math.PI * 0.25 ) * d0to1;
          centerx = p2d1x / 2;
          centery = p2d1y / 2;
          let intesectionPt = intersection(p2d0x, p2d0y, d2to0, p2d1x, p2d1y, d1to2);
          if( intesectionPt[ 0 ] < intesectionPt[ 1 ] || intesectionPt[ 2 ] > intesectionPt[ 3 ] || intesectionPt[ 0 ] == intesectionPt[ 1 ] && intesectionPt[ 2 ] >= intesectionPt[ 3 ] ){
            p2d2x = intesectionPt[ 0 ];
            p2d2y = intesectionPt[ 2 ];
          }else{
            p2d2x = intesectionPt[ 1 ];
            p2d2y = intesectionPt[ 3 ];
          }
      }else if( d1to2 >= d0to1 && d1to2 >= d2to0 ){
        p2d1x = 0;
        p2d1y = 0;
        p2d2x = Math.cos( Math.PI * 0.25 ) * d1to2;
        p2d2y = Math.sin( Math.PI * 0.25 ) * d1to2;
        centerx = p2d2x / 2;
        centery = p2d2y / 2;
        let intesectionPt = intersection(p2d1x, p2d1y, d0to1, p2d2x, p2d2y, d2to0);
        if( intesectionPt[ 0 ] < intesectionPt[ 1 ] || intesectionPt[ 2 ] > intesectionPt[ 3 ] || intesectionPt[ 0 ] == intesectionPt[ 1 ] && intesectionPt[ 2 ] >= intesectionPt[ 3 ] ){
          p2d0x = intesectionPt[ 0 ];
          p2d0y = intesectionPt[ 2 ];
        }else{
          p2d0x = intesectionPt[ 1 ];
          p2d0y = intesectionPt[ 3 ];
        }
      }else if( d2to0 >= d0to1 && d2to0 >= d1to2 ){
        p2d2x = 0;
        p2d2y = 0;
        p2d0x = Math.cos( Math.PI * 0.25 ) * d2to0;
        p2d0y = Math.sin( Math.PI * 0.25 ) * d2to0;
        centerx = p2d0x / 2;
        centery = p2d0y / 2;
        let intesectionPt = intersection(p2d2x, p2d2y, d1to2, p2d0x, p2d0y, d0to1);
        if( intesectionPt[ 0 ] < intesectionPt[ 1 ] || intesectionPt[ 2 ] > intesectionPt[ 3 ] || intesectionPt[ 0 ] == intesectionPt[ 1 ] && intesectionPt[ 2 ] >= intesectionPt[ 3 ] ){
          p2d1x = intesectionPt[ 0 ];
          p2d1y = intesectionPt[ 2 ];
        }else{
          p2d1x = intesectionPt[ 1 ];
          p2d1y = intesectionPt[ 3 ];
        }
      }
      let minX = Math.min( Math.min( p2d0x, p2d1x ), p2d2x ),
          minY = Math.min( Math.min( p2d0y, p2d1y ), p2d2y ),
          maxX = Math.max( Math.max( p2d0x, p2d1x ), p2d2x ),
          maxY = Math.max( Math.max( p2d0y, p2d1y ), p2d2y );
      uvStore.push( {
        id: u,
        pt0: { x: p2d0x, y: p2d0y },
        pt1: { x: p2d1x, y: p2d1y },
        pt2: { x: p2d2x, y: p2d2y },
        bounds:{
          minX:minX,
          minY:minY,
          maxX:maxX,
          maxY:maxY
        },
        width : maxX - minX,
        height : maxY - minY,
        center:{ x: centerx, y: centery }
      } );
    }
    uvStore.sort( ( a, b ) => a.height - b.height );
    let halfUvL = uvStore.length / 2;
    for( let uvi = 0; uvi < halfUvL; uvi++ ){
      let uvOb = uvStore[ uvi * 2 + 1 ],
          distCenterTo0 = Math.sqrt( Math.pow( uvOb.pt0.x - uvOb.center.x, 2 ) + Math.pow( uvOb.pt0.y - uvOb.center.y, 2 ) ),
          distCenterTo1 = Math.sqrt( Math.pow( uvOb.pt1.x - uvOb.center.x, 2 ) + Math.pow( uvOb.pt1.y - uvOb.center.y, 2 ) ),
          distCenterTo2 = Math.sqrt( Math.pow( uvOb.pt2.x - uvOb.center.x, 2 ) + Math.pow( uvOb.pt2.y - uvOb.center.y, 2 ) ),
          angleCenterTo0 = Math.atan2( uvOb.pt0.y - uvOb.center.y, uvOb.pt0.x - uvOb.center.x ) + Math.PI,
          angleCenterTo1 = Math.atan2( uvOb.pt1.y - uvOb.center.y, uvOb.pt1.x - uvOb.center.x ) + Math.PI,
          angleCenterTo2 = Math.atan2( uvOb.pt2.y - uvOb.center.y, uvOb.pt2.x - uvOb.center.x ) + Math.PI;
      uvOb.pt0 = {  x: uvOb.center.x + Math.cos( angleCenterTo0 ) * distCenterTo0,
                    y: uvOb.center.y + Math.sin( angleCenterTo0 ) * distCenterTo0 };
      uvOb.pt1 = {  x: uvOb.center.x + Math.cos( angleCenterTo1 ) * distCenterTo1,
                    y: uvOb.center.y + Math.sin( angleCenterTo1 ) * distCenterTo1 };
      uvOb.pt2 = {  x: uvOb.center.x + Math.cos( angleCenterTo2 ) * distCenterTo2,
                    y: uvOb.center.y + Math.sin( angleCenterTo2 ) * distCenterTo2 };
      uvOb.bounds = {
        minX: Math.min( Math.min( uvOb.pt0.x, uvOb.pt1.x ), uvOb.pt2.x ),
        minY: Math.min( Math.min( uvOb.pt0.y, uvOb.pt1.y ), uvOb.pt2.y ),
        maxX: Math.max( Math.max( uvOb.pt0.x, uvOb.pt1.x ), uvOb.pt2.x ),
        maxY: Math.max( Math.max( uvOb.pt0.y, uvOb.pt1.y ), uvOb.pt2.y )
      }
      uvOb.width = uvOb.bounds.maxX - uvOb.bounds.minX;
      uvOb.height = uvOb.bounds.maxY - uvOb.bounds.minY;
    }

    let totalWidth = 0;
    uvStore.forEach( el => { totalWidth += el.width });
    let rawSize = Math.floor( Math.sqrt( totalWidth ) ) + 1,
        posx = 0,
        posy = 0;
    uvStore.forEach( ( el, i, arr ) => {
      if( Math.floor( i * 0.5 ) === i * 0.5 ){

      }else{

      }
    });
    //console.log( JSON.stringify( uvStore ) )
  }
}
