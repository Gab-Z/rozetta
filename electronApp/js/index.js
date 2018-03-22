const ipc = require('electron').ipcRenderer;
const bindings = require("bindings");
const whoami = bindings("module");

const defaults = {
  mapW:200,
  mapH:200,
  depMin:1,
  depMax:10,
  tileSize:5,
  start:{ x: 2, y: 2 }
}
var testJs = false;
function to1d( tx, ty ){
  return ty * defaults.mapW + tx;
}
function to2d( t ){
  let y = Math.floor( t / defaults.mapW ),
      nbCellsInRows = y * defaults.mapW,
      x = t - nbCellsInRows;
  return [ x, y ];
}

function random2dMap( w = defaults.mapW, h = defaults.mapH, min = defaults.depMin, max = defaults.depMax ){
  let yArr = [],
      delta = max - min;
  for( let y = 0; y < h; y++ ){
    let xArr = [];
    for( let x = 0; x < w; x++ ){
      xArr.push( Math.floor( min + Math.random() * delta ) );
    }
    yArr.push( xArr );
  }
  return yArr;
}

function fillMap( map, x, y ){
  let retMap = new Array( map.length );
  //alert(typeof retMap[0])
  map.forEach( ( cell, i, map ) => {
    retMap[ i ] = new Array( cell.length )
  });
  let searchList = [ [ x, y ] ];
  retMap[ y ][ x ] = map[ y ][ x ];
  let end = false;
  for( let n = 0; end == false; n++ ){
    searchList = fillMapStep( map, retMap, searchList );
    if( searchList.length == 0 ) end = true;
  }
  return retMap;
}

function testTile( srcX, srcY, offsetX, offsetY, value, _map, _retMap, _list ){
  let tX = srcX + offsetX,  tY = srcY + offsetY,
      retTarget = _retMap[ tY ][ tX ],
      srcTarget = _map[ tY ][ tX ];
  if( typeof retTarget == "undefined" || value + srcTarget < retTarget ){
    _retMap[ tY ][ tX ] = value + srcTarget;
    //alert(retMap[ y - 1 ][ x - 1 ])
    pushIfNew( _list, [ tX, tY ] );
  }
}

function fillMapStep( map, retMap, searchList ){
  let newList = [],
      mapLy = map.length,
      mapLx = map[ 0 ].length;
  searchList.forEach( ( pt, i, list ) => {
    let x = pt[ 0 ],
        y = pt[ 1 ],
        value = retMap[ y ][ x ];
    // LEFT
    if( x > 0 ){
      if( y > 0) testTile( x, y, -1, -1, value, map, retMap, newList );
      testTile( x, y, -1, 0, value, map, retMap, newList );
      if( y + 1 < mapLy ) testTile( x, y, -1, 1, value, map, retMap, newList );
    }
    //MIDDLE
    if( y > 0 ) testTile( x, y, 0, -1, value, map, retMap, newList );
    if( y + 1 < mapLy ) testTile( x, y, 0, 1, value, map, retMap, newList );
    //RIGHT
    if( x + 1 < mapLx ){
      if( y > 0) testTile( x, y, 1, -1, value, map, retMap, newList );
      testTile( x, y, 1, 0, value, map, retMap, newList );
      if( y + 1 < mapLy ) testTile( x, y, 1, 1, value, map, retMap, newList );
    }
  })
  return newList;
}

