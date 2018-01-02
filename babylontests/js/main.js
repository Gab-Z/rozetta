window.onload = function(){
  var mesh = deer.meshes[0];
  var meshPositions = mesh.positions,
      meshIndices = mesh.indices;
      alert(meshPositions.length / 3 + ", " + meshIndices.length)
      startTestWithCustomVertexData( meshPositions, meshIndices )
}
function startTestWithBasicVertexData( positions, indices ){
  var babylonCanvas = document.body.appendChild( document.createElement( "canvas" ) );
  babylonCanvas.id = "babylonCanvas";
  engine = new BABYLON.Engine(babylonCanvas, true);
  scene = new BABYLON.Scene(engine);
  scene.clearColor =  new BABYLON.Color4(0,0,0,0);
  camera = new BABYLON.ArcRotateCamera("Camera", Math.PI / 2, Math.PI / 2, 2, new BABYLON.Vector3(0, 0, 0), scene);
  camera.attachControl(babylonCanvas, true);
  // Add lights to the scene
  var light1 = new BABYLON.HemisphericLight("light1", new BABYLON.Vector3(1, -1, 0), scene);

  var customMesh = new BABYLON.Mesh("custom", scene);

  var vertexData = new BABYLON.VertexData();
  var normals = [];
  BABYLON.VertexData.ComputeNormals( positions, indices, normals );
  vertexData.positions = positions;
  vertexData.indices = indices;
  vertexData.normals = normals;
  vertexData.applyToMesh( customMesh );
  customMesh.convertToFlatShadedMesh();

  var myMaterial = new BABYLON.StandardMaterial("myMaterial", scene);
  myMaterial.diffuseColor = new BABYLON.Color3(0.5, 0.5, 0.5);
  myMaterial.specularColor = new BABYLON.Color3(0.1, 0.1, 0.1);
  myMaterial.emissiveColor = new BABYLON.Color3(0.2, 0.2, 0.2);
  myMaterial.ambientColor = new BABYLON.Color3(0.23, 0.98, 0.53);
  myMaterial.backFaceCulling = false;
  customMesh.material = myMaterial;

  customMesh.isPickable = true;

  customMesh.enableEdgesRendering();
  customMesh.edgesWidth = 0.5;
  customMesh.edgesColor = new BABYLON.Color4(0, 0, 1, 1);

  babylonCanvas.addEventListener("click", function (evt){
    var pickResult = scene.pick(scene.pointerX, scene.pointerY);
    console.log( JSON.stringify( pickResult.hit ) );
  });
  engine.runRenderLoop(function () { // Register a render loop to repeatedly render the scene
    scene.render();
  });
};
function startTestWithCustomVertexData( _positions, _indices ){
  var babylonCanvas = document.body.appendChild( document.createElement( "canvas" ) );
  babylonCanvas.id = "babylonCanvas";
  engine = new BABYLON.Engine(babylonCanvas, true);
  scene = new BABYLON.Scene(engine);
  scene.clearColor =  new BABYLON.Color4(0,0,0,0);
  camera = new BABYLON.ArcRotateCamera("Camera", Math.PI / 2, Math.PI / 2, 2, new BABYLON.Vector3(0, 0, 0), scene);
  camera.attachControl(babylonCanvas, true);
  // Add lights to the scene
  var light1 = new BABYLON.HemisphericLight("light1", new BABYLON.Vector3(1, -1, 0), scene);

  var customMesh = new BABYLON.Mesh("custom", scene);
  var nbTriangles = _indices.length / 3,
      separatePositions = new Array( _indices.length * 3 ),
      separateIndices = new Array( _indices.length  ),
      _uvs =  new Array( _indices.length * 2 );

  for( var h = 0; h < _indices.length; h++ ){
    separatePositions[ h * 3 ] = _positions[ _indices[ h ] * 3 ];
    separatePositions[ h * 3 + 1 ] = _positions[ _indices[ h ] * 3 + 1 ];
    separatePositions[ h * 3 + 2 ] = _positions[ _indices[ h ] * 3 + 2 ];

    separateIndices[ h ] = h;
  }
  var vertexData = new BABYLON.VertexData();
  var normals = [];
  BABYLON.VertexData.ComputeNormals( separatePositions, separateIndices, normals );
  vertexData.positions = separatePositions;
  vertexData.indices = separateIndices;
  vertexData.normals = normals;
  vertexData.applyToMesh( customMesh );
  customMesh.convertToFlatShadedMesh();

  var myMaterial = new BABYLON.StandardMaterial("myMaterial", scene);
  myMaterial.diffuseColor = new BABYLON.Color3(0.5, 0.5, 0.5);
  myMaterial.specularColor = new BABYLON.Color3(0.1, 0.1, 0.1);
  myMaterial.emissiveColor = new BABYLON.Color3(0.2, 0.2, 0.2);
  myMaterial.ambientColor = new BABYLON.Color3(0.23, 0.98, 0.53);
  myMaterial.backFaceCulling = false;
  customMesh.material = myMaterial;

  customMesh.isPickable = true;

  babylonCanvas.addEventListener("click", function (evt){
    var pickResult = scene.pick(scene.pointerX, scene.pointerY);
    console.log( JSON.stringify( pickResult.hit ) );
  });
  engine.runRenderLoop(function () { // Register a render loop to repeatedly render the scene
    scene.render();
  });

};
