/***
 * Shear.FieldPoint:
 */
'use strict';

var Shear = Shear || {};

Shear.FieldPoint = function() {
  this.position = new Bach.Vector();
  this.velocity = 1.0; // c
};

Shear.FieldPoint.prototype.initialize = function(svgGroup, initialPosition, direction, startTime) {
  this.initialPosition = initialPosition;
  this.direction = direction;
  this.startTime = startTime;

  this.width = 3.0;
  this.circle = svgGroup.circle(0.5*this.width).attr("fill", "blue");
};

Shear.FieldPoint.prototype.update = function(time, scale) {
  var t = time-this.startTime;
  var x = scale*(this.initialPosition.x+this.direction.x*t*this.velocity);
  var y = scale*(this.initialPosition.y+this.direction.y*t*this.velocity);
  this.circle.center(x, y);
};
