/***
 * Bach.RelativisticCube is a moving point object that is observable from another reference plane.
 *
 * Copyright (C) 2015 Lawrence Gunn - All Rights Reserved
 *
 * @file relativistic-point.js
 * @class Bach.RelativisticCube
 */
'use strict';

/***
 * Create and initialize a point
 * @constructor
 * @param {object} config
 * @param {Bach.Vector} config.initial
 * @param {Bach.Vector} config.direction
 * @param {float} config.speed - Speed as a fraction of light. 0.0 is not moving, 1.0 is speed of light.
 * @param {Bach.Vector} config.size - the size, x, y, z, of the box if standing still relative to the observer
 * @constructor
 */
Bach.RelativisticCube = function(config) {
  this.points = [];
  this.initial = config.initial || new Bach.Vector();
  this.direction = config.direction || new Bach.Vector(1.0);
  this.speed = _.isUndefined(config.speed) ? 0.0 : config.speed;
  this.size = config.size;

  var pos = new Bach.Vector();
  for(var i=0; i<2; i++) {           // x
    pos.x = (i-0.5)*this.size.x;
    for(var j=0; j<2; j++) {         // y
      pos.y = (j-0.5)*this.size.y;
      for (var k = 0; k < 2; k++) {  // z
        pos.z = (k-0.5)*this.size.z;
        var pt = new Bach.RelativisticPoint({
          
        });
        pt.
        this.points.push(). pos.clone());
      }
    }
  }

  this.frontIndices     = [[0, 1], [1, 2], [2, 3], [3, 0]];
  this.backIndices      = [[4, 5], [5, 6], [6, 7], [7, 4]];
  this.frontBackIndices = [[0, 4], [1, 5], [2, 6], [3, 7]];
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
Bach.RelativisticCube.prototype.calculateObserverTime = function(tStart, tEnd, observer, stepSize) {
  for(var i=0; i<this.points.length; i++) {
    this.points[i].calculateObserverTime(tStart, tEnd, observer, stepSize);
  }
};

