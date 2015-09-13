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
  var pos = new Bach.Vector(t*this.initialVelocity+0.5*this.accelerationX*t*t);
  pos.x = -pos.x;
  return pos;
};

Shear.StraightPath.prototype.velocity = function(t) {
  var vel = new Bach.Vector(this.initialVelocity+this.accelerationX*t, 0);
  vel.x = -vel.x;
  return vel;
};

Shear.StraightPath.prototype.acceleration = function(t) {
  var accel = new Bach.Vector(this.accelerationX, 0);
  accel.x = -accel.x;
  return accel;
};
