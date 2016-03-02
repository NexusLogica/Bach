/***
 * Bach Straight Particle Simulation
 */

'use strict';

Bach = Bach || {};

/***
 * Create run and display a simulation from the configuration.
 * @param config
 * @param {BachField.ChargedParticle} config.particle - The particle.
 * @param {Number} config.startTime - Optional. default is zero.
 * @param {Number} config.endTime - The end time.
 * @param {Bach.Trajectory} config.trajectory - A trajectory object.
 * @param {JSON} config.rendererConfig - configuration for the renderer.
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
  this.fillRenderer(this.startTime);

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

  var numSteps = Math.round((this.endTime-this.startTime)/stepSize);
  var defaultSpacing = numSteps % 10;
  var fieldPointSpacing = this.fromConfig('fieldPointSpacing', defaultSpacing);

  var lastIteration = false;
  var maxIterations = 200;
  for(var i=0; i<maxIterations; i++) {

    this.particle.updatePosition(time, (i % fieldPointSpacing) === 0);

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
    this.config.rendererConfig || {}
  ));
  this.renderer.signals.beforeRender.add(this.beforeRender.bind(this));
};

Bach.ParticleSimulation.prototype.fillRenderer = function(time) {

  this.addFramesToScene();

  this.particleGraphic = new Bach.ParticleGraphic({
    particle: this.particle,
    time: time,
    parent3d: this.renderer.scene,
    characteristicLength: this.characteristicLength
  });
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

    this.particleGraphic.updatePosition(this.animation.currentTime);
  }
};

Bach.ParticleSimulation.prototype.addFramesToScene = function() {
  this.calculateBoundingBox();
};

Bach.ParticleSimulation.prototype.calculateBoundingBox = function() {
  var all = [];
  all.push.apply(all, this.getMinMax(this.particle.positions));
  var final = this.getMinMax(all);
  this.boundingBox = { min: final[0], max: final[1] };
  this.characteristicLength = this.boundingBox.min.distanceTo(this.boundingBox.max);
};

Bach.ParticleSimulation.prototype.setAnimationTime = function(t) {
  var all = [];
  all.push.apply(all, this.getMinMax(this.particle.positions));
  var final = this.getMinMax(all);
  this.boundingBox = { min: final[0], max: final[1] };
  this.characteristicLength = this.boundingBox.min.distanceTo(this.boundingBox.max);
};

Bach.ParticleSimulation.prototype.updateParticlePosition = function(time) {
  var state = this.particle.getStateAtTime(time);
  this.particleGraphic.position.copy(state.position);
};

/***
 * Return two points representing a bounding box min and max for the points.
 * @param {Array.<THREE.Vector3>} points - The points to find min and max of.
 * @return {Array.<THREE.Vector3>} - An array of two THREE.Vector3 points representing the min and max.
 */
Bach.ParticleSimulation.prototype.getMinMax = function(points) {
  // Must have points.
  if(points.length === 0) {
    console.error('Bach.ParticleSimulation.getMinMax: zero length array.'+(new Error('NA')).stack);
    return;
  }

  var min = points[0].clone();
  var max = points[1].clone();

  for(var i=1; i<points.length; i++) {
    var p = points[i];
    if(p.x < min.x) {
      min.x = p.x;
    } else if(p.x > max.x) {
      max.x = p.x;
    }
    if(p.y < min.y) {
      min.y = p.y;
    } else if(p.y > max.y) {
      max.y = p.y;
    }
    if(p.z < min.z) {
      min.z = p.z;
    } else if(p.z > max.z) {
      max.z = p.z;
    }
  }

  return [ min, max ];
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
