/***
 * Bach charged particle. A virtual particle emitting a field.
 *
 */
'use strict';

var BachField = BachField || {};

/***
 *
 * @param config
 * @param {Bach.Trajectory} config.trajectory - The particle trajectory.
 * @constructor
 */
BachField.ChargedParticle = function(config) {
  this.config = _.clone(config);
  this.currentPosition = undefined;

  this.fieldLines = [];
  for(var i=0; i<this.config.fieldLines.length; i++) {
    this.fieldLines.push(new BachField.FieldLine(this.config.fieldLines[i]));
  }

  this.trajectory = this.config.trajectory;

  this.times = [];
  this.positions = [];
  this.velocities = [];
  this.accelerations = [];
};

BachField.ChargedParticle.prototype.updatePosition = function(time) {
  var state = this.trajectory.updatePosition(time);

  this.times.push(time);
  this.positions.push(state.position);
  this.velocities.push(state.velocity);
  this.accelerations.push(state.acceleration);

  for(var i=0; i<this.fieldLines.length; i++) {
    var fieldLine = this.fieldLines[i];
    fieldLine.addPoint(time, state);
  }

  this.currentState = state;
  return state;
};

BachField.ChargedParticle.prototype.getPositionAtTime = function(time) {
  var dist = Bach.C*(time-this.startTime);
  if(dist < 0) {
    return undefined;
  }

  this.currentPosition = this.startPoint.clone().addScaledVector(this.direction, (time-this.startTime)*Bach.C);
};

