/***
 * Bach.RelativisticPoint is a moving point object that is observable from another reference plane.
 *
 * Copyright (C) 2015 Lawrence Gunn - All Rights Reserved
 *
 * @file relativistic-point.js
 * @class Bach.RelativisticPoint
 */
'use strict';

var Bach = Bach || {};

/***
 * Create and initialize a point
 * @constructor
 * @param {object} config
 * @param {Bach.Vector} config.initial
 * @param {Bach.Vector} config.direction
 * @param {float} config.speed - Speed as a fraction of light. 0.0 is not moving, 1.0 is speed of light.
 * @constructor
 */
Bach.RelativisticPoint = function(config) {
  this.initial = config.initial || new Bach.Vector();
  this.direction = config.direction || new Bach.Vector(1.0);
  this.speed = _.isUndefined(config.speed) ? 0.0 : config.speed;
  this.timeSteps = [];
  this.c = 1.0; // The speed of light taken as light-seconds per second (hence, 1.0).
};

/***
 * Calculate a list of times where the time and position for self are equated to a time when light for that
 * position hits the observer.
 * @method calculateObserverTime
 * @param {float} tStart - the start time relative to self
 * @param {float} tEnd - the end time relative to self
 * @param {Bach.Vector} observer - position of the observer
 * @param {float} stepSize - size of increments from tStart to tEnd
 */
Bach.RelativisticPoint.prototype.calculateObserverTime = function(tStart, tEnd, observer, stepSize) {
  this.tStart = tStart;
  this.tEnd = tEnd;

  var t = this.tStart;
  while(t <= this.tEnd) {
    this.findTime(t, observer);
    t += stepSize;
    if(t > this.tEnd) {
      t = this.tEnd;
    }
  }
};

/***
 * Find the times and positions related to the point's time.
 * @method findTime
 * @param t
 * @param observer
 */
Bach.RelativisticPoint.prototype.findTime = function(t, observer) {
  // Where is it
  var distanceSinceStart = (t-this.tStart)*this.speed;
  var pos = this.initial.add(this.direction.scalarMultiply(distanceSinceStart));
  var distFromObsToPos = observer.distance(pos); // in light seconds
  var timeFromObsToPos = distFromObsToPos/this.c;
  var tTotal = t+oToPos;

  // Where is it when it actually when it is observed?
  var distanceSinceStartWhenObserved = (tTotal-this.tStart)*this.speed;
  var posWhenObserved = this.initial.add(this.direction.scalarMultiply(distanceSinceStartWhenObserved));

  this.timeSteps.push({
    "t": t,
    "observedTime": tTotal,
    "observedPos": pos,
    "actualPosWhenObserved": posWhenObserved });
};
