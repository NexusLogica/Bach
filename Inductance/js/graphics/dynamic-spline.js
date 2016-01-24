/***
 * Bach.DynamicSpline: A spline curve object for use with changing geometries and numbers of segments.
 *
 * Copyright (C) 2015 Lawrence Gunn - All Rights Reserved
 *
 * @file dynamic-spline.js
 * @class Bach.DynamicSpline
 */
'use strict';

Bach = Bach || {};

/***
 * Create the spline object and add it to the scene.
 * @param configuration
 * @param configuration.initialNumPoints {Number} - Number of points in initial buffer. If exceeded the buffer will be resized.
 * @param configuration.points {Array} - Optional, an array of points to plot
 * @constructor
 */
Bach.DynamicSpline = function(configuration) {
  this.config = _.clone(configuration);
  this.initialNumPoints = this.config.initialNumPoints || 100;
  this.points = this.config.points || [];
  this.color = this.config.hasOwnProperty('color') ? this.config.color: 0xff0000;
  this.lineWidth = this.config.hasOwnProperty('lineWidth') ? this.config.lineWidth: 2;

  this.create3dObjects();
  if(this.points) {
    this.updatePoints(this.points);
  }

  if(this.config.dock) {
    this.config.dock.add(this.line);
  }
};

Bach.DynamicSpline.prototype.create3dObjects = function() {
  //this.geometry = new THREE.Geometry();
  this.geometry = new THREE.BufferGeometry();

  // There are 3 vertices per point.
  var positions = new Float32Array(this.initialNumPoints*3);
  this.geometry.addAttribute('position', new THREE.BufferAttribute(positions, 3));

  var drawCount = this.points.length;
  this.geometry.setDrawRange(0, drawCount);

  // material
  this.material = new THREE.LineBasicMaterial({ color: this.color, linewidth: this.lineWidth });

  // line
  this.line = new THREE.Line(this.geometry,  this.material);
  this.config.dock.add(this.line);
};

Bach.DynamicSpline.prototype.updatePoints = function(points) {
  this.points = points;
  var n = this.points.length;

  var ptArray = [];
  for(var i=0; i<n; i++) {
    var pt = this.points[i];
    ptArray.push(new THREE.Vector3(pt[0], pt[1], pt[2]));
  }

  var curve = new THREE.SplineCurve3(ptArray);

  this.displayPoints = 50;
  var curvePts = curve.getPoints(this.displayPoints);

  var ps = this.geometry.attributes.position.array;
  var j = 0;
  for(i=0; i<curvePts.length; i++) {
    var cpt = curvePts[i];
    ps[j++] = cpt.x;
    ps[j++] = cpt.y;
    ps[j++] = cpt.z;
  }

  this.geometry.attributes.position.needsUpdate = true;
  this.geometry.setDrawRange(0, this.displayPoints);
};
