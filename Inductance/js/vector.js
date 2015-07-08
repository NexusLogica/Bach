/***
 * Bach.Vector is a three dimensional vector.
 *
 * Copyright (C) 2015 Lawrence Gunn - All Rights Reserved
 *
 * @file vector.js
 * @class Bach.Vector
 */
'use strict';

var Bach = Bach || {};

Bach.Vector = function(x, y, z) {
  if(x && x.hasOwnProperty(x)) {
    this.x = x.x;
    this.y = x.y;
    this.z = x.z;
  } else {
    this.x = x === undefined ? 0.0 : x;
    this.y = y === undefined ? 0.0 : y;
    this.z = z === undefined ? 0.0 : z;
  }
};

Bach.Vector.prototype.shorten = function(basePoint, newLen) {
  var vec = new Bach.Vector(this, basePoint);
  var len = vec.vectorLength();
  var ratio = (len-newLen)/len;
  vec.x *= ratio;
  vec.y *= ratio;
  vec.z *= ratio;
  return new Bach.Vector(basePoint.x-vec.x, basePoint.y-vec.y, basePoint.z-vec.z);
};

Bach.Vector.prototype.vectorLength = function() {
  return Math.sqrt(this.x*this.x+this.y*this.y+this.z*this.z);
};

Bach.Vector.prototype.normalize = function() {
  var lenInv = 1.0/Math.sqrt(this.x*this.x+this.y*this.y+this.z*this.z);
  this.x *= lenInv;
  this.y *= lenInv;
  this.z *= lenInv;
  return this;
};

Bach.Vector.prototype.dotProduct = function(vec) {
  return this.x*vec.x+this.y*vec.y+this.z*this.z;
};

Bach.Vector.prototype.distance = function(vec) {
  var x = this.x-vec.x;
  var y = this.y-vec.y;
  var z = this.z-vec.z;
  return Math.sqrt(x*x+y*y+z*z);
};

Bach.Vector.prototype.clone = function() {
  return new Bach.Vector(this.x, this.y, this.z);
};

Bach.Vector.prototype.offset = function(x, y, z) {
  this.x += x;
  this.y += y;
  this.z += z;
  return new Bach.Vector(this.x, this.y, this.z);
};
