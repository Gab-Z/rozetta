const ipc = require('electron').ipcRenderer;
const bindings = require("bindings");
const td = bindings("towerdef");


const defaults = {
  tileSize:50,
  mapW:8,
  mapH:8,
  start:{x:1,y:3},
  end:{x:7,y:3}
}
const towerDef = new td.TowerDefense( defaults.mapW,defaults.mapH, [ defaults.start.x,defaults.start.y ], [ defaults.end.x,defaults.end.y ] );

alert(towerDef.testClass())

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
    console.log( "start add");
    let add = towerDef.addStructure( "Wall", [ x, y ] );
    if(add == false)console.log(add)
    console.log( "add : " + JSON.stringify( add ) );
    if( add ) document.body.appendChild( arrToTable(add) );
    if(add.forEach){
    //  ctx.clearRect(0,0,cv.width, cv.height)
      add.forEach( el => {
        if(el.hasOwnProperty("typeName")){
          let p = el.structures;
          let pl = p.length / 2;
          for( let i = 0; i < pl; i++ ){
            ctx.fillRect(p[ i * 2 ] * defaults.tileSize, p[ i * 2 + 1 ] * defaults.tileSize, defaults.tileSize, defaults.tileSize );
          }
        }
      })
    }
  })
  return cv;
}
document.getElementById( "mainCont" ).appendChild( drawMap( towerDef ) )
function arrToTable(ar){
  if(document.getElementById("mapTable")){
    let old = document.getElementById("mapTable");
    old.parentNode.removeChild( old );
  }
  var table = document.createElement("table"),
      tb = table.appendChild(document.createElement("tbody"));
  table.id = "mapTable";
  for( let r = 0; r < defaults.mapH; r++ ){
    let row = tb.appendChild(document.createElement("tr"));
    for( let c = 0; c < defaults.mapW; c++ ){
      let cell = row.appendChild(document.createElement("td"));
      cell.textContent = ar[ r * defaults.mapW + c ];
    }
  }
  return table;
}
