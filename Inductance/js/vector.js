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
  if(x && x.hasOwnProperty('x')) {
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

/***
 * Multiply each element by a scalar and return a new vector with the result.
 * @method scalarMultiply
 * @param {float} s - Scalar to multiply by.
 * @returns {Bach.Vector}
 */
Bach.Vector.prototype.scalarMultiply = function(s) {
  return new Bach.Vector(this.x*s, this.y*s, this.z*s);
};

/***
 * Add a scalar value to each element and return a new vector with the result.
 * @method scalarAdd
 * @param {float} s - Scalar to add.
 * @returns {Bach.Vector}
 */
Bach.Vector.prototype.scalarAdd = function(s) {
  return new Bach.Vector(this.x+s, this.y+s, this.z+s);
};

/***
 * Add this vector to another and return a new vector of the sum.
 * @method add
 * @param {Bach.Vector} v - A vector
 * @returns {Bach.Vector} - Return a new vector that sums self and v.
 */
Bach.Vector.prototype.add = function(v) {
  return new Bach.Vector(this.x+v.x, this.y+v.y, this.z+v.z);
};

Bach.Vector.prototype.normalize = function() {
  var lenInv = 1.0/Math.sqrt(this.x*this.x+this.y*this.y+this.z*this.z);
  this.x *= lenInv;
  this.y *= lenInv;
  this.z *= lenInv;
  return this;
};

/***
 * Returns the dot product value of self and vec.
 * @method dotProduct
 * @param vec
 * @returns {number}
 */
Bach.Vector.prototype.dotProduct = function(vec) {
  return this.x*vec.x+this.y*vec.y+this.z*this.z;
};

/***
 * Returns the distance between self and vec.
 * @method distance
 * @param {Bach.Vector} vec - The vector to measure the distance from.
 * @returns {number}
 */
Bach.Vector.prototype.distance = function(vec) {
  var x = this.x-vec.x;
  var y = this.y-vec.y;
  var z = this.z-vec.z;
  return Math.sqrt(x*x+y*y+z*z);
};

/***
 * Apply the function to the vector values.
 * @method applyToNew
 * @param func
 * @returns {Bach.Vector}
 */
Bach.Vector.prototype.applyToNew = function(func) {
  return new Bach.Vector(func(this.x), func(this.y), func(this.z));
};

/***
 * Create a copy of self.
 * @method clone
 * @returns {Bach.Vector}
 */
Bach.Vector.prototype.clone = function() {
  return new Bach.Vector(this.x, this.y, this.z);
};
