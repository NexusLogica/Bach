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
  var material = new THREE.MeshBasicMaterial({color: 0x0000ff});
  this.mesh = new THREE.Mesh(geometry, material);
  this.config.parent3d.add(this.mesh);

  this.updateParticlePosition(this.config.time);
};

Bach.ParticleGraphic.prototype.updateParticlePosition = function(time) {
  var state = this.particle.getStateAtTime(time);
  this.mesh.position.copy(state.position);
};