function fillMapStep2( map, retMap, searchList ){
  let newList = [],
      mapLy = map.length,
      mapLx = map[ 0 ].length;
  searchList.forEach( ( pt, i, list ) => {
    let x = pt[ 0 ],
        y = pt[ 1 ],
        value = retMap[ y ][ x ];
    // LEFT
    if( x > 0 ){
      if( y > 0){
        let retTopLeft = retMap[ y - 1 ][ x - 1 ],
            srcTopLeft = map[ y - 1 ][ x - 1 ];
        if( typeof retTopLeft == "undefined" || value + srcTopLeft < retTopLeft ){
          retMap[ y - 1 ][ x - 1 ] = value + srcTopLeft;
          //alert(retMap[ y - 1 ][ x - 1 ])
          pushIfNew( newList, [ x - 1, y - 1 ] );
        }
      }
      let retLeft = retMap[ y ][ x - 1 ],
          srcLeft = map[ y ][ x - 1 ];
      if( typeof retLeft == "undefined" || value + srcLeft < retLeft ){
        retMap[ y ][ x - 1 ] = value + srcLeft;
        //alert(retMap[ y ][ x - 1 ])
        pushIfNew( newList, [ x - 1, y ] );
      }
      if( y + 1 < mapLy ){
        let retBotLeft = retMap[ y + 1 ][ x - 1 ],
            srcBotLeft = map[ y + 1 ][ x - 1 ];
        if( typeof retBotLeft == "undefined" || value + srcBotLeft < retBotLeft ){
          retMap[ y + 1 ][ x - 1 ] = value + srcBotLeft;
          //alert(  retMap[ y + 1 ][ x - 1 ])
          pushIfNew( newList, [ x - 1, y + 1 ] );
        }
      }
    }
    //MIDDLE
    if( y > 0 ){
      let retTop = retMap[ y - 1 ][ x ],
          srcTop = map[ y - 1 ][ x ];
      if( typeof retTop == "undefined" || value + srcTop < retTop ){
        retMap[ y - 1 ][ x ] = value + srcTop;
        //alert(  retMap[ y - 1 ][ x ])
        pushIfNew( newList, [ x, y - 1 ] );
      }
    }
    if( y + 1 < mapLy ){
      let retBot = retMap[ y + 1 ][ x ],
          srcBot = map[ y + 1 ][ x ];
      if( typeof retBot == "undefined" || value + srcBot < retBot ){
        retMap[ y + 1 ][ x ] = value + srcBot;
        //alert(retMap[ y + 1 ][ x ])
        pushIfNew( newList, [ x, y + 1 ] );
      }
    }
    //RIGHT
    if( x + 1 < mapLx ){
      if( y > 0){
        let retTopRight = retMap[ y - 1 ][ x + 1 ],
            srcTopRight = map[ y - 1 ][ x + 1 ];
        if( typeof retTopRight == "undefined" || value + srcTopRight < retTopRight ){
          retMap[ y - 1 ][ x + 1 ] = value + srcTopRight;
          //alert(retMap[ y - 1 ][ x + 1 ])
          pushIfNew( newList, [ x + 1, y - 1 ] );
        }
      }
      let retRight = retMap[ y ][ x + 1 ],
          srcRight = map[ y ][ x + 1 ];
      if( typeof retRight == "undefined" || value + srcRight < retRight ){
        retMap[ y ][ x + 1 ] = value + srcRight;
        //alert(retMap[ y ][ x + 1 ])
        pushIfNew( newList, [ x + 1, y ] );
      }
      if( y + 1 < mapLy ){
        let retBotRight = retMap[ y + 1 ][ x + 1 ],
            srcBotRight = map[ y + 1 ][ x + 1 ];
        if( typeof retBotRight == "undefined" || value + srcBotRight < retBotRight ){
          retMap[ y + 1 ][ x + 1 ] = value + srcBotRight;
          //alert(  retMap[ y + 1 ][ x + 1 ])
          pushIfNew( newList, [ x + 1, y + 1 ] );
        }
      }
    }
  })
  return newList;
}

function pushIfNew( arr, pt ){
  let isNew = true,
      l = arr.length,
      x = pt[ 0 ],
      y = pt[ 1 ];
  for( let i = 0; i < l; i++ ){
    let el = arr[ i ];
    if( el[ 0 ] == x && el[ 1 ] == y ){
      isNew = false;
      break;
    }
  }
  if( isNew ) arr.push( pt );
}

