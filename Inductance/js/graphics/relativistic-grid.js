/***
 * Bach.Inductance is a namespace for the Inductance app.
 *
 * Copyright (C) 2015 Lawrence Gunn - All Rights Reserved
 *
 * @file relativistic-grid.js
 * @class Bach.Inductance
 */
'use strict';

Bach.Inductance3D.RelativisticGrid = function() {
};

/***
 *
 * @method create
 * @param config
 * @param {[Bach.RelativisticPoint]} config.points - An array of Bach.RelativisticPoint's.
 * @param {Number} config.follow - size and position should be based on Bach.Inductance3D's FollowUniversal, FollowRelativistic, or SizeRealPositionRelative.
 * @param {Array} config.connectionSets - vertex numbers to connect. This is of the form { color: _color, lineVertexSets: [ [0, 1,], [1, 2], ...] }.
 */
Bach.Inductance3D.RelativisticGrid.prototype.create = function(config) {
  this.config = _.clone(config);
  this.points = this.config.points;
  this.follow = this.config.follow;
  this.connectionSets = this.config.connectionSets;
  this.pointPositions = [];

  this.getMinAndMaxTimes();

  if(this.config.hasOwnProperty('startTime')) {
    this.startTime = this.config.startTime;
  } else if(this.follow === Bach.Inductance3D.FollowUniversal) {
    this.startTime = this.startTimeUniversal;
    this.endTime = this.endTimeUniversal;
  } else { // FollowRelativistic or SizeRealPositionRelative
    this.startTime = this.startTimeObserved;
    this.endTime = this.endTimeObserved;
  }

  var numVertices = 0;
  var i, j, k;
  for(i=0; i<this.connectionSets.length; i++) {
    numVertices += this.connectionSets[i].indices.length;
  }

  this.vertices = [];
  for(j=0; j<numVertices; j++) {
    this.vertices[j] = new THREE.Vector3(0.0, 0.0, 0.0);
  }

  for(k=0; k<this.points.length; k++) {
    this.pointPositions[k] = new THREE.Vector3(0.0, 0.0, 0.0);
  }

  this.updateVertices(this.startTime);

  this.geometry = new THREE.Geometry();
  this.geometry.vertices = this.vertices;

  var colorIndex = 0;
  for(i=0; i<this.connectionSets.length; i++) {
    var connectionSet = this.connectionSets[i];
    connectionSet.threeJsColor = new THREE.Color(connectionSet.color);
    for(j=0; j<connectionSet.indices.length; j++) {
      this.geometry.colors[colorIndex] = connectionSet.threeJsColor;
      colorIndex++;
      this.geometry.colors[colorIndex] = connectionSet.threeJsColor;
      colorIndex++;
    }
  }

  var material = new THREE.LineBasicMaterial({
    linewidth: 2,
    color: 0xffffff,
    vertexColors: THREE.VertexColors
  });

  this.threeJsLine = new THREE.Line(this.geometry, material, THREE.LinePieces);
  return this.threeJsLine;
};

/***
 * @method buildGridGraphics
 */
Bach.Inductance3D.RelativisticGrid.prototype.buildGridGraphics = function() {
  this.geometry = new THREE.BufferGeometry();
  this.material = new THREE.LineBasicMaterial({ vertexColors: THREE.FaceColors });

  this.lines = THREE.Line(this.geometry, this.material, THREE.LinePieces);
  this.vertices = new Float32Array( this.points.length * 3 );
  this.updatePoints(0.0);
//  this.colors = new Float32Array( segments * 3 );
};

/***
 * @method updateOnRender
 */
Bach.Inductance3D.RelativisticGrid.prototype.updateOnRender = function() {

};

/***
 * @method updateVertices
 */
Bach.Inductance3D.RelativisticGrid.prototype.updateVertices = function(t) {
  if(this.geometry) {
    this.geometry.verticesNeedUpdate = true;
    this.geometry.elementsNeedUpdate = true;
    this.geometry.morphTargetsNeedUpdate = true;
    this.geometry.uvsNeedUpdate = true;
    this.geometry.normalsNeedUpdate = true;
    this.geometry.colorsNeedUpdate = true;
    this.geometry.tangentsNeedUpdate = true;
  }

  var num = this.points.length;
  var i, j, results;
  if(this.follow === Bach.Inductance3D.FollowUniversal) {
    for (i = 0; i < num; i++) {
      results = this.points[i].retrieveByUniversalTime(t);

      // A bit of a hack - the three.js copy function just copies x, y, and z, which Bach.Vector also has.
      this.pointPositions[i].copy(results.universalPosition);
    }
  } else {
    for (i = 0; i < num; i++) {
      results = this.points[i].retrieveByObservedTime(t);

      // A bit of a hack - the three.js copy function just copies x, y, and z, which Bach.Vector also has.
      this.pointPositions[i].copy(results.observedPosition);
    }
  }

  // Now the tricky part. Match the pointPositions to the vertices.
  var vertexIndex = 0;
  for(i=0; i<this.connectionSets.length; i++) {
    var connectionSet = this.connectionSets[i];
    for(j=0; j<connectionSet.indices.length; j++) {
      var indexSet = connectionSet.indices[j];
      this.vertices[vertexIndex] = this.pointPositions[indexSet[0]];
      vertexIndex++;
      this.vertices[vertexIndex] = this.pointPositions[indexSet[1]];
      vertexIndex++;
    }
  }
};

Bach.Inductance3D.RelativisticGrid.prototype.getMinAndMaxTimes = function() {
  var point = this.points[0];
  var times = point.getObservedTimeRange();
  this.startTimeObserved = times.start;
  this.endTimeObserved = times.end;
  this.startTimeUniversal = point.sampledDataByUniversalTime.x[0];
  this.endTimeUniversal = point.sampledDataByUniversalTime.x[point.sampledDataByUniversalTime.x.length-1];
  for(var i=1; i<this.points.length; i++) {
    point = this.points[i];

    times = point.getObservedTimeRange();

    if(times.start > this.startTimeObserved) {
      this.startTimeObserved = times.start;
    }

    if(times.end < this.endTimeObserved) {
      this.endTimeObserved = times.end;
    }

    var startUniversal = point.sampledDataByUniversalTime.x[0];
    var endUniversal = point.sampledDataByUniversalTime.x[point.sampledDataByUniversalTime.x.length-1];

    if(startUniversal > this.startTimeUniversal) {
      this.startTimeUniversal = this.startTimeUniversal;
    }
    if(endUniversal < this.endTimeUniversal) {
      this.endTimeUniversal = endUniversal;
    }
  }
};
