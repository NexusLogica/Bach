/***
 */
'use strict';

var Shear = Shear || {};

Shear.StraightPath = function() {

};

Shear.StraightPath.prototype.configure = function(config) {
  this.initialVelocity = config.initialVelocity;
  this.acceleration = config.acceleration;
};

Shear.StraightPath.prototype.position = function(t) {
  var pos = new Bach.Vector(t*this.initialVelocity+0.5*this.acceleration*t*t);
  pos.x = -pos.x;
  return pos;
};
