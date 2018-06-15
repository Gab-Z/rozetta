 var ThetaStarSearch = (function() {
  function ThetaStarSearch(heuristic) {
      this.Heuristic = heuristic;
      this.OpenList = [];
      this.ClosedList = [];
    }

  ThetaStarSearch.prototype.smoothPath = function(path) {
    var corner, i, len, smoothedPath;
    smoothedPath = [];
    if (!path) {
      return path;
    }
    smoothedPath.push(path[0]);
    for (i = 0, len = path.length; i < len; i++) {
      corner = path[i];
      var prevCorner;
      if(path[i - 1]){
        prevCorner = path[i - 1];
      }
      if (!(LineOfSight(smoothedPath[smoothedPath.length - 1], corner))) {
        smoothedPath.push(prevCorner);
      }
    }
    smoothedPath.push(path[path.length - 1]);
    return smoothedPath;
  };

  ThetaStarSearch.prototype.addToClosed = function(node) {
    var corner, i, inClosed, inListNode, indexOfNode, indexToRemove, j, k, len, len1, len2, num, ourFval, ref, ref1, ref2, results;
    this.ClosedList.push(node);
    indexToRemove = this.OpenList.indexOf(node);
    if (!(indexToRemove < 0)) {
      this.OpenList.splice(indexToRemove, 1);
    }
    //ref = node.getSuccessors();
    ref = getSuccessors( node );
    results = [];
    for (i = 0, len = ref.length; i < len; i++) {
      corner = ref[i];
      inClosed = false;
      ref1 = this.ClosedList;
      for (j = 0, len1 = ref1.length; j < len1; j++) {
        inListNode = ref1[j];
        if (inListNode.x === corner.x) {
          if (inListNode.y === corner.y) {
            inClosed = true;
            break;
          }
        }
      }
      if (inClosed) {
        continue;
      }
      indexOfNode = -1;
      num = 0;
      ref2 = this.OpenList;
      for (k = 0, len2 = ref2.length; k < len2; k++) {
        inListNode = ref2[k];
        if (inListNode.x === corner.x) {
          if (inListNode.y === corner.y) {
            indexOfNode = num;
            break;
          }
        }
        num += 1;
      }
      //node and corner


      if (indexOfNode < 0) {
        //Not found in Open
        corner.gVal = node.gVal + Math.sqrt(((corner.x - node.x) * (corner.x - node.x)) + ((corner.y - node.y) * (corner.y - node.y)));

        if (this.Heuristic === "EuclideanDistance") {
          corner.hVal = EuclideanDistance(corner, this.Goalcorner);
        } else if (this.Heuristic === "OctileDistance") {
          corner.hVal = OctileDistance(corner, this.Goalcorner);
        } else {
          corner.hVal = ManhattanDistance(corner, this.Goalcorner);
        }
        if (node.parent) {
          if (LineOfSight(node.parent, corner)) {
             corner.gVal = node.parent.gVal + Math.sqrt(((corner.x - node.parent.x) * (corner.x - node.parent.x)) + ((corner.y - node.parent.y) * (corner.y - node.parent.y)));
             corner.fVal = corner.gVal + corner.hVal;
            corner.parent = node.parent;

          }else{
            corner.fVal = corner.gVal + corner.hVal;
            corner.parent = node;
          }
        }else{
           corner.fVal = corner.gVal + corner.hVal;
          corner.parent = node;
        }

        results.push(this.OpenList.push(corner));
      } else {
        //Found in Open
        if (this.Heuristic === "EuclideanDistance") {
          ourFval = node.gVal + Math.sqrt(((corner.x - node.x) * (corner.x - node.x)) + ((corner.y - node.y) * (corner.y - node.y)))
          + EuclideanDistance(corner, this.Goalcorner);
        } else if (this.Heuristic === "OctileDistance") {
          ourFval = node.gVal + Math.sqrt(((corner.x - node.x) * (corner.x - node.x)) + ((corner.y - node.y) * (corner.y - node.y)))
          + OctileDistance(corner, this.Goalcorner);
        } else {
          ourFval = node.gVal + Math.sqrt(((corner.x - node.x) * (corner.x - node.x)) + ((corner.y - node.y) * (corner.y - node.y)))
          + ManhattanDistance(corner, this.Goalcorner);
        }

        if (node.parent) {
          if (LineOfSight(node.parent, corner)) {
            //ourFval = node.parent.gVal + EuclideanDistance(node.parent, corner);
            if (this.Heuristic === "EuclideanDistance") {
              ourFval = node.parent.gVal + Math.sqrt(((corner.x - node.parent.x) * (corner.x - node.parent.x)) + ((corner.y - node.parent.y) * (corner.y - node.parent.y)))
              + EuclideanDistance(corner, this.Goalcorner);
            } else if (this.Heuristic === "OctileDistance") {
              ourFval = node.parent.gVal + Math.sqrt(((corner.x - node.parent.x) * (corner.x - node.parent.x)) + ((corner.y - node.parent.y) * (corner.y - node.parent.y)))
              + OctileDistance(corner, this.Goalcorner);
            } else {
              ourFval = node.parent.gVal + Math.sqrt(((corner.x - node.parent.x) * (corner.x - node.parent.x)) + ((corner.y - node.parent.y) * (corner.y - node.parent.y)))
              + ManhattanDistance(corner, this.Goalcorner);
            }
          }
        }
        if (ourFval < this.OpenList[indexOfNode].fVal) {
          this.OpenList.splice(indexOfNode, 1);
          corner.gVal = node.gVal + Math.sqrt(((corner.x - node.x) * (corner.x - node.x)) + ((corner.y - node.y) * (corner.y - node.y)));
          if (node.parent) {
          if (this.Heuristic === "EuclideanDistance") {
            corner.hVal = EuclideanDistance(corner, this.Goalcorner);
          } else if (this.Heuristic === "OctileDistance") {
            corner.hVal = OctileDistance(corner, this.Goalcorner);
          } else {
            corner.hVal = ManhattanDistance(corner, this.Goalcorner);
          }

          if (LineOfSight(node.parent, corner)) {
             corner.gVal = node.parent.gVal + Math.sqrt(((corner.x - node.parent.x) * (corner.x - node.parent.x)) + ((corner.y - node.parent.y) * (corner.y - node.parent.y)));
             corner.fVal = corner.gVal + corner.hVal;
            corner.parent = node.parent;

            }else{
              corner.fVal = corner.gVal + corner.hVal;
              corner.parent = node;
            }
          }else{
             corner.fVal = corner.gVal + corner.hVal;
            corner.parent = node;
          }
          results.push(this.OpenList.push(corner));
        } else {
          results.push(void 0);
        }
      }

    }
    return results;
  };

  ThetaStarSearch.prototype.search = function(startcorner, goalcorner) {
    var curr, currNode, i, len, minFval, node, path, reachedGoal, ref;
    this.Goalcorner = goalcorner;
    path = [];
    if (startcorner === goalcorner) {
      console.log("Start is Goal");
      return path;
    }
    startcorner.gVal = 0;
    if (this.Heuristic === "EuclideanDistance") {
      startcorner.hVal = EuclideanDistance(startcorner, goalcorner);
      startcorner.fVal = EuclideanDistance(startcorner, goalcorner);
    } else if (this.Heuristic === "OctileDistance") {
      startcorner.hVal = OctileDistance(startcorner, goalcorner);
      startcorner.fVal = OctileDistance(startcorner, goalcorner);
    } else {
      startcorner.hVal = ManhattanDistance(startcorner, goalcorner);
      startcorner.fVal = ManhattanDistance(startcorner, goalcorner);
    }
    this.addToClosed(startcorner);
    reachedGoal = false;
    while (this.OpenList.length) {
      minFval = 9999999999;
      currNode = null;
      ref = this.OpenList;

      //var openListLength = $scope.openList.length;
      //$scope.openList[openListLength] = [];

      for (i = 0, len = ref.length; i < len; i++) {
        node = ref[i];

        //openList
        var nodeInfo = [];
        nodeInfo[0] = node.x;
        nodeInfo[1] = node.y;
        var openParent = [];
        openParent[0] = node.parent.x;
        openParent[1] = node.parent.y;
        nodeInfo[2] = openParent;
        //$scope.openList[openListLength].push(nodeInfo);
        //openlist visualization

        if (node.fVal <= minFval) {
          minFval = node.fVal;
          currNode = node;
        }
      }
      if (currNode) {
        this.addToClosed(currNode);
        /*
        var tem = $scope.tempPath2.length;
          $scope.tempPath2[tem] = [];
          $scope.tempPath2[tem][0] = [];
          $scope.tempPath2[tem][1] = [];
          $scope.tempPath2[tem][0][0] = currNode.x;
          $scope.tempPath2[tem][0][1] = currNode.y;
          $scope.tempPath2[tem][1][0] = currNode.parent.x;
          $scope.tempPath2[tem][1][1] = currNode.parent.y;
        */
        if (currNode.x == goalcorner.x && currNode.y == goalcorner.y
          && currNode.cornerType == goalcorner.cornerType) {
          console.log("Reached goal!");
          reachedGoal = true;
          break;
        }
      } else {
        console.log("Open list empty");
        break;
      }
    }
    if (reachedGoal) {
      curr = currNode;
      while (curr.x != startcorner.x || curr.y != startcorner.y
          || curr.cornerType != startcorner.cornerType) {
        path.push(curr);
        //var tem = $scope.tempPath2.length;
        curr = curr.parent;

      }
      path.push(startcorner);
    }
    path = path.reverse();
    path = this.smoothPath(path);
    return path;
  };

  return ThetaStarSearch;

})();
const getSuccessors = function( node ) {
    var successors;
    successors = [];
    let w = towerDef.width,
        h = towerDef.height;
    if ( node.y > 0 && IsTraversable( node.x, node.y -1 )) {
        successors.push( { x: node.x, y: node.y - 1 } );
    }
    if (node.y > 0 && node.x > 0  && IsTraversable( node.x -1, node.y -1 )) {
        successors.push( { x: node.x - 1, y: node.y - 1 } );
    }
    if ( node.y > 0 && node.x < w - 1  && IsTraversable( node.x + 1, node.y - 1 )) {
        successors.push(  { x: node.x + 1, y: node.y - 1 }  );
    }
    if ( node.x > 0  && IsTraversable( node.x - 1, node.y )) {
        successors.push( { x: node.x - 1, y: node.y } );
    }
    if ( node.x < w - 1  && IsTraversable( node.x + 1, node.y )) {
        successors.push( { x: node.x + 1, y: node.y } );
    }
    if ( node.y < h - 1 && node.x > 0  && IsTraversable( node.x - 1, node.y + 1 ) ) {
        successors.push( { x: node.x - 1, y: node.y + 1 } );
    }
    if (node.y < h - 1 && node.x < w - 1  && IsTraversable( node.x - 1, node.y + 1 ) ) {
        successors.push( { x: node.x + 1, y: node.y + 1 } );
    }
    if ( node.y < h - 1  && IsTraversable( node.x, node.y + 1 ) ) {
        successors.push( { x: node.x, y: node.y + 1 } );
    }
    return successors;
};
