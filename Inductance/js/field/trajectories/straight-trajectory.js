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
 * @param {Number} config.initialSpeed - A scalar value representing speed. This is relative to the speed of light.
 * @param {THREE.Vector3} config.initialPosition - A vector with units of meters that is the position of the emitting particle is at when the field point leaves it.
 * @constructor
 */
BachField.StraightTrajectory = function(config) {
  this.config = _.clone(config);
  this.initialPosition = this.config.initialPosition || new THREE.Vector3(0.0, 0.0, 0.0);
  this.initialSpeed = this.config.initialSpeed;
  this.initialDirection = this.config.initialDirection;
  this.acceleration = !_.isUndefined(this.config.acceleration) ? this.config.acceleration : 0.0;

  this.direction = this.initialDirection.clone();
  this.currentState = undefined;

  this.times = [];
  this.positions = [];
  this.velocities = [];
  this.accelerations = [];
};

BachField.StraightTrajectory.prototype.updatePosition = function(time) {
  var currentTime = time;
  var currentPosition = this.initialPosition.addScaledVector(this.initialDirection, this.initialSpeed*time+0.5*this.acceleration*time*time);
  var currentVelocity = this.initialDirection.clone().multiplyScalar(this.initialSpeed+this.acceleration*time);
  var currentAcceleration = this.initialDirection.clone().multiplyScalar(this.acceleration);

  this.times.push(time);
  this.positions.push(currentPosition);
  this.velocities.push(currentVelocity);
  this.accelerations.push(currentAcceleration);

  this.currentState = {
    time: currentTime,
    position: currentPosition,
    velocity: currentVelocity,
    acceleration: currentAcceleration
  };
  return this.currentState;
};
