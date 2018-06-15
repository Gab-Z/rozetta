var lines = [];
var canvases = [];

var AStarSearch, Absolute, Cell, Corner, CornerAtDiagonalIntersection, EuclideanDistance, Grid, IsTraversable, LineOfSight, ManhattanDistance, Obstacle, OctileDistance, ThetaStarSearch, corner, goal, grid, i, kCornerNormal, kCornerNorthEast, kCornerNorthWest, kCornerSouthEast, kCornerSouthWest, len, myGrid, path, searchie, square, start;


var point = {
  start: undefined,
  goal: undefined,
};
kCornerNormal = "normal";

kCornerNorthWest = "northWest";

kCornerNorthEast = "northEast";

kCornerSouthWest = "southWest";

kCornerSouthEast = "southEast";
 var unit;

var mapComplex = [
    [0,4],
    [0,7],
    [0,11],
    [0,14],
    [0,15],
    [0,16],
    [0,17],
    [0,18],
    [0,22],
    [0,27],
    [1,2],
    [1,4],
    [1,7],
    [1,11],
    [1,18],
    [1,22],
    [1,25],
    [1,27],
    [2,0],
    [2,1],
    [2,2],
    [2,7],
    [2,14],
    [2,15],
    [2,16],
    [2,17],
    [2,18],
    [2,22],
    [2,25],
    [3,4],
    [3,5],
    [3,6],
    [3,7],
    [3,11],
    [3,14],
    [3,15],
    [3,16],
    [3,17],
    [3,18],
    [3,19],
    [3,20],
    [3,22],
    [3,23],
    [3,24],
    [3,25],
    [3,27],
    [3,28],
    [3,29],
    [4,2],
    [4,7],
    [4,8],
    [4,9],
    [4,10],
    [4,11],
    [4,16],
    [5,2],
    [5,4],
    [5,7],
    [5,11],
    [5,12],
    [5,13],
    [5,16],
    [5,19],
    [5,21],
    [5,22],
    [5,24],
    [5,25],
    [5,27],
    [6,0],
    [6,1],
    [6,2],
    [6,4],
    [6,5],
    [6,6],
    [6,7],
    [6,11],
    [6,13],
    [6,16],
    [6,17],
    [6,19],
    [6,22],
    [6,24],
    [6,27],
    [7,7],
    [7,8],
    [7,19],
    [7,22],
    [7,24],
    [7,27],
    [8,2],
    [8,4],
    [8,8],
    [8,9],
    [8,10],
    [8,11],
    [8,13],
    [8,14],
    [8,15],
    [8,16],
    [8,19],
    [8,20],
    [8,21],
    [8,22],
    [8,24],
    [8,25],
    [8,26],
    [8,27],
    [9,1],
    [9,2],
    [9,4],
    [9,5],
    [9,8],
    [9,11],
    [9,13],
    [9,16],
    [9,19],
    [9,21],
    [9,22],
    [10,1],
    [10,2],
    [10,4],
    [10,5],
    [10,7],
    [10,8],
    [10,11],
    [10,13],
    [10,16],
    [10,21],
    [10,22],
    [10,27],
    [11,1],
    [11,2],
    [11,7],
    [11,11],
    [11,16],
    [11,17],
    [11,18],
    [11,19],
    [11,21],
    [11,22],
    [11,23],
    [11,24],
    [11,27],
    [11,28],
    [11,29],
    [12,1],
    [12,2],
    [12,4],
    [12,5],
    [12,7],
    [12,8],
    [12,10],
    [12,11],
    [12,13],
    [12,14],
    [12,15],
    [12,16],
    [12,19],
    [13,1],
    [13,2],
    [13,4],
    [13,5],
    [13,8],
    [13,10],
    [13,19],
    [13,22],
    [13,25],
    [13,26],
    [13,27],
    [14,2],
    [14,4],
    [14,8],
    [14,10],
    [14,14],
    [14,16],
    [14,22],
    [14,25],
    [15,4],
    [15,5],
    [15,10],
    [15,14],
    [15,16],
    [15,17],
    [15,18],
    [15,19],
    [15,20],
    [15,21],
    [15,22],
    [15,24],
    [15,25],
    [16,0],
    [16,1],
    [16,2],
    [16,8],
    [16,9],
    [16,10],
    [16,11],
    [16,12],
    [16,13],
    [16,14],
    [16,16],
    [16,24],
    [16,25],
    [16,26],
    [16,27],
    [16,28],
    [17,5],
    [17,10],
    [17,16],
    [17,19],
    [17,20],
    [17,24],
    [17,25],
    [18,2],
    [18,3],
    [18,5],
    [18,13],
    [18,14],
    [18,15],
    [18,16],
    [18,19],
    [18,20],
    [18,21],
    [18,24],
    [19,2],
    [19,3],
    [19,7],
    [19,8],
    [19,9],
    [19,10],
    [19,11],
    [19,13],
    [19,19],
    [19,20],
    [19,21],
    [19,24]
];

var mapSal = [
    [0,7],
    [0,9],
    [0,11],
    [0,14],
    [0,15],
    [0,16],
    [0,23],
    [0,25],
    [0,26],
    [0,28],
    [0,29],
    [1,14],
    [1,15],
    [1,16],
    [1,23],
    [2,0],
    [2,1],
    [2,2],
    [2,3],
    [2,4],
    [2,7],
    [2,9],
    [2,11],
    [2,14],
    [2,15],
    [2,16],
    [2,23],
    [2,25],
    [2,26],
    [2,28],
    [2,29],
    [3,4],
    [3,5],
    [3,6],
    [3,7],
    [3,8],
    [3,9],
    [3,10],
    [3,11],
    [3,12],
    [3,14],
    [3,15],
    [3,16],
    [3,17],
    [3,18],
    [3,19],
    [3,22],
    [3,23],
    [4,0],
    [4,1],
    [4,25],
    [4,26],
    [4,28],
    [4,29],
    [5,1],
    [5,7],
    [6,3],
    [6,4],
    [6,5],
    [6,6],
    [6,7],
    [6,16],
    [6,17],
    [6,18],
    [6,19],
    [6,20],
    [6,21],
    [6,22],
    [6,23],
    [6,25],
    [6,26],
    [6,28],
    [6,29],
    [7,1],
    [7,3],
    [7,18],
    [7,23],
    [8,0],
    [8,1],
    [8,3],
    [8,18],
    [8,23],
    [8,24],
    [8,25],
    [8,26],
    [8,27],
    [8,28],
    [8,29],
    [9,3],
    [9,7],
    [9,8],
    [9,12],
    [9,13],
    [9,18],
    [9,19],
    [9,20],
    [9,21],
    [9,22],
    [9,23],
    [9,24],
    [9,25],
    [9,26],
    [9,27],
    [9,28],
    [9,29],
    [10,0],
    [10,1],
    [10,2],
    [10,3],
    [10,7],
    [10,8],
    [10,12],
    [10,13],
    [10,23],
    [10,24],
    [10,25],
    [10,26],
    [10,27],
    [10,28],
    [10,29],
    [11,19],
    [11,20],
    [11,21],
    [11,22],
    [11,23],
    [11,24],
    [11,25],
    [11,26],
    [11,27],
    [11,28],
    [11,29],
    [12,3],
    [12,4],
    [12,8],
    [12,9],
    [12,14],
    [12,15],
    [12,23],
    [12,24],
    [12,25],
    [12,26],
    [12,27],
    [12,28],
    [12,29],
    [13,3],
    [13,4],
    [13,8],
    [13,9],
    [13,14],
    [13,15],
    [13,23],
    [13,24],
    [13,25],
    [13,26],
    [13,27],
    [13,28],
    [13,29],
    [14,19],
    [14,20],
    [14,21],
    [14,22],
    [14,23],
    [14,24],
    [14,25],
    [14,26],
    [14,27],
    [14,28],
    [14,29],
    [15,23],
    [15,24],
    [15,25],
    [15,26],
    [15,27],
    [15,28],
    [15,29],
    [16,0],
    [16,1],
    [16,2],
    [16,5],
    [16,6],
    [16,7],
    [16,8],
    [16,9],
    [16,10],
    [16,17],
    [16,23],
    [16,24],
    [16,25],
    [16,26],
    [16,27],
    [16,28],
    [16,29],
    [17,6],
    [17,13],
    [17,17],
    [17,19],
    [17,22],
    [17,23],
    [17,24],
    [17,25],
    [17,26],
    [17,27],
    [17,28],
    [17,29],
    [18,2],
    [18,6],
    [18,13],
    [18,17],
    [18,19],
    [18,22],
    [18,23],
    [18,24],
    [18,25],
    [18,26],
    [18,27],
    [18,28],
    [18,29],
    [19,2],
    [19,6],
    [19,13],
    [19,17],
    [19,19],
    [19,22],
    [19,23],
    [19,24],
    [19,25],
    [19,26],
    [19,27],
    [19,28],
    [19,29]
];

