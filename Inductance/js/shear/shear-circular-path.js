/***
 */
'use strict';

var Shear = Shear || {};

Shear.CircularPath = function() {

};

Shear.CircularPath.prototype.configure = function(config) {
  this.initialVelocity = config.initialVelocity;
  this.accelerationX = config.acceleration;
  this.delay = config.delay;
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

  var speed = this.initialVelocity;
  if(t > this.delay) {
    speed += this.accelerationX*(t-this.delay);
  }
  var vel = new Bach.Vector(speed*Math.sin(radians+Math.PI/2), speed*Math.cos(radians+Math.PI/2));
  return vel;
};

Shear.CircularPath.prototype.acceleration = function(t) {
  return new Bach.Vector(t > this.delay ? this.accelerationX : 0.0, 0);
};

Shear.CircularPath.prototype.rotation = function(t) {
  var distanceTraveled = this.distanceTraveled(t);

  var circumference = 2.0*Math.PI*this.radius;
  var angle = distanceTraveled/circumference*360;
  var radians = Math.PI*angle/180;

  return radians;
};

Shear.CircularPath.prototype.distanceTraveled = function(t) {
  var d = this.initialVelocity*t;
  if(t > this.delay) {
    var ta = t-this.delay;
    d += 0.5*this.accelerationX*ta*ta;
  }
  return d;
};
