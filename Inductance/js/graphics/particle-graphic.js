/***
 * Bach Particle Graphic
 */

'use strict';

Bach = Bach || {};

/***
 * Create run and display a simulation from the configuration.
 * @param config
 * @param {Bach.ChargedParticle} config.particle - The particle object.
 * @param {THREE.Object3D} config.parent3d - The parent object.
 * @param {Number} config.characteristicLength - Used to scale the size of objects.
 * @constructor
 */
Bach.ParticleGraphic = function(config) {
  this.config = _.clone(config);
  this.particle = this.config.particle;

  this.particlePath = new Bach.DynamicSpline({
    initialNumPoints: this.particle.positions.length * 3,
    points: this.particle.positions,
    parent3d: this.config.parent3d
  });

  var geometry = new THREE.SphereGeometry(this.config.characteristicLength * 0.01, 16, 16);
  var material = new THREE.MeshBasicMaterial({color: 0xff0000});
  this.mesh = new THREE.Mesh(geometry, material);
  this.config.parent3d.add(this.mesh);

  this.fieldLineGraphics = [];

  var lines = this.particle.fieldLines;
  for(var i=0; i<lines.length; i++) {
    var fieldLineGraphic = new Bach.FieldLineGraphic({
      fieldLine: lines[i],
      parent3d: config.parent3d,
      time: this.config.time,
      characteristicLength: config.characteristicLength
    });
    this.fieldLineGraphics.push(fieldLineGraphic);
  }

  this.updatePosition(this.config.time);
};

Bach.ParticleGraphic.prototype.updatePosition = function(time) {
  var state = this.particle.getStateAtTime(time);
  this.mesh.position.copy(state.position);

  for(var i=0; i<this.fieldLineGraphics.length; i++) {
    this.fieldLineGraphics[i].updatePosition(time, state.position);
  }
};
