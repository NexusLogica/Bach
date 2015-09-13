/***
 */
'use strict';

var Shear = Shear || {};

Shear.StraightPath = function() {

};

Shear.StraightPath.prototype.configure = function(config) {
  this.initialVelocity = config.initialVelocity;
  this.accelerationX = config.acceleration;
};

Shear.StraightPath.prototype.position = function(t) {
  var pos = new Bach.Vector(this.initialVelocity*t+0.5*this.accelerationX*t*t);
  return pos;
};

Shear.StraightPath.prototype.velocity = function(t) {
  var vel = new Bach.Vector(this.initialVelocity+this.accelerationX*t, 0);
  return vel;
};

Shear.StraightPath.prototype.acceleration = function(t) {
  var accel = new Bach.Vector(this.accelerationX, 0);
  return accel;
};

Shear.StraightPath.prototype.distanceTraveled = function(t) {
  return this.initialVelocity*t+0.5*this.accelerationX*t*t;
};
