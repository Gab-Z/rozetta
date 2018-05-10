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
//const app = new PIXI.Application( { width: defaults.tileSize * defaults.mapW, height: defaults.tileSize * defaults.mapH + defaults.menuHeight * 2 } );
const app = new PIXI.Application( { width: defaults.screenW,
                                    height: defaults.screenH } );
//var floors = towerDef.floors;
const store = {};
//const floorSprites = {};

setPIXIRenderer();
loadFloorsImgs();

/*
let pickCont = startDOMPicker();
document.body.appendChild( pickCont );
*/

console.log( "test : " + JSON.stringify( towerDef.getStructuresDefs() ) );

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
function onclick2( e ){
  let x = Math.floor( e.data.global.x / defaults.tileSize );
  let y = Math.floor( e.data.global.y / defaults.tileSize );
  console.log( x + ", " + y );
}

function loadStructuresImgs(){
  let _structures = towerDef.getStructuresDefs(),
      arr = [];
  _structures.forEach( struc => {
    console.log( "add : " + struc.typeName + " / " + struc.imgUrl )
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
    strucCont.buttonMode = true;

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
  console.log("start");
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
  strucSprite.alpha = 0.4;
  strucSprite.zIndex = 80;
  strucSprite.structureData = {
    name: _typeName,
    gridWidth: def.gridWidth,
    gridHeight: def.gridHeight
  };
  previewCont.addChild( strucSprite );

  let m = app.renderer.plugins.interaction.mouse.global,
      x = Math.floor( m.x / ts ) - Math.floor( def.gridWidth / 2 ),
      y = Math.floor( m.y / ts ) - Math.floor( def.gridHeight / 2 );
  strucSprite.position.set( x * ts, y * ts );

  stageCont.on( "mousemove", dragStructPosPreview );
  window.addEventListener( "mousedown", cancelStructPosPreview );
  stageCont.on( "mousedown", startStructPositioning );
}
function cancelStructPosPreview( e ){
  if( e.button == 0 ) return false;
  window.removeEventListener( "mousedown", cancelStructPosPreview );
  onDragEnd();
  resetStructPicker();
  return false;
}
function dragStructPosPreview( e ){
  let m = e.data.getLocalPosition( this ),
      previewSprit = app.stage  .getChildByName( "stageCont" )
                                .getChildByName( "structPreviewCont" )
                                .getChildByName( "previewSprite" ),
      ts = defaults.tileSize,
      structureData = previewSprit.structureData,
      x = Math.floor( m.x / ts ) - Math.floor( structureData.gridWidth / 2 ),
      y = Math.floor( m.y / ts ) - Math.floor( structureData.gridHeight / 2 );
  previewSprit.position.set( x * ts, y * ts );
}
function endStructPosPreview(){
  let structPreviewCont = app.stage .getChildByName( "stageCont" )
                                    .getChildByName( "structPreviewCont" );
  if( ! structPreviewCont ) return false;
  structPreviewCont.destroy( {  children: true,
                                texture: false,
                                baseTexture: false } );
}
function onDragEnd( e ){
  endStructPosPreview();
  app.stage.getChildByName( "stageCont" ).removeAllListeners();
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
  //let tilingSprite = new PIXI.extras.TilingSprite( store.draggedElement.sprite.texture, 100, 100 );
  let stageCont = app.stage.getChildByName( "stageCont" ),
      previewCont = stageCont.getChildByName( "structPreviewCont" ),
      previewSprite = previewCont.getChildByName( "previewSprite" ),
      structPosCont = new PIXI.Container();
  structPosCont.name = "structPosCont";
  structPosCont.srcTex = previewSprite.texture,
  structPosCont.structureData = previewSprite.structureData;
  previewCont.destroy( {  children: true,
                          texture: false,
                          baseTexture: false } );
  structPosCont.position.set( 0, 0 );
  stageCont.addChild( structPosCont );

  structPosCont.invalidFilter = new PIXI.filters.ColorMatrixFilter();
  structPosCont.invalidFilter.matrix = [
    1,  0,  0,  0,  0,
    0.15,  0,  0,  0,  0,
    0,  0,  0,  0,  0,
    0,  0,  0,  1,  0,
  ]

  stageCont.off( "mousemove", dragStructPosPreview );
  let m = e.data.getLocalPosition( this ),
      ts = defaults.tileSize,
      structData = structPosCont.structureData,
      x = Math.floor( m.x / ts ) - Math.floor( structData.gridWidth / 2 ),
      y = Math.floor( m.y / ts ) - Math.floor( structData.gridHeight / 2 );
  structPosCont.draggedZone = {
    start:  { x: x, y: y },
    end:    { x: x + structData.gridWidth,
              y: y + structData.gridHeight }
  }
  //if( store.draggedElement.dragGraph && store.draggedElement.dragGraph.destroy ) store.draggedElement.dragGraph.destroy();
  let dragAreaGraph = new PIXI.Graphics();
  dragAreaGraph.name = "dragAreaGraph";
  dragAreaGraph.position.set( 0, 0 );
  structPosCont.addChild( dragAreaGraph );
  dragAreaGraph.lineStyle(0);

  //store.structSpriteContainer.on( "rightdown", cancelStructPositioning );
  window.removeEventListener( "mousedown", cancelStructPosPreview );
  window.addEventListener( "mousedown", cancelStructPositioning, false );
  stageCont.on( "mousemove", onMoveStructPositionning );

  stageCont.on( "mouseup", onEndStructPositioning );

  onMoveStructPositionning( e, m );

  //console.log( towerDef.getMoveMap() )
}
function onMoveStructPositionning( e, _localPt ){
  let m = _localPt || e.data.getLocalPosition( this ),
      stageCont = app.stage.getChildByName( "stageCont" ),
      structPosCont = stageCont.getChildByName( "structPosCont" ),
      posData = getStructPosDragData( m, structPosCont.draggedZone, structPosCont.structureData  ),
      ts = defaults.tileSize;
  /*
  store.draggedElement.sprite.position.set( posData.drawStart.x * ts, posData.drawStart.y * ts );
  store.draggedElement.sprite.width = posData.spritePreviewBoundsWidth;
  store.draggedElement.sprite.height = posData.spritePreviewBoundsHeight;
  */
  /*
  PosData{
    drawStart: drawStart,
    drawEnd: end,
    spritePreviewBoundsWidth: spritePreviewBoundsWidth,
    spritePreviewBoundsHeight: spritePreviewBoundsHeight,
    structPositions: structPositions
  }
  */
  let nbSprites = posData.structPositions.length / 2,
      tex = structPosCont.srcTex,
      spw = structPosCont.structureData.gridWidth * ts,
      sph = structPosCont.structureData.gridHeight * ts;

  structPosCont.children.forEach( child => {
    if( child instanceof PIXI.Sprite ) child.destroy();
  } );
  //store.draggedElement.sprites = [];
  for( let i = 0; i < nbSprites; i++ ){
    let sp = new PIXI.Sprite( tex );
    sp.width = spw;
    sp.height = sph;
    sp.position.set( posData.structPositions[ i * 2 ] * ts, posData.structPositions[ i * 2 + 1 ] * ts );
    //store.draggedElement.sprites.push( sp );
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
      //draggedZone = store.draggedZone,
      //dragEl = store.draggedElement,
      start = draggedZone.start,
      end = draggedZone.end,
      diff = {  x: mx - start.x,
                y: my - start.y},
      delta = { x: Math.abs( diff.x ),
                y:Math.abs( diff.y )},
      axis = delta.x >= delta.y ? "x" : "y",
      direction = diff[ axis ] >= 0 ? 1 : -1,
      drawStart = { x: axis == "x" && direction == -1 ? start.x + structureData.gridWidth : start.x,
                    y: axis == "y" && direction == -1 ? start.y + structureData.gridHeight : start.y },
      first_end_x = axis == "x" ? mx + ( direction == 1 ? 1 : 0 ) : start.x + structureData.gridWidth,
      first_end_y = axis == "y" ? my + ( direction == 1 ? 1 : 0 ) : start.y + structureData.gridHeight;
  end.x = axis == "x" && direction == 1 ? Math.max( first_end_x, start.x + structureData.gridWidth ) : first_end_x ;
  end.y = axis == "y" && direction == 1 ? Math.max( first_end_y, start.y + structureData.gridHeight ) : first_end_y ;

  let dragWidth = Math.abs( ( end.x - drawStart.x ) * ts ),
      dragHeight = Math.abs( ( end.y - drawStart.y ) * ts ),
      nbTileX = Math.floor( dragWidth / ( structureData.gridWidth * ts ) ),
      nbTileY = Math.floor( dragHeight / ( structureData.gridHeight * ts ) ),
      spritePreviewBoundsWidth = nbTileX * structureData.gridWidth * ts * ( axis == "x" && direction == -1 ? -1 : 1),
      spritePreviewBoundsHeight = nbTileY * structureData.gridWidth * ts * ( axis == "y" && direction == -1 ? -1 : 1);

  let minX = Math.min( drawStart.x, end.x ),
      minY = Math.min( drawStart.y, end.y );

  let structPositions = [];
  for( let tx = 0; tx < nbTileX; tx++ ){
    let posx = minX + tx * structureData.gridWidth;
    for( let ty = 0; ty < nbTileY; ty++ ){
      structPositions.push( posx, minY + ty * structureData.gridHeight );
    }
  }
  return {
    drawStart: drawStart,
    drawEnd: end,
    spritePreviewBoundsWidth: spritePreviewBoundsWidth,
    spritePreviewBoundsHeight: spritePreviewBoundsHeight,
    structPositions: structPositions
  }
}
function cancelStructPositioning( e ){
  if( e.button == 0 ) return false;
  let stageCont = app.stage.getChildByName( "stageCont" ),
      structPosCont = stageCont.getChildByName( "structPosCont" ),
      typeName = structPosCont.structureData.name;
  structPosCont.destroy( {  children: true,
                            texture: false,
                            baseTexture: false });
  window.removeEventListener( "mousedown", cancelStructPositioning );
  stageCont.off( "mousemove", onMoveStructPositionning );
  stageCont.off( "mouseup", onEndStructPositioning );
  //startStructPreview( typeName );
  clickStructPicker( false, typeName );
}
function onEndStructPositioning( e ){
  console.log( "end drag pos" )
  let m = e.data.getLocalPosition( this ),
      posData = getStructPosDragData( m );
}

  /*
  setCanvas();
  drawGrid( document.getElementById( "canvas0" ) );
  document.getElementById( "canvas0" ).addEventListener( "click", cvClick );

  function cvClick( e ){
    let pos = getMouseTile( e );
    console.log( pos.x + ", " + pos.y );

  }
  function getMouseTile( e ){
    let cv = e.currentTarget;
    return { x: Math.floor( ( e.pageX - cv.offsetLeft ) / defaults.tileSize ),
             y: Math.floor( ( e.pageY - cv.offsetTop ) / defaults.tileSize ) };
  }
  function setCanvas(){
    let mainCont = document.getElementById( "mainCont" );
    mainCont.innerHTML = "";
    let cv = document.createElement( "canvas" ),
        width = defaults.mapW * defaults.tileSize,
        height = defaults.mapH * defaults.tileSize;
    cv.id = "canvas0";
    cv.width = width;
    cv.height = height;
    cv.style.width = width + "px";
    cv.style.height = height + "px";
    cv.style.backgroundColor = "#ffffff";
    mainCont.appendChild( cv );
  }
  function drawGrid( cv ){
    let lineWidth = 2,
        ctx = cv.getContext( "2d" ),
        d = defaults;
    ctx.globalAlpha = 1;
    ctx.fillStyle = "rgba(181, 50, 50, 1)";
    //ctx.fillRect( 0, 0, cv.width, cv.height );

    ctx.fillStyle = "rgba(0, 96, 185, 1)";
    //ctx.lineWidth = .5;
    //ctx.beginPath();
    for( let row = 1; row < d.mapH; row++ ){
      //ctx.moveTo( 0, row * d.tileSize );
      //ctx.lineTo( d.mapW * d.tileSize, row * d.tileSize );
      ctx.fillRect( 0, row * d.tileSize - ( lineWidth * .5 ), d.mapW * d.tileSize, lineWidth  )
    }
    for( let col = 1; col < d.mapW; col++ ){
      //ctx.moveTo( col * d.tileSize, 0 );
      //ctx.lineTo( col * d.tileSize, d.mapH * d.tileSize );
      ctx.fillRect( col * d.tileSize - ( lineWidth * .5 ), 0, lineWidth, d.mapH * d.tileSize )
    }
    //ctx.stroke();
  }
  function drawMap( arr ){
    let cv = document.getElementById( "canvas0" ),
        ctx = cv.getContext( "2d" ),
        d = defaults;
    cv.clear();
    //ctx.clearRect( 0, 0, cv.width, cv.height );
    drawGrid( cv );
    arr.forEach( ( cell, i ) => {
      if( cell < -1 ){
        let pos = to2d( i );
        console.log( "draw : " + pos.x+"/"+pos.y)
        ctx.fillRect( pos.x * d.tileSize, pos.y * d.tileSize, d.tileSize, d.tileSize );
      }
    })
  }
  function to2d( _i ){
    let d = defaults,
        y = Math.floor( _i / d.mapW );
    return {  x : _i - ( y * d.mapW ),
              y : y }
  }

  function startDOMPicker(){
    let _structures = towerDef.getStructuresDefs();
    let cont = document.createElement( "div" );
    cont.id = "pickercont";
    cont.classList.add( "frnscc" );
    _structures.forEach( struc => {
      let pic = new Image();
      let callBackObj = {
        cont: cont
      }
      let cbkFunc = domLoaderCallback.bind( callBackObj );
      callBackObj.func = cbkFunc;
      pic.addEventListener( "load", cbkFunc );
      pic.src = struc.imgUrl;
    })
    return cont;
  }
  function domLoaderCallback( e ){
    let cbkObj = this,
        pic = e.currentTarget,
        cont = cbkObj.cont;
    pic.removeEventListener( "load", cbkObj.func );
    let picCont = cont.appendChild( document.createElement( "div" ) );
    picCont.appendChild( pic );
  }


*/
