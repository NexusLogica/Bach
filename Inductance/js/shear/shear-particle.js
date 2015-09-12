/***
 */
'use strict';

var Shear = Shear || {};

Shear.Particle = function() {
  this.position = new Bach.Vector();
};

Shear.Particle.prototype.initialize = function(svgGroup) {
  this.width = 20.0;
  this.circle = svgGroup.circle(0.5*this.width).attr("fill", "red").center(0, 0);
};

Shear.Particle.prototype.update = function(time) {
  //var pos = this.path(time);
  //this.circle.move(pos.x, pos.y);
};
