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
 * @param {Number} configuration.initialNumPoints - Number of points in initial buffer. If exceeded the buffer will be resized.
 * @param {Array} configuration.points - An array THREE.Vector3 points to plot. This can initially be zero length.
 * @param configuration.parent3d
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

  if(this.config.parent3d) {
    this.config.parent3d.add(this.curve);
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
  this.curve = new THREE.Line(this.geometry,  this.material);
  this.config.parent3d.add(this.curve);
};

/***
 * Update the rendered curve with new points.
 * @param points - An array of THREE.Vector3 points to be interpolated and rendered.
 */
Bach.DynamicSpline.prototype.updatePoints = function(points) {
  var curve = new THREE.CatmullRomCurve3(points);

  this.displayPoints = 50;
  var curvePts = curve.getPoints(this.displayPoints);

  var ps = this.geometry.attributes.position.array;
  var j = 0;
  for(var i=0; i<curvePts.length; i++) {
    var cpt = curvePts[i];
    ps[j++] = cpt.x;
    ps[j++] = cpt.y;
    ps[j++] = cpt.z;
  }

  this.geometry.attributes.position.needsUpdate = true;
  this.geometry.setDrawRange(0, this.displayPoints);
};