var mapEqaud = [
    [1,3],
    [1,5],
    [1,6],
    [1,8],
    [1,12],
    [1,13],
    [1,14],
    [1,15],
    [1,16],
    [1,18],
    [1,19],
    [1,20],
    [1,21],
    [1,22],
    [1,23],
    [1,24],
    [1,25],
    [1,26],
    [2,3],
    [2,6],
    [2,8],
    [2,9],
    [2,10],
    [2,12],
    [2,13],
    [2,14],
    [2,15],
    [2,16],
    [2,18],
    [2,26],
    [3,3],
    [3,4],
    [3,5],
    [3,6],
    [3,8],
    [3,9],
    [3,10],
    [3,12],
    [3,13],
    [3,14],
    [3,15],
    [3,16],
    [3,18],
    [3,26],
    [6,3],
    [6,4],
    [6,5],
    [6,6],
    [6,11],
    [6,12],
    [6,13],
    [6,14],
    [6,15],
    [6,16],
    [6,17],
    [7,3],
    [7,6],
    [7,11],
    [7,17],
    [8,3],
    [8,11],
    [8,13],
    [8,15],
    [8,17],
    [9,3],
    [9,6],
    [9,11],
    [9,13],
    [9,15],
    [9,17],
    [9,21],
    [9,22],
    [9,23],
    [9,24],
    [9,25],
    [9,26],
    [10,3],
    [10,6],
    [10,11],
    [10,13],
    [10,15],
    [10,17],
    [10,21],
    [10,23],
    [10,26],
    [11,3],
    [11,6],
    [11,11],
    [11,13],
    [11,15],
    [11,17],
    [11,21],
    [11,23],
    [11,26],
    [12,6],
    [12,11],
    [12,13],
    [12,15],
    [12,17],
    [12,26],
    [13,3],
    [13,6],
    [13,11],
    [13,17],
    [13,21],
    [13,26],
    [14,3],
    [14,4],
    [14,5],
    [14,6],
    [14,11],
    [14,12],
    [14,13],
    [14,14],
    [14,15],
    [14,16],
    [14,17],
    [14,21],
    [14,23],
    [15,21],
    [15,23],
    [15,24],
    [15,26],
    [16,3],
    [16,4],
    [16,5],
    [16,6],
    [16,21],
    [16,23],
    [16,26],
    [17,3],
    [17,21],
    [17,23],
    [17,26],
    [18,3],
    [18,6],
    [18,21],
    [18,22],
    [18,23],
    [18,24],
    [18,25],
    [18,26],
    [19,3],
    [19,6],
    [19,9],
    [19,10],
    [19,11],
    [19,14],
    [19,15],
    [19,16],
    [19,17],
    [19,18],
    [19,19]
];

var mapManhattan = [
    [0,0],
    [0,1],
    [0,2],
    [0,3],
    [0,6],
    [0,7],
    [0,8],
    [0,9],
    [0,10],
    [0,11],
    [0,13],
    [0,14],
    [0,16],
    [0,17],
    [0,18],
    [0,21],
    [0,22],
    [0,23],
    [0,25],
    [0,26],
    [0,27],
    [0,29],
    [2,0],
    [2,1],
    [2,3],
    [2,6],
    [2,7],
    [2,8],
    [2,9],
    [2,10],
    [2,11],
    [2,13],
    [2,14],
    [2,16],
    [2,17],
    [2,18],
    [2,21],
    [2,22],
    [2,23],
    [2,25],
    [2,27],
    [2,29],
    [3,0],
    [3,1],
    [3,3],
    [3,6],
    [3,7],
    [3,8],
    [3,9],
    [3,10],
    [3,11],
    [3,13],
    [3,14],
    [3,16],
    [3,17],
    [3,18],
    [3,21],
    [3,22],
    [3,23],
    [3,25],
    [3,27],
    [3,29],
    [4,0],
    [4,1],
    [4,16],
    [4,17],
    [4,18],
    [5,3],
    [5,6],
    [5,7],
    [5,8],
    [5,9],
    [5,10],
    [5,13],
    [5,14],
    [5,16],
    [5,17],
    [5,18],
    [5,21],
    [5,22],
    [5,23],
    [5,25],
    [5,26],
    [5,27],
    [5,29],
    [6,1],
    [6,2],
    [6,3],
    [6,6],
    [6,7],
    [6,8],
    [6,9],
    [6,13],
    [6,14],
    [6,16],
    [6,17],
    [6,21],
    [6,22],
    [6,23],
    [6,25],
    [6,26],
    [6,27],
    [6,29],
    [7,1],
    [7,2],
    [7,3],
    [7,6],
    [7,7],
    [7,8],
    [7,11],
    [8,6],
    [8,7],
    [8,10],
    [8,11],
    [8,13],
    [8,14],
    [8,16],
    [8,17],
    [8,18],
    [8,21],
    [8,22],
    [8,23],
    [8,24],
    [8,25],
    [8,27],
    [8,29],
    [9,0],
    [9,1],
    [9,2],
    [9,3],
    [9,6],
    [9,10],
    [9,11],
    [9,13],
    [9,14],
    [9,21],
    [9,22],
    [9,23],
    [9,24],
    [9,25],
    [9,26],
    [9,27],
    [9,29],
    [10,0],
    [10,1],
    [10,2],
    [10,6],
    [10,13],
    [10,14],
    [10,16],
    [10,17],
    [10,18],
    [10,21],
    [10,22],
    [10,23],
    [10,24],
    [10,25],
    [10,26],
    [10,27],
    [10,29],
    [11,0],
    [11,1],
    [11,2],
    [11,3],
    [11,6],
    [11,7],
    [11,9],
    [11,10],
    [11,11],
    [11,13],
    [11,14],
    [11,16],
    [11,17],
    [11,18],
    [11,21],
    [11,22],
    [11,23],
    [11,24],
    [11,25],
    [11,27],
    [13,0],
    [13,1],
    [13,3],
    [13,6],
    [13,7],
    [13,9],
    [13,10],
    [13,13],
    [13,16],
    [13,17],
    [13,18],
    [13,21],
    [13,22],
    [13,24],
    [13,26],
    [13,27],
    [13,29],
    [14,0],
    [14,1],
    [14,3],
    [14,6],
    [14,7],
    [14,9],
    [14,10],
    [14,11],
    [14,13],
    [14,14],
    [14,16],
    [14,17],
    [14,18],
    [14,21],
    [14,22],
    [14,24],
    [14,26],
    [14,27],
    [14,29],
    [15,0],
    [15,1],
    [15,3],
    [15,6],
    [15,7],
    [15,9],
    [15,10],
    [15,11],
    [15,13],
    [15,14],
    [15,16],
    [15,17],
    [15,21],
    [15,22],
    [15,24],
    [15,26],
    [15,27],
    [15,29],
    [16,6],
    [16,29],
    [17,1],
    [17,2],
    [17,3],
    [17,6],
    [17,7],
    [17,9],
    [17,10],
    [17,11],
    [17,13],
    [17,14],
    [17,16],
    [17,17],
    [17,18],
    [17,21],
    [17,22],
    [17,24],
    [17,25],
    [17,27],
    [17,29],
    [18,1],
    [18,2],
    [18,3],
    [18,9],
    [18,10],
    [18,13],
    [18,14],
    [18,16],
    [18,17],
    [18,18],
    [18,21],
    [18,22],
    [18,27],
    [18,29],
    [19,1],
    [19,2],
    [19,3],
    [19,6],
    [19,7],
    [19,9],
    [19,10],
    [19,11],
    [19,13],
    [19,14],
    [19,21],
    [19,24],
    [19,25],
    [19,27],
    [19,29]
];

