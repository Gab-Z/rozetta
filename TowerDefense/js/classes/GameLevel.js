const gameLevelDefaults = {
  mapWidth: 10,
  mapHeight: 10,
  startPoints: [ 0, 0 ],
  endPoints = [ 9, 9 ]
};

class GameLevel {

  constructor( _params = {} ){
    let params = Object.assign( gameLevelDefaults, _params );
    this.module = new require( "bindings" )( "towerdef" ).TowerDefense( params.mapWidth,
                                                                        params.mapHeight,
                                                                        params.startPoints,
                                                                        params.endPoints );
    this.app = new PIXI.Application( { width: window.screen.width,
                                       height: window.screen.height,
                                       antialias: true } );

    this.dragData;
    this.activeStateName = "empty";
    this.activeTool = { name: false,
                        texName:false
    };

  }

  initDragData(){
    this.dragData.positions = { start:  { x: 0, y: 0 },
                                end:    { x: 0, y: 0 } }
  }

  setEvtState( stateName ){
    console.log( "setEvtState : " + stateName );
    evtStates[ activeEvtStateName ].forEach( listenerName => {
      listeners[ listenerName ].off();
    });
    this.activeEvtStateName = stateName;
    evtStates[ activeEvtStateName ].forEach( listenerName => {
      listeners[ listenerName ].on();
    });
  }

}
const listeners = {
  clickStructPicker:{
    on: () => { app.stage.getChildByName( "structPickerCont" ).on( "click", clickStructPicker ); },
    off: () => { app.stage.getChildByName( "structPickerCont" ).off( "click", clickStructPicker ); }
  },
  moveStructPosPreview: {
    on: () => { app.stage.getChildByName( "stageCont" ).on( "mousemove", moveStructPosPreview ); },
    off: () => { app.stage.getChildByName( "stageCont" ).off( "mousemove", moveStructPosPreview ); }
  },
  cancelStructPosPreview: {
    on: () => { window.addEventListener( "mousedown", cancelStructPosPreview, false ); },
    off: () => { window.removeEventListener( "mousedown", cancelStructPosPreview); }
  },
  startStructPositioning: {
    on: () => { app.stage.getChildByName( "stageCont" ).on( "mousedown", startStructPositioning ); },
    off: () => { app.stage.getChildByName( "stageCont" ).off( "mousedown", startStructPositioning ); }
  },
  setStructPreviewRotationByWheel:{
    on: () => { window.addEventListener( "mousewheel", setStructPreviewRotationByWheel, false ); },
    off: () => { window.removeEventListener( "mousewheel", setStructPreviewRotationByWheel, false ); }
  },
  cancelStructPositioning: {
    on: () => { window.addEventListener( "mousedown", cancelStructPositioning, false ); },
    off: () => {  window.removeEventListener( "mousedown", cancelStructPositioning, false );}
  },
  dragStructPositionning: {
    on: () => { app.stage.getChildByName( "stageCont" ).on( "mousemove", dragStructPositionning ); },
    off: () => { app.stage.getChildByName( "stageCont" ).off( "mousemove", dragStructPositionning ); }
  },
  onEndStructPositioning: {
    on: () => { app.stage.getChildByName( "stageCont" ).on( "mouseup", onEndStructPositioning ); },
    off: () => { app.stage.getChildByName( "stageCont" ).off( "mouseup", onEndStructPositioning ); }
  },
  structSelectMove: {
    on: () => { app.stage.getChildByName( "stageCont" ).on( "mousemove", structSelectMove ); },
    off: () => { app.stage.getChildByName( "stageCont" ).off( "mousemove", structSelectMove ); }
  },
  openStructMenu: {
    on: () => { app.stage.getChildByName( "stageCont" ).on( "click", openStructMenu ); },
    off: () => { app.stage.getChildByName( "stageCont" ).off( "click", openStructMenu ); }
  },
  closeStructMenubyOuterClick: {
    on: () => { app.stage.interactive = true;
                app.stage.on( "mousedown", closeStructMenubyOuterClick, true );
              },
    off: () => {  app.stage.off( "mousedown", closeStructMenubyOuterClick );
                  app.stage.interactive = false;
                }
  },
  cancelStructMenu: {
    on: () => { window.addEventListener( "mousedown", cancelStructMenu, false ); },
    off: () => { window.removeEventListener( "mousedown", cancelStructMenu ); }
  },
  closeStructMenu: {
    on: () => { app.stage.getChildByName( "stageCont" ).on( "mouseup" ); },
    off: () => { app.stage.getChildByName( "stageCont" ).off( "mouseup" ); }
  },
  clickToolsPicker: {
    on: () => { app.stage.getChildByName( "toolsPickerCont" ).on( "click", clickToolsPicker );},
    off: () => { app.stage.getChildByName( "toolsPickerCont" ).off( "click", clickToolsPicker ); }
  },
  dragMove: {
    on: () => { app.stage.getChildByName( "stageCont" ).on( "mousemove", dragMove ); },
    off: () => { app.stage.getChildByName( "stageCont" ).off( "mousemove", dragMove ); }
  },
  endDrag: {
    on: () => { app.stage.getChildByName( "stageCont" ).on( "mouseup", endDrag ); },
    off: () => { app.stage.getChildByName( "stageCont" ).off( "mouseup", endDrag ); }
  },
  cancelDrag: {
    on: () => { window.addEventListener( "mousedown", cancelDrag, false ); },
    off: () => { window.removeEventListener( "mousedown", cancelDrag ); }
  },
  dragPreviewMove: {
    on: () => { app.stage.getChildByName( "stageCont" ).on( "mousemove", dragPreviewMove ); },
    off: () => { app.stage.getChildByName( "stageCont" ).off( "mousemove", dragPreviewMove ); }
  },
  startDrag: {
    on: () => { app.stage.getChildByName( "stageCont" ).on( "mousedown", startDrag ); },
    off: () => { app.stage.getChildByName( "stageCont" ).off( "mousedown", startDrag ); }
  },
  cancelDragPreview:{
    on: () => { window.addEventListener( "mousedown", cancelDragPreview, false ); },
    off: () => { window.removeEventListener( "mousedown", cancelDragPreview ); }
  }
}
const evtStates = {
  empty: [],
  basicSelect: [
    "clickStructPicker",
    "openStructMenu",
    "structSelectMove",
    "clickToolsPicker"
  ],
  structPosPreviewNoRotation:[
    "moveStructPosPreview",
    "clickStructPicker",
    "cancelStructPosPreview",
    "startStructPositioning",
    "clickToolsPicker"
  ],
  structPosPreviewWithRotation: [
    "moveStructPosPreview",
    "clickStructPicker",
    "cancelStructPosPreview",
    "startStructPositioning",
    "setStructPreviewRotationByWheel",
    "clickToolsPicker"
  ],
  structPosDrag: [
    "dragStructPositionning",
    "onEndStructPositioning",
    "cancelStructPositioning"
  ],
  openedStructMenu: [
    "clickStructPicker",
    "closeStructMenubyOuterClick",
    "cancelStructMenu",
    "clickToolsPicker"
  ],
  startDragPreview: [
    "dragPreviewMove",
    "startDrag",
    "cancelDragPreview",
    "clickStructPicker"
  ],
  drag: [
    "dragMove",
    "endDrag",
    "cancelDrag"
  ]

};
const funcTools = {

  destroyStructures: {
    init: ( e ) => {
      dragData = {
        callback: destroyStructZone,
        drawFunc: "drawDragSquare",
        positions: false
      }
      let stageCont = app.stage.getChildByName( "stageCont" ),
          previewCont = stageCont.addChild( new PIXI.Container() ),
          prevSprite = previewCont.addChild( new PIXI.Sprite( PIXI.loader.resources[ activeTool.texName ].texture ) ),
          ts = defaults.tileSize,
          m = e.data.getLocalPosition( stageCont );
      previewCont.name = "destructToolCont";
      prevSprite.name = "destructToolPrevSprite";
      prevSprite.width = ts;
      prevSprite.height = ts;
      dragPreviewMove( false, m );
      setEvtState( "startDragPreview" );
    },
    cancel: () => {
      app.stage.getChildByName( "stageCont" ).getChildByName( "destructToolCont" ).destroy( { children: true, texture: false, baseTexture: false } );
    }
  }
}
