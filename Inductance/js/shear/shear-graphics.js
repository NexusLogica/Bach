/***
 */
'use strict';

var Shear = Shear || {};

Shear.Graphics = function() {

};

Shear.Graphics.prototype.initialize = function(container) {
  this.svg = SVG($(container).get(0)).size(container.width(), container.height());
  this.observerCoordGroup = this.svg.group();

  this.w = this.svg.width();
  this.h = this.svg.height();
  this.observerCoordGroup.move(0.5*this.w, 0.5*this.h);
};

Shear.Graphics.prototype.update = function(particlePosition, particleVelocity, scale) {
  this.observerCoordGroup.move(0.5*this.w-particlePosition.x*scale, 0.5*this.h-particlePosition.y*scale);
};