var mapSkyline = [
    [1,15],
    [1,16],
    [1,17],
    [1,18],
    [1,19],
    [2,15],
    [2,19],
    [3,14],
    [3,15],
    [3,16],
    [3,19],
    [3,20],
    [3,23],
    [3,24],
    [3,25],
    [3,26],
    [3,27],
    [4,14],
    [4,20],
    [4,23],
    [4,27],
    [5,14],
    [5,20],
    [5,23],
    [5,24],
    [5,26],
    [5,27],
    [5,29],
    [6,12],
    [6,13],
    [6,14],
    [6,15],
    [6,16],
    [6,20],
    [6,23],
    [6,27],
    [6,29],
    [7,16],
    [7,17],
    [7,19],
    [7,20],
    [7,21],
    [7,23],
    [7,24],
    [7,26],
    [7,27],
    [7,29],
    [8,9],
    [8,10],
    [8,11],
    [8,12],
    [8,13],
    [8,16],
    [8,21],
    [8,23],
    [8,27],
    [8,28],
    [8,29],
    [9,9],
    [9,12],
    [9,13],
    [9,16],
    [9,21],
    [9,23],
    [9,24],
    [9,26],
    [9,27],
    [10,9],
    [10,12],
    [10,13],
    [10,15],
    [10,16],
    [10,17],
    [10,18],
    [10,21],
    [10,27],
    [11,9],
    [11,13],
    [11,15],
    [11,18],
    [11,21],
    [11,22],
    [11,23],
    [11,24],
    [11,25],
    [11,27],
    [12,9],
    [12,10],
    [12,13],
    [12,15],
    [12,18],
    [12,21],
    [12,22],
    [12,25],
    [13,9],
    [13,12],
    [13,13],
    [13,15],
    [13,18],
    [13,21],
    [13,22],
    [13,24],
    [13,25],
    [13,26],
    [13,27],
    [14,4],
    [14,5],
    [14,9],
    [14,10],
    [14,15],
    [14,18],
    [14,20],
    [14,21],
    [14,22],
    [14,26],
    [14,27],
    [15,2],
    [15,3],
    [15,4],
    [15,5],
    [15,8],
    [15,9],
    [15,10],
    [15,11],
    [15,13],
    [15,15],
    [15,18],
    [15,22],
    [15,25],
    [15,26],
    [15,27],
    [16,1],
    [16,2],
    [16,3],
    [16,5],
    [16,7],
    [16,8],
    [16,10],
    [16,13],
    [16,18],
    [16,24],
    [16,26],
    [16,27],
    [17,1],
    [17,7],
    [17,10],
    [17,13],
    [17,15],
    [17,18],
    [17,22],
    [17,27],
    [18,1],
    [18,4],
    [18,5],
    [18,10],
    [18,13],
    [18,15],
    [18,18],
    [18,22],
    [18,24],
    [18,26],
    [18,27],
    [19,1],
    [19,5],
    [19,7],
    [19,15],
    [19,22],
    [19,27]
];

var expanding = 0;

var app = angular.module('anyAngle', ['ngMaterial']);

