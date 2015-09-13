/***
 */
'use strict';

var Shear = Shear || {};

Shear.CircularPath = function() {

};

Shear.CircularPath.prototype.configure = function(config) {
  this.initialVelocity = config.initialVelocity;
  this.accelerationX = config.acceleration;
  this.radius = config.radius;

  this.center = new Bach.Vector(0.0, -this.radius, 0.0);
};

Shear.CircularPath.prototype.position = function(t) {
  var distanceTraveled = this.distanceTraveled(t);

  var circumference = 2.0*Math.PI*this.radius;
  var angle = distanceTraveled/circumference*360;
  var radians = Math.PI*angle/180;

  var pos = new Bach.Vector(this.radius*Math.sin(radians), this.radius*Math.cos(radians)+this.radius);
  return pos;
};

Shear.CircularPath.prototype.velocity = function(t) {
  var distanceTraveled = this.distanceTraveled(t);

  var circumference = 2.0*Math.PI*this.radius;
  var angle = distanceTraveled/circumference*360;
  var radians = Math.PI*angle/180;

  var speed = this.initialVelocity+this.accelerationX*t;
  var vel = new Bach.Vector(speed*Math.sin(radians+Math.PI/2), speed*Math.cos(radians+Math.PI/2));
  return vel;
};

Shear.CircularPath.prototype.acceleration = function(t) {
  var accel = new Bach.Vector(this.accelerationX, 0);
  return accel;
};

Shear.CircularPath.prototype.rotation = function(t) {
  var distanceTraveled = this.distanceTraveled(t);

  var circumference = 2.0*Math.PI*this.radius;
  var angle = distanceTraveled/circumference*360;
  var radians = Math.PI*angle/180;

  return radians;
};

Shear.CircularPath.prototype.distanceTraveled = function(t) {
  return this.initialVelocity*t+0.5*this.accelerationX*t*t;
};
