/***
 * Bach straight trajectory.
 *
 */
'use strict';

var BachField = BachField || {};

/***
 *
 * @param config
 * @param {THREE.Vector3} config.direction - A unit vector that is the direction of travel of the field point.
 * @param {THREE.Vector3} config.startPoint - A vector with units of meters that is the position of the emitting particle is at when the field point leaves it.
 * @param {Number} config.startTime - The time, in seconds, when the field point is emitted.
 * @constructor
 */
BachField.StraightTrajectory = function(config) {
  this.config = _.clone(config);
  this.initialPosition = this.config.initialPosition;
  this.initialSpeed = this.config.initialSpeed;
  this.initialDirection = this.config.initialDirection;
  this.acceleration = this.config.acceleration;

  this.direction = this.initialDirection.clone();
  this.currentPosition = undefined;

  this.times = [];
  this.positions = [];
  this.velocities = [];
  this.accelerations = [];
};

BachField.ChargedParticle.prototype.updatePosition = function(time) {
  this.currentTime = time;
  this.currentPosition = this.initialPosition.addVectors(this.initialDirection, this.initialSpeed*time+0.5*this.acceleration*time*time);
  this.currentVelocity = this.initialDirection.clone().multiplyScalar(this.initialSpeed+this.acceleration*time);
  this.currentAcceleration = this.initialDirection.clone().multiplyScalar(this.acceleration);

  this.times.push(time);
  this.positions.push(this.currentPosition);
  this.velocities.push(this.currentVelocity);
  this.accelerations.push(this.currentAcceleration);
};
