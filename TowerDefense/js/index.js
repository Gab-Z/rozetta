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
  screenH: window.screen.height,
  structureMenu :{
    buttonDiameter: 40,
    buttonMargin: 2
  }
};
const towerDef = new td.TowerDefense( defaults.mapW, defaults.mapH, [ 0, 0 ], [ defaults.mapW - 1, defaults.mapH - 1 ] );
const app = new PIXI.Application( { width: defaults.screenW,
                                    height: defaults.screenH,
                                    antialias: true } );
const listeners = {
  clickStructPicker:{
    on: () => { app.stage.getChildByName( "structPickerCont" ).on( "click", clickStructPicker ); },
    off: () => { app.stage.getChildByName( "structPickerCont" ).off( "click", clickStructPicker ); }
  },
  moveStructPosPreview: {
    on: () => { app.stage.getChildByName( "stageCont" ).on( "mousemove", moveStructPosPreview ); },
    off: () => { app.stage.getChildByName( "stageCont" ).off( "mousemove", moveStructPosPreview ); }
  },
  cancelStructPosPreview: {
    on: () => { window.addEventListener( "mousedown", cancelStructPosPreview, false ); },
    off: () => { window.removeEventListener( "mousedown", cancelStructPosPreview); }
  },
  startStructPositioning: {
    on: () => { app.stage.getChildByName( "stageCont" ).on( "mousedown", startStructPositioning ); },
    off: () => { app.stage.getChildByName( "stageCont" ).off( "mousedown", startStructPositioning ); }
  },
  setStructPreviewRotationByWheel:{
    on: () => { window.addEventListener( "mousewheel", setStructPreviewRotationByWheel, false ); },
    off: () => { window.removeEventListener( "mousewheel", setStructPreviewRotationByWheel, false ); }
  },
  cancelStructPositioning: {
    on: () => { window.addEventListener( "mousedown", cancelStructPositioning, false ); },
    off: () => {  window.removeEventListener( "mousedown", cancelStructPositioning, false );}
  },
  dragStructPositionning: {
    on: () => { app.stage.getChildByName( "stageCont" ).on( "mousemove", dragStructPositionning ); },
    off: () => { app.stage.getChildByName( "stageCont" ).off( "mousemove", dragStructPositionning ); }
  },
  onEndStructPositioning: {
    on: () => { app.stage.getChildByName( "stageCont" ).on( "mouseup", onEndStructPositioning ); },
    off: () => { app.stage.getChildByName( "stageCont" ).off( "mouseup", onEndStructPositioning ); }
  },
  structSelectMove: {
    on: () => { app.stage.getChildByName( "stageCont" ).on( "mousemove", structSelectMove ); },
    off: () => { app.stage.getChildByName( "stageCont" ).off( "mousemove", structSelectMove ); }
  },
  openStructMenu: {
    on: () => { app.stage.getChildByName( "stageCont" ).on( "click", openStructMenu ); },
    off: () => { app.stage.getChildByName( "stageCont" ).off( "click", openStructMenu ); }
  },
  closeStructMenubyOuterClick: {
    on: () => { app.stage.interactive = true;
                app.stage.on( "mouseup", closeStructMenubyOuterClick );
              },
    off: () => {  app.stage.off( "mouseup", closeStructMenubyOuterClick );
                  app.stage.interactive = false;
                }
  },
  cancelStructMenu: {
    on: () => { window.addEventListener( "mousedown", cancelStructMenu, false ); },
    off: () => { window.removeEventListener( "mousedown", cancelStructMenu ); }
  },
  closeStructMenu: {
    on: () => { app.stage.getChildByName( "stageCont" ).on( "mouseup" ); },
    off: () => { app.stage.getChildByName( "stageCont" ).off( "mouseup" ); }
  },
  clickToolsPicker: {
    on: () => { app.stage.getChildByName( "toolsPickerCont" ).on( "click", clickToolsPicker );},
    off: () => { app.stage.getChildByName( "toolsPickerCont" ).off( "click", clickToolsPicker ); }
  },
  dragMove: {
    on: () => { app.stage.getChildByName( "stageCont" ).on( "mousemove", dragMove ); },
    off: () => { app.stage.getChildByName( "stageCont" ).off( "mousemove", dragMove ); }
  },
  endDrag: {
    on: () => { app.stage.getChildByName( "stageCont" ).on( "mouseup", endDrag ); },
    off: () => { app.stage.getChildByName( "stageCont" ).off( "mouseup", endDrag ); }
  },
  cancelDrag: {
    on: () => { window.addEventListener( "mousedown", cancelDrag, false ); },
    off: () => { window.removeEventListener( "mousedown", cancelDrag ); }
  },
  dragPreviewMove: {
    on: () => { app.stage.getChildByName( "stageCont" ).on( "mousemove", dragPreviewMove ); },
    off: () => { app.stage.getChildByName( "stageCont" ).off( "mousemove", dragPreviewMove ); }
  },
  startDrag: {
    on: () => { app.stage.getChildByName( "stageCont" ).on( "mousedown", startDrag ); },
    off: () => { app.stage.getChildByName( "stageCont" ).off( "mousedown", startDrag ); }
  },
  cancelDragPreview:{
    on: () => { window.addEventListener( "mousedown", cancelDragPreview, false ); },
    off: () => { window.removeEventListener( "mousedown", cancelDragPreview ); }
  }
}
const evtStates = {
  empty: [],
  basicSelect: [
    "clickStructPicker",
    "openStructMenu",
    "structSelectMove",
    "clickToolsPicker"
  ],
  structPosPreviewNoRotation:[
    "moveStructPosPreview",
    "clickStructPicker",
    "cancelStructPosPreview",
    "startStructPositioning",
    "clickToolsPicker"
  ],
  structPosPreviewWithRotation: [
    "moveStructPosPreview",
    "clickStructPicker",
    "cancelStructPosPreview",
    "startStructPositioning",
    "setStructPreviewRotationByWheel",
    "clickToolsPicker"
  ],
  structPosDrag: [
    "dragStructPositionning",
    "onEndStructPositioning",
    "cancelStructPositioning"
  ],
  openedStructMenu: [
    "clickStructPicker",
    "closeStructMenubyOuterClick",
    "cancelStructMenu",
    "clickToolsPicker"
  ],
  startDragPreview: [
    "dragPreviewMove",
    "startDrag",
    "cancelDragPreview"
  ],
  drag: [
    "dragMove",
    "endDrag",
    "cancelDrag"
  ]

}
var activeEvtStateName = "empty";
var dragData;
function initDragData(){
  dragData.positions = {  start:  { x: 0, y: 0 },
                          end:    { x: 0, y: 0 } }
}
function setEvtState( stateName ){
  console.log( "setEvtState : " + stateName );
  evtStates[ activeEvtStateName ].forEach( listenerName => {
    listeners[ listenerName ].off();
  });
  activeEvtStateName = stateName;
  evtStates[ activeEvtStateName ].forEach( listenerName => {
    listeners[ listenerName ].on();
  });
}
var activeTool = {
  name: "",
  texName:""
}

