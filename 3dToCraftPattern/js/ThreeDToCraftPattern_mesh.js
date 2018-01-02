class ThreeDToCraftPattern_mesh{
  constructor( positions, indices, scene, name ){
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
          orderedDists = [ d0to1, d1to2, d2to0 ].sort( sortNumber ),
          p2d0x = 0,
          p2d0y = 0,
          p2d1x = d0to1,
          p2d1y = 0,
          pt2Intersection = intersection(p2d0x, p2d0y, d2to0, p2d1x, p2d1y, d1to2),
    }
  }
}
