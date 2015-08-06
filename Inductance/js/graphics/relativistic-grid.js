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
  this.points = config.points;
  this.follow = config.follow;
  this.connectionSets = config.connectionSets;
  this.pointPositions = [];

  this.getMinAndMaxTimes();

  if(config.hasOwnProperty('initialTime')) {
    this.initialTime = config.initialTime;
  } else if(this.follow === Bach.Inductance3D.FollowUniversal) {
    this.initialTime = 0.0;
  } else { // FollowRelativistic or SizeRealPositionRelative
    this.initialTime = 0.0;
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

  this.updateVertices(this.initialTime);

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
  for(var i=1; i<this.points.length; i++) {

  }
};
