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
    BABL.canvas3d.width = opts.canvas3dWidth;
    BABL.canvas3d.height = opts.canvas3dHeight;
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
    this.BABYLON.canvas3d.addEventListener("click", function (evt) {
      let self = this;
      self.clickMesh( evt );
    }.bind( self ));
  }
  findMeshByName( name ){
    return this.meshes.find( mesh => mesh.name === name );
  }
  clickMesh( e ){
    let scene = this.BABYLON.scene,
        pickResult = scene.pick(scene.pointerX, scene.pointerY);
    if(pickResult.hit){
       let BabylonMesh = pickResult.pickedMesh,
           mesh = this.findMeshByName( BabylonMesh.name ),
           indices = BabylonMesh.getIndices(),
           index0 = indices[pickResult.faceId * 3],
           index1 = indices[pickResult.faceId * 3 + 1],
           index2 = indices[pickResult.faceId * 3 + 2],
           uvs = BabylonMesh.getVerticesData(BABYLON.VertexBuffer.UVKind),
           texCv = mesh.BABYLON.textureCanvas,
           cvSize = texCv.width,
           u0 = uvs[ index0 * 2 ] * cvSize,
           v0 = uvs[ index0 * 2 + 1 ] * cvSize,
           u1 = uvs[ index1 * 2 ] * cvSize,
           v1 = uvs[ index1 * 2 + 1 ] * cvSize,
           u2 = uvs[ index2 * 2 ] * cvSize,
           v2 = uvs[ index2 * 2 + 1 ] * cvSize,
           dynamicTex = mesh.BABYLON.dynamicTexture,
           texCtx = dynamicTex.getContext();
       mesh.drawTexUvs();
       texCtx.fillStyle = "#da9e2b";
       texCtx.beginPath();
       texCtx.moveTo(u0,v0);
       texCtx.lineTo(u1,v1);
       texCtx.lineTo(u2,v2);
       texCtx.closePath();
       texCtx.fill();
       dynamicTex.update(false);
    }
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
function distanceVector( v1, v2 ){
    var dx = v1.x - v2.x;
    var dy = v1.y - v2.y;
    var dz = v1.z - v2.z;
    return Math.sqrt( dx * dx + dy * dy + dz * dz );
};
