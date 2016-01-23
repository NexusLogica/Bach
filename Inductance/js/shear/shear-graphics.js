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

Shear.Graphics.prototype.afterConfigure = function(runConfig, path, scale) {
  if(runConfig.pathType === "circular") {
    this.observerCoordGroup.circle(2.0*runConfig.radius*scale)
      .center(path.center.x*scale, -path.center.y*scale)
      .attr({ "stroke-width": 1, "stroke": "black", "fill": "none" });
  }

  this.velocityIndicator = this.observerCoordGroup.line(0,0,0,0).stroke({ "width": 0.75, "color": "green" });
  this.lightSpeedIndicator = this.observerCoordGroup.line(0,0,0,0).stroke({ "width": 0.25, "color": "green" });
};

Shear.Graphics.prototype.update = function(particlePosition, particleVelocity, scale) {
  var x = particlePosition.x*scale;
  var y = particlePosition.y*scale;
  this.observerCoordGroup.move(0.5*this.w-x, 0.5*this.h-y);
  var c = particleVelocity.clone().normalize();
  var velocityScale = 0.01;
  this.velocityIndicator.plot(x, y, x+particleVelocity.x*scale*velocityScale, y+particleVelocity.y*scale*velocityScale);
  this.lightSpeedIndicator.plot(x, y, x+c.x*scale*velocityScale, y+c.y*scale*velocityScale);
};
