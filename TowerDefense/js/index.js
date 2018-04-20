console.log(" js page script starts ...")
const ipc = require('electron').ipcRenderer;
const bindings = require("bindings");
const td = bindings("towerdef");
const defaults = {
  tileSize:30,
  mapW:8,
  mapH:8,
  start:{x:1,y:3},
  end:{x:7,y:3}
};
const towerDef = new td.TowerDefense(defaults.mapW,defaults.mapH);

setCanvas();
drawGrid( document.getElementById( "canvas0" ) );
document.getElementById( "canvas0" ).addEventListener( "click", cvClick );
let start = Date.now();
var t = towerDef.testClass();
/*
for( let j = 0; j < 4000; j++ ){
  var arrb = new Array(250000);
  for(let u=0;u <250000;u++){
    arrb[u]=arr[u]
  }
}
var t=0;
*/

console.log( "t : " + t.length + ", " + (Date.now() - start) );
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
