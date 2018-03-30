const ipc = require('electron').ipcRenderer;
const bindings = require("bindings");
const td = bindings("towerdef");
const towerDef = new td.TowerDefense( 80, 50 );
console.log( towerDef.width +", "+towerDef.height+", "+towerDef.length);
const defaults = {
  tileSize:10
}
function drawMap( _map, _options = {} ){
  let options = Object.assign( defaults, _options ),//{ ...defaults, ...moptions },
      cv = document.createElement( "canvas" ),
      ctx = cv.getContext( "2d" ),
      ts = options.tileSize,
      w = _map.width,
      h = _map.height,
      cw = w * ts,
      ch = h * ts;
  cv.width = cw;
  cv.height = ch;
  cv.style.width = cw + "px";
  cv.style.height = ch + "px";
  ctx.lineWidth = .5;
  ctx.strokeStyle = "#000000";
  ctx.beginPath();
  for(let y = 1; y < h; y++ ){
    let ly = y * ts;
    ctx.moveTo( 0, ly );
    ctx.lineTo( cw, ly );
  }
  for( let x = 1; x < w; x ++ ){
    let lx = x * ts;
    ctx.moveTo( lx, 0 );
    ctx.lineTo( lx, ch );
  }
  ctx.stroke();
  cv.addEventListener( "click", function cvClick( e ){
    let cv = e.currentTarget,
        x = Math.floor( ( e.pageX - cv.offsetLeft ) / defaults.tileSize ),
        y = Math.floor( ( e.pageY - cv.offsetTop ) / defaults.tileSize );
    console.log( x+", "+y)
  })
  return cv;
}
document.getElementById( "mainCont" ).appendChild( drawMap( towerDef ) )