const funcTools = {

  destroyStructures: {
    init: ( e ) => {
      dragData = {
        callback: destroyStructZone,
        drawFunc: "drawDragSquare",
        positions: false
      }
      let stageCont = app.stage.getChildByName( "stageCont" ),
          previewCont = stageCont.addChild( new PIXI.Container() ),
          prevSprite = previewCont.addChild( new PIXI.Sprite( PIXI.loader.resources[ activeTool.texName ].texture ) ),
          ts = defaults.tileSize,
          m = e.data.getLocalPosition( stageCont );
      previewCont.name = "destructToolCont";
      prevSprite.name = "destructToolPrevSprite";
      prevSprite.width = ts;
      prevSprite.height = ts;
      dragPreviewMove( false, m );
      setEvtState( "startDragPreview" );
    },
    cancel: () => {
      app.stage.getChildByName( "stageCont" ).getChildByName( "destructToolCont" ).destroy( {   children: true, texture: false, baseTexture: false } );
    }
  }

}
function clickToolsPicker( e ){
  console.log( "tools picker clicked" );
  if( ! e || ! e.target || ! e.target.funcName  ) return false;
  activeTool.name = e.target.funcName;
  activeTool.texName = e.target.texName;
  funcTools[ activeTool.name ].init( e );
}

