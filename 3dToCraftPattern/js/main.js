var app;
window.addEventListener( "load", init, false );
function init(){
  app = new ThreeDToCraftPattern_app( );
  let deerMesh = deer.meshes[0];
  app.createMesh( "deer", deerMesh.positions, deerMesh.indices )
  window.removeEventListener( "load", init );
}