app.controller('mainController', function($scope,$http) {


      $scope.init = function(){
        canvases[0]=document.getElementById("mainCanvas");
        canvases[1]=document.getElementById("mainCanvas2");
        canvases[2]=document.getElementById("mainCanvas3");


        lines[0] = canvases[0].getContext("2d");
        lines[1] = canvases[1].getContext("2d");
        lines[2] = canvases[2].getContext("2d");

        lines[0].clearRect(0, 0, canvases[0].width, canvases[0].height);
        lines[1].clearRect(0, 0, canvases[1].width, canvases[1].height);
        lines[2].clearRect(0, 0, canvases[2].width, canvases[2].height);


        unit = 30;

        lines[0] .lineWidth=8;
        lines[0] .beginPath();
        lines[0] .strokeStyle = '#04FAA8';


        lines[1] .lineWidth=1;
        lines[1] .beginPath();
        lines[1] .strokeStyle = '#C95555';


        lines[2].lineWidth = 2;
        lines[2].beginPath();
        lines[2].strokeStyle = '#80AEFF';

        $scope.tempPath = [];
        $scope.tempPath2 = [];
        $scope.openList = [];
        //Initialize search type
        $scope.searchType = "AStar";


        ///Initialize expansion and total length
        expanding = 0;
        $scope.expansion = 0;
        $scope.totalLength = 0;
        $scope.pointType = "start";
        $scope.cornerTypeStart = "northEast";
        $scope.cornerTypeGoal = "northEast";
        $scope.simulationMode = "immediate";
        $scope.simulationSpeed = 250;
        point = {};


        //init grid
        $scope.size = {
          row: 20,
          column: 30
        };
        $scope.cell = [];
        $scope.corners = [];
        for(var i = 0 ; i < $scope.size.row; i ++){
            $scope.cell[i] = [];
          for (var j = 0; j < $scope.size.column ; j++){
            $scope.cell[i][j] = {};
            $scope.cell[i][j].isObstacle = false;
            $scope.cell[i][j].corners = [];
          }
        }
         for(var i = 0 ; i < $scope.size.row; i ++){
          for (var j = 0; j < $scope.size.column ; j++){
            $scope.cell[i][j].isObstacle = false;
            $scope.cell[i][j].corners = [];
          }
        }
         $scope.cornersMap = [];
        $scope.corners = [];
        for(var i = 0 ; i < $scope.size.row + 1; i ++){
            $scope.cornersMap[i] = [];
          for (var j = 0; j < $scope.size.column + 1; j++){
            $scope.cornersMap[i][j] = [];
          }
        }

        $scope.submitMap();
        return true;
      };

      $scope.clearCanvas = function(){
        canvases[0]=document.getElementById("mainCanvas");
        canvases[1]=document.getElementById("mainCanvas2");
        canvases[2]=document.getElementById("mainCanvas3");

        lines[0] = canvases[0].getContext("2d");
        lines[1] = canvases[1].getContext("2d");
        lines[2] = canvases[2].getContext("2d");

        lines[0].clearRect(0, 0, canvases[0].width, canvases[0].height);
        lines[1].clearRect(0, 0, canvases[1].width, canvases[1].height);
        lines[2].clearRect(0, 0, canvases[2].width, canvases[2].height);

        unit = 30;

        lines[0] .lineWidth=8;
        lines[0] .beginPath();
        lines[0] .strokeStyle = '#04FAA8';


        lines[1] .lineWidth=1;
        lines[1] .beginPath();
        lines[1] .strokeStyle = '#C95555';


        lines[2].lineWidth = 2;
        lines[2].beginPath();
        lines[2].strokeStyle = '#80AEFF';

        $scope.tempPath = [];
        $scope.tempPath2 = [];
        $scope.openList = [];
        expanding = 0;
        $scope.expansion = 0;
        $scope.totalLength = 0;
      };


      $scope.getNumber = function(num) {
          return new Array(num);
      };
      $scope.gridClicked = function(row, column){

            $scope.cell[row][column].isObstacle = !$scope.cell[row][column].isObstacle;
            $scope.submitMap();

            if(point.start && point.goal){

              $scope.submitStartGoal('start', $scope.cornerTypeStart, point.startY, point.startX);
              $scope.submitStartGoal('goal', $scope.cornerTypeGoal, point.goalY, point.goalX);
              $scope.clearCanvas();
              $scope.search();

            }
      };
      $scope.modeChanged = function(type){
        if(point.start && point.goal){
          $scope.simulationMode = type;
          $scope.submitMap();
          $scope.clearCanvas();
          $scope.search();
        }
      };
      $scope.searchTypeChanged = function(type){
          if(point.start && point.goal){
              $scope.searchType = type;
              $scope.submitMap();
              $scope.clearCanvas();
              $scope.search();
          }
      };

    $scope.cornerTypeChanged = function(type, cornerType){
          if(point.start && point.goal){
              var row;
              var column;

              if(type == 'start'){
                if($scope.cornerTypeStart == 'northEast'){
                  row = point.start.y;
                  column = point.start.x - 1;
                }else if($scope.cornerTypeStart == 'northWest'){
                  row = point.start.y;
                  column = point.start.x;
                }else if($scope.cornerTypeStart == 'southEast'){
                  row = point.start.y - 1;
                  column = point.start.x - 1;
                }else if($scope.cornerTypeStart == 'southWest'){
                  row = point.start.y - 1;
                  column = point.start.x;
                }

              }else{
                if($scope.cornerTypeGoal == 'northEast'){
                  row = point.goal.y;
                  column = point.goal.x - 1;
                }else if($scope.cornerTypeGoal == 'northWest'){
                  row = point.goal.y;
                  column = point.goal.x;
                }else if($scope.cornerTypeGoal == 'southEast'){
                  row = point.goal.y - 1;
                  column = point.goal.x - 1;
                }else if($scope.cornerTypeGoal == 'southWest'){
                  row = point.goal.y - 1;
                  column = point.goal.x;
                }

              }
              $scope.submitStartGoal(type, cornerType, row, column);
              $scope.submitMap();
              $scope.clearCanvas();
              $scope.search();
          }
    };





    square = function(x) {
      return x * x;
    };

    Absolute = function(a) {
      if (a < 0) {
        a = -a;
      }
      return a;
    };

    EuclideanDistance = function(startcorner, goalcorner) {
      return Math.sqrt((square(startcorner.x - goalcorner.x)) + (square(startcorner.y - goalcorner.y)));
    };

    ManhattanDistance = function(startCorner, goalCorner) {
      return Absolute(goalCorner.y - startCorner.y) + Absolute(goalCorner.x - startCorner.x);
    };

    OctileDistance = function(startCorner, goalCorner) {
      var dx, dy;
      dx = Absolute(startCorner.x - goalCorner.x);
      dy = Absolute(startCorner.y - goalCorner.y);
      return dx + dy + (1 - Math.sqrt(2)) * (Math.min(dx, dy));
    };

    CornerAtDiagonalIntersection = function(x, y) {
      if ($scope.cornersMap[y][x][0].cornerType == kCornerNormal)
        return false;
      return true;
    };

    IsTraversable = function(x, y) {
      if (myGrid[y][x].isObstacle) {
        return false;
      }
      return true;
    };


    //Line of Sight
    LineOfSight = function(startCorner, goalCorner) {
      var dx, dy, f, sx, sy, x1, x2, x_offset, y1, y2, y_offset;
      x1 = startCorner.x;
      x2 = goalCorner.x;
      y1 = startCorner.y;
      y2 = goalCorner.y;

      dy = y2 - y1;
      dx = x2 - x1;
      f = 0;
      sy = 0;
      sx = 0;
      if(dy < 0){
        dy = -dy;
        sy = -1;
      }
      else{
        sy = 1
      }

      if(dx < 0){
        dx = -dx;
        sx = -1;
      }
      else{
        sx = 1;
      }

      if(dx >= dy){
        while(x1 != x2){
          f = f + dy;
          if(f >= dx){
            if(!IsTraversable(x1 + ((sx - 1)/2), y1 + (sy - 1)/2)){
              return false;
            }

            y1 = y1 + sy;
            f = f - dx;
          }
          if(f != 0 && !IsTraversable(x1 + ((sx - 1)/2), y1 + (sy - 1)/2)){
            return false;
          }
          if(dy == 0 && !IsTraversable(x1 + ((sx - 1)/2), y1)  && !IsTraversable(x1 + ((sx - 1)/2), y1 - 1)){
            return false;
          }
          x1 = x1 + sx;
        }
      }
      else{
        while(y1 != y2){
          f = f + dx;
          if(f >= dy){
             if(!IsTraversable(x1 + ((sx - 1)/2), y1 + (sy - 1)/2)){
              return false;
            }
            x1 = x1 + sx;
            f = f - dy;
          }
          if(f != 0 && !IsTraversable(x1 + ((sx - 1)/2), y1 + (sy - 1)/2)){
            return false;
          }
          if(dy == 0 && !IsTraversable(x1, y1 + ((sy - 1)/2))  && !IsTraversable(x1 - 1, y1 + ((sy - 1)/2))){
            return false;
          }
          y1 = y1 +sy;
        }
      }
      return true;
  };


    //Theta Star Function
  ThetaStarSearch = (function() {
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
      ref = node.getSuccessors();
      results = [];
      for (let i = 0, let len = ref.length; i < len; i++) {
        corner = ref[i];
        inClosed = false;
        ref1 = this.ClosedList;
        for (let j = 0, let len1 = ref1.length; j < len1; j++) {
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

        var openListLength = $scope.openList.length;
        $scope.openList[openListLength] = [];
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
          $scope.openList[openListLength].push(nodeInfo);
          //openlist visualization

          if (node.fVal <= minFval) {
            minFval = node.fVal;
            currNode = node;
          }
        }
        if (currNode) {
          this.addToClosed(currNode);
          var tem = $scope.tempPath2.length;
            $scope.tempPath2[tem] = [];
            $scope.tempPath2[tem][0] = [];
            $scope.tempPath2[tem][1] = [];
            $scope.tempPath2[tem][0][0] = currNode.x;
            $scope.tempPath2[tem][0][1] = currNode.y;
            $scope.tempPath2[tem][1][0] = currNode.parent.x;
            $scope.tempPath2[tem][1][1] = currNode.parent.y;
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
          var tem = $scope.tempPath2.length;
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


    //AStar
  AStarSearch = (function() {
    function AStarSearch(heuristic) {
      this.Heuristic = heuristic;
      this.OpenList = [];
      this.ClosedList = [];
    }

    AStarSearch.prototype.smoothPath = function(path) {
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

    AStarSearch.prototype.addToClosed = function(node) {
      var corner, i, inClosed, inListNode, indexOfNode, indexToRemove, j, k, len, len1, len2, num, ourFval, ref, ref1, ref2, results;
      this.ClosedList.push(node);
      indexToRemove = this.OpenList.indexOf(node);
      if (!(indexToRemove < 0)) {
        this.OpenList.splice(indexToRemove, 1);
      }
      ref = node.getSuccessors();

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
              //found in open node
              indexOfNode = num;
              break;
            }
          }
          num += 1;
        }
        if (indexOfNode < 0) {
          //Not Found in open node
          corner.gVal = node.gVal + Math.sqrt(((corner.x - node.x) * (corner.x - node.x)) + ((corner.y - node.y) * (corner.y - node.y)));

          if (this.Heuristic === "EuclideanDistance") {
            corner.hVal = EuclideanDistance(corner, this.Goalcorner);
          } else if (this.Heuristic === "OctileDistance") {
            corner.hVal = OctileDistance(corner, this.Goalcorner);
          } else {
            corner.hVal = ManhattanDistance(corner, this.Goalcorner);
          }
          corner.fVal = corner.gVal + corner.hVal;
          corner.parent = node;
          results.push(this.OpenList.push(corner));
        } else {
          //Found in open node
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

          if (ourFval < this.OpenList[indexOfNode].fVal) {
            this.OpenList.splice(indexOfNode, 1);
            corner.gVal = node.gVal + Math.sqrt(((corner.x - node.x) * (corner.x - node.x)) + ((corner.y - node.y) * (corner.y - node.y)));
            if (this.Heuristic === "EuclideanDistance") {
              corner.hVal = EuclideanDistance(corner, this.Goalcorner);
            } else if (this.Heuristic === "OctileDistance") {
              corner.hVal = OctileDistance(corner, this.Goalcorner);
            } else {
              corner.hVal = ManhattanDistance(corner, this.Goalcorner);
            }
            corner.fVal = corner.gVal + corner.hVal;
            corner.parent = node;
            results.push(this.OpenList.push(corner));
          } else {
            results.push(void 0);
          }
        }
      }
      return results;
    };

    AStarSearch.prototype.search = function(startcorner, goalcorner) {
      var curr, currNode, i, len, minFval, node, path, reachedGoal, ref;
      this.Goalcorner = goalcorner;
      path = [];

      if (startcorner === goalcorner) {
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

        //openlist visualization
          var openListLength = $scope.openList.length;
          $scope.openList[openListLength] = [];
        for (i = 0, len = ref.length; i < len; i++) {
          node = ref[i];


          var nodeInfo = [];
          nodeInfo[0] = node.x;
          nodeInfo[1] = node.y;
          var openParent = [];
          openParent[0] = node.parent.x;
          openParent[1] = node.parent.y;
          nodeInfo[2] = openParent;
          $scope.openList[openListLength].push(nodeInfo);
          //openlist visualization
          if (node.fVal < minFval) {
            minFval = node.fVal;
            currNode = node;
          }
        }
        if (currNode) {


          this.addToClosed(currNode);
          var tem = $scope.tempPath2.length;
          $scope.tempPath2[tem] = [];
          $scope.tempPath2[tem][0] = [];
          $scope.tempPath2[tem][1] = [];
          $scope.tempPath2[tem][0][0] = currNode.x;
          $scope.tempPath2[tem][0][1] = currNode.y;
          $scope.tempPath2[tem][1][0] = currNode.parent.x;
          $scope.tempPath2[tem][1][1] = currNode.parent.y;
          //Add visualization code for Open and Closed lists here

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
        while (curr != startcorner) {
          path.push(curr);
          curr = curr.parent;
        }
        path.push(startcorner);
      }
      path = path.reverse();
      //Smoothing
      if($scope.searchType == "AStarWithSmoothing"){
        path = this.smoothPath(path);
      }

      return path;
    };

    return AStarSearch;

  })();

      var mapstring = []; //this array of strings represents the map

      function checkCase(y,x){

          var northWest = false;
          var northEast = false;
          var southWest = false;
          var southEast = false;
          var result = [];
          //check northWest Block

          if(x == 0 || y == 0){
            northWest = true;
          }else{
            if($scope.cell[y- 1][x - 1].isObstacle){
                northWest = true;
            }
          }

          //check northEast Block
          if(y == 0 || x == 30){
            northEast = true;
          }else{
            if($scope.cell[y -1][x].isObstacle){
                northEast = true;
            }
          }

          //check southWest Block
          if(x == 0 || y == 20){
            southWest = true;
          }else{
            if($scope.cell[y][x - 1].isObstacle){
                southWest = true;
            }
          }
          //check southEast Block
          if(x == 30 || y == 20){
            southEast = true;
          }else{
            if($scope.cell[y][x].isObstacle){
                southEast = true;
            }
          }

          if(!northWest && !northEast && !southWest && !southEast){
            result[0] = 0;
            result[1] = [];
            result[1] = [1,1,1,1];
            return result; // all way normal
          }

          //three way normal
          if(northWest && !northEast && !southWest && !southEast){
             result[0] = 0;
            result[1] = [];
            result[1] = [0,1,1,1];
            return result; //except northwest
          }

          if(!northWest && northEast && !southWest && !southEast){
             result[0] = 0;
            result[1] = [];
            result[1] = [1,0,1,1];
            return result; //except northeast
          }
          if (!northWest && !northEast && southWest && !southEast){
             result[0] = 0;
            result[1] = [];
            result[1] = [1,1,1,0];
            return result; //except southwest
          }
          if(!northWest && !northEast && !southWest && southEast){
             result[0] = 0;
            result[1] = [];
            result[1] = [1,1,0,1];
            return result; //except southeast
          }


          //two way normal
          if(northWest && northEast && !southWest && !southEast){
            result[0] = 0;
            result[1] = [];
            result[1] = [0,0,1,1];
            return result; //except northwest and east
          }
          if(!northWest && northEast && !southWest && southEast){
                result[0] = 0;
            result[1] = [];
            result[1] = [1,0,0,1];
            return result; //except northeast and southeast
          }
          if(!northWest && !northEast && southWest && southEast){
               result[0] = 0;
            result[1] = [];
            result[1] = [1,1,0,0];
            return result; //except southwest and southeast
          }

          if(northWest && !northEast && southWest && !southEast){
            result[0] = 0;
            result[1] = [];
            result[1] = [0,1,1,0];
            return result; //except southwest and northwest
          }


          //one corner
          if(northWest && northEast && southWest && !southEast){
            result[0] = 1;  // one corner
            result[1] = [];
            result[1] = [0,0,1,0];
            return result;
          }
          if (northWest && northEast && !southWest && southEast){
               result[0] = 1;  // one corner
            result[1] = [];
            result[1] = [0,0,0,1];
            return result;
          }
          if(!northWest && northEast && southWest && southEast){
              result[0] = 1;  // one corner
            result[1] = [];
            result[1] = [1,0,0,0];
            return result;
          }
          if (northWest && !northEast && southWest && southEast){
             result[0] = 1;  // one corner
            result[1] = [];
            result[1] = [0,1,0,0];
            return result;
          }


        //two corners
        if(!northWest && northEast && southWest && !southEast){
          result[0] = 2;  // two corners
            result[1] = [];
            result[1] = [1,0,1,0];
            return result;
        }
        if(northWest && !northEast && !southWest && southEast){
             result[0] = 2;  // two corners
            result[1] = [];
            result[1] = [0,1,0,1];
            return result;
        }

      };

      function checkNeighborhood(corner){
        var x = corner.x;
        var y = corner.y;

        //check not normal corner cases
        if(corner.cornerType == "northWest"){
          //south
          for(var k = 0; k < $scope.cornersMap[y + 1][x].length; k++){
            if($scope.cornersMap[y + 1][x][k].cornerType== "southWest" || $scope.cornersMap[y + 1][x][k].cornerType == "normal"){
              corner.South = $scope.cornersMap[y + 1][x][k];
            }
          }
          //east
          for(var k = 0; k < $scope.cornersMap[y][x + 1].length; k++){
            if($scope.cornersMap[y][x + 1][k].cornerType == "northEast" || $scope.cornersMap[y][x + 1][k].cornerType == "normal"){
              corner.East = $scope.cornersMap[y][x + 1][k];
            }
          }
          //southeast
          for(var k = 0; k < $scope.cornersMap[y + 1][x + 1].length; k++){
            if($scope.cornersMap[y + 1][x + 1][k].cornerType == "southEast" || $scope.cornersMap[y + 1][x + 1][k].cornerType == "normal"){
              corner.SouthEast = $scope.cornersMap[y + 1][x + 1][k];
            }
          }
        }
        else if(corner.cornerType == "northEast"){
          //west
          for(var k = 0; k < $scope.cornersMap[y][x - 1].length; k++){
            if($scope.cornersMap[y][x - 1][k].cornerType == "northWest" || $scope.cornersMap[y][x - 1][k].cornerType == "normal"){
              corner.West = $scope.cornersMap[y][x - 1][k];
            }
          }
          //south
          for(var k = 0; k < $scope.cornersMap[y + 1][x].length; k++){
            if($scope.cornersMap[y + 1][x][k].cornerType == "southEast" || $scope.cornersMap[y + 1][x][k].cornerType == "normal"){
              corner.South = $scope.cornersMap[y + 1][x][k];
            }
          }
          //southwest
          for(var k = 0; k < $scope.cornersMap[y + 1][x - 1].length; k++){
            if($scope.cornersMap[y + 1][x - 1][k].cornerType == "southWest" || $scope.cornersMap[y + 1][x - 1][k].cornerType == "normal"){
              corner.SouthWest = $scope.cornersMap[y + 1][x - 1][k];
            }
          }
        }
        else if(corner.cornerType == "southEast"){
          //north
          for(var k = 0; k < $scope.cornersMap[y - 1][x].length; k++){
            if($scope.cornersMap[y - 1][x][k].cornerType == "northEast" || $scope.cornersMap[y - 1][x][k].cornerType == "normal"){
              corner.North = $scope.cornersMap[y - 1][x][k];
            }
          }
          //northWest
          for(var k = 0; k < $scope.cornersMap[y - 1][x - 1].length; k++){
            if($scope.cornersMap[y - 1][x - 1][k].cornerType == "northWest" || $scope.cornersMap[y - 1][x - 1][k].cornerType == "normal"){
              corner.NorthWest = $scope.cornersMap[y - 1][x - 1][k];
            }
          }
          //west
          for(var k = 0; k < $scope.cornersMap[y][x - 1].length; k++){
            if($scope.cornersMap[y][x - 1][k].cornerType == "southWest" || $scope.cornersMap[y][x - 1][k].cornerType == "normal"){
              corner.West = $scope.cornersMap[y][x - 1][k];
            }
          }

        }
        else if(corner.cornerType == "southWest"){
          //north
          for(var k = 0; k < $scope.cornersMap[y - 1][x].length; k++){
            if($scope.cornersMap[y - 1][x][k].cornerType == "northWest" || $scope.cornersMap[y - 1][x][k].cornerType == "normal"){
              corner.NorthWest = $scope.cornersMap[y - 1][x][k];
            }
          }
          //northEast
          for(var k = 0; k < $scope.cornersMap[y - 1][x + 1].length; k++){
            if($scope.cornersMap[y - 1][x + 1][k].cornerType == "northEast" || $scope.cornersMap[y - 1][x + 1][k].cornerType == "normal"){
              corner.NorthEast = $scope.cornersMap[y - 1][x + 1][k];
            }
          }
          //east
          for(var k = 0; k < $scope.cornersMap[y][x + 1].length; k++){
            if($scope.cornersMap[y][x + 1][k].cornerType == "southEast" || $scope.cornersMap[y][x + 1][k].cornerType == "normal"){
              corner.East = $scope.cornersMap[y][x + 1][k];
            }
          }
        }
        else if(corner.cornerType == "normal"){

          var northWest = false;
          var northEast = false;
          var southWest = false;
          var southEast = false;
          //check northWest Block
          if(x == 0 || y == 0){
            northWest = true;
          }else{
            if($scope.cell[y - 1][x - 1].isObstacle){
                northWest = true;
            }
          }
          //check northEast Block
          if(y == 0 || x == 30){
            northEast = true;
          }else{
            if($scope.cell[y - 1][x].isObstacle){
                northEast = true;
            }
          }
          //check southWest Block
          if(x == 0 || y == 20){
            southWest = true;
          }else{
            if($scope.cell[y][x - 1].isObstacle){
                southWest = true;
            }
          }
          //check southEast Block
          if(x == 30 || y == 20){
            southEast = true;
          }else{
            if($scope.cell[y][x].isObstacle){
                southEast = true;
            }
          }
          var addNorthWest = false;
          var addNorth = false;
          var addNorthEast = false;
          var addEast = false;
          var addSouthEast = false;
          var addSouth = false;
          var addSouthWest = false;
          var addWest = false;

          if(x == 7 &&y == 2){
          console.log("here");
        }
          //all way case
          if(!northWest && !northEast && !southWest && !southEast){
            addNorthWest = true;
            addNorth = true;
            addNorthEast = true;
            addEast = true;
            addSouthEast = true;
            addSouth = true;
            addSouthWest = true;
            addWest = true;
          }
          //three way normal
          if(northWest && !northEast && !southWest && !southEast){
            addNorth = true;
            addNorthEast = true;
            addEast = true;
            addSouthEast = true;
            addSouth = true;
            addSouthWest = true;
            addWest = true;
          }
          if(!northWest && northEast && !southWest && !southEast){
            addNorthWest = true;
            addNorth = true;
            addEast = true;
            addSouthEast = true;
            addSouth = true;
            addSouthWest = true;
            addWest = true;
          }
          if (!northWest && !northEast && southWest && !southEast){
            addNorthWest = true;
            addNorth = true;
            addNorthEast = true;
            addEast = true;
            addSouthEast = true;
            addSouth = true;
            addWest = true;
          }
          if(!northWest && !northEast && !southWest && southEast){
            addNorthWest = true;
            addNorth = true;
            addNorthEast = true;
            addEast = true;
            addSouth = true;
            addSouthWest = true;
            addWest = true;
          }


          //two way normal
          if(northWest && northEast && !southWest && !southEast){
            addEast = true;
            addSouthEast = true;
            addSouth = true;
            addSouthWest = true;
            addWest = true;
          }
          if(!northWest && northEast && !southWest && southEast){
            addNorthWest = true;
            addNorth = true;
            addSouth = true;
            addSouthWest = true;
            addWest = true;
          }
          if(!northWest && !northEast && southWest && southEast){
            addNorthWest = true;
            addNorth = true;
            addNorthEast = true;
            addEast = true;
            addWest = true;
          }
          if(northWest && !northEast && southWest && !southEast){
            addNorth = true;
            addNorthEast = true;
            addEast = true;
            addSouthEast = true;
            addSouth = true;
          }


          if(addNorthWest){
             //add northWest
            for(var k = 0; k < $scope.cornersMap[y - 1][x - 1].length; k++){
              if($scope.cornersMap[y - 1][x - 1][k].cornerType == "northWest" || $scope.cornersMap[y - 1][x - 1][k].cornerType == "normal"){
                corner.NorthWest = $scope.cornersMap[y - 1][x - 1][k];
              }
            }
          }
          if(addNorth){
            //add north
            for(var k = 0; k < $scope.cornersMap[y - 1][x].length; k++){
              if($scope.cornersMap[y - 1][x][k].cornerType == "northWest" || $scope.cornersMap[y - 1][x][k].cornerType == "northEast" ||$scope.cornersMap[y - 1][x][k].cornerType == "normal"){
                corner.North = $scope.cornersMap[y - 1][x][k];
              }
            }
          }
          if(addNorthEast){
            //add northEast
            for(var k = 0; k < $scope.cornersMap[y - 1][x + 1].length; k++){
              if($scope.cornersMap[y - 1][x + 1][k].cornerType == "northEast" || $scope.cornersMap[y - 1][x + 1][k].cornerType == "normal"){
                corner.NorthEast = $scope.cornersMap[y - 1][x + 1][k];
              }
            }
          }
          if(addEast){
            //add east
            for(var k = 0; k < $scope.cornersMap[y][x + 1].length; k++){
              if($scope.cornersMap[y][x + 1][k].cornerType == "northEast" || $scope.cornersMap[y][x + 1][k].cornerType == "southEast" || $scope.cornersMap[y][x + 1][k].cornerType == "normal"){
                corner.East = $scope.cornersMap[y][x + 1][k];
              }
            }
          }
          if(addSouthEast){
            //add southEast
            for(var k = 0; k < $scope.cornersMap[y + 1][x + 1].length; k++){
              if($scope.cornersMap[y + 1][x + 1][k].cornerType == "southEast" || $scope.cornersMap[y + 1][x + 1][k].cornerType == "normal"){
                corner.SouthEast = $scope.cornersMap[y + 1][x + 1][k];
              }
            }
          }
          if(addSouth){
            //add south
            for(var k = 0; k < $scope.cornersMap[y + 1][x].length; k++){
              if($scope.cornersMap[y + 1][x][k].cornerType == "southEast" || $scope.cornersMap[y + 1][x][k].cornerType == "southWest" || $scope.cornersMap[y + 1][x][k].cornerType == "normal"){
                corner.South = $scope.cornersMap[y + 1][x][k];
              }
            }
          }
          if(addSouthWest){
            //add southWest
            for(var k = 0; k < $scope.cornersMap[y + 1][x - 1].length; k++){
              if($scope.cornersMap[y + 1][x - 1][k].cornerType == "southWest" || $scope.cornersMap[y + 1][x - 1][k].cornerType == "normal"){
                corner.SouthWest = $scope.cornersMap[y + 1][x - 1][k];
              }
            }
          }
          if(addWest){
            //add west
            for(var k = 0; k < $scope.cornersMap[y][x - 1].length; k++){
              if($scope.cornersMap[y][x - 1][k].cornerType == "southWest" || $scope.cornersMap[y][x - 1][k].cornerType == "northWest" ||$scope.cornersMap[y][x - 1][k].cornerType == "normal"){
                corner.West = $scope.cornersMap[y][x - 1][k];
              }
            }
          }
        }
      };

      CornerObj = (function() {
            function Corner(x, y, cornerType) {
                this.x = x;
                this.y = y;
                this.cornerType = cornerType;
            }

            Corner.North = null;

            Corner.NorthWest = null;

            Corner.NorthEast = null;

            Corner.East = null;

            Corner.West = null;

            Corner.South = null;

            Corner.SouthWest = null;

            Corner.SouthEast = null;

            Corner.fVal = 999999999999999999;

            Corner.gVal = 999999999999999999;

            Corner.hVal = 999999999999999999;

            Corner.parent = null;

            Corner.prototype.getSuccessors = function() {
                var successors;
                successors = [];
                if (this.North) {
                    successors.push(this.North);
                }
                if (this.NorthWest) {
                    successors.push(this.NorthWest);
                }
                if (this.NorthEast) {
                    successors.push(this.NorthEast);
                }
                if (this.West) {
                    successors.push(this.West);
                }
                if (this.East) {
                    successors.push(this.East);
                }
                if (this.SouthWest) {
                    successors.push(this.SouthWest);
                }
                if (this.SouthEast) {
                    successors.push(this.SouthEast);
                }
                if (this.South) {
                    successors.push(this.South);
                }
                return successors;
            };

            return Corner;
        })();


      function notifyingSetting() {
         $scope.notifyingSettingStartingPoint = false;
         $scope.notifyingSettingGoalPoint = false;
         $scope.$apply();
      }
      $scope.submitMap = function(){
        var temp;
        //init map
       $scope.cornersMap = [];
        $scope.corners = [];
        for(var i = 0 ; i < $scope.size.row + 1; i ++){
            $scope.cornersMap[i] = [];
          for (var j = 0; j < $scope.size.column + 1; j++){
            $scope.cornersMap[i][j] = [];
          }
        }

        for(var i = 0 ; i < $scope.size.row; i ++){

          for (var j = 0; j < $scope.size.column ; j++){
            $scope.cell[i][j].corners = [];
          }
        }

          for(var i = 0; i < $scope.size.row + 1; i ++){
            for(var j = 0; j < $scope.size.column + 1; j++){
                temp = checkCase(i,j);
                if(temp == undefined){
                  temp = [];
                  temp[0] = 10;
                }
                //if normal corner
                if(temp[0] == 0){
                    var cornerObj = new CornerObj(j,i,"normal");
                    $scope.cornersMap[i][j].push(cornerObj);
                    $scope.corners.push(cornerObj);
                    for(var k = 0; k < temp[1].length; k++){
                      if(temp[1][k] == 1){
                        if(k == 0){
                          //northwest cell
                          $scope.cell[i - 1][j - 1].corners[$scope.cell[i - 1][j - 1].corners.length] = cornerObj;
                        }
                        if(k == 1){
                          //northeast cell
                          $scope.cell[i - 1][j].corners[$scope.cell[i - 1][j].corners.length] = cornerObj;
                        }
                        if(k == 2){
                          //southeast cell
                          $scope.cell[i][j].corners[$scope.cell[i][j].corners.length] = cornerObj;
                        }
                        if(k == 3){
                          //southwest cell
                          $scope.cell[i][j - 1].corners[$scope.cell[i][j - 1].corners.length] = cornerObj;
                        }
                      }
                    }
                }

                //if one corner
                if(temp[0] == 1){
                  for(var k = 0; k < temp[1].length; k++){
                      if(temp[1][k] == 1){
                        if(k == 0){
                          //northwest cell
                          var cornerObj = new CornerObj(j,i,"southEast");
                          $scope.cell[i - 1][j - 1].corners[$scope.cell[i - 1][j - 1].corners.length] = cornerObj;
                        }
                        if(k == 1){
                          //northeast cell
                          var cornerObj = new CornerObj(j,i,"southWest");
                          $scope.cell[i - 1][j].corners[$scope.cell[i - 1][j].corners.length] = cornerObj;
                        }
                        if(k == 2){
                          //southeast cell
                          var cornerObj = new CornerObj(j,i,"northWest");
                          $scope.cell[i][j].corners[$scope.cell[i][j].corners.length] = cornerObj;
                        }
                        if(k == 3){
                          //southwest cell
                          var cornerObj = new CornerObj(j,i,"northEast");
                          $scope.cell[i][j - 1].corners[$scope.cell[i][j - 1].corners.length] = cornerObj;
                        }
                        $scope.cornersMap[i][j].push(cornerObj);
                          $scope.corners.push(cornerObj);
                      }
                    }
                }

                // two corners special case
                 if(temp[0] == 2){
                  for(var k = 0; k < temp[1].length; k++){
                      if(temp[1][k] == 1){
                        if(k == 0){
                          //northwest cell
                          var cornerObj = new CornerObj(j,i,"southEast");
                          $scope.cell[i - 1][j - 1].corners[$scope.cell[i - 1][j - 1].corners.length] = cornerObj;
                        }
                        if(k == 1){
                          //northeast cell
                          var cornerObj = new CornerObj(j,i,"southWest");
                          $scope.cell[i - 1][j].corners[$scope.cell[i - 1][j].corners.length] = cornerObj;
                        }
                        if(k == 2){
                          //southeast cell
                          var cornerObj = new CornerObj(j,i,"northWest");
                          $scope.cell[i][j].corners[$scope.cell[i][j].corners.length] = cornerObj;
                        }
                        if(k == 3){
                          //southwest cell
                          var cornerObj = new CornerObj(j,i,"northEast");
                          $scope.cell[i][j - 1].corners[$scope.cell[i][j - 1].corners.length] = cornerObj;
                        }
                        $scope.cornersMap[i][j].push(cornerObj);
                          $scope.corners.push(cornerObj);
                      }
                    }
                }

            }
          }

          //load each corner's neighborhoods
          for(var i = 0; i < $scope.size.row + 1; i ++){
            for(var j = 0; j < $scope.size.column + 1; j++){
              for(var o = 0; o < $scope.cornersMap[i][j].length; o++){
                checkNeighborhood($scope.cornersMap[i][j][o]);
              }
            }
          }

        var holder = "";
        for(var j = 0 ; j < 20; j++){
            for(var k = 0 ; k < 30; k++){
              if($scope.cell[j][k].isObstacle){
                holder += "[" + j +  "," + k + "],\n";
              }

            }
         }
         return true;
      };
      $scope.reset = function(){
        $scope.init();
      }

      $scope.submitStartGoal = function (type, cornerType, row,column) {
          $scope.cornerType = cornerType;
          row = parseInt(row);
          column = parseInt(column);
          if(type == 'start'){
            point.startX = column;
            point.startY = row;
            point.startType = cornerType;
          }else{
            point.goalX = column;
            point.goalY = row;
            point.goalType = cornerType;

          }

          var corners;
             if(!$scope.cell[row][column].isObstacle){
               if($scope.cornerType == "northWest"){
                for(var i = 0 ; i < $scope.cornersMap[row][column].length ; i++){
                    if($scope.cornersMap[row][column][i].type = $scope.cornerType){
                      if(type == "start"){
                        point.start = $scope.cornersMap[row][column][i];
                      }else{
                        point.goal = $scope.cornersMap[row][column][i];
                      }
                      break;
                      }
                  }

                 // corners = $scope.cornersMap[row][column];
                }else if($scope.cornerType == "northEast"){
                  for(var i = 0 ; i < $scope.cornersMap[row][column + 1].length ; i++){
                    if($scope.cornersMap[row][column + 1][i].type = $scope.cornerType){
                      if(type == "start"){
                        point.start = $scope.cornersMap[row][column + 1][i];
                      }else{
                        point.goal = $scope.cornersMap[row][column + 1][i];
                      }
                      break;
                      }
                  }


                }else if($scope.cornerType == "southWest"){
                  for(var i = 0 ; i < $scope.cornersMap[row + 1][column].length ; i++){
                    if($scope.cornersMap[row + 1][column][i].type = $scope.cornerType){
                      if(type == "start"){
                        point.start = $scope.cornersMap[row + 1][column][i];
                      }else{
                        point.goal = $scope.cornersMap[row + 1][column][i];
                      }
                      break;
                      }
                  }

                 // corners = $scope.cornersMap[row + 1][column];
                }else if($scope.cornerType == "southEast"){
                  for(var i = 0 ; i < $scope.cornersMap[row + 1][column + 1].length ; i++){
                    if($scope.cornersMap[row + 1][column + 1][i].type = $scope.cornerType){
                      if(type == "start"){
                        point.start = $scope.cornersMap[row + 1][column + 1][i];
                      }else{
                        point.goal = $scope.cornersMap[row + 1][column + 1][i];
                      }
                      break;
                      }
                  }

               //   corners = $scope.cornersMap[row + 1][column + 1];
                }


        }

      };

      $scope.search = function(){
         // $scope.submitMap();
          myGrid = $scope.cell;

          if($scope.searchType == "AStar" || $scope.searchType == "AStarWithSmoothing"){
            searchie = new AStarSearch("OctileDistance");
          }else if($scope.searchType == "ThetaStar"){
            searchie = new ThetaStarSearch("EuclideanDistance");
          }


          path = searchie.search(point.start, point.goal);
          $scope.tempPath = [];

          for (i = 0, len = path.length; i < len; i++) {
            corner = path[i];
            $scope.tempPath[i] = [];
            $scope.tempPath[i][0] = corner.x;
            $scope.tempPath[i][1] = corner.y;
          }

          //start and goal

         lines[0].strokeStyle = "#C41F1F";
         lines[0].beginPath();
         var x = point.goalX;
         var y = point.goalY;
         if($scope.cornerTypeGoal == "northEast"){
            x += 1;
         }else if($scope.cornerTypeGoal == "southEast"){
            x += 1;
            y += 1;
         }else if($scope.cornerTypeGoal == "southWest"){
            y += 1;
         }
         lines[0].arc(x * unit, y * unit,5,0,2*Math.PI);
         lines[0].stroke();
         x = point.startX;
          y = point.startY;
         if($scope.cornerTypeStart == "northEast"){
            x += 1;
         }else if($scope.cornerTypeStart == "southEast"){
            x += 1;
            y += 1;
         }else if($scope.cornerTypeStart == "southWest"){
            y += 1;
         }

         lines[0].beginPath();
         lines[0].strokeStyle = "#1F78C4";
         lines[0].arc(x * unit, y * unit,5,0,2*Math.PI);
         lines[0].stroke();


          setTimeout(drawExpansion,100);
      };



      function drawExpansion(){

        lines[1].moveTo($scope.tempPath2[expanding][0][0] * unit ,$scope.tempPath2[expanding][0][1] * unit);
        lines[1].lineTo($scope.tempPath2[expanding][1][0] * unit,$scope.tempPath2[expanding][1][1] * unit);
        $scope.expansion++;
        $scope.$apply();
        if(expanding == $scope.tempPath2.length - 1){
          lines[1].stroke();
          drawSolution();
        }else{
          lines[1].stroke();
          expanding++;
          lines[2].clearRect(0,0,canvases[2].width,canvases[2].height);

          for(var k = 0; k < $scope.openList[expanding].length; k++){
             lines[2].beginPath();
             lines[2].strokeStyle = "#80AEFF";
             lines[2].arc($scope.openList[expanding][k][0] * unit,$scope.openList[expanding][k][1] * unit,5,0,2*Math.PI);

             lines[2].stroke();
             lines[2].beginPath();
             lines[2].strokeStyle = "#FFC9C9";
             lines[2].moveTo($scope.openList[expanding][k][2][0] * unit ,$scope.openList[expanding][k][2][1] * unit);
             lines[2].lineTo($scope.openList[expanding][k][0] * unit,$scope.openList[expanding][k][1] * unit);
             lines[2].stroke();

          }
          if($scope.simulationMode == "immediate"){
            drawExpansion();
          }else{
            setTimeout(drawExpansion, 500 - $scope.simulationSpeed);
          }

         //
        }
      };

      function drawSolution(){
         if($scope.totalLength == 0){
          lines[0].strokeStyle = "#04FAA8";
            for(var i = 0 ; i <  $scope.tempPath.length - 1 ; i++){

              lines[0].beginPath();
              $scope.totalLength += Math.sqrt((($scope.tempPath[i][0] - $scope.tempPath[i + 1][0]) * ($scope.tempPath[i][0] - $scope.tempPath[i + 1][0])) + (($scope.tempPath[i][1] - $scope.tempPath[i + 1][1]) * ($scope.tempPath[i][1] - $scope.tempPath[i + 1][1])));
              lines[0].moveTo($scope.tempPath[i][0] * unit ,$scope.tempPath[i][1] * unit);
              lines[0].lineTo($scope.tempPath[i + 1][0] * unit,$scope.tempPath[i + 1][1] * unit);
              lines[0].stroke();
            }
            $scope.$apply();

         }



      };


      $scope.mapSelected = function(map){
        var res = $scope.init();
        var index = 0;
        if(map == "mapComplex"){
            for(var j = 0 ; j < 20; j++){
              for(var k = 0 ; k < 30; k++){
                $scope.cell[j][k].isObstacle = false;
                if(mapComplex[index] != undefined){
                  if(mapComplex[index][0] == j && mapComplex[index][1] == k){
                     $scope.cell[j][k].isObstacle = true;
                     index++;
                  }
                }

            }
          }
        }else if(map == "mapSal"){
          for(var j = 0 ; j < 20; j++){
              for(var k = 0 ; k < 30; k++){
                $scope.cell[j][k].isObstacle = false;
                if(mapSal[index] != undefined){
                if(mapSal[index][0] == j && mapSal[index][1] == k){
                 $scope.cell[j][k].isObstacle = true;
                 index++;
               }
              }
            }
          }
        }else if(map == "mapEqaud"){
          for(var j = 0 ; j < 20; j++){
              for(var k = 0 ; k < 30; k++){
                $scope.cell[j][k].isObstacle = false;
                if(mapEqaud[index] != undefined){
                if(mapEqaud[index][0] == j && mapEqaud[index][1] == k){
                 $scope.cell[j][k].isObstacle = true;
                 index++;
               }
              }
            }
          }
        }else if(map == "mapManhattan"){
          for(var j = 0 ; j < 20; j++){
              for(var k = 0 ; k < 30; k++){
                $scope.cell[j][k].isObstacle = false;
                if(mapManhattan[index] != undefined){
                if(mapManhattan[index][0] == j && mapManhattan[index][1] == k){
                 $scope.cell[j][k].isObstacle = true;
                 index++;
               }
              }
            }
          }
        }else if(map == "mapSkyline"){
          for(var j = 0 ; j < 20; j++){
              for(var k = 0 ; k < 30; k++){
                $scope.cell[j][k].isObstacle = false;
                if(mapSkyline[index] != undefined){
                if(mapSkyline[index][0] == j && mapSkyline[index][1] == k){
                 $scope.cell[j][k].isObstacle = true;
                 index++;
               }
              }
            }
          }
        }

        var res2 = $scope.submitMap();
      };

      $scope.doublesetChange =function(){
        $scope.doubleset = !$scope.doubleset;
      };
       $scope.doubleset = false;

      $scope.init();
      $scope.mapSelected('mapManhattan');
      $scope.submitStartGoal('start', $scope.cornerTypeStart, 1, 0);
      $scope.submitStartGoal('goal', $scope.cornerTypeStart, 18, 28);
      $scope.search();

});



 app.directive('ngRightClick', function($parse) {
    return function(scope, element, attrs) {
        var fn = $parse(attrs.ngRightClick);
        element.bind('contextmenu', function(event) {


                if(!point.start){
                  scope.submitStartGoal('start', scope.cornerTypeStart, attrs.row, parseInt(attrs.column));
                }else if(point.start && !point.goal){
                  scope.submitStartGoal('goal', scope.cornerTypeGoal,attrs.row, parseInt(attrs.column));
                  scope.search();
                }else if(point.start && point.goal && !scope.doubleset){
                  scope.submitStartGoal('start', scope.cornerTypeStart,attrs.row, parseInt(attrs.column));
                  scope.doublesetChange();
                  scope.submitMap();
                  scope.clearCanvas();
                }else if(point.start && point.goal && scope.doubleset){
                  scope.submitStartGoal('goal', scope.cornerTypeGoal, attrs.row, parseInt(attrs.column));
                  scope.doublesetChange();
                  scope.submitMap();
                  scope.clearCanvas();
                  scope.search();
                }



        });
    };

});
