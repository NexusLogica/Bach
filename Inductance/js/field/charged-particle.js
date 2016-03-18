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

  this.floatTimes = undefined;
  this.floatTables = {};

  this.search = new Bach.TableSearch();
  this.interp = new Bach.PolynomialInterpolation();
};

/***
 * Using the trajectory, find the position, velocity and acceleration at the given time. The values are stored
 * in arrays of THREE.Vector3's.
 * @param time - The time, in seconds.
 * @param {Boolean} createNewFieldPoint - If true then add a new field point to each field line.
 * @returns {*}
 */
BachField.ChargedParticle.prototype.updatePosition = function(time, createNewFieldPoint) {
  var state = this.trajectory.updatePosition(time);

  this.times.push(time);
  this.positions.push(state.position);
  this.velocities.push(state.velocity);
  this.accelerations.push(state.acceleration);

  if(createNewFieldPoint) {
    for (var i = 0; i < this.fieldLines.length; i++) {
      var fieldLine = this.fieldLines[i];
      fieldLine.addPoint(time, state);
    }
  }

  this.currentState = state;
  return state;
};

/***
 * This is called after all of the calls to updatePosition are done. It is used to create vectors
 * of Float64Array' for each states x, y, and z.
 */
BachField.ChargedParticle.prototype.updateTables = function() {
  this.floatTimes = new Float64Array(this.times);

  this.updateTable('positions');
  this.updateTable('velocities');
  this.updateTable('accelerations');
};

/***
 * Private: A helper function for converting THREE.Vector3 arrays into Float64 arrays of that type.
 * @param tableName
 */
BachField.ChargedParticle.prototype.updateTable = function(tableName) {
  var dims = ['x', 'y', 'z'];
  var ar = this[tableName];
  for(var i = 0; i<dims.length; i++) {
    var floats = new Float64Array(ar.length);
    var dim = dims[i];
    for(var j = 0; j<ar.length; j++) {
      floats[j] = ar[j][dim];
    }
    this.floatTables[tableName+dim.toUpperCase()] = floats;
  }
};

BachField.ChargedParticle.prototype.getStateAtTime = function(time) {
  var index = this.search.find(time, this.floatTimes);
  if(index > this.floatTimes.length-3) {
    index = this.floatTimes.length-3;
  }

  var state = {};
  state.position = new THREE.Vector3(
    this.interp.interpolate(time, this.floatTimes, this.floatTables.positionsX, index),
    this.interp.interpolate(time, this.floatTimes, this.floatTables.positionsY, index),
    this.interp.interpolate(time, this.floatTimes, this.floatTables.positionsZ, index)
  );
  return state;
};

