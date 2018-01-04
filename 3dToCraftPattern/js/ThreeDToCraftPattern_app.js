const ThreeDToCraftPattern_app_defaults = {
  canvas3dWidth:512,
  canvas3dHeight:512
};
class ThreeDToCraftPattern_app{
  constructor( options = {} ){
    let opts = Object.assign( ThreeDToCraftPattern_app_defaults, options );
    this.container = document.createElement( "div" );
    this.container.id = "ThreeDToCraft_container";
    ( opts.parent || document.body ).appendChild( this.container );
    this.canvas2d = this.container.appendChild( document.createElement( "canvas" ) );
    this.canvas2d.id = "ThreeDToCraft_canvas2d";
    this.meshes = [];
    this.activeMesh = "none";
    this.BABYLON = {};
    let BABL = this.BABYLON;
    BABL.canvas3d = this.container.appendChild( document.createElement( "canvas" ) );
    BABL.canvas3d.id = "ThreeDToCraft_canvas3d";

    BABL.engine = new BABYLON.Engine( BABL.canvas3d, true );
    BABL.scene = new BABYLON.Scene( BABL.engine );
    BABL.scene.clearColor =  new BABYLON.Color4( 0.2, 0.2, 0.2, 1 );
    BABL.camera = new BABYLON.ArcRotateCamera( "Camera", Math.PI / 2, Math.PI / 2, 2, new BABYLON.Vector3( 0, 0, 0 ), BABL.scene );
    BABL.camera.attachControl( BABL.canvas3d, true );
    BABL.hemiLight = new BABYLON.HemisphericLight( "hemiLight", new BABYLON.Vector3( 1, -1, 0 ), BABL.scene );
    let self = this;
    self.BABYLON.engine.runRenderLoop( function(){
      let self = this;
      self.BABYLON.scene.render();
    }.bind( self ) );
  }
  createMesh( name, positions, indices ){
    let self = this;
    let newMesh = new ThreeDToCraftPattern_mesh( name, positions, indices, self.BABYLON.scene );
    self.meshes.push( newMesh );
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