function mapToTable(map){
  var table = document.createElement("table"),
      tbody = table.appendChild( document.createElement("tbody") );
  for( let y = 0; y < defaults.mapH; y++ ){
    var row = tbody.appendChild( document.createElement("tr") );
    for(let x = 0; x < defaults.mapW; x++ ){
      let cell = row.appendChild( document.createElement("td") );
      cell.textContent = map[ to1d( x, y ) ];
    }
  }
  return table;
}
function map2dToTable(_map){
  var table = document.createElement("table"),
      tbody = table.appendChild( document.createElement("tbody") );
  for( let y = 0; y < defaults.mapH; y++ ){
    var row = tbody.appendChild( document.createElement("tr") );
    for(let x = 0; x < defaults.mapW; x++ ){
      let cell = row.appendChild( document.createElement("td") );
      cell.textContent = _map[ y ][x];
    }
  }
  return table;
}
var startTime = Date.now();

var map,
    pathMap;

if( testJs ){
  var map = random2dMap(),
      pathMap = fillMap( map, defaults.start.x, defaults.start.y );
}else{
  whoami.initArray( defaults.mapW, defaults.mapH, defaults.depMin, defaults.depMax );
  map = whoami.getArray();//random2dMap(),
  pathMap = whoami.getFilledMap( defaults.start.x, defaults.start.y );
}
alert( ( Date.now() - startTime ) / 1000 );
document.body.appendChild(map2dToTable(pathMap))

var mainCont = document.getElementById( "mainCont" ),
    cv =  mainCont.appendChild( document.createElement( "canvas" ) ),
    ctx = cv.getContext( "2d" );
cv.width = defaults.mapW * defaults.tileSize;
cv.height = defaults.mapH * defaults.tileSize;
cv.style.width = ( defaults.mapW * defaults.tileSize ) + "px";
cv.style.height = ( defaults.mapH * defaults.tileSize ) + "px";
cv.style.position = "fixed";
cv.style.left = 0;
cv.style.top = 0;
cv.style.zIndex = 10;
if(testJs){
  map.forEach( ( row, y ) => {
    row.forEach( ( cell, i ) => {
      let hue = 140 - ( ( ( cell - defaults.depMin ) / ( defaults.depMax - defaults.depMin) ) * 140 );
      ctx.fillStyle = 'hsl('+ hue +',100%,50%)';
      //  alert( pos[0] * defaults.tileSize+","+ pos[1] * defaults.tileSize)
      ctx.fillRect( i * defaults.tileSize, y * defaults.tileSize, defaults.tileSize, defaults.tileSize );

    })
  });
}else{
  map.forEach( ( cell, i, _map ) => {
    let pos = to2d( i );
    let hue = 140 - ( ( ( cell - defaults.depMin ) / ( defaults.depMax - defaults.depMin) ) * 140 );
    ctx.fillStyle = 'hsl('+ hue +',100%,50%)';
    //  alert( pos[0] * defaults.tileSize+","+ pos[1] * defaults.tileSize)
    ctx.fillRect( pos[0] * defaults.tileSize, pos[1] * defaults.tileSize, defaults.tileSize, defaults.tileSize );

  });
}


var pathCv = mainCont.appendChild( document.createElement( "canvas" ) ),
    pathCtx = pathCv.getContext( "2d" );
pathCv.style.zIndex = 20;
pathCv.id = "pathCv";
pathCv.width = defaults.mapW * defaults.tileSize;
pathCv.height = defaults.mapH * defaults.tileSize;
pathCv.style.width = ( defaults.mapW * defaults.tileSize ) + "px";
pathCv.style.height = ( defaults.mapH * defaults.tileSize ) + "px";
pathCv.style.position = "fixed";
pathCv.style.left = 0;
pathCv.style.top = 0;
pathCtx.fillStyle = "black";

if(testJs){
  pathCv.addEventListener( "mousemove", trackJs );
}else{
  pathCv.addEventListener( "mousemove", track );
}

