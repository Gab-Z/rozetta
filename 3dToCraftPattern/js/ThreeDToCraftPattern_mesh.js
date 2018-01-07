const ThreeDToCraftPattern_mesh_defaults = {
  textureSize : 2048
};
class ThreeDToCraftPattern_mesh{
  constructor( name, positions, indices, scene, options = {} ){
    let opts = Object.assign( ThreeDToCraftPattern_mesh_defaults, options ),
        self = this;
    this.BABYLON = {};
    this.name = name;
    this.BABYLON.textureSize = opts.textureSize;
    this.BABYLON.mesh = new BABYLON.Mesh( name, scene );

    this.BABYLON.textureCanvas = document.createElement("canvas");
    this.BABYLON.textureCanvas.classList.add("texCanvas");
    this.BABYLON.textureCanvas.id = name + "_texCanvas";
    this.BABYLON.textureCanvas.width = this.BABYLON.textureSize;
    this.BABYLON.textureCanvas.height = this.BABYLON.textureSize;

    //document.body.appendChild( this.BABYLON.textureCanvas )

    this.BABYLON.dynamicTexture = new BABYLON.DynamicTexture( name + "_dynamicTex", this.BABYLON.textureCanvas, scene, true);

    let vertexData = new BABYLON.VertexData(),
        separateGeometry = this.separateFaces( indices, positions ),
        generated_normals = [],
        uvs = this.computeUvGrid( separateGeometry.positions, separateGeometry.indices );
    BABYLON.VertexData.ComputeNormals( separateGeometry.positions, separateGeometry.indices, generated_normals );
    vertexData.positions = separateGeometry.positions;
    vertexData.indices = separateGeometry.indices;
    vertexData.normals = generated_normals;
    vertexData.uvs = uvs;
    vertexData.applyToMesh( this.BABYLON.mesh );
    this.BABYLON.mesh.convertToFlatShadedMesh();

    this.BABYLON.mesh.material = new BABYLON.StandardMaterial( name + "_mat", scene );
    this.BABYLON.mesh.material.diffuseTexture = this.BABYLON.dynamicTexture;
    //this.BABYLON.mesh.material.diffuseColor = new BABYLON.Color3(0.5, 0.5, 0.5);
    this.BABYLON.mesh.material.specularColor = new BABYLON.Color3(0.1, 0.1, 0.1);
    this.BABYLON.mesh.material.emissiveColor = new BABYLON.Color3(0.2, 0.2, 0.2);
    this.BABYLON.mesh.material.ambientColor = new BABYLON.Color3(0.23, 0.98, 0.53);
    this.BABYLON.mesh.material.backFaceCulling = false;

  //  this.BABYLON.mesh.enableEdgesRendering(0.00000001);
		this.BABYLON.mesh.edgesWidth = 25.0;
		this.BABYLON.mesh.edgesColor = new BABYLON.Color4(0, 0, 1, 1);

    //this.BABYLON.mesh.material.wireframe = true;

    this.BABYLON.mesh.position.x = 0;
    this.BABYLON.mesh.position.y = 0;
    this.BABYLON.mesh.position.z = 0;

    this.drawTexUvs();
  }
  drawTexUvs(){
    let tex = this.BABYLON.dynamicTexture,
        w = this.BABYLON.textureSize;
    if( ! this.BABYLON.uvCanvas ){
      this.BABYLON.uvCanvas = document.createElement("canvas");
      this.BABYLON.uvCanvas.width = w;
      this.BABYLON.uvCanvas.height = w;
      let uvCtx = this.BABYLON.uvCanvas.getContext( "2d" ),
          uvs = this.BABYLON.mesh.getVerticesData( BABYLON.VertexBuffer.UVKind ),
          indices = this.BABYLON.mesh.getIndices(),
          nbTri = indices.length / 3,
          edgeWidth = 0.1 * ( w / 512 );
      uvCtx.fillStyle = "#000000";
      for( let i = 0; i < nbTri; i++ ){
        let i0 = indices[ i * 3 ],
            i1 = indices[ i * 3 + 1],
            i2 = indices[ i * 3 + 2 ],
            p0 = { x: uvs[ i0 * 2 ] * w, y: uvs[ i0 * 2 + 1 ] * w },
            p1 = { x: uvs[ i1 * 2 ] * w, y: uvs[ i1 * 2 + 1 ] * w },
            p2 = { x: uvs[ i2 * 2 ] * w, y: uvs[ i2 * 2 + 1 ] * w },
            angle0to1 = Math.atan2( p1.y - p0.y, p1.x - p0.x ),
            angle1to2 = Math.atan2( p2.y - p1.y, p2.x - p1.x ),
            angle2to0 = Math.atan2( p0.y - p2.y, p0.x - p2.x ),
            dist0to1 = Math.sqrt( Math.pow(  p1.x - p0.x, 2 ) + Math.pow(  p1.y - p0.y, 2 ) ),
            dist1to2 = Math.sqrt( Math.pow(  p2.x - p1.x, 2 ) + Math.pow(  p2.y - p1.y, 2 ) ),
            dist2to0 = Math.sqrt( Math.pow(  p0.x - p2.x, 2 ) + Math.pow(  p0.y - p2.y, 2 ) );
        uvCtx.save();
        uvCtx.translate( p0.x, p0.y )
        uvCtx.rotate( angle0to1 );
        uvCtx.fillRect( -edgeWidth, -edgeWidth, dist0to1 + 2 * edgeWidth, 2 * edgeWidth );
        uvCtx.restore();

        uvCtx.save();
        uvCtx.translate( p1.x, p1.y )
        uvCtx.rotate( angle1to2 );
        uvCtx.fillRect( -edgeWidth, -edgeWidth, dist1to2 + 2 * edgeWidth, 2 * edgeWidth );
        uvCtx.restore();

        uvCtx.save();
        uvCtx.translate( p2.x, p2.y )
        uvCtx.rotate( angle2to0 );
        uvCtx.fillRect( -edgeWidth, -edgeWidth, dist2to0 + 2 * edgeWidth, 2 * edgeWidth );
        uvCtx.restore();
      }
    }
    let cv = this.BABYLON.textureCanvas,
        ctx = cv.getContext( "2d" );
    ctx.fillStyle = "#e99e60";
    ctx.fillRect( 0, 0, w, w );
    ctx.drawImage( this.BABYLON.uvCanvas, 0, 0 );
    /*
    let cv = this.BABYLON.textureCanvas,
        ctx = cv.getContext( "2d" ),
        tex = this.BABYLON.dynamicTexture,
        w = cv.width,
        uvs = this.BABYLON.mesh.getVerticesData( BABYLON.VertexBuffer.UVKind ),
        indices = this.BABYLON.mesh.getIndices(),
        nbTri = indices.length / 3,
        edgeWidth = 0.1 * ( w / 512 );
    ctx.fillStyle = "#e99e60";
    ctx.fillRect( 0, 0, w, w );
    ctx.lineWidth = 0.5;
    ctx.fillStyle = "#000000";
    for( let i = 0; i < nbTri; i++ ){
    //  ctx.fillRect( 0, 0, w, w )
      let i0 = indices[ i * 3 ],
          i1 = indices[ i * 3 + 1],
          i2 = indices[ i * 3 + 2 ],
          p0 = { x: uvs[ i0 * 2 ] * w, y: uvs[ i0 * 2 + 1 ] * w },
          p1 = { x: uvs[ i1 * 2 ] * w, y: uvs[ i1 * 2 + 1 ] * w },
          p2 = { x: uvs[ i2 * 2 ] * w, y: uvs[ i2 * 2 + 1 ] * w },
          angle0to1 = Math.atan2( p1.y - p0.y, p1.x - p0.x ),
          angle1to2 = Math.atan2( p2.y - p1.y, p2.x - p1.x ),
          angle2to0 = Math.atan2( p0.y - p2.y, p0.x - p2.x ),
          dist0to1 = Math.sqrt( Math.pow(  p1.x - p0.x, 2 ) + Math.pow(  p1.y - p0.y, 2 ) ),
          dist1to2 = Math.sqrt( Math.pow(  p2.x - p1.x, 2 ) + Math.pow(  p2.y - p1.y, 2 ) ),
          dist2to0 = Math.sqrt( Math.pow(  p0.x - p2.x, 2 ) + Math.pow(  p0.y - p2.y, 2 ) );
      ctx.save();
      ctx.translate( p0.x, p0.y )
      ctx.rotate( angle0to1 );
      ctx.fillRect( -edgeWidth, -edgeWidth, dist0to1 + 2 * edgeWidth, 2 * edgeWidth );
      ctx.restore();

      ctx.save();
      ctx.translate( p1.x, p1.y )
      ctx.rotate( angle1to2 );
      ctx.fillRect( -edgeWidth, -edgeWidth, dist1to2 + 2 * edgeWidth, 2 * edgeWidth );
      ctx.restore();

      ctx.save();
      ctx.translate( p2.x, p2.y )
      ctx.rotate( angle2to0 );
      ctx.fillRect( -edgeWidth, -edgeWidth, dist2to0 + 2 * edgeWidth, 2 * edgeWidth );
      ctx.restore();

    }
    */
    tex.update( false );
  }
  drawTexUvs3( ){
    let cv = this.BABYLON.textureCanvas,
        ctx = cv.getContext( "2d" ),
        tex = this.BABYLON.dynamicTexture,
        w = cv.width,
        uvs = this.BABYLON.mesh.getVerticesData( BABYLON.VertexBuffer.UVKind ),
        indices = this.BABYLON.mesh.getIndices(),
        nbTri = indices.length / 3,
        edgeWidth = 0.1 * ( w / 512 );
    ctx.fillStyle = "#e99e60";
    ctx.fillRect( 0, 0, w, w );
    ctx.lineWidth = 0.5;
    ctx.fillStyle = "#000000";
    for( let i = 0; i < nbTri; i++ ){
    //  ctx.fillRect( 0, 0, w, w )
      let i0 = indices[ i * 3 ],
          i1 = indices[ i * 3 + 1],
          i2 = indices[ i * 3 + 2 ],
          p0 = { x: uvs[ i0 * 2 ] * w, y: uvs[ i0 * 2 + 1 ] * w },
          p1 = { x: uvs[ i1 * 2 ] * w, y: uvs[ i1 * 2 + 1 ] * w },
          p2 = { x: uvs[ i2 * 2 ] * w, y: uvs[ i2 * 2 + 1 ] * w },
          angle0to1 = Math.atan2( p1.y - p0.y, p1.x - p0.x ),
          angle1to2 = Math.atan2( p2.y - p1.y, p2.x - p1.x ),
          angle2to0 = Math.atan2( p0.y - p2.y, p0.x - p2.x ),
          dist0to1 = Math.sqrt( Math.pow(  p1.x - p0.x, 2 ) + Math.pow(  p1.y - p0.y, 2 ) ),
          dist1to2 = Math.sqrt( Math.pow(  p2.x - p1.x, 2 ) + Math.pow(  p2.y - p1.y, 2 ) ),
          dist2to0 = Math.sqrt( Math.pow(  p0.x - p2.x, 2 ) + Math.pow(  p0.y - p2.y, 2 ) );
      ctx.save();
      ctx.translate( p0.x, p0.y )
      ctx.rotate( angle0to1 );
      ctx.fillRect( -edgeWidth, -edgeWidth, dist0to1 + 2 * edgeWidth, 2 * edgeWidth );
      ctx.restore();

      ctx.save();
      ctx.translate( p1.x, p1.y )
      ctx.rotate( angle1to2 );
      ctx.fillRect( -edgeWidth, -edgeWidth, dist1to2 + 2 * edgeWidth, 2 * edgeWidth );
      ctx.restore();

      ctx.save();
      ctx.translate( p2.x, p2.y )
      ctx.rotate( angle2to0 );
      ctx.fillRect( -edgeWidth, -edgeWidth, dist2to0 + 2 * edgeWidth, 2 * edgeWidth );
      ctx.restore();

    }
    tex.update( false );
  }
  drawTexUvs2( ){
    let cv = this.BABYLON.textureCanvas,
        ctx = cv.getContext( "2d" ),
        tex = this.BABYLON.dynamicTexture,
        w = cv.width,
        uvs = this.BABYLON.mesh.getVerticesData( BABYLON.VertexBuffer.UVKind ),
        indices = this.BABYLON.mesh.getIndices(),
        nbTri = indices.length / 3;
    ctx.fillStyle = "#e99e60";
    ctx.fillRect( 0, 0, w, w );
    ctx.lineWidth = 0.5;
    for( let i = 0; i < nbTri; i++ ){
    //  ctx.fillRect( 0, 0, w, w )
      let i0 = indices[ i * 3 ],
          i1 = indices[ i * 3 + 1],
          i2 = indices[ i * 3 + 2 ],
          p0x = uvs[ i0 * 2 ] * w,
          p0y = uvs[ i0 * 2 + 1 ] * w,
          p1x = uvs[ i1 * 2 ] * w,
          p1y = uvs[ i1 * 2 + 1 ] * w,
          p2x = uvs[ i2 * 2 ] * w,
          p2y = uvs[ i2 * 2 + 1 ] * w;
      ctx.beginPath();
      ctx.moveTo( p0x, p0y );
      ctx.lineTo( p1x, p1y );
      ctx.lineTo( p2x, p2y );
      ctx.closePath();
      ctx.stroke();

    }
    tex.update( false );
  }
  separateFaces( indices, positions ){
    let nbPoints = indices.length,
        nbTriangles = nbPoints / 3,
        separatePositions = new Array( nbPoints * 3 ),
        separateIndices = new Array( nbPoints );
    indices.forEach( ( PtIdx, i, arr ) => {
      separatePositions[ i * 3 ] = positions[ PtIdx * 3 ];
      separatePositions[ i * 3 + 1 ] = positions[ PtIdx * 3 + 1 ];
      separatePositions[ i * 3 + 2 ] = positions[ PtIdx * 3 + 2 ];

      separateIndices[ i ] = i;
    });
    return { positions: separatePositions, indices: separateIndices };
  }
  computeUvGrid( positions, indices ){
    let nbTriangles = indices.length / 3,
        uvStore = [],
        tileW = 0,
        tileH = 0;
    for( let u = 0; u < nbTriangles; u++ ){
      let i0 = indices[ u * 3 ],
          i1 = indices[ u * 3 + 1 ],
          i2 = indices[ u * 3 + 2],
          p3d0 = { x: positions[ i0 * 3 ], y: positions[ i0 * 3 + 1 ], z: positions[ i0 * 3 + 2 ] },
          p3d1 = { x: positions[ i1 * 3 ], y: positions[ i1 * 3 + 1 ], z: positions[ i1 * 3 + 2 ] },
          p3d2 = { x: positions[ i2 * 3 ], y: positions[ i2 * 3 + 1 ], z: positions[ i2 * 3 + 2 ] },
          d0to1 = distanceVector( p3d0, p3d1 ),
          d1to2 = distanceVector( p3d1, p3d2 ),
          d2to0 = distanceVector( p3d2, p3d0 ),
          p2d0x, p2d0y,
          p2d1x, p2d1y,
          p2d2x, p2d2y,
          longestSide,
          centerx, centery;
      if( d0to1 >= d1to2 && d0to1 >= d2to0 ){
        p2d0x = Math.cos( Math.PI * 0.25 ) * d0to1;
        p2d0y = 0;
        p2d1x = 0;
        p2d1y = Math.sin( Math.PI * 0.25 ) * d0to1;
        centerx = p2d0x / 2;
        centery = p2d1y / 2;
        longestSide = d0to1;
        let intesectionPt = intersection(p2d0x, p2d0y, d2to0, p2d1x, p2d1y, d1to2);
        if( intesectionPt[ 0 ] <= intesectionPt[ 1 ] ){
            p2d2x = intesectionPt[ 0 ];
            p2d2y = intesectionPt[ 2 ];
          }else{
            p2d2x = intesectionPt[ 1 ];
            p2d2y = intesectionPt[ 3 ];
          }
      }else if( d1to2 > d0to1 && d1to2 >= d2to0 ){
        p2d1x = Math.cos( Math.PI * 0.25 ) * d1to2;
        p2d1y = 0;
        p2d2x = 0;
        p2d2y = Math.sin( Math.PI * 0.25 ) * d1to2;
        centerx = p2d1x / 2;
        centery = p2d2y / 2;
        longestSide = d1to2;
        let intesectionPt = intersection(p2d1x, p2d1y, d0to1, p2d2x, p2d2y, d2to0);
        if( intesectionPt[ 0 ] <= intesectionPt[ 1 ] ){
          p2d0x = intesectionPt[ 0 ];
          p2d0y = intesectionPt[ 2 ];
        }else{
          p2d0x = intesectionPt[ 1 ];
          p2d0y = intesectionPt[ 3 ];
        }
      }else{
        p2d2x = Math.cos( Math.PI * 0.25 ) * d2to0;
        p2d2y = 0;
        p2d0x = 0;
        p2d0y = Math.sin( Math.PI * 0.25 ) * d2to0;
        centerx = p2d2x / 2;
        centery = p2d0y / 2;
        longestSide = d2to0;
        let intesectionPt = intersection(p2d2x, p2d2y, d1to2, p2d0x, p2d0y, d0to1);
        if( intesectionPt[ 0 ] <= intesectionPt[ 1 ] ){
          p2d1x = intesectionPt[ 0 ];
          p2d1y = intesectionPt[ 2 ];
        }else{
          p2d1x = intesectionPt[ 1 ];
          p2d1y = intesectionPt[ 3 ];
        }
      }
      let minX = Math.min( p2d0x, p2d1x, p2d2x ),
          minY = Math.min( p2d0y, p2d1y, p2d2y ),
          maxX = Math.max( p2d0x, p2d1x, p2d2x ),
          maxY = Math.max( p2d0y, p2d1y, p2d2y );
      p2d0x += -minX;
      p2d1x += -minX;
      p2d2x += -minX;
      p2d0y += -minY;
      p2d1y += -minY;
      p2d2y += -minY;
      centerx += -minX;
      centery += -minY;
      minX = Math.min( p2d0x, p2d1x, p2d2x ),
      minY = Math.min( p2d0y, p2d1y, p2d2y ),
      maxX = Math.max( p2d0x, p2d1x, p2d2x ),
      maxY = Math.max( p2d0y, p2d1y, p2d2y );
      tileH = Math.max( tileH, maxY );
      tileW = Math.max( tileW, maxX );
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
        center:{ x: centerx, y: centery },
        longestSide: longestSide
      } );
      /*
      alert(JSON.stringify({
        p0:{ x: p2d0x, y: p2d0y },
        p1:{ x: p2d1x, y: p2d1y },
        p2:{ x: p2d2x, y: p2d2y }
      }))
      this.drawTri({x:p2d0x,y:p2d0y},{x:p2d1x,y:p2d1y},{x:p2d2x,y:p2d2y});
      */
    }
    uvStore.sort( ( a, b ) => a.longestSide - b.longestSide );
    let nbPairs = Math.round( uvStore.length / 2 ),
        pairs = [],
        minPairW = Infinity,
        maxPairW = 0,
        totalWidth = 0;
    for( let p = 0; p  < nbPairs; p++ ){
      let id0 = uvStore.length - 1 - p,
          id1 = p;
      if( id0 < id1 ) break;
      let uv0 =  uvStore[ id0 ],
          uv1 = id0 > id1 ? uvStore[ id1 ] : false;
      if( uv1 ){
        let distCenterTo0 = Math.sqrt( Math.pow( uv1.pt0.x - uv1.center.x, 2 ) + Math.pow( uv1.pt0.y - uv1.center.y, 2 ) ),
            distCenterTo1 = Math.sqrt( Math.pow( uv1.pt1.x - uv1.center.x, 2 ) + Math.pow( uv1.pt1.y - uv1.center.y, 2 ) ),
            distCenterTo2 = Math.sqrt( Math.pow( uv1.pt2.x - uv1.center.x, 2 ) + Math.pow( uv1.pt2.y - uv1.center.y, 2 ) ),
            angleCenterTo0 = Math.atan2( uv1.pt0.y - uv1.center.y, uv1.pt0.x - uv1.center.x ) + Math.PI,
            angleCenterTo1 = Math.atan2( uv1.pt1.y - uv1.center.y, uv1.pt1.x - uv1.center.x ) + Math.PI,
            angleCenterTo2 = Math.atan2( uv1.pt2.y - uv1.center.y, uv1.pt2.x - uv1.center.x ) + Math.PI,
            centerOffsetX = uv0.center.x - uv1.center.x,
            centerOffsetY = uv0.center.y - uv1.center.y,
            marginX = 0;//( uv0.bounds.maxX - uv0.center.x ) * 0.25;

        uv1.pt0 = { x: uv1.center.x + Math.cos( angleCenterTo0 ) * distCenterTo0 + centerOffsetX + marginX,
                    y: uv1.center.y + Math.sin( angleCenterTo0 ) * distCenterTo0 + centerOffsetY };
        uv1.pt1 = { x: uv1.center.x + Math.cos( angleCenterTo1 ) * distCenterTo1 + centerOffsetX + marginX,
                    y: uv1.center.y + Math.sin( angleCenterTo1 ) * distCenterTo1 + centerOffsetY };
        uv1.pt2 = { x: uv1.center.x + Math.cos( angleCenterTo2 ) * distCenterTo2 + centerOffsetX + marginX,
                    y: uv1.center.y + Math.sin( angleCenterTo2 ) * distCenterTo2 + centerOffsetY };
      }
      let bound;
      if( uv1 ){
        bound = {
          minX: Math.min( uv0.pt0.x, uv0.pt1.x, uv0.pt2.x, uv1.pt0.x, uv1.pt1.x, uv1.pt2.x ),
          maxX: Math.max( uv0.pt0.x, uv0.pt1.x, uv0.pt2.x, uv1.pt0.x, uv1.pt1.x, uv1.pt2.x ),
          minY: Math.min( uv0.pt0.y, uv0.pt1.y, uv0.pt2.y, uv1.pt0.y, uv1.pt1.y, uv1.pt2.y ),
          maxY: Math.max( uv0.pt0.y, uv0.pt1.y, uv0.pt2.y, uv1.pt0.y, uv1.pt1.y, uv1.pt2.y ),
        }
      }else{
        bound = {
          minX: Math.min( uv0.pt0.x, uv0.pt1.x, uv0.pt2.x ),
          maxX: Math.max( uv0.pt0.x, uv0.pt1.x, uv0.pt2.x ),
          minY: Math.min( uv0.pt0.y, uv0.pt1.y, uv0.pt2.y ),
          maxY: Math.max( uv0.pt0.y, uv0.pt1.y, uv0.pt2.y ),
        }
      }
      let pair = {
        id0: id0,
        id1: uv1 ? id1 : "none",
        bounds:bound
      }
      pair.height = pair.bounds.maxY -  pair.bounds.minY;
      pair.width = pair.bounds.maxX -  pair.bounds.minX;
      minPairW = Math.min( minPairW, pair.width );
      maxPairW = Math.max( maxPairW, pair.width );
      totalWidth += pair.width;
      pairs.push( pair );
    }
    pairs.sort( ( a, b ) => a.height - b.height );
    let maxNbRows = Math.ceil( ( totalWidth - maxPairW ) / minPairW ) + 1,
        memH = 0,
        memW = 0,
        nbPairsW = 0,
        nbPairsH = 0;
    for( let c = 0; c < maxNbRows; c++ ){
      let rowWidth = maxPairW + c * minPairW,
          posx = 0,
          posy = 0,
          rowH = 0,
          memPairsInRow = 0,
          nbPairsInRow = 0,
          nbRows = 1;
      pairs.forEach( ( pair, i, ar ) => {
        if( posx + pair.width > rowWidth ){
          posx = 0;
          posy += rowH;
          rowH = 0;
          memPairsInRow = Math.max( memPairsInRow, nbPairsInRow );
          nbPairsInRow = 0;
          nbRows++;
        }
        posx += pair.width;
        nbPairsInRow++;
        rowH = Math.max( rowH, pair.height );

      })
      let colHeight = posy + rowH;
      memPairsInRow = Math.max( memPairsInRow, nbPairsInRow );
      if( colHeight ==  rowWidth ){
        memW = rowWidth;
        memH = colHeight;
        nbPairsW = memPairsInRow;
        nbPairsH = nbRows;
        break;
      }else if( ( memH >= memW && colHeight < rowWidth ) || ( memW > memH && rowWidth < colHeight ) ){
        let memDelta = Math.abs( memH - memW ),
            delta = Math.abs( colHeight - rowWidth );
        if( memDelta <= delta ){
          break;
        }else{
          memW = rowWidth;
          memH = colHeight;
          nbPairsW = memPairsInRow;
          nbPairsH = nbRows;
          break;
        }
      }
      memW = rowWidth;
      memH = colHeight;
      nbPairsW = memPairsInRow;
      nbPairsH = nbRows;
    }
    let marginAbs = 1,
        marginTexSize = 512,
        marginWidth = nbPairsW * 4,
        marginHeight = nbPairsH * 2,
        higherMarginSize = Math.max( marginWidth, marginHeight ),
        higherSide = Math.max( memW, memH ),
        fullTexRatio = marginTexSize / higherSide,
        marginRatio = marginAbs * ( higherSide / marginTexSize ),
        margin = marginAbs * fullTexRatio,
        sideSize = higherSide + marginRatio * higherMarginSize,
        ratio = 1 / sideSize,
        posy = 0,
        uvs = new Array( indices.length * 2 ),
        pl = pairs.length,
        px = 0,
        py = 0,
        rH = 0,
        marginX = 0,
        marginY = 1;
    for( let r = 0; r < pl; r++ ){
      let pair = pairs[ r ],
          uvOb1 = uvStore[ pair.id0 ],
          uvOb2 = pair.id1 !== "none" ? uvStore[ pair.id1 ] : false;
      if( px + ( marginX * marginRatio ) + pair.width > sideSize ){
        px = 0;
        py += rH;
        rH = 0;
        marginX = 1;
        marginY += 2;
      }
      uvOb1.pt0.x = (uvOb1.pt0.x + px + ( marginX * marginRatio ) ) * ratio;
      uvOb1.pt0.y = (uvOb1.pt0.y + py + ( marginY * marginRatio ) ) * ratio;
      uvOb1.pt1.x = (uvOb1.pt1.x + px + ( marginX * marginRatio ) ) * ratio;
      uvOb1.pt1.y = (uvOb1.pt1.y + py + ( marginY * marginRatio ) ) * ratio;
      uvOb1.pt2.x = (uvOb1.pt2.x + px + ( marginX * marginRatio ) ) * ratio;
      uvOb1.pt2.y = (uvOb1.pt2.y + py + ( marginY * marginRatio ) ) * ratio;
      marginX+= 2;
      if( uvOb2 ){
        uvOb2.pt0.x = (uvOb2.pt0.x + px + ( marginX * marginRatio ) ) * ratio;
        uvOb2.pt0.y = (uvOb2.pt0.y + py + ( marginY * marginRatio ) ) * ratio;
        uvOb2.pt1.x = (uvOb2.pt1.x + px + ( marginX * marginRatio ) ) * ratio;
        uvOb2.pt1.y = (uvOb2.pt1.y + py + ( marginY * marginRatio ) ) * ratio;
        uvOb2.pt2.x = (uvOb2.pt2.x + px + ( marginX * marginRatio ) ) * ratio;
        uvOb2.pt2.y = (uvOb2.pt2.y + py + ( marginY * marginRatio ) ) * ratio;
      }
      marginX++;
      px += pair.width;
      rH = Math.max( rH, pair.height );
    }

    for( let o = 0; o < pl; o++ ){
      let pair = pairs[ o ],
          uvOb1 = uvStore[ pair.id0 ],
          uvOb2 = pair.id1 !== "none" ? uvStore[ pair.id1 ] : false,
          idx1 = uvOb1.id,
          i0 = indices[ idx1 * 3 ],
          i1 = indices[ idx1 * 3 + 1 ],
          i2 = indices[ idx1 * 3 + 2];
      uvs[ i0 * 2 ] = uvOb1.pt0.x;
      uvs[ i0 * 2 + 1] = uvOb1.pt0.y;
      uvs[ i1 * 2 ] = uvOb1.pt1.x;
      uvs[ i1 * 2 + 1] = uvOb1.pt1.y;
      uvs[ i2 * 2 ] = uvOb1.pt2.x;
      uvs[ i2 * 2 + 1] = uvOb1.pt2.y;
      if( uvOb2 ){
        let idx2 = uvOb2.id,
            i20 = indices[ idx2 * 3 ],
            i21 = indices[ idx2 * 3 + 1 ],
            i22 = indices[ idx2 * 3 + 2];
          uvs[ i20 * 2 ] = uvOb2.pt0.x;
          uvs[ i20 * 2 + 1] = uvOb2.pt0.y;
          uvs[ i21 * 2 ] = uvOb2.pt1.x;
          uvs[ i21 * 2 + 1] = uvOb2.pt1.y;
          uvs[ i22 * 2 ] = uvOb2.pt2.x;
          uvs[ i22 * 2 + 1] = uvOb2.pt2.y;
      }
    }
    return uvs;
  }
  drawPair(uv1, uv2){
    let cv = this.BABYLON.textureCanvas,
        ctx = cv.getContext( "2d" ),
        tex = this.BABYLON.dynamicTexture,
        w = cv.width,
        minX = Math.min(uv1.pt0.x,uv1.pt1.x,uv1.pt2.x,  uv2.pt0.x,uv2.pt1.x,uv2.pt2.x),
        maxX = Math.max(uv1.pt0.x,uv1.pt1.x,uv1.pt2.x,  uv2.pt0.x,uv2.pt1.x,uv2.pt2.x),
        offsetX = -minX,
        ow = maxX - minX,
        minY = Math.min(uv1.pt0.y,uv1.pt1.y,uv1.pt2.y,  uv2.pt0.y,uv2.pt1.y,uv2.pt2.y),
        maxY = Math.max(uv1.pt0.y,uv1.pt1.y,uv1.pt2.y,  uv2.pt0.y,uv2.pt1.y,uv2.pt2.y),
        offsetY = -minY,
        oh = maxY - minY,
        longest = Math.max( maxX + offsetX, oh + offsetY ),
        ratio = (1 / longest) * w;

    let tri1 = {
          pt0: { x: ( uv1.pt0.x + offsetX ) * ratio, y: ( uv1.pt0.y + offsetY ) * ratio },
          pt1: { x: ( uv1.pt1.x + offsetX ) * ratio, y: ( uv1.pt1.y + offsetY ) * ratio },
          pt2: { x: ( uv1.pt2.x + offsetX ) * ratio, y: ( uv1.pt2.y + offsetY ) * ratio },
        },
        d1_01 = Math.sqrt( Math.pow( tri1.pt0.x - tri1.pt1.x, 2 ) + Math.pow( tri1.pt0.y - tri1.pt1.y, 2 ) ),
        d1_12 = Math.sqrt( Math.pow( tri1.pt1.x - tri1.pt2.x, 2 ) + Math.pow( tri1.pt1.y - tri1.pt2.y, 2 ) ),
        d1_20 = Math.sqrt( Math.pow( tri1.pt2.x - tri1.pt0.x, 2 ) + Math.pow( tri1.pt2.y - tri1.pt0.y, 2 ) ),
        center1;
    if( d1_01 >= d1_12 && d1_01 >= d1_20 ){
      center1 = { x: tri1.pt0.x + ( tri1.pt1.x - tri1.pt0.x ) / 2,
                  y: tri1.pt0.y + ( tri1.pt1.y - tri1.pt0.y ) / 2 };
    }else if( d1_12 > d1_01 && d1_12 >= d1_20 ){
      center1 = { x: tri1.pt1.x + ( tri1.pt2.x - tri1.pt1.x ) / 2,
                  y: tri1.pt1.y + ( tri1.pt2.y - tri1.pt1.y ) / 2 };
    }else if( d1_20 > d1_01 && d1_20 > d1_12 ){
      center1 = { x: tri1.pt2.x + ( tri1.pt0.x - tri1.pt2.x ) / 2,
                  y: tri1.pt2.y + ( tri1.pt0.y - tri1.pt2.y ) / 2 };
    }
    let tri2 = {
          pt0: { x: ( uv2.pt0.x + offsetX ) * ratio, y: ( uv2.pt0.y + offsetY ) * ratio },
          pt1: { x: ( uv2.pt1.x + offsetX ) * ratio, y: ( uv2.pt1.y + offsetY ) * ratio },
          pt2: { x: ( uv2.pt2.x + offsetX ) * ratio, y: ( uv2.pt2.y + offsetY ) * ratio },
        },
        d2_01 = Math.sqrt( Math.pow( tri2.pt0.x - tri2.pt1.x, 2 ) + Math.pow( tri2.pt0.y - tri2.pt1.y, 2 ) ),
        d2_12 = Math.sqrt( Math.pow( tri2.pt1.x - tri2.pt2.x, 2 ) + Math.pow( tri2.pt1.y - tri2.pt2.y, 2 ) ),
        d2_20 = Math.sqrt( Math.pow( tri2.pt2.x - tri2.pt0.x, 2 ) + Math.pow( tri2.pt2.y - tri2.pt0.y, 2 ) ),
        center2;
    if( d2_01 >= d2_12 && d2_01 >= d2_20 ){
      center2 = { x: tri2.pt0.x + ( tri2.pt1.x - tri2.pt0.x ) / 2,
                  y: tri2.pt0.y + ( tri2.pt1.y - tri2.pt0.y ) / 2 };
    }else if( d2_12 > d2_01 && d2_12 >= d2_20 ){
      center2 = { x: tri2.pt1.x + ( tri2.pt2.x - tri2.pt1.x ) / 2,
                  y: tri2.pt1.y + ( tri2.pt2.y - tri2.pt1.y ) / 2 };
    }else if( d2_20 > d2_01 && d2_20 > d2_12 ){
      center2 = { x: tri2.pt2.x + ( tri2.pt0.x - tri2.pt2.x ) / 2,
                  y: tri2.pt2.y + ( tri2.pt0.y - tri2.pt2.y ) / 2 };
    }
    let decalX = center1.x - center2.x,
        decalY = center1.y - center2.y

    minX = Math.min( tri1.pt0.x, tri1.pt1.x, tri1.pt2.x, tri2.pt0.x, tri2.pt1.x, tri2.pt2.x ),
    minY = Math.min( tri1.pt0.y, tri1.pt1.y, tri1.pt2.y, tri2.pt0.y, tri2.pt1.y, tri2.pt2.y );

    tri1.pt0.x += -minX;
    tri1.pt1.x += -minX;
    tri1.pt2.x += -minX;
    tri1.pt0.y += -minY;
    tri1.pt1.y += -minY;
    tri1.pt2.y += -minY;

    tri2.pt0.x += -minX;
    tri2.pt1.x += -minX;
    tri2.pt2.x += -minX;
    tri2.pt0.y += -minY;
    tri2.pt1.y += -minY;
    tri2.pt2.y += -minY;

    center1.x += -minX;
    center1.y += -minY;

    ctx.fillStyle = "#e99e60";
    ctx.fillRect( 0, 0, w, w );
    ctx.beginPath();
    ctx.moveTo( tri1.pt0.x, tri1.pt0.y );
    ctx.lineTo( tri1.pt1.x, tri1.pt1.y );
    ctx.lineTo( tri1.pt2.x, tri1.pt2.y );
    ctx.closePath();
    ctx.stroke();

    ctx.beginPath();
    ctx.moveTo( tri2.pt0.x, tri2.pt0.y );
    ctx.lineTo( tri2.pt1.x, tri2.pt1.y );
    ctx.lineTo( tri2.pt2.x, tri2.pt2.y );
    ctx.closePath();
    ctx.stroke();

    ctx.beginPath();
    ctx.moveTo( center1.x, center1.y );
    ctx.arc( center1.x, center1.y , 10, 0, 2*Math.PI, false);

    ctx.moveTo( center2.x, center2.y );
    ctx.arc( center2.x, center2.y , 10, 0, 2*Math.PI, false);
    ctx.fillStyle = "#000000";
    ctx.fill();
    alert("check")
  }
  drawTri(p0,p1,p2){
    let cv = this.BABYLON.textureCanvas,
        ctx = cv.getContext( "2d" ),
        tex = this.BABYLON.dynamicTexture,
        w = cv.width,
        minX = Math.min(p0.x,p1.x,p2.x),
        maxX = Math.max(p0.x,p1.x,p2.x),
        offsetX = -minX,
        ow = maxX - minX,
        minY = Math.min(p0.y,p1.y,p2.y),
        maxY = Math.max(p0.y,p1.y,p2.y),
        offsetY = -minY,
        oh = maxY - minY,
        longest = Math.max( maxX + offsetX, oh + offsetY ),
        ratio = w / longest
    ctx.fillStyle = "#e99e60";
    ctx.fillRect( 0, 0, w, w );
    ctx.beginPath();
    ctx.moveTo( (p0.x + offsetX) * ratio, (p0.y + offsetY) * ratio );
    ctx.lineTo( (p1.x + offsetX) * ratio, (p1.y + offsetY) * ratio );
    ctx.lineTo( (p2.x + offsetX) * ratio, (p2.y + offsetY) * ratio );
    ctx.closePath();
    ctx.stroke();
  }

}