const store = {};

setPIXIRenderer();
//loadFloorsImgs();
loadCommonTextures();
//console.log( "test : " + JSON.stringify( towerDef.getStructuresDefs() ) );
console.log( "test : " + JSON.stringify( towerDef.getCommonTextures() ) );

function setPIXIRenderer(){
  //Add the canvas that Pixi automatically created for you to the HTML document
  app.view.id = "pixiCanvas";
  document.body.appendChild( app.view );
}
function loadCommonTextures(){
  PIXI.loader.add( towerDef.getCommonTextures() )
  .on("progress", loadProgressHandler)
  .load( loadFloorsImgs )
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
  stageCont.hitArea = new PIXI.Rectangle( 0, 0, window.innerWidth, window.innerHeight );

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
function setupToolsPicker(){
  let toolsPickerCont = new PIXI.Container(),
      spriteSize = Math.floor( defaults.menuHeight * 0.8 ),
      margin = 5,
      spriteSlotSize = spriteSize + margin * 2,
      buttonList = [
        { textureName: "destroyStructure",
          funcName: "destroyStructures" }
      ],
      direction = {
        x: 0,
        y: 1
      }
  toolsPickerCont.name = "toolsPickerCont";
  toolsPickerCont.interactive = true;
  buttonList.forEach( ( buttOb, i ) => {
    let sprite = toolsPickerCont.addChild( new PIXI.Sprite( PIXI.loader.resources[ buttOb.textureName ].texture ) ),
        sizeRatio = spriteSize / Math.max( sprite.texture.width, sprite.texture.height );
    sprite.width = sprite.texture.width * sizeRatio;
    sprite.height = sprite.texture.height * sizeRatio;
    sprite.position.set( direction.x * i * spriteSlotSize + margin , direction.y * i * spriteSlotSize + margin );
    sprite.funcName = buttOb.funcName;
    sprite.texName = buttOb.textureName;
    sprite.interactive = true;
  })

  toolsPickerCont.position.set( defaults.mapW * defaults.tileSize, 0 );
  app.stage.addChild( toolsPickerCont );
  setEvtState( "basicSelect" );
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
    let sizeRatio = spriteSize / Math.max( strucSprite.texture.width, strucSprite.texture.height );
    strucSprite.width = strucSprite.texture.width * sizeRatio;
    strucSprite.height = strucSprite.texture.height * sizeRatio;
    strucSprite.position.set( 0, 0 );

    let text = new PIXI.Text( struc.cost,{ fontFamily: 'Arial', fontSize: 24, fill : 0xe5dfee, align : 'center', wordWrap:true, wordWrapWidth: spriteSize });
    text.anchor.set( 0.5, 0 );
    text.position.set( spriteSize * 0.5, spriteSize + spriteSize * 0.05 );
    strucCont.addChild( strucSprite );
    strucCont.addChild( text );
    spritePickerCont.addChild( strucCont );
  } )
  app.stage.addChild( spritePickerCont );

  setupToolsPicker();
  drawWays();
}
function clickStructPicker( e, _name, _rotation ){
  cancelStructPosPreview();
  cancelStructPositioning();
  closeStructMenu();
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
  cancelStructureSelection();
  let sprite = cont.getChildByName( "sprite_" + name );
  sprite.tint = 0xcade6a;
  startStructPreview( name, _rotation || 0 );
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
function startStructPreview( _typeName, _rotation ){
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
    rotation: _rotation || 0,
    anchor: [ 0, 0 ]
  };
  previewCont.addChild( strucSprite );
  rotateStructPreview();
  let m = app.renderer.plugins.interaction.mouse.global,
      x = Math.floor( m.x / ts ) - Math.floor( def.gridWidth / 2 ),
      y = Math.floor( m.y / ts ) - Math.floor( def.gridHeight / 2 );
  strucSprite.position.set( x * ts, y * ts );
  if( def.rotates ){
    setEvtState( "structPosPreviewWithRotation" );
  }else{
    setEvtState( "structPosPreviewNoRotation" );
  }
  stageCont.getChildByName( "structuresCont" ).children.forEach( structSprite => { structSprite.interactive = false; } );
}
function cancelStructPosPreview( e ){
  if( e && e.button == 0 ) return false;
  endStructPosPreview();
  resetStructPicker();
  app.stage .getChildByName( "stageCont" )
            .getChildByName( "structuresCont" ).children.forEach( structSprite => { structSprite.interactive = true; } );
  setEvtState( "basicSelect" );
}
function moveStructPosPreview( e ){
  console.log("moveStructPosPreview");
  let m = e.data.getLocalPosition( this ),
      previewSprit = app.stage  .getChildByName( "stageCont" )
                                .getChildByName( "structPreviewCont" )
                                .getChildByName( "previewSprite" ),
      ts = defaults.tileSize,
      structureData = previewSprit.structureData,
      spriteRot = structureData.rotation,
      invertOrientation = spriteRot == 0 || spriteRot == 2 ? false : true,
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
  previewSprit.position.set( x * ts, y * ts );
  previewSprit.tint = posTest[ 0 ] == true ? 0xffffff : 0xff0000;
}
function endStructPosPreview(){

  let stageCont = app.stage.getChildByName( "stageCont" ),
      structPreviewCont = stageCont.getChildByName( "structPreviewCont" );
  if( ! structPreviewCont ) return false;
  console.log("endStructPosPreview");
  structPreviewCont.destroy( {  children: true,
                                texture: false,
                                baseTexture: false } );
  stageCont.cursor = "default";
}
function setStructPreviewRotationByWheel( e ){
  let delta = Math.max( -1, Math.min( 1, e.wheelDelta ) );
  setStructPreviewRotation( delta );
}
function setStructPreviewRotation( delta ){
  let previewSprit = app.stage  .getChildByName( "stageCont" )
                                .getChildByName( "structPreviewCont" )
                                .getChildByName( "previewSprite" ),
      sRot = previewSprit.structureData.rotation + delta;
  if( sRot > 3 ){
    sRot = 0;
  }else if( sRot < 0 ){
    sRot = 3;
  }
  previewSprit.structureData.rotation = sRot;
  rotateStructPreview();
}
function rotateStructPreview(){
  let previewSprit = app.stage  .getChildByName( "stageCont" )
                                .getChildByName( "structPreviewCont" )
                                .getChildByName( "previewSprite" ),
      structureData = previewSprit.structureData,
      sRot = structureData.rotation,
      anchor = anchorFromRot( sRot );
  previewSprit.structureData.rotation = sRot;
  previewSprit.anchor.set( anchor[ 0 ], anchor[ 1 ] );
  previewSprit.rotation = ( sRot * 0.5 ) * Math.PI;
  previewSprit.structureData.anchor = anchor;
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
  setEvtState( "structPosDrag" );
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
  let typeName = structPosCont.structureData.name,
      rotation = structPosCont.structureData.rotation;
  structPosCont.destroy( {  children: true,
                            texture: false,
                            baseTexture: false } );
  clickStructPicker( false, typeName, rotation );
}
function onEndStructPositioning( e ){
  let m = e.data.getLocalPosition( this ),
      stageCont = app.stage.getChildByName( "stageCont" ),
      structPosCont = stageCont.getChildByName( "structPosCont" ),
      structureData = structPosCont.structureData,
      posData = getStructPosDragData( m, structPosCont.draggedZone, structureData  ),
      addstructures = towerDef.addStructures( posData.structPositions, structureData.name, structureData.rotation );
  if( addstructures == false ) console.log( "You can't close any part of the map" );
  cancelStructPositioning();
  updateStructures();
}
function updateStructures(){
  let structuresList = towerDef.getStructures(),
      stageCont = app.stage.getChildByName( "stageCont" ),
      structuresCont = stageCont.getChildByName( "structuresCont" ),
      ts = defaults.tileSize,
      nbC = structuresCont.children.length;
  for( let d  = 0; d < nbC; d++ ){
    structuresCont.children[ 0 ].destroy();
  }
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
      sp.name = "sucture_" + sId;
      sp.id = sId;
      sp.structure = true;
      sp.anchor.set( anchor[ 0 ], anchor[ 1 ] );
      sp.rotation = sRot * 0.5 * Math.PI;
      structuresCont.addChild( sp );
      sp.interactive = true;
      //sp.on( "mouseover", structSpriteOver );
      //sp.on( "mouseout", strucSpriteOut );
    }
  })
  //console.log( towerDef.getMoveMap() )
  //drawMoveMap();
}
function cancelStructureSelection(){
  console.log("cancelStructureSelection");
  setEvtState( "basicSelect" );
  structSelectMove( false );
}
function structSelectMove( e ){
  console.log("move");
  let passEvent = e.target ? e : false;
  window.requestAnimationFrame( structSelectMoveRAF.bind( passEvent ) );
}
function structSelectMoveRAF(){
  let e = this,
      target = e.target || false,
      structuresCont = app.stage.getChildByName( "stageCont" ).getChildByName( "structuresCont" ),
      old = structuresCont.getChildByName( "structHover" ),
      oldId = old && old.id ? old.id : -1,
      targetId = target && target.structure ? target.id : 0,
      ts = defaults.tileSize,
      pt = targetId ? e.data.global : 0,
      tilePt = targetId ? { x: Math.floor( pt.x / ts ), y: Math.floor( pt.y / ts ) } : 0,
      isPtOnStruct = targetId ? towerDef.isPtOnStructById( targetId, tilePt.x, tilePt.y ) : 0;
  if( oldId > 0 && ( oldId != targetId || ! isPtOnStruct ) ){
    old.tint = 0xffffff;
    old.name = "sucture_" + old.id;
  }
  if( isPtOnStruct ){
    target.tint = 0xfae846;
    target.name = "structHover";
  }
}
function anchorFromRot( _rot ){
  let anchor;
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
function openStructMenu( e ){
  if( ! e.target || ! e.target.structure ) return false;
  //closeStructMenu();
  let sprite = e.target,
      pt = e.data.global,
      ts = defaults.tileSize,
      tilePt = {  x: Math.floor( pt.x / ts ),
                  y: Math.floor( pt.y / ts ) };
  if( ! towerDef.isPtOnStructById( sprite.id, tilePt.x, tilePt.y ) ) return false;
  let structureData = towerDef.getStructureById( sprite.id ),
      stageCont = app.stage.getChildByName( "stageCont" ),
      menuCont = stageCont.addChild( new PIXI.Container() ),
      centerx = ( structureData.x + structureData.gridWidth / 2 ) * ts,
      centery = ( structureData.y + structureData.gridHeight / 2 ) * ts;
  menuCont.name = "menuCont";
  let menu = menuCont.addChild( drawStructMenu( sprite.id ) );
  menu.position.set( centerx, centery );
  setEvtState( "openedStructMenu" );
}
function drawStructMenu( structureId ){
  let menuGraph = new PIXI.Graphics(),
      menuData = defaults.structureMenu,
      buttonDiameter = menuData.buttonDiameter,
      buttonRadius = buttonDiameter * 0.5,
      buttonSlotSize = buttonDiameter + 2 * menuData.buttonMargin,
      menuRows = 2,
      menuCols = 3,
      menuWidth = menuCols * buttonSlotSize,
      menuHeight = menuRows * buttonSlotSize;
  menuGraph.name = "structureMenu"
  menuGraph.beginFill( 0x4c84a7, 0.8 );
  menuGraph.drawRoundedRect (0, 0, menuWidth, menuHeight, buttonRadius * 0.75 );
  menuGraph.interactive = true;
  let destroyBut = createStructMenuBut( "destroyStructure", buttonRadius );
  destroyBut.position.set( buttonRadius +  buttonSlotSize , buttonRadius + ( menuRows - 1 ) * buttonSlotSize );
  destroyBut.interactive = true;
  menuGraph.addChild( destroyBut );
  destroyBut.targetId = structureId;
  destroyBut.on( "click", clickDestroyBut );
  return menuGraph;
}
function closeStructMenubyOuterClick( e ){
  if( e.target && ! isDeepChildOf( e.target, "menuCont", "stageCont" ) ){
    console.log( "outer close" );
    closeStructMenu();
    setEvtState( "basicSelect" );
  }
}
function cancelStructMenu( e ){
  if( e && e.button == 0 ) return false;
  console.log("cancelStructMenu xxx");
  closeStructMenu();
  setEvtState( "basicSelect" );
}
function closeStructMenu(){
  let stageCont = app.stage.getChildByName( "stageCont" ),
      menuCont = stageCont.getChildByName( "menuCont" );
  if( menuCont ){ menuCont.destroy( { children: true, texture: false, baseTexture: false } ); }
}
function createStructMenuBut( texName, radius ){
  let tex = PIXI.loader.resources[ texName ].texture,
      graph = new PIXI.Graphics(),
      sprite = new PIXI.Sprite( tex ),
      //longestTexSize = Math.max( tex.width, tex.height),
      texHalfW = tex.width * 0.5,
      texHalfH = tex.height * 0.5,
      texDiagonal = Math.sqrt( texHalfW * texHalfW + texHalfH * texHalfH ),
      butDiameter = radius * 2,
      sizeRatio = radius / texDiagonal;
  graph.beginFill( 0x1f3159, 0.9 );
  graph.drawCircle( 0, 0, radius );
  sprite.anchor.set( 0.5, 0.5 );
  sprite.width = tex.width * sizeRatio;
  sprite.height = tex.height * sizeRatio;
  graph.addChild( sprite );
  return graph;
}
function clickDestroyBut( e ){
  console.log( e.currentTarget.targetId );
  closeStructMenu();
  let destroy = towerDef.destroyStructById( e.currentTarget.targetId );
  if( destroy == 0 ){
    console.log( "You can't remove a structure that would create a hole in the map" );
  }
  updateStructures();
}
function isDeepChildOf( startElement, searchedParentName, endElementName ){
  let searchEl = startElement;
  for( let i = 0; i >= 0; i++ ){
    if( searchEl == app.stage ) return false;
    let parent  = searchEl.parent;
    if( parent && parent.name){
      if( parent.name == searchedParentName ){
        return true;
      }else if( parent.name == endElementName ){
        return false
      }
    }else{
      return false;
    }
    searchEl = parent;
  }
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
function drawWays(){
  let floorCont = app.stage.getChildByName( "floorCont" ),
      floorContIndex = app.stage.getChildIndex( floorCont ),
      ways = towerDef.getWays(),
      startPoints = ways.startPoints,
      sl = startPoints.length / 2,
      endPoints = ways.endPoints,
      el = endPoints.length / 2,
      ts = defaults.tileSize,
      waysCont = new PIXI.Container(),
      startGraph = new PIXI.Graphics(),
      endGraph = new PIXI.Graphics();
      //alert( "floor : " + floorContIndex + " // " + app.stage.getChildIndex( stageCont ) );
  waysCont.name = "waysCont";
  waysCont.position.set( 0, 0 );

  waysCont.addChild( startGraph );
  waysCont.addChild( endGraph );

  app.stage.addChildAt( waysCont, floorContIndex + 1 );
  startGraph.beginFill( 0x6ef51c, 0.6 );
  for( let s  = 0; s < sl; s++ ){
    startGraph.drawRect( startPoints[ s * 2 ] * ts, startPoints[ s * 2 + 1 ] * ts, ts, ts );
  }
  endGraph.beginFill( 0xc73229, 0.6 );
  for( let e  = 0; e < el; e++ ){
    endGraph.drawRect( endPoints[ e * 2 ] * ts, endPoints[ e * 2 + 1 ] * ts, ts, ts );
  }
}
function dragPreviewMove( e, _m ){
  let m = _m || e.data.getLocalPosition( this ),
      ts = defaults.tileSize,
      x = Math.floor( m.x / ts ),
      y = Math.floor( m.y / ts );
  window.requestAnimationFrame( dragPreviewMoveRAF.bind( { x: x, y: y} ) );
}
function dragPreviewMoveRAF(){
  let destructCont = app.stage.getChildByName( "stageCont" ).getChildByName( "destructToolCont" );
  if( ! destructCont ) return false;
  let sprite = destructCont.getChildByName( "destructToolPrevSprite" ),
      posOb = this,
      ts = defaults.tileSize;
  sprite.position.set( posOb.x * ts, posOb.y * ts );
}
function cancelDragPreview( e ){
  if( e && e.button == 0 ) return false;
  app.stage.getChildByName( "stageCont" ).getChildByName( "destructToolCont" ).destroy( {   children: true,
                                                                                            texture: false,
                                                                                            baseTexture: false } );
  setEvtState( "basicSelect" );
}
function destroyStructZone(){
  console.log("destroyStructZone");
  let d = dragData.positions;
  let ret = towerDef.destroyStructsByZone( d.start.x, d.start.y, d.end.x, d.end.y );
  console.log(ret);
  updateStructures();
}
function dragMove( e, _m ){
  let stageCont = e.currentTarget,
      m = _m || e.data.getLocalPosition( this ),
      ts = defaults.tileSize,
      x = Math.floor( m.x / ts ),
      y = Math.floor( m.y / ts );
  dragData.positions.end.x = x;
  dragData.positions.end.y = y;
  console.log("dragMove");
  if( dragData.drawFunc ) window.requestAnimationFrame( dragDrawFuncs[ dragData.drawFunc ].draw );
}
function startDrag( e ){
  funcTools[ activeTool.name ].cancel( e );
  let m = e.data.getLocalPosition( this ),
      ts = defaults.tileSize,
      x = Math.floor( m.x / ts ),
      y = Math.floor( m.y / ts );
  initDragData();
  dragData.positions.start.x = x;
  dragData.positions.start.y = y;
  if( dragData.drawFunc ) dragDrawFuncs[ dragData.drawFunc ].init();
  dragMove( false, m );
  setEvtState( "drag" );
}
function endDrag( e ){
  if( dragData.drawFunc ) dragDrawFuncs[ dragData.drawFunc ].cancel();
  dragData.callback();
  setEvtState( "basicSelect" );
}
function cancelDrag( e ){
  if( e && e.button == 0 ) return false;
  if( dragData.drawFunc ) dragDrawFuncs[ dragData.drawFunc ].cancel();
  funcTools[ activeTool.name ].init();
}
const dragDrawFuncs = {
  drawDragSquare: {
    init: ()=>{
      let drawCont = app.stage.getChildByName( "stageCont" ).addChild( new PIXI.Container() ),
          squareGraph = drawCont.addChild( new PIXI.Graphics() ),
          sprite = drawCont.addChild( new PIXI.Sprite( PIXI.loader.resources[ activeTool.texName ].texture ) ),
          ts = defaults.tileSize;
      drawCont.name = "dragDrawCont";
      squareGraph.name = "dragDrawGraph";
      sprite.name = "dragDrawSprite";
      sprite.width = ts;
      sprite.height = ts;
    },
    draw: ()=>{
      let drawCont = app.stage.getChildByName( "stageCont" ).getChildByName( "dragDrawCont");
      if( ! drawCont ) return false;
      let squareGraph = drawCont.getChildByName( "dragDrawGraph" ),
          sprite = drawCont.getChildByName( "dragDrawSprite" ),
          pos = dragData.positions,
          ts = defaults.tileSize,
          sx = Math.min( pos.start.x, pos.end.x ),
          sy = Math.min( pos.start.y, pos.end.y ),
          ex = Math.max( pos.start.x, pos.end.x ),
          ey = Math.max( pos.start.y, pos.end.y ),
          offsetx = pos.start.x < pos.end.x ? 1 : 0,
          offsety = pos.start.y < pos.end.y ? 1 : 0;

      squareGraph.clear();
      squareGraph.beginFill( 0xd3d5b4, 0.4 );
      squareGraph.drawRoundedRect ( sx * ts, sy * ts, (ex - sx + 1 ) * ts, ( ey - sy + 1 ) * ts, 0 );

      sprite.position.set( pos.end.x *ts, pos.end.y * ts );
    },
    cancel: ()=>{
      app.stage.getChildByName( "stageCont" ).getChildByName( "dragDrawCont" ).destroy( { children: true, texture: false, baseTexture: false } );
    }
  }
}
