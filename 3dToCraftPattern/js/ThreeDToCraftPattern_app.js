const ThreeDToCraftPattern_app_defaults = {

};
class ThreeDToCraftPattern_app{
  constructor( options = {} ){
    let opts = Object.assign( ThreeDToCraftPattern_app_defaults, options );
    this.container = document.createElement( "div" );
    this.container.id = "ThreeDToCraft_container";
    this.canvas3d = this.container.appendChild( document.createElement( "canvas" ) );
    this.canvas3d.id = "ThreeDToCraft_canvas3d";
    this.canvas2d = this.container.appendChild( document.createElement( "canvas" ) );
    this.canvas2d.id = "ThreeDToCraft_canvas2d";
    this.meshes = [];
    this.activeMesh = "none";
    this.BABYLON = {};
    this.BABYLON.engine = new BABYLON.Engine( this.canvas3d, true );
    this.BABYLON.scene = new BABYLON.Scene( this.BABYLON.engine );
    this.BABYLON.clearColor =  new BABYLON.Color4( 0, 0, 0, 0 );
    this.BABYLON.camera = new BABYLON.ArcRotateCamera( "Camera", Math.PI / 2, Math.PI / 2, 2, new BABYLON.Vector3( 0, 0, 0 ), this.BABYLON.scene );
    this.BABYLON.camera.attachControl( this.canvas3d, true );
    // Add lights to the scene
    this.BABYLON.hemiLight = new BABYLON.HemisphericLight( "hemiLight", new BABYLON.Vector3( 1, -1, 0 ), this.BABYLON.scene );

  }
  createMesh( name, positions, indices ){

  }
  addMesh(  ){

  }
}
function intersection(x0, y0, r0, x1, y1, r1) {
  var a, dx, dy, d, h, rx, ry;
  var x2, y2;
  dx = x1 - x0;
  dy = y1 - y0;
  d = Math.sqrt((dy*dy) + (dx*dx));
  if (d > (r0 + r1)) {
      return false;
  }
  if (d < Math.abs(r0 - r1)) {
      return false;
  }
  a = ((r0*r0) - (r1*r1) + (d*d)) / (2.0 * d) ;
  x2 = x0 + (dx * a/d);
  y2 = y0 + (dy * a/d);
  h = Math.sqrt((r0*r0) - (a*a));
  rx = -dy * (h/d);
  ry = dx * (h/d);
  var xi = x2 + rx;
  var xi_prime = x2 - rx;
  var yi = y2 + ry;
  var yi_prime = y2 - ry;
  return [xi, xi_prime, yi, yi_prime];
}
function sortNumber(a,b) {
    return a - b;
}
