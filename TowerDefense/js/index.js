const ipc = require('electron').ipcRenderer;
const bindings = require("bindings");
const td = bindings("towerdef");
const PIXI = require("pixi.js");
const defaults = {
  tileSize: 50,
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
  let _floors = this;
  let nbTiles = defaults.mapW * defaults.mapH;
  let tmpContainer;
  if( nbTiles <= 1500 ){
    tmpContainer = new PIXI.particles.ParticleContainer();
  }else{
    tmpContainer = new PIXI.Container();
  }
  let spriteList = [];
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
      spriteList.push( sprite );
      //sprite.destroy();
    }
  }
  store.renderTex = PIXI.RenderTexture.create( defaults.tileSize * defaults.mapW, defaults.tileSize * defaults.mapH );
  store.floorContainer = new PIXI.particles.ParticleContainer();
  store.floorContainer.width = defaults.tileSize * defaults.mapW;
  store.floorContainer.height = defaults.tileSize * defaults.mapH;
  store.floorContainer.position.set( 0, 0 );
  store.floorContainer.zOrder = 10;

  app.renderer.render( tmpContainer, store.renderTex );
  store.floorSprite = new PIXI.Sprite( store.renderTex );

  store.floorContainer.addChild( store.floorSprite );
  app.stage.addChild( store.floorContainer );
  store.floorContainer.cacheAsBitmap = true;

  spriteList.forEach( _sprite => { _sprite.destroy(); } );
  tmpContainer.destroy();

  store.structSpriteContainer = new PIXI.Container();
  store.structSpriteContainer.width = defaults.tileSize * defaults.mapW;
  store.structSpriteContainer.height = defaults.tileSize * defaults.mapH;
  store.structSpriteContainer.position.set( 0, 0 );
  store.structSpriteContainer.zOrder = 30;


  store.structSpriteContainer.interactive = true;
  app.stage.addChild( store.structSpriteContainer );

  let mask = new PIXI.Graphics();
  //mask.isMask = true;
  mask.position.set( 0, 0 );
//  mask.tint = 0x808080;
//  mask.width = defaults.tileSize * defaults.mapW;
//  mask.height =  defaults.tileSize * defaults.mapH;
  app.stage.addChild( mask );
  mask.lineStyle(0);
  mask.clear();
  mask.beginFill(0x8bc5ff, 0.4);
  mask.moveTo( 0, 0 );
  mask.lineTo( defaults.tileSize * defaults.mapW, 0 );
  mask.lineTo( defaults.tileSize * defaults.mapW, defaults.tileSize * defaults.mapH );
  mask.lineTo( 0, defaults.tileSize * defaults.mapH );

  store.structSpriteContainer.mask = mask;

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
  .load( setupStructurePicker.bind( _structures ) )

  store.structureDefs = _structures;

}
function setupStructurePicker(){
  let _structures = this,
      nbStrucs = _structures.length,
      spriteSize = Math.floor( defaults.menuHeight * 0.8 ),
      spriteTop = defaults.tileSize * defaults.mapH + ( defaults.menuHeight - spriteSize ) * 0.5,
      spriteMargin = spriteSize * 0.20;

  store.spriteListContainer = new PIXI.Container();
  store.spriteListContainer.position.set( 0, defaults.tileSize * defaults.mapH + spriteMargin );
  store.spriteListContainer.interactive = true;

  _structures.forEach( ( struc, i ) => {
    console.log( "draw : " + struc.typeName );
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
    store.spriteListContainer.addChild( strucCont );

    //strucCont.on( "pointerdown", clickSpriteList );

  } )
  app.stage.addChild( store.spriteListContainer );
  store.spriteListContainer.on( "click", clickSpriteList );
}
function clickSpriteList( e ){
  console.log( e.target.name );
  let cont = e.target;
  cont.parent.children.forEach( spriteCont => {
    spriteCont.getChildByName( "sprite_" + spriteCont.name ).tint = 0xFFFFFF;
  })
  let sprite = cont.getChildByName( "sprite_" + cont.name );
  sprite.tint = 0xcade6a;
  startStructurePositionning( cont.name )
}
function startStructurePositionning( _typeName ){
  onDragEnd();
  let strucSprite = new PIXI.Sprite( PIXI.loader.resources[ _typeName ].texture ),
      def = getStructureDef( _typeName );
  strucSprite.width = def.gridWidth * defaults.tileSize;
  strucSprite.height = def.gridHeight * defaults.tileSize;
  strucSprite.alpha = 0.4;
  store.structSpriteContainer.addChild( strucSprite );
  store.draggedElement = {  sprite: strucSprite,
                            structureType: _typeName,
                            gridWidth: def.gridWidth,
                            gridHeight: def.gridHeight };

  store.structSpriteContainer.on( "mousemove", onStructPosPreview );
  store.structSpriteContainer.on( "pointerdown", onStartStructPositioning );
  //app.view.addEventListener( "mouseup", onDragEnd, false );
  console.log( "dragg start" );
}

function onStructPosPreview( e ){
  let m = e.data.getLocalPosition( this ),
      x = Math.floor( m.x / defaults.tileSize ) - Math.floor( store.draggedElement.gridWidth / 2 ),
      y = Math.floor( m.y / defaults.tileSize ) - Math.floor( store.draggedElement.gridHeight / 2 );
  store.draggedElement.sprite.position.set( x * defaults.tileSize, y * defaults.tileSize );
}
function endStructPosPreview(){
  if( store.draggedElement ){
    store.draggedElement.sprite.destroy();
    store.draggedElement = false;
  }
}

