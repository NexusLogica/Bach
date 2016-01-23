/***
 */
'use strict';

var Shear = Shear || {};

Shear.Run = function() {
  this.numFieldPoints = 32;
  this.fieldPointSets = [];
  this.scale = 10000.0; // pixels per light-second
  this.timeScale = 0.001;
  this.state = 'notStarted';
  this.pausedAt = undefined;
};

Shear.Run.prototype.initialize = function(canvasContainerElement, controlContainerElement) {
  this.$container = canvasContainerElement;

  this.graphics = new Shear.Graphics();
  this.graphics.initialize(canvasContainerElement);

  this.controls = new Shear.Controls();
  this.controls.initialize(this, controlContainerElement);
};

Shear.Run.prototype.configureRunParameters = function(runParameters) {
  this.runParameters = runParameters;
  if(runParameters.pathType === "straight") {
    this.path = new Shear.StraightPath();
    this.path.configure(runParameters);
  } else if(runParameters.pathType === "circular") {
    this.path = new Shear.CircularPath();
    this.path.configure(runParameters);
  }
  this.particleInitialPosition = new Bach.Vector(0, 0, 0);

  this.graphics.afterConfigure(runParameters, this.path, this.scale);
};

Shear.Run.prototype.start = function() {

  console.assert(this.path);

  this.particle = new Shear.Particle();
  this.particle.initialize(this.graphics.observerCoordGroup);

  this.rate = 1.0;
  this.startRealTime = window.performance.now();
  this.next = 1.0;
  this.pause();
};

Shear.Run.prototype.pause = function() {
  this.state = 'paused';
  clearInterval(this.intervalTimer);
  this.pausedAt = window.performance.now();
};

Shear.Run.prototype.play = function() {
  if(this.state === 'notStarted') {
    this.start();
  }
  this.state = 'playing';

  if(this.pausedAt) {
    var now = window.performance.now();
    this.startRealTime += now-this.pausedAt;
    this.pausedAt = undefined;
  }

  var _this = this;
  this.intervalTimer = setInterval(function() {
    _this.update();
  }, 60);
};

Shear.Run.prototype.update = function() {
  var now = window.performance.now();
  var tReal = 0.001*(now-this.startRealTime);
  var tSim = tReal*this.timeScale;

  var particlePosition = this.path.position(tSim);
  var velocity = this.path.velocity(tSim);
  var acceleration = this.path.acceleration(tSim).vectorLength();
  var rotation = this.path.rotation(tSim);
  particlePosition.add(this.particleInitialPosition);

  var speed = velocity.vectorLength();

  $(this.$container).find('.time').text(sprintf('%5.4f', tSim));
  $(this.$container).find('.speed').text(sprintf('%5.4f', speed));
  $(this.$container).find('.acceleration').text(sprintf('%5.4f', acceleration));
  $(this.$container).find('.distance').text(sprintf('%5.4f', this.path.distanceTraveled(tSim)));
  $(this.$container).find('.angle').text(sprintf('%5.4f', 180*rotation/Math.PI));

  this.graphics.update(particlePosition, velocity, this.scale);
  this.particle.update(particlePosition, this.scale);

  if(tReal > this.next) {
    this.generateFieldPoints(particlePosition, rotation, tSim);
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

Shear.Run.prototype.generateFieldPoints = function(particlePosition, particleRotation, time) {
  var set = [];
  var angle = Math.PI*particleRotation/180;
  var incAngle = Math.PI*(360.0/this.numFieldPoints)/180.0;
  for(var i=0; i<this.numFieldPoints; i++) {
    var point = new Shear.FieldPoint(i%2 ? "red" : "blue");
    var direction = new Bach.Vector(Math.sin(angle), Math.cos(angle));
    point.initialize(this.graphics.observerCoordGroup, particlePosition, direction, time);
    set.push(point);
    angle += incAngle;
  }
  this.fieldPointSets.push(set);
};

