/***
 * Bach field point. A virtual point/particle within the field emanating from a charged particle.
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
BachField.FieldPoint = function(config) {
  this.config = _.clone(config);
  this.direction = config.direction;
  this.startPoint = config.startPoint;
  this.startTime = config.startTime;
};

BachField.FieldPoint.prototype.getPosition = function(time) {
  var dist = Bach.C*(time-this.startTime);
  if(dist < 0) {
    return undefined;
  }

  return this.startPoint.clone().addScaledVector(this.direction, (time-this.startTime)*Bach.C);
};