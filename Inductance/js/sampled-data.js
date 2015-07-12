/***
 * Bach.SampledData is a data structure consisting of an independent variable and multiple dependent variables.
 * The data is stored in arrays, one for the independent and one for each of the dependent variables. Access is
 * via index into the array or by interpolating for an independent variable.
 *
 * Copyright (C) 2015 Lawrence Gunn - All Rights Reserved
 *
 * Revisions: Original definition by Lawrence Gunn.
 * 2015/07/11
 *
 * @file sampled-data.js
 * @class Bach.SampledData
 */
'use strict';

var Bach = Bach || {};

/**
 * A numerical algorithm class storing arrays of a single independent variable and multiple dependent variables.
 *
 * @class Bach.SampledData
 * @constructor
 * @param {integer} numY - number of y values
 */
Bach.SampledData = function(numY) {
  this.className  = 'Bach.SampledData';
  this.numY = numY;
  this.x = [];
  this.y = [];
  for(var i=0; i<this.numY; i++) {
    this.y.push([]);
  }

  this.search = new Bach.TableSearch();
  this.interp = new Bach.PolynomialInterpolation();
};

/***
 * Append an independent variable and the corresponding dependent variables.
 * @method append
 * @param {float} x - independent variable
 * @param {Array} y - array of dependent variables
 */
Bach.SampledData.prototype.append = function(x, y) {
  this.x.push(x);
  for(var i=0; i<this.numY; i++) {
    this.y[i].push(y[i]);
  }
};

/***
 * Retrieve by index an independent variable and the corresponding dependent variables.
 * @method retrieveByIndex
 * @param {number} index - index of the samples
 * @return {Object} - object with x as the independent variable and y as the array of dependent variables
 */
Bach.SampledData.prototype.retrieveByIndex = function(index) {
  var y = [];
  for(var i=0; i<this.numY; i++) {
    y.push(this.y[i][index]);
  }
  return {
    x: this.x[index],
    y: y
  };
};

/***
 * Retrieve by index an independent variable and the corresponding dependent variables.
 * @method retrieve
 * @param {float} targetX - index of the samples
 * @return {Array} - an array of interpolated dependent variables
 */
Bach.SampledData.prototype.retrieve = function(targetX) {
  var index = this.search.find(targetX, this.x);
  if(index > this.x.length-3) {
    index = this.x.length-3;
  }
  return this.interp.interpolateArrayOfVectors(targetX, this.x, this.y, index);
};
