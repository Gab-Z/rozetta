console.log(" js page script starts ...");
HTMLCanvasElement.prototype.clear = function(){
  let w = this.width, h = this.height;
  this.getContext( "2d" ).clearRect( 0, 0, this.width, this.height );
}
const ipc = require('electron').ipcRenderer;
const bindings = require("bindings");
const td = bindings("towerdef");
const PIXI = require("pixi.js");
const defaults = {
  tileSize:30,
  mapW:20,
  mapH:20
};
const towerDef = new td.TowerDefense( defaults.mapW, defaults.mapH, [ 0, 0 ], [ defaults.mapW - 1, defaults.mapH - 1 ] );
const app = new PIXI.Application( { width: defaults.tileSize * defaults.mapW, height: defaults.tileSize * defaults.mapH } );
//var floors = towerDef.floors;
const store = {};
//const floorSprites = {};

setPIXIRenderer();
loadFloorsImgs();

//console.log( JSON.stringify( towerDef.getStructuresDefs() ) );

function setPIXIRenderer(){
  //Add the canvas that Pixi automatically created for you to the HTML document
  app.view.id = "pixiCanvas";
  document.body.appendChild(app.view);
  app.view.addEventListener( "mousedown", e => {
    //console.log( JSON.stringify( app.renderer.plugins.interaction.mouse.global ) );
    let m = app.renderer.plugins.interaction.mouse.global,
        x = Math.floor( m.x / defaults.tileSize ),
        y = Math.floor( m.y / defaults.tileSize );
    console.log( x + ", " + y );
  }, false )
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
  app.renderer.render( tmpContainer, store.renderTex );
  store.floorSprite = new PIXI.Sprite( store.renderTex );

  store.floorContainer.addChild( store.floorSprite );
  app.stage.addChild( store.floorContainer );
  store.floorContainer.cacheAsBitmap = true;
  spriteList.forEach( _sprite => { _sprite.destroy(); } );
  tmpContainer.destroy();
}
function onclick2( e ){
  let x = Math.floor( e.data.global.x / defaults.tileSize );
  let y = Math.floor( e.data.global.y / defaults.tileSize );
  console.log( x + ", " + y );
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
*/
