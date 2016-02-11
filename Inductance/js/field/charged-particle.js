/***
 * Bach charged particle. A virtual particle emitting a field.
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
BachField.ChargedParticle = function(config) {
  this.config = _.clone(config);
  this.startPoint = config.startPoint;
  this.currentPosition = undefined;

  this.fieldLines = [];
  for(var i=0; i<this.config.fieldLines.length; i++) {
    this.fieldLines.push(new BachField.FieldLine(this.config.fieldLines[i]));
  }

  this.trajectory = this.config.trajectory;

  var times = [];
  var positions = [];
  var velocities = [];
  var accelerations = [];
};

BachField.ChargedParticle.prototype.updatePosition = function(time) {
  this.times.push(time);
  var state = this.trajectory.getPositionalState(time);
  this.position.push(state.position);
  this.velocities.push(state.velocity);
  this.accelerations.push(state.acceleration);

  for(var i=0; i<this.fieldLines.length; i++) {
    var fieldLine = this.fieldLines[i];
    fieldLine.addPoint(time, state);
  }
};

BachField.ChargedParticle.prototype.getPositionAtTime = function(time) {
  var dist = Bach.C*(time-this.startTime);
  if(dist < 0) {
    return undefined;
  }

  this.currentPosition = this.startPoint.clone().addScaledVector(this.direction, (time-this.startTime)*Bach.C);
};

