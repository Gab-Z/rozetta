const ipc = require('electron').ipcRenderer;
const bindings = require("bindings");
const td = bindings("towerdef");
const towerDef = new td.TowerDefense( 8,6, 0,0, 7,5 );
console.log( towerDef.width +", "+towerDef.height+", "+towerDef.length);
const defaults = {
  tileSize:60
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
        ctx = cv.getContext( "2d" ),
        x = Math.floor( ( e.pageX - cv.offsetLeft ) / defaults.tileSize ),
        y = Math.floor( ( e.pageY - cv.offsetTop ) / defaults.tileSize );

    let add = towerDef.addStructure( x, y, "Wall")
    console.log( "job : " + x+", "+y+" / "+( add==false ? "false" :("true"+ add.length)));
    if(add == false ){
      console.log("false")
    }else{
      console.log("array: "+add.length+" : "+ add.toString())
    }
    if(add.length){
    //  ctx.clearRect(0,0,cv.width, cv.height)
      add.forEach( el => {
        if(el.hasOwnProperty("x")){
          ctx.fillRect(el.x * defaults.tileSize, el.y * defaults.tileSize, defaults.tileSize, defaults.tileSize )
        }
      })
    }
  })
  return cv;
}
document.getElementById( "mainCont" ).appendChild( drawMap( towerDef ) )
