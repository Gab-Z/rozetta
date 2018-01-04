const ThreeDToCraftPattern_mesh_defaults = {
  textureSize : 512
};
class ThreeDToCraftPattern_mesh{
  constructor(  name, positions, indices, scene, options = {} ){
    let opts = Object.assign( ThreeDToCraftPattern_mesh_defaults, options );
    this.BABYLON = {};
    this.name = name;
    this.BABYLON.textureSize = opts.textureSize;
    this.BABYLON.mesh = new BABYLON.Mesh( name, scene );
    let vertexData = new BABYLON.VertexData(),
        nbTriangles = indices.length / 3,
        nbPoints = indices.length,
        separatePositions = new Array( indices.length * 3 ),
        separateIndices = new Array( indices.length ),
        generated_uvs =  [],
        generated_normals = [];
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
    uvStore.forEach( el => {
      totalWidth += el.width;

      let offsetY = Math.min( el.pt0.y, el.pt1.y, el.pt2.y, 0 );
      if( offsetY < 0 ){
        offsetY = offsetY * -1;
        el.pt0.y += offsetY;
        el.pt1.y += offsetY;
        el.pt2.y += offsetY;
        el.bounds = {
          minX: Math.min( el.pt0.x, el.pt1.x, el.pt2.x ),
          minY: Math.min( el.pt0.y, el.pt1.y, el.pt2.y ),
          maxX: Math.max( el.pt0.x, el.pt1.x, el.pt2.x ),
          maxY: Math.max( el.pt0.y, el.pt1.y, el.pt2.y )
        }
        el.width = el.bounds.maxX - el.bounds.minX;
        el.height = el.bounds.maxY - el.bounds.minY;
      }
    });
    let rawSize = Math.floor( Math.sqrt( totalWidth ) ) + 1,
        posx = 0,
        posy = 0,
        maxLineHeight = 0,
        margin = 1,
        uvWidth = 0,
        uvHeight = 0;
    uvStore.forEach( ( uvEl, i, arr ) => {
      let iIsPair = Math.floor( i * 0.5 ) === i * 0.5 ? true : false;
      if( posx + uvEl.width + ( iIsPair ? 0 : margin ) > rawSize ){
        uvWidth = Math.max( uvWidth, posx + uvEl.width + ( iIsPair ? 0 : margin ) );
        posx = 0;
        posy += maxLineHeight;
        maxLineHeight = 0;
      }
      maxLineHeight = Math.max( uvEl.height, maxLineHeight );
      uvHeight = Math.max( uvHeight, posy + uvEl.height );
      if( iIsPair ){
        uvEl.offset = { x: posx, y: posy };
      }else{
        uvEl.offset = { x: posx + margin, y: posy };
        let doubleTriangleWidth = Math.max( posx > 0 ? arr[ i - 1 ].width : 0, uvEl.width + margin );
        posx += doubleTriangleWidth;
      }
    });

    let higherDimension = Math.max( uvWidth, uvHeight ),
        resizeRatio = 0.98 / higherDimension;
    uvStore.forEach( uvO => {
      uvO.pt0.x = ( uvO.pt0.x + uvO.offset.x ) * resizeRatio + 0.01;
      uvO.pt0.y = ( uvO.pt0.y + uvO.offset.y ) * resizeRatio + 0.01;
      uvO.pt1.x = ( uvO.pt1.x + uvO.offset.x ) * resizeRatio + 0.01;
      uvO.pt1.y = ( uvO.pt1.y + uvO.offset.y ) * resizeRatio + 0.01;
      uvO.pt2.x = ( uvO.pt2.x + uvO.offset.x ) * resizeRatio + 0.01;
      uvO.pt2.y = ( uvO.pt2.y + uvO.offset.y ) * resizeRatio + 0.01;
      generated_uvs.push( uvO.pt0.x, uvO.pt0.y,
                          uvO.pt1.x, uvO.pt1.y,
                          uvO.pt2.x, uvO.pt2.y );
    })
    BABYLON.VertexData.ComputeNormals( separatePositions, separateIndices, generated_normals );
    vertexData.positions = separatePositions;
    vertexData.indices = separateIndices;
    vertexData.normals = generated_normals;
    vertexData.uvs = generated_uvs;
    vertexData.applyToMesh( this.BABYLON.mesh );

    this.BABYLON.textureCanvas = document.createElement("canvas");
    this.BABYLON.textureCanvas.classList.add("texCanvas");
    this.BABYLON.textureCanvas.id = name + "_texCanvas";
    this.BABYLON.textureCanvas.width = this.BABYLON.textureSize;
    this.BABYLON.textureCanvas.height = this.BABYLON.textureSize;

    this.BABYLON.dynamicTexture = new BABYLON.DynamicTexture( name + "_dynamicTex", this.BABYLON.textureCanvas, scene, true);

    this.BABYLON.material = new BABYLON.StandardMaterial( name + "_mat", scene );
    //this.BABYLON.material.diffuseTexture = this.BABYLON.dynamicTexture;
    this.BABYLON.material.diffuseColor = new BABYLON.Color3(0.5, 0.5, 0.5);
    this.BABYLON.material.specularColor = new BABYLON.Color3(0.1, 0.1, 0.1);
    this.BABYLON.material.emissiveColor = new BABYLON.Color3(0.2, 0.2, 0.2);
    this.BABYLON.material.ambientColor = new BABYLON.Color3(0.23, 0.98, 0.53);
    this.BABYLON.material.backFaceCulling = false;

    this.BABYLON.mesh.material = this.BABYLON.material;
    this.BABYLON.mesh.convertToFlatShadedMesh();

    let texCtx = this.BABYLON.textureCanvas.getContext( "2d" );
    texCtx.fillStyle = "#3a9bab";
    texCtx.fillRect( 0, 0, this.BABYLON.textureCanvas.width, this.BABYLON.textureCanvas.height );
    this.BABYLON.dynamicTexture.update(false);


    console.log(    this.BABYLON.mesh.getIndices().length + ", "
                  + this.BABYLON.mesh.getVerticesData(BABYLON.VertexBuffer.PositionKind ).length / 3  + ", "
                  + this.BABYLON.mesh.getVerticesData(BABYLON.VertexBuffer.UVKind ) .length / 2 + ", "
    );
    this.drawUvs();

    //console.log( JSON.stringify( uvStore ) )
  }
  drawUvs(){
    if( ! document.getElementById( this.name + "_texCanvas" ) ) document.body.appendChild( this.BABYLON.textureCanvas );
    let tex = this.BABYLON.dynamicTexture,
        ctx = tex.getContext(),
        indices = this.BABYLON.mesh.getIndices(),
        uvs = this.BABYLON.mesh.getVerticesData( BABYLON.VertexBuffer.UVKind ),
        cvSize = this.BABYLON.textureSize,
        nbTriangles = indices.length / 3;
    ctx.fillStyle = "#75b6b8";
    ctx.fillRect( 0, 0, cvSize, cvSize );
    for( let i = 0; i < nbTriangles; i++ ){
      let index0 = indices[ i * 3 ],
          index1 = indices[ i * 3 + 1],
          index2 = indices[ i * 3 + 2],
          x0 = uvs[ index0 * 2 ] * cvSize,
          y0 = uvs[ index0 * 2 + 1 ] * cvSize,
          x1 = uvs[ index1 * 2 ] * cvSize,
          y1 = uvs[ index1 * 2 + 1 ] * cvSize,
          x2 = uvs[ index2 * 2 ] * cvSize,
          y2 = uvs[ index2 * 2 + 1 ] * cvSize;
      ctx.beginPath();
      ctx.moveTo( x0, y0 );
      ctx.lineTo( x1, y1 );
      ctx.lineTo( x2, y2 );
      ctx.closePath();
      ctx.stroke();
    }
    tex.update(false);
  }
}