function onDragEnd( e ){
  endStructPosPreview();
  store.structSpriteContainer.off( "mousemove", onStructPosPreview );
  app.view.removeEventListener( "mouseup", onDragEnd );
}

function getStructureDef( _name ){
  let d = store.structureDefs,
      l = d.length;
  for( let i = 0; i < l; i++ ){
    if( d[ i ].typeName == _name ) return d[ i ];
  }
  return false;
}

function onStartStructPositioning( e ){
  let tilingSprite = new PIXI.extras.TilingSprite( store.draggedElement.sprite.texture, 100, 100 );
  store.draggedElement.sprite.destroy();
  store.draggedElement.sprite = tilingSprite;
//  store.draggedElement.tileContainer = new PIXI.Container();
//  store.draggedElement.tileContainer.addChild( store.draggedElement.sprite );
  store.structSpriteContainer.addChild( store.draggedElement.sprite );
  store.draggedElement.sprite.zOrder = 40;
  store.draggedElement.sprite.tileScale.x = ( store.draggedElement.gridWidth * defaults.tileSize ) / store.draggedElement.sprite.texture.width;
  store.draggedElement.sprite.tileScale.y = ( store.draggedElement.gridHeight * defaults.tileSize ) / store.draggedElement.sprite.texture.height;

  store.structSpriteContainer.off( "mousemove", onStructPosPreview );
  store.structSpriteContainer.off( "pointerdown", onStartStructPositioning );
  let m = e.data.getLocalPosition( this ),
      x = Math.floor( m.x / defaults.tileSize ) - Math.floor( store.draggedElement.gridWidth / 2 ),
      y = Math.floor( m.y / defaults.tileSize ) - Math.floor( store.draggedElement.gridHeight / 2 );
  store.draggedZone = {
    start:  { x: x, y: y },
    end:    { x: x + store.draggedElement.gridWidth,
              y: y + store.draggedElement.gridHeight }
  }
  store.dragGraph = new PIXI.Graphics();
  store.dragGraph.position.set( 0, 0 );
  store.structSpriteContainer.addChild( store.dragGraph );
  store.dragGraph.lineStyle(0);


  store.structSpriteContainer.on( "mousemove", onMoveStructPositionning );
  store.structSpriteContainer.on( "pointerdown", onEndStructPositioning );
}
function onMoveStructPositionning( e ){
  let m = e.data.getLocalPosition( this ),
      mx = Math.floor( m.x / defaults.tileSize ),
      my = Math.floor( m.y / defaults.tileSize ),
      draggedZone = store.draggedZone,
      dragEl = store.draggedElement,
      start = draggedZone.start,
      end = draggedZone.end,
      diff = {  x: mx - start.x,
                y: my - start.y},
      delta = { x: Math.abs( diff.x ),
                y:Math.abs( diff.y )},
      axis = delta.x >= delta.y ? "x" : "y",
      direction = diff[ axis ] >= 0 ? 1 : -1,
      drawStart = { x: axis == "x" && direction == -1 ? start.x + dragEl.gridWidth : start.x,
                    y: axis == "y" && direction == -1 ? start.y + dragEl.gridHeight : start.y },
      first_end_x = axis == "x" ? mx + ( direction == 1 ? 1 : 0 ) : start.x + dragEl.gridWidth,
      first_end_y = axis == "y" ? my + ( direction == 1 ? 1 : 0 ) : start.y + dragEl.gridHeight;
  end.x = axis == "x" && direction == 1 ? Math.max( first_end_x, start.x + dragEl.gridWidth ) : first_end_x ;
  end.y = axis == "y" && direction == 1 ? Math.max( first_end_y, start.y + dragEl.gridHeight ) : first_end_y ;

  let ts = defaults.tileSize,
      dragWidth = Math.abs( ( end.x - drawStart.x ) * ts ),
      dragHeight = Math.abs( ( end.y - drawStart.y ) * ts ),

      nbTileX = Math.floor( dragWidth / ( dragEl.gridWidth * ts ) ),
      nbTileY = Math.floor( dragHeight / ( dragEl.gridHeight * ts ) );
  console.log( nbTileX + "/" + nbTileY );
  store.draggedElement.sprite.position.set( drawStart.x * ts, drawStart.y * ts );
  //store.draggedElement.sprite.width = ( end.x - drawStart.x ) * ts;
  //store.draggedElement.sprite.height = ( end.y - drawStart.y ) * ts;
  store.draggedElement.sprite.width = nbTileX * dragEl.gridWidth * ts * ( axis == "x" && direction == -1 ? -1 : 1);
  store.draggedElement.sprite.height = nbTileY * dragEl.gridWidth * ts * ( axis == "y" && direction == -1 ? -1 : 1);

  store.dragGraph.clear();
  store.dragGraph.beginFill( 0xd3d5b4, 0.4 );
  store.dragGraph.moveTo( drawStart.x * ts, drawStart.y * ts );
  store.dragGraph.lineTo( end.x * ts, drawStart.y * ts );
  store.dragGraph.lineTo( end.x * ts, end.y * ts );
  store.dragGraph.lineTo( drawStart.x * ts, end.y * ts );
}

function onEndStructPositioning( e ){

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
