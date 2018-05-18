//"use strict"
const ipc = require('electron').ipcRenderer;
const bindings = require("bindings");
const td = bindings("towerdef");
const PIXI = require("pixi.js");
const defaults = {
  tileSize: 30,
  mapW: 20,
  mapH: 20,
  menuHeight: 50,
  screenW: window.screen.width,
  screenH: window.screen.height
};
const towerDef = new td.TowerDefense( defaults.mapW, defaults.mapH, [ 0, 0 ], [ defaults.mapW - 1, defaults.mapH - 1 ] );
const app = new PIXI.Application( { width: defaults.screenW,
                                    height: defaults.screenH } );
//console.log( "test : " + JSON.stringify( towerDef.getStructuresDefs() ) );
//console.log( towerDef.getStructureGrid( "shapeL", 3  ).toString() )

const store = {};

setPIXIRenderer();
loadFloorsImgs();

//console.log( "test : " + JSON.stringify( towerDef.getStructuresDefs() ) );

function setPIXIRenderer(){
  //Add the canvas that Pixi automatically created for you to the HTML document
  app.view.id = "pixiCanvas";
  document.body.appendChild( app.view );
  /*
  app.view.addEventListener( "mousedown", e => {
    //console.log( JSON.stringify( app.renderer.plugins.interaction.mouse.global ) );
    let m = app.renderer.plugins.interaction.mouse.global,
        x = Math.floor( m.x / defaults.tileSize ),
        y = Math.floor( m.y / defaults.tileSize );
    console.log( x + ", " + y );
  }, false )
  */
}
function loadFloorsImgs(){
  let _floors = towerDef.getFloors();
  //console.log( JSON.stringify( _floors ) );
  let arr = [];
  for( let k in _floors ){
    arr.push( _floors[ k ].imgUrl );
    arr.push({
      name: k,
      url: _floors[ k ].imgUrl
    })
  }
  PIXI.loader.add( arr )
  .on("progress", loadProgressHandler)
  .load( setupFloorSprites.bind( _floors ) )
}
function loadProgressHandler( loader, resource ){
  console.log("loading: " + resource.url);
  console.log("progress: " + loader.progress + "%");
}
function setupFloorSprites(){
  let _floors = this,
      nbTiles = defaults.mapW * defaults.mapH,
      stageWidth = defaults.tileSize * defaults.mapW,
      stageHeight = defaults.tileSize * defaults.mapH,
      tmpContainer;

  let stageCont = new PIXI.Container();
  stageCont.name = "stageCont";
  stageCont.width = stageWidth;
  stageCont.height = stageHeight;
  stageCont.position.set( 0, 0 );
  stageCont.interactive = true;
  //stageCont.buttonMode = true;
  stageCont.zIndex = 30;
  let mask = new PIXI.Graphics();
  //mask.isMask = true;
  mask.position.set( 0, 0 );
  mask.lineStyle(0);
  mask.beginFill(0x8bc5ff, 0.4);
  mask.moveTo( 0, 0 );
  mask.lineTo( defaults.tileSize * defaults.mapW, 0 );
  mask.lineTo( defaults.tileSize * defaults.mapW, defaults.tileSize * defaults.mapH );
  mask.lineTo( 0, defaults.tileSize * defaults.mapH );
  stageCont.mask = mask;
  stageCont.hitArea = new PIXI.Rectangle( 0, 0, stageWidth, stageHeight );;

  if( nbTiles <= 1500 ){
    tmpContainer = new PIXI.particles.ParticleContainer();
  }else{
    tmpContainer = new PIXI.Container();
  }

  for( let k in _floors ){
    let floorObj = _floors[ k ],
        floorName = k,
        floorPositions = floorObj.positions,
        fpl = floorPositions.length / 2;
    for( let f = 0; f < fpl; f++ ){
      let sprite = new PIXI.Sprite( PIXI.loader.resources[ k ].texture );
      sprite.position.set( floorPositions[ f * 2 ] * defaults.tileSize, floorPositions[ f * 2 + 1 ] * defaults.tileSize );
      sprite.width = defaults.tileSize;
      sprite.height = defaults.tileSize;
      tmpContainer.addChild( sprite );
    }
  }
  let renderTex = PIXI.RenderTexture.create( stageWidth, stageHeight ),
      floorContainer = new PIXI.particles.ParticleContainer();
  floorContainer.name = "floorCont";
  floorContainer.width = stageWidth;
  floorContainer.height = stageHeight;
  floorContainer.position.set( 0, 0 );
  //floorContainer.zIndex = 10;

  app.renderer.render( tmpContainer, renderTex );
  let floorSprite = new PIXI.Sprite( renderTex );
  floorContainer.addChild( floorSprite );

  floorContainer.cacheAsBitmap = true;

  tmpContainer.destroy( true );

  let structuresCont = new PIXI.Container();
  structuresCont.name = "structuresCont";
  structuresCont.width = stageWidth;
  structuresCont.height = stageHeight;
  structuresCont.position.set( 0, 0 );
  structuresCont.zIndex = 30;
  stageCont.addChild( structuresCont );

  app.stage.addChild( floorContainer );
  app.stage.addChild( stageCont );
  app.stage.addChild( mask );

  loadStructuresImgs();
}
function loadStructuresImgs(){
  let _structures = towerDef.getStructuresDefs(),
      arr = [];
  _structures.forEach( struc => {
  //  console.log( "add : " + struc.typeName + " / " + struc.imgUrl )
    arr.push( {
      name: struc.typeName,
      url: struc.imgUrl
    } )
  })
  PIXI.loader.add( arr )
  .on("progress", loadProgressHandler)
  .load( setupStructPicker.bind( _structures ) )

  store.structureDefs = _structures;

}
function setupStructPicker(){
  let _structures = this,
      nbStrucs = _structures.length,
      spriteSize = Math.floor( defaults.menuHeight * 0.8 ),
      spriteTop = defaults.tileSize * defaults.mapH + ( defaults.menuHeight - spriteSize ) * 0.5,
      spriteMargin = spriteSize * 0.20;

  let spritePickerCont = new PIXI.Container();
  spritePickerCont.name = "structPickerCont";
  spritePickerCont.position.set( 0, defaults.tileSize * defaults.mapH + spriteMargin );
  spritePickerCont.interactive = true;

  _structures.forEach( ( struc, i ) => {
    let strucCont = new PIXI.Container();
    strucCont.position.set( spriteMargin + i * ( spriteSize + spriteMargin ), 0 );
    strucCont.name = struc.typeName;
    strucCont.interactive = true;
    //strucCont.buttonMode = true;

    let strucSprite = new PIXI.Sprite( PIXI.loader.resources[ struc.typeName ].texture );
    strucSprite.name = "sprite_" + struc.typeName;
    strucSprite.width = spriteSize;
    strucSprite.height = spriteSize;
    strucSprite.position.set( 0, 0 );

    let text = new PIXI.Text( struc.cost,{ fontFamily: 'Arial', fontSize: 24, fill : 0xe5dfee, align : 'center', wordWrap:true, wordWrapWidth: spriteSize });
    text.anchor.set( 0.5, 0 );
    text.position.set( spriteSize * 0.5, spriteSize + spriteSize * 0.05 );
    strucCont.addChild( strucSprite );
    strucCont.addChild( text );
    spritePickerCont.addChild( strucCont );

    //strucCont.on( "pointerdown", clickStructPicker );

  } )
  app.stage.addChild( spritePickerCont );
  spritePickerCont.on( "click", clickStructPicker );
}
function clickStructPicker( e, _name ){
  cancelStructPosPreview();
  cancelStructPositioning();
  let cont,
      name,
      spritePickerCont = app.stage.getChildByName( "structPickerCont" );
  if( _name ){
    cont = spritePickerCont.getChildByName( _name );
    name = _name;
  }else{
    name = e.target.name;
    cont = spritePickerCont.getChildByName( name );
  }
  resetStructPicker();
  clearStructPreview();
  let sprite = cont.getChildByName( "sprite_" + name );
  sprite.tint = 0xcade6a;
  startStructPreview( name );

}
function resetStructPicker(){
  app.stage.getChildByName( "structPickerCont" ).children.forEach( spriteCont => {
    spriteCont.getChildByName( "sprite_" + spriteCont.name ).tint = 0xFFFFFF;
  })
}
function clearStructPreview(){
  let previewCont = app.stage.getChildByName( "stageCont" ).getChildByName( "structPreviewCont" );
  if( previewCont ) previewCont.destroy( {  children: true,
                                            texture: false,
                                            baseTexture: false } );
}
function startStructPreview( _typeName ){
  //console.log("start");
  clearStructPreview();
  let previewCont = new PIXI.Container();
  previewCont.name = "structPreviewCont";
  previewCont.position.set( 0, 0 );
  previewCont.zIndex = 50;
  let stageCont = app.stage.getChildByName( "stageCont" );
  stageCont.addChild( previewCont );
  previewCont.tint = 0x5727d9;

  let strucSprite = new PIXI.Sprite( PIXI.loader.resources[ _typeName ].texture ),
      def = getStructureDef( _typeName ),
      ts = defaults.tileSize;
  strucSprite.name = "previewSprite";
  strucSprite.width = def.gridWidth * ts;
  strucSprite.height = def.gridHeight * ts;
  strucSprite.alpha = 0.8;
  strucSprite.zIndex = 80;
  strucSprite.anchor.set( 0.0, 0.0 );
  strucSprite.structureData = {
    name: _typeName,
    gridWidth: def.gridWidth,
    gridHeight: def.gridHeight,
    rotation: 0,
    anchor: [ 0, 0 ]
  };

  previewCont.addChild( strucSprite );

  let m = app.renderer.plugins.interaction.mouse.global,
      x = Math.floor( m.x / ts ) - Math.floor( def.gridWidth / 2 ),
      y = Math.floor( m.y / ts ) - Math.floor( def.gridHeight / 2 );
  strucSprite.position.set( x * ts, y * ts );

  stageCont.on( "mousemove", moveStructPosPreview );
  window.addEventListener( "mousedown", cancelStructPosPreview );

  stageCont.on( "mousedown", startStructPositioning );

  if( def.rotates ){
    window.addEventListener( "mousewheel", rotateStructPreview, false );
  }

  stageCont.getChildByName( "structuresCont" ).children.forEach( structSprite => { structSprite.interactive = false; } );

  //stageCont.cursor="none";

}
function cancelStructPosPreview( e ){
  if( e && e.button == 0 ) return false;
  window.removeEventListener( "mousedown", cancelStructPosPreview );
  window.removeEventListener( "mousewheel", rotateStructPreview );
  onDragEnd();
  resetStructPicker();
  app.stage .getChildByName( "stageCont" )
            .getChildByName( "structuresCont" ).children.forEach( structSprite => { structSprite.interactive = true; } );
}
function moveStructPosPreview( e ){
  let m = e.data.getLocalPosition( this ),
      previewSprit = app.stage  .getChildByName( "stageCont" )
                                .getChildByName( "structPreviewCont" )
                                .getChildByName( "previewSprite" ),
      ts = defaults.tileSize,
      structureData = previewSprit.structureData,
      spriteRot = structureData.rotation,
      invertOrientation = spriteRot == 0 || spriteRot == Math.PI ? false : true,
      gridWidth = invertOrientation ? structureData.gridHeight : structureData.gridWidth,
      gridHeight = invertOrientation ? structureData.gridWidth : structureData.gridHeight,
      x = Math.floor( m.x / ts ) - Math.floor( gridWidth / 2 ),
      y = Math.floor( m.y / ts ) - Math.floor( gridHeight / 2 ),
      posData = getStructPosDragData(
        m,
        { start:  { x: x, y: y },
          end:    { x: x + gridWidth,
                    y: y + gridHeight } },
        structureData  ),
      posTest = towerDef.testStructuresPos( posData.structPositions, structureData.name,  structureData.rotation );
      //console.log( "move positions : " + posData.structPositions );
    //  console.log( "move name : " + structureData.name )
      //console.log( "move rot : " + structureData.rotation )
  previewSprit.position.set( x * ts, y * ts );
  previewSprit.tint = posTest[ 0 ] == true ? 0xffffff : 0xff0000;
}
function endStructPosPreview(){
  let stageCont = app.stage.getChildByName( "stageCont" ),
      structPreviewCont = stageCont.getChildByName( "structPreviewCont" );
  if( ! structPreviewCont ) return false;
  structPreviewCont.destroy( {  children: true,
                                texture: false,
                                baseTexture: false } );
  stageCont.cursor = "default";
}
function onDragEnd( e ){
  endStructPosPreview();
  app.stage.getChildByName( "stageCont" ).removeAllListeners();
}
function rotateStructPreview( e ){
  var delta = Math.max( -1, Math.min( 1, e.wheelDelta ) ),
      previewSprit = app.stage  .getChildByName( "stageCont" )
                                .getChildByName( "structPreviewCont" )
                                .getChildByName( "previewSprite" ),
      sRot = previewSprit.structureData.rotation + delta;
  if( sRot >= 4 ){
    sRot = 0;
  }else if( sRot < 0 ){
    sRot = 3;
  }
  previewSprit.structureData.rotation = sRot;
  previewSprit.structureData.anchor = anchorFromRot( sRot );
  let anchor = previewSprit.structureData.anchor;
  previewSprit.anchor.set( anchor[ 0 ], anchor[ 1 ] );
  previewSprit.rotation = ( sRot * 0.5 ) * Math.PI;
}
function getStructureDef( _name ){
  let d = store.structureDefs,
      l = d.length;
  for( let i = 0; i < l; i++ ){
    if( d[ i ].typeName == _name ) return d[ i ];
  }
  return false;
}
function startStructPositioning( e ){
  let stageCont = app.stage.getChildByName( "stageCont" ),
      previewCont = stageCont.getChildByName( "structPreviewCont" ),
      previewSprite = previewCont.getChildByName( "previewSprite" ),
      structPosCont = new PIXI.Container();
  structPosCont.name = "structPosCont";
  structPosCont.srcTex = previewSprite.texture;
  structPosCont.structureData = previewSprite.structureData;
  previewCont.destroy( {  children: true,
                          texture: false,
                          baseTexture: false } );
  structPosCont.position.set( 0, 0 );
  stageCont.addChild( structPosCont );

  structPosCont.invalidFilter = new PIXI.filters.ColorMatrixFilter();
  structPosCont.invalidFilter.contrast();
  /*
  structPosCont.invalidFilter.matrix = [
    1,  0,  0,  0,  0,
    0.15,  0,  0,  0,  0,
    0,  0,  0,  0,  0,
    0,  0,  0,  1,  0,
  ]
  */

  let m = e.data.getLocalPosition( this ),
      ts = defaults.tileSize,
      structureData = structPosCont.structureData,
      x = Math.floor( m.x / ts ) - Math.floor( structureData.gridWidth / 2 ),
      y = Math.floor( m.y / ts ) - Math.floor( structureData.gridHeight / 2 );
  structPosCont.draggedZone = {
    start:  { x: x, y: y },
    end:    { x: x + structureData.gridWidth,
              y: y + structureData.gridHeight }
  };
  let dragAreaGraph = new PIXI.Graphics();
  dragAreaGraph.name = "dragAreaGraph";
  dragAreaGraph.position.set( 0, 0 );
  structPosCont.addChild( dragAreaGraph );
  dragAreaGraph.lineStyle(0);

  stageCont.off( "mousedown", startStructPositioning );
  stageCont.off( "mousemove", moveStructPosPreview );
  window.removeEventListener( "mousedown", cancelStructPosPreview );
  window.addEventListener( "mousedown", cancelStructPositioning, false );
  stageCont.on( "mousemove", dragStructPositionning );
  stageCont.on( "mouseup", onEndStructPositioning );

  dragStructPositionning( e, m );
}
function dragStructPositionning( e, _localPt ){
  let m = _localPt || e.data.getLocalPosition( this ),
      stageCont = app.stage.getChildByName( "stageCont" ),
      structPosCont = stageCont.getChildByName( "structPosCont" ),
      structureData = structPosCont.structureData,

      sRot = structureData.rotation,
      invertOrientation = sRot == 0 || sRot == 2 ? false : true,
      gridWidth = invertOrientation ? structureData.gridHeight : structureData.gridWidth,
      gridHeight = invertOrientation ? structureData.gridWidth : structureData.gridHeight,

      posData = getStructPosDragData( m, structPosCont.draggedZone, structureData  ),
      posTest = towerDef.testStructuresPos( posData.structPositions, structureData.name, structureData.rotation ),
      ts = defaults.tileSize,
      nbSprites = posData.structPositions.length / 2,
      tex = structPosCont.srcTex,
      spw = structureData.gridWidth * ts,
      sph = structureData.gridHeight * ts,
      rot = ( sRot * 0.5 ) * Math.PI,
      anchor = structureData.anchor;
  structPosCont.children.forEach( child => {
    if( child instanceof PIXI.Sprite ) child.destroy();
  } );
  for( let i = 0; i < nbSprites; i++ ){
    let sp = new PIXI.Sprite( tex ),
        isValid = posTest[ i ];
    sp.width = spw;
    sp.height = sph;
    sp.rotation = rot;
    sp.anchor.set( anchor[ 0 ], anchor[ 1 ] );
    sp.position.set( ( posData.structPositions[ i * 2 ] ) * ts, ( posData.structPositions[ i * 2 + 1 ]  ) * ts );
    if( ! isValid ){
      sp.tint = 0xff0000;
      sp.filters = [ structPosCont.invalidFilter ];
    }
    structPosCont.addChild( sp );
  }
  let dragAreaGraph = structPosCont.getChildByName( "dragAreaGraph" );
  dragAreaGraph.clear();
  dragAreaGraph.beginFill( 0xd3d5b4, 0.4 );
  dragAreaGraph.moveTo( posData.drawStart.x * ts, posData.drawStart.y * ts );
  dragAreaGraph.lineTo( posData.drawEnd.x * ts, posData.drawStart.y * ts );
  dragAreaGraph.lineTo( posData.drawEnd.x * ts, posData.drawEnd.y * ts );
  dragAreaGraph.lineTo( posData.drawStart.x * ts, posData.drawEnd.y * ts );
}
function getStructPosDragData( mouse, draggedZone, structureData ){
  let ts = defaults.tileSize,
      mx = Math.floor( mouse.x / ts ),
      my = Math.floor( mouse.y / ts ),
      start = draggedZone.start,
      end = draggedZone.end,
      rotationIndice = structureData.rotation,
      invertSides = rotationIndice == 1 || rotationIndice == 3 ? true : false,
      gridWidth = invertSides ? structureData.gridHeight : structureData.gridWidth,
      gridHeight = invertSides ? structureData.gridWidth : structureData.gridHeight,
      diff = {  x: mx - start.x,
                y: my - start.y},
      delta = { x: Math.abs( diff.x ),
                y:Math.abs( diff.y )},
      axis = delta.x >= delta.y ? "x" : "y",
      direction = diff[ axis ] >= 0 ? 1 : -1,
      drawStart = { x: axis == "x" && direction == -1 ? start.x + gridWidth : start.x,
                    y: axis == "y" && direction == -1 ? start.y + gridHeight : start.y },
      first_end_x = axis == "x" ? mx + ( direction == 1 ? 1 : 0 ) : start.x + gridWidth,
      first_end_y = axis == "y" ? my + ( direction == 1 ? 1 : 0 ) : start.y + gridHeight;
  end.x = axis == "x" && direction == 1 ? Math.max( first_end_x, start.x + gridWidth ) : first_end_x ;
  end.y = axis == "y" && direction == 1 ? Math.max( first_end_y, start.y + gridHeight ) : first_end_y ;

  let dragWidth = Math.abs( ( end.x - drawStart.x ) * ts ),
      dragHeight = Math.abs( ( end.y - drawStart.y ) * ts ),
      nbTileX = Math.floor( dragWidth / ( gridWidth * ts ) ),
      nbTileY = Math.floor( dragHeight / ( gridHeight * ts ) ),
      spritePreviewBoundsWidth = nbTileX * gridWidth * ts * ( axis == "x" && direction == -1 ? -1 : 1),
      spritePreviewBoundsHeight = nbTileY * gridWidth * ts * ( axis == "y" && direction == -1 ? -1 : 1),
      xOffset = axis == "x" && direction == -1 ? 1 : 0,
      yOffset = axis == "y" && direction == -1 ? 1 : 0,
      widthOffset = gridWidth * direction,
      heightOfset = gridHeight * direction,
      structPositions = [];

  for( let tx = 0; tx < nbTileX; tx++ ){
    let posx = drawStart.x + ( tx + xOffset ) * widthOffset;
    for( let ty = 0; ty < nbTileY; ty++ ){
      structPositions.push( posx, drawStart.y + ( ty + yOffset ) * heightOfset );
    }
  }
  return {  drawStart: drawStart,
            drawEnd: end,
            spritePreviewBoundsWidth: spritePreviewBoundsWidth,
            spritePreviewBoundsHeight: spritePreviewBoundsHeight,
            structPositions: structPositions
  }
}
function cancelStructPositioning( e ){
  if( e && e.button == 0 ) return false;
  let stageCont = app.stage.getChildByName( "stageCont" ),
      structPosCont = stageCont.getChildByName( "structPosCont" );
  if( ! structPosCont || ! structPosCont.structureData ) return false;
  let  typeName = structPosCont.structureData.name;
  structPosCont.destroy( {  children: true,
                            texture: false,
                            baseTexture: false } );
  window.removeEventListener( "mousedown", cancelStructPositioning );
  stageCont.off( "mousemove", dragStructPositionning );
  stageCont.off( "mouseup", onEndStructPositioning );
  //startStructPreview( typeName );
  clickStructPicker( false, typeName );
}
function onEndStructPositioning( e ){
  //console.log( "end drag pos" )
  let m = e.data.getLocalPosition( this ),
      stageCont = app.stage.getChildByName( "stageCont" ),
      structPosCont = stageCont.getChildByName( "structPosCont" ),
      structureData = structPosCont.structureData,
      posData = getStructPosDragData( m, structPosCont.draggedZone, structureData  );
  towerDef.addStructures( posData.structPositions, structureData.name, structureData.rotation );
  cancelStructPositioning();
  updateStructures();
  //console.log( JSON.stringify( towerDef.getStructures() ) )
}
function updateStructures(){
  let structuresList = towerDef.getStructures(),
      stageCont = app.stage.getChildByName( "stageCont" ),
      structuresCont = stageCont.getChildByName( "structuresCont" ),
      ts = defaults.tileSize;
  structuresList.forEach( strucType => {
    let tex = PIXI.loader.resources[ strucType.typeName ].texture,
        positions = strucType.positions,
        width = strucType.gridWidth * ts,
        height = strucType.gridHeight * ts,
        pl = positions.length / 4;
    for( let i = 0; i < pl; i++ ){
      let idx = i * 4,
          sId = positions[ idx ],
          sRot = positions[ idx + 1 ],
          sx = positions[ idx + 2 ]  * ts,
          sy = positions[ idx + 3 ] * ts,
          anchor = anchorFromRot( sRot ),
          sp = new PIXI.Sprite( tex );
      sp.width = width;
      sp.height = height;
      sp.position.set( sx, sy );
      sp.name = sId;
      sp.anchor.set( anchor[ 0 ], anchor[ 1 ] );
      sp.rotation = sRot * 0.5 * Math.PI;
      structuresCont.addChild( sp );
      //sp.interactive = true;
      sp.on( "mouseover", structSpriteOver );
      sp.on( "mouseout", strucSpriteOut );
    }
  })
  //console.log( towerDef.getMoveMap() )
  //drawMoveMap();
}
function anchorFromRot( _rot ){
  let anchor = [ 0, 0 ];
  if( _rot == 1 ){
    anchor = [ 0, 1 ];
  }else if( _rot == 2 ){
    anchor = [ 1, 1 ];
  }else if( _rot == 3 ){
    anchor = [ 1, 0 ];
  }else{
    anchor = [ 0, 0 ];
  }
  return anchor;
}
function structSpriteOver( e ){
  e.currentTarget.tint = 0xfae846;
}
function strucSpriteOut( e ){
  e.currentTarget.tint = 0xffffff;
}
function drawMoveMap(){
  let moveMap = towerDef.getMoveMap(),
      stageCont = app.stage.getChildByName( "stageCont" ),
      tmpCont = stageCont.getChildByName( "tmpCont" );
  if( tmpCont ) tmpCont.destroy();
  tmpCont = new PIXI.Container();
  tmpCont.position.set( 0, 0 );
  stageCont.addChild( tmpCont );
  let l = moveMap.length / 3;
  for( let i = 0; i < l; i++ ){
    let val = moveMap[ i * 3 ],
        px = moveMap[ i * 3 + 1 ],
        py = moveMap[ i * 3 + 2 ],
        graph = new PIXI.Graphics(),
        ts = defaults.tileSize;
    if( val > -2 ) continue;
    graph.position.set( 0, 0 );
    tmpCont.addChild( graph );
    graph.lineStyle(0);

    graph.beginFill( 0x99ff33, 1 );
    graph.moveTo( px * ts, py * ts );
    graph.lineTo( px * ts + ts, py * ts );
    graph.lineTo( px * ts + ts, py * ts + ts );
    graph.lineTo( px * ts, py * ts + ts );
  }
}
