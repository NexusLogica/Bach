/***
 */
'use strict';

var Shear = Shear || {};

Shear.Graphics = function() {

};

Shear.Graphics.prototype.initialize = function(container) {
  this.svg = SVG($(container).get(0)).size(container.width(), container.height());
  this.observerCoordGroup = this.svg.group();

  var w = this.svg.width();
  var h = this.svg.height();
  this.observerCoordGroup.move(0.5*w, 0.5*h);

};
