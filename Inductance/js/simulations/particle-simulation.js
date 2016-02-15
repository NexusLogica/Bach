/***
 * Bach Straight Particle Simulation
 */

'use strict';

Bach = Bach || {};

/***
 * Create run and display a simulation from the configuration.
 * @param config
 * @param {Number} config.startTime - Optional. default is zero.
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
  this.endTime = this.fromConfig('endTime', function() { throw 'Bach.ParticleSimulation: Must have an end time'});
  this.playbackTime = this.fromConfig('playbackTime', 10.0);

  this.playbackTimeScaling = this.playbackTime/(this.endTime-this.startTime);

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
      onenterlooping: function() {
        this.animation.isAnimating = true;
        this.animation.looping = true;
        this.animation.currentTime = this.startTime;
        this.animation.refresh = true;
      }.bind(this)
    }
  });
};


Bach.ParticleSimulation.prototype.startAnimation = function(loop, rateScaling) {

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
  if(this.animation.refresh) {
    this.animation.startComputerTime = now;
  } else {
    var dt = now-this.animation.startComputerTime;
    this.animation.time += dt*this.animation.playbackTimeScaling;
  }

  //this.particle.
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

Bach.ParticleSimulation.prototype.addParticleTrajectory = function() {
  this.particlePath = new Bach.DynamicSpline({
    initialNumPoints: this.particle.positions.length*3,
    points: this.particle.positions,
    parent3d: this.renderer.scene
  });

  var geometry = new THREE.SphereGeometry(this.characteristicLength*0.01, 16, 16);
  var material = new THREE.MeshBasicMaterial( {color: 0x0000ff} );
  this.particleGraphic = new THREE.Mesh( geometry, material );
  this.renderer.scene.add(this.particleGraphic);
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
