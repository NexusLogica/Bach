/***
 * Bach Field Line Graphic
 */

'use strict';

Bach = Bach || {};

/***
 * Create run and display a simulation from the configuration.
 * @param config
 * @param {Bach.ChargedParticle} config.particle - The particle object.
 * @param {Number} config.endTime - The end time.
 * @param {Bach.Trajectory} config.trajectory - A trajectory object.
 * @param {JSON} config.renderer - configuration for the renderer.
 * @param config
 * @param config
 * @param config
 * @param config
 * @param config
 * @constructor
 */
Bach.ParticleSimulation = function(config) {
  this.config = _.clone(config);
  this.particle = this.config.particle;

  this.startTime = this.fromConfig('startTime', 0.0);
  this.endTime = this.fromConfig('endTime', function() { throw 'Bach.ParticleSimulation: Must have an end time'; });
  this.playbackTime = this.fromConfig('playbackTime', 10.0);

  this.playbackTimeScaling = (this.endTime-this.startTime)/this.playbackTime;

  this.runSimulation();
  this.configureRenderer();
  this.fillRenderer();

  this.animation = {
    isAnimating: false,
    looping: false,
    refresh: false
  };


  this.renderer.renderContinuously();

  this.stateMachine = StateMachine.create({
    initial: 'notAnimating',
      events: [
        { name: 'beginLooping',   from: 'notAnimating', to: 'looping'      },
        { name: 'beginAnimation', from: 'notAnimating', to: 'animating'    },
        { name: 'stopLooping',    from: 'looping',      to: 'notAnimating' }
      ],
    callbacks: {
      onenteranimating: function() {
        this.animation.isAnimating = true;
        this.animation.looping = true;
        this.animation.currentTime = this.startTime;
        this.animation.refresh = true;
      }.bind(this)
    }
  });
};


Bach.ParticleSimulation.prototype.startAnimation = function(loop, rateScaling) {
  if(this.stateMachine.can('beginAnimation')) {
    this.stateMachine.beginAnimation();
  } else {
    console.error();
  }
};

Bach.ParticleSimulation.prototype.stopAnimation = function() {

};

Bach.ParticleSimulation.prototype.runSimulation = function() {

  var time = this.startTime;
  var stepSize = this.config.stepSize;
  if(_.isUndefined(stepSize) || stepSize <= 0.0) {
    throw 'Bach.ParticleSimulation.runSimulation: step size was zero or negative: '+stepSize;
  }

  var initialDirection = this.fromConfig('initialDirection', new THREE.Vector3(1.0, 0.0, 0.0));
  var startPoint = this.fromConfig('startPoint', new THREE.Vector3(0.0, 0.0, 0.0));

  var lastIteration = false;
  var maxIterations = 200;
  for(var i=0; i<maxIterations; i++) {

    this.particle.updatePosition(time);

    if(lastIteration) {
      break;
    }
    time += stepSize;
    if(time >= this.endTime) {
      time = this.endTime;
      lastIteration = true;
    }
  }

  // Update the floating point array tables in the particle.
  this.particle.updateTables();
};


Bach.ParticleSimulation.prototype.configureRenderer = function() {
  this.renderer = new Bach.RelativityRenderer($.extend({},
    this.config.renderer || {}
  ));
  this.renderer.signals.beforeRender.add(this.beforeRender.bind(this));
};

Bach.ParticleSimulation.prototype.fillRenderer = function() {

  this.addFramesToScene();
  this.addParticleTrajectory();
};

/***
 * Called before each render. Update when doing animations.
 */
Bach.ParticleSimulation.prototype.beforeRender = function() {
  var now = (new Date()).getTime();
  if(this.animation.isAnimating) {

    if(this.animation.lastTime) {
      if(this.animation.looping) {
        this.animation.refresh = true;
      } else {
        this.animation.isAnimating = false;
      }
    }

    if (this.animation.isAnimating && this.animation.refresh) {
      this.animation.lastComputerTime = now;
      this.animation.totalComputerTime = 0;
      this.animation.lastTime = false;
      this.animation.refresh = false;
      this.animation.currentTime = this.startTime;
    } else {

      var dt = now - this.animation.lastComputerTime;

      // If we are debugging or there was a system delay keep the delta t to a small value.
      if(dt > 60) { // 60 milliseconds
        dt = 30;
      }
      this.animation.currentTime += 0.001*dt*this.playbackTimeScaling;

      if(this.animation.currentTime > this.endTime) {
        this.animation.currentTime = this.endTime;
        this.animation.lastTime = true;
      } else if(this.animation.currentTime === this.endTime) {
        if(this.animation.looping) {
          this.animation.currentTime = this.startTime;
        } else {
          this.animation.isAnimating = false;
        }
      }
    }

    this.updateParticlePosition(this.animation.currentTime);
  }
};

Bach.ParticleSimulation.prototype.addParticleTrajectory = function() {
  this.particlePath = new Bach.DynamicSpline({
    initialNumPoints: this.particle.positions.length * 3,
    points: this.particle.positions,
    parent3d: this.renderer.scene
  });

  var geometry = new THREE.SphereGeometry(this.characteristicLength * 0.01, 16, 16);
  var material = new THREE.MeshBasicMaterial({color: 0x0000ff});
  this.particleGraphic = new THREE.Mesh(geometry, material);
  this.renderer.scene.add(this.particleGraphic);

  this.updateParticlePosition(this.startTime);
};

Bach.ParticleSimulation.prototype.updateParticlePosition = function(time) {
  var state = this.particle.getStateAtTime(time);
  this.particleGraphic.position.copy(state.position);
};

Bach.ParticleSimulation.prototype.fromConfig = function(key, defaultValue) {
  if(!_.isUndefined(this.config[key])) {
    return this.config[key];
  } else {
    if(_.isFunction(defaultValue)) {
      return defaultValue();
    }
    return defaultValue;
  }
};
