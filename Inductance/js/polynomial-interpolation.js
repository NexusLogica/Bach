/***
 * Bach.PolynomialInterpolation is a one dimensional interpolator using a polynomial function.
 *
 * Copyright (C) 2015 Lawrence Gunn - All Rights Reserved
 *
 * Revisions: Original definition by Lawrence Gunn.
 * 1990/05/08
 * The interpolation index classes provide means of choosing an interpolation
 * index based upon an index in a table.
 *
 * Revised for RW library and better readability.
 * 1992/07/29.
 *
 * Table search functions and interpolation index class separated into
 * two different classes.
 * 1992/09/08.
 *
 * Updated by Lawrence Gunn.
 * 2013/01/29
 *
 * Ported to JavaScript.
 * 2015/07/10
 *
 * @file table-search.js
 * @class Bach.PolynomialInterpolation
 */
'use strict';

var Bach = Bach || {};

/**
 * A numerical algorithm class for finding the index of numbers bracketing a target number. The array must be either
 * monotonically increasing or decreasing array of numbers.
 *
 * @class Bach.PolynomialInterpolation
 * @constructor
 */
Bach.PolynomialInterpolation = function(interpSize) {
  this.className  = 'Bach.PolynomialInterpolation';
  this.differenceVector = [];
  this.errorEstimate = 0.0;
  this.errorEstimateVector = [];
  this.interpSize = _.isUndefined(interpSize) ? 3 : interpSize;
  this.checkExtrapFlag = false;
  this.aTinyValue = 1.0e-25;
};

Bach.PolynomialInterpolation.prototype.interpolateArrayOfVectors = function(xTarget, x, yArray, baseIndex) {
  var numY = yArray.length;
  this.errorEstimateVector = [];

  var results = [];
  this.errorEstimateVector = [];

  var xSlice = x.slice(baseIndex, baseIndex+this.interpSize);
  for(var i=0;i<numY;i++) {
    results.push(this.interpolate(xTarget, xSlice, yArray[i].slice(baseIndex, baseIndex+this.interpSize), 0));
    this.errorEstimateVector.push(this.errorEstimate);
  }
};

Bach.PolynomialInterpolation.prototype.checkForExtrapolation = function(xTarget, x) {
  var ascending = (x[0] < x[this.interpSize-1]);

  if((ascending && (x[this.interpSize-1] < xTarget && xTarget < x[0])) ||
    (!ascending && (x[0] < xTarget && xTarget <= x[this.interpSize-1]))) {
    console.log('WARNING: Interpolator::CheckForExtrapolation: Attempted extrapolation to '+xTarget+' (range '+x[0]+' to '+x(this.interpSize-1)+')');
  }
};

Bach.PolynomialInterpolation.prototype.interpolate = function(xTarget, x, y, index) {
  if(index !== 0) {
    return this.performPolynomialInterpolation(xTarget, x.block(index, 0, this.interpSize, 1), y.block(index, 0, this.interpSize, 1));
  } else {
    return this.performPolynomialInterpolation(xTarget, x, y);
  }
};

Bach.PolynomialInterpolation.prototype.performPolynomialInterpolation = function(xTarget, x, y) {
  if(this.checkExtrapFlag) {
    this.checkForExtrapolation(xTarget,x);
  }

  var c = [], d = [];
  for(var i=0; i<this.interpSize; i++) {
    c[i] = y[i];
    d[i] = y[i] + this.aTinyValue;
  }

  var differenceVector = [];
  var minDiff = Math.abs(x[0]-xTarget);
  var minDiffIndex = 0;
  for(i=1; i<this.interpSize; i++) {
    var diff = Math.abs(x[i]-xTarget);
    if(diff < minDiff) {
      minDiff = diff;
      minDiffIndex = i;
    }
  }

  if(minDiff === 0.0)  {
    this.errorEstimate = 0.0;
    return y[minDiffIndex];
  }

  var yTarget = y[minDiffIndex];
  minDiffIndex--;

  // For each column in the interpolation table...
  for(var m=1; m<this.interpSize; m++) {

    // This loop could be vectorized but with the small values
    // of i likely encountered, the vector method may involve
    // substancially more overhead.
    for(i=0; i<this.interpSize-m; i++) {
      var idif = x[i]-xTarget;
      var imdif = x[i+m]-xTarget;
      var width = this.c[i+1]-this.d[i];
      var denominator = idif-imdif;

      if(denominator === 0.0) {
        // If the polynomial interpolation fails then it is reasable that a rational
        // interpolation will succeed.
        // TODO: Add a fallback.
        console.assert(denominator !== 0.0);
        return Number.NaN;

        //RationalInterp tryRationalInterp(this.interpSize);
        //return tryRationalInterp.Interpolate(xTarget, x, y);

        // NXError(NX_ERROR,"NXPolynomialInterp::polynomialInterp",
        //  "Two x vector elements are equal");
      }

      denominator = width/denominator;
      c[i] = idif*denominator;
      d[i] = imdif*denominator;
    }

    if(2*(minDiffIndex+1) < (this.interpSize-m))  {
      this.errorEstimate = c[minDiffIndex+1];
    }
    else  {
      this.errorEstimate = d[minDiffIndex];
      minDiffIndex--;
    }

    yTarget += this.errorEstimate;
  }

  return yTarget;
};