const turnCoords = [ -1,0,   -1,-1,   0,-1,    1,-1,    1,0,   1,1,   0,1,   -1,1 ];
function track( e ){
  var sx = Math.floor( e.clientX / defaults.tileSize ),
      sy = Math.floor( e.clientY / defaults.tileSize ),
      pt1d = to1d( sx, sy );
      maxX = defaults.mapW,
      maxY = defaults.mapH,
      pcv = e.currentTarget,
      pctx = pcv.getContext( "2d" ),
      ret = [ [ sx, sy ] ],
      end = false;
  console.log( pcv.tagName );

  if( sx < 0 || sy < 0 || sx >= maxX || sy >= maxY)return false;
  pctx.clearRect( 0,0,pcv.width,pcv.height);
  for( let i = 0; end == false; i++ ){
    let coords = ret[ i ],
        x = coords[ 0 ],
        y = coords[ 1 ],
        time = Infinity,
        target = false;
    for( let n = 0; n < 8; n++ ){
      let tx = x + turnCoords[ n * 2 ],
          ty = y + turnCoords[ n * 2 + 1],
          t1d = to1d( tx, ty );

      if( tx < 0 || ty < 0 || tx >= maxX || ty >= maxY ) continue;
      if( pathMap[ t1d ] < time ){
        time = pathMap[ t1d ];
        target = [ tx, ty ];
      }
    }

    ret.push( target );
    if( target[0] == defaults.start.x && target[1] == defaults.start.y ){
      end = true;
    }
  }

  pctx.fillStyle = "black";
  pctx.beginPath();

  ret.forEach( cell => {
    console.log( cell[ 0 ] + "," +  cell[ 1 ] + ":" + cell[ 0 ] * defaults.tileSize + "/" + cell[ 1 ] * defaults.tileSize );
    pctx.rect( cell[ 0 ] * defaults.tileSize, cell[ 1 ] * defaults.tileSize, defaults.tileSize, defaults.tileSize );
  })
  pctx.fill();

}
function trackJs( e ){
  var sx = Math.floor( e.clientX / defaults.tileSize ),
      sy = Math.floor( e.clientY / defaults.tileSize ),
      maxX = defaults.mapW,
      maxY = defaults.mapH,
      pcv = e.currentTarget,
      pctx = pcv.getContext( "2d" ),
      ret = [ [ sx, sy ] ],
      end = false;
      console.log("coucou"+sx+", "+sy)
  if( sx < 0 || sy < 0 || sx >= maxX || sy >= maxY)return false;
  pctx.clearRect( 0,0,pcv.width,pcv.height);
  for( let i = 0; end==false; i++ ){
    var coords = ret[ i ],
        x = coords[ 0 ],
        y = coords[ 1 ],
        time = Infinity,
        target = false;
        console.log( "test : "+x+", "+y)
    for( let n = 0; n < 8; n++ ){
      let tx = x + turnCoords[ n * 2 ],
          ty = y + turnCoords[ n * 2 + 1];

      if( tx < 0 || ty < 0 || tx >= maxX || ty >= maxY ) continue;
      if( pathMap[ ty ][ tx ] < time ){
        time = pathMap[ ty ][ tx ];
        target = [ tx, ty ];
      }
    }
    if( target[0] == defaults.start.x && target[1] == defaults.start.y ){

      end = true;
    }
    ret.push( target );
  }
console.log("move")
  pctx.fillStyle = "black";
  pctx.beginPath();

  ret.forEach( cell => {
    console.log( cell[ 0 ] + "," +  cell[ 1 ] + ":" + cell[ 0 ] * defaults.tileSize + "/" + cell[ 1 ] * defaults.tileSize );
    pctx.rect( cell[ 0 ] * defaults.tileSize, cell[ 1 ] * defaults.tileSize, defaults.tileSize, defaults.tileSize );
  })
  pctx.fill();
}

document.body.addEventListener( "click", e => {
  ipc.send('bodyClick');
})
