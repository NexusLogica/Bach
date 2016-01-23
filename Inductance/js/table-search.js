/***
 * Bach.TableSearch is a hunting method for finding indices to use in interpolations.
 *
 * Copyright (C) 2015 Lawrence Gunn - All Rights Reserved
 *
 * @file table-search.js
 * @class Bach.TableSearch
 */
'use strict';

var Bach = Bach || {};

/**
 * A numerical algorithm class for finding the index of numbers bracketing a target number. The array must be either
 * monotonically increasing or decreasing array of numbers.
 *
 * @class Bach.TableSearch
 * @constructor
 */
Bach.TableSearch = function() {
  this.className  = 'Bach.TableSearch';
  this.indexLow = 0;
  this.indexHigh = 0;
  this.ascending = true;
  this.size = 0;
};

/**
 * Returns the last low index found.
 * @returns {float}
 * @constructor
 */
Bach.TableSearch.prototype.getIndexLow = function() {
  return this.indexLow;
};

/**
 * Find the index in the array 'x' that is where the index is below xTarget and index+1 is above xTarget. If the target
 * is before the beginning of the array then it returns -1. If it is beyond the end of the array it returns x.length-1.
 *
 * @method find
 * @param {float} xTarget
 * @param {Array} x
 * @returns {number}
 * @constructor
 */
Bach.TableSearch.prototype.find = function(xTarget, x) {
  this.size = x.length;
  this.ascending = (x[this.size-1] > x[0]);

  // Due to a poor initial guess try bisection.
  if(this.indexLow <= -1 || this.indexLow > this.size-1)  {
    this.indexLow  = -1;
    this.indexHigh = this.size;
  }
  else {
    if ((xTarget >= x[this.indexLow]) === this.ascending) {
      if(this.indexLow === this.size-1) {
        return this.indexLow;
      }
      this.huntUp(xTarget,x);
    }
    else {
      if(this.indexLow === 0)  {
        this.indexLow = -1;
        return this.indexLow;
      }
      this.huntDown(xTarget, x);
    }
  }

  this.bisection(xTarget,x);
  return this.indexLow;
};

Bach.TableSearch.prototype.huntUp = function(xTarget, x) {
  var increment = 1;
  this.indexHigh = this.indexLow+1;

  while((xTarget >= x[this.indexHigh]) === this.ascending) {

    // Double the hunting increment.
    this.indexLow = this.indexHigh;
    increment += increment;
    this.indexHigh = this.indexLow+increment;

    // If we are off the high end of the vector, leave.
    if(this.indexHigh > this.size-1) {
      this.indexHigh = this.size;
      break;
    }
  }
};

Bach.TableSearch.prototype.huntDown = function(xTarget, x) {
  var increment = 1;
  this.indexHigh = this.indexLow;
  this.indexLow -= 1;

  while(xTarget < x[this.indexLow] === this.ascending) {
    // Double the hunting increment.
    this.indexHigh = this.indexLow;
    increment += increment;
    this.indexLow = this.indexHigh-increment;

    // If we are off the low end of the vector, leave.
    if(this.indexLow < 0) {
      this.indexLow = -1;
      break;
    }
  }
};

Bach.TableSearch.prototype.bisection = function(xTarget, x) { // xTarget is float, x is array of floats
  while(this.indexHigh-this.indexLow !== 1) {
    var indexMiddle = (this.indexHigh+this.indexLow) >> 1;

    if((xTarget > x[indexMiddle]) === this.ascending) {
      this.indexLow = indexMiddle;
    }
    else {
      this.indexHigh = indexMiddle;
    }
  }
};
