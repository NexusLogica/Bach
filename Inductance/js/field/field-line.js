/***
 * Bach field line. A line made up of a set of virtual points/particles within the field emenating from a charged particle.
 *
 */
'use strict';

var BachField = BachField || {};

/***
 *
 * @param config
 * @param {THREE.Vector3} config.direction - A unit vector that is the direction of travel that the points/particles are emitted.
 * @constructor
 */
BachField.FieldLine = function(config) {
  this.config = _.clone(config);
  this.direction = config.direction;
  this.points = [];
};

BachField.FieldLine.prototype.addPoint = function(time, particle) {
  this.points.push(new BachField.FieldPoint(
    direction: this.direction.clone(),
    startPoint: particle. config.startPoint;
    startTime = config.startTime;

  ));
};