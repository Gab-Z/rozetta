var app;
window.addEventListener( "load", init, false );
function init(){
  app = new ThreeDToCraftPattern_app( );
  window.removeEventListener( "load", init );
}
