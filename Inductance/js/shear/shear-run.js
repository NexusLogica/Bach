/***
 */
'use strict';

var Shear = Shear || {};

Shear.Run = function() {
  this.numFieldPoints = 25;
  this.fieldPointSets = [];
  this.scale = 10000.0; // pixels per light-second
  this.timeScale = 0.001;
};

Shear.Run.prototype.start = function(canvasContainerElement) {
  this.graphics = new Shear.Graphics();
  this.graphics.initialize(canvasContainerElement);

  this.particle = new Shear.Particle();
  this.particle.initialize(this.graphics.observerCoordGroup);

  this.rate = 1.0;
  this.startRealTime = window.performance.now();
  this.next = 1.0;
  this.update();

  var _this = this;
  setInterval(function() {
    _this.update();
  }, 60);
//   var loop = function() {
//     _this.update();
//     requestAnimationFrame(loop);
//   };

//   requestAnimationFrame(loop);

};

Shear.Run.prototype.update = function() {
  var now = window.performance.now();
  var tReal = 0.001*(now-this.startRealTime);
  var tSim = tReal*this.timeScale;

  var particlePositionKLUDGE = new Bach.Vector();

  if(tReal > this.next) {
    this.generateFieldPoints(particlePositionKLUDGE, tSim);
    this.next += 1.0;
  }

  for(var i=0; i<this.fieldPointSets.length; i++) {
    var set = this.fieldPointSets[i];
    for(var j=0; j<set.length; j++) {
      var pt = set[j];
      pt.update(tSim, this.scale);
    }
  }
};

Shear.Run.prototype.generateFieldPoints = function(particlePosition, time) {
  var set = [];
  var angle = 0.0;
  var incAngle = Math.PI*(360.0/this.numFieldPoints)/180.0;
  for(var i=0; i<this.numFieldPoints; i++) {
    var point = new Shear.FieldPoint();
    var direction = new Bach.Vector(Math.sin(angle), Math.cos(angle));
    point.initialize(this.graphics.observerCoordGroup, particlePosition, direction, time);
    set.push(point);
    angle += incAngle;
  }
  this.fieldPointSets.push(set);
};

