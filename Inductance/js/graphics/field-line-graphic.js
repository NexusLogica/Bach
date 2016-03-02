/***
 * Bach Field Line Graphic
 */

'use strict';

Bach = Bach || {};

/***
 * Create run and display a simulation from the configuration.
 * @param config
 * @param {Bach.FieldLine} config.fieldLine - The field line object.
 * @param {THREE.Object3D} config.parent3d - The parent object.
 * @param {Number} config.characteristicLength - Used to scale the size of objects.
 * @constructor
 */
Bach.FieldLineGraphic = function(config) {
  this.config = _.clone(config);
  this.fieldLine = this.config.fieldLine;

  //this.fieldLineSpline = new Bach.DynamicSpline({
  //  initialNumPoints: this.particle.positions.length * 3,
  //  points: this.fieldLine.positions,
  //  parent3d: this.config.parent3d
  //});

  this.fieldPointGraphics = [];

  for(var i=0; i<this.fieldLine.points.length; i++) {
    var point = this.fieldLine.points[i];
    var pointGraphic = new Bach.FieldPointGraphic({
      fieldPoint: point,
      parent3d: this.config.parent3d,
      time: this.config.time,
      characteristicLength: config.characteristicLength
    });
    this.fieldPointGraphics.push(pointGraphic);
  }
};

Bach.FieldLineGraphic.prototype.updatePosition = function(time) {
  //var state = this.particle.getStateAtTime(time);
  //this.mesh.position.copy(state.position);

  for(var i=0; i<this.fieldPointGraphics.length; i++) {
    this.fieldPointGraphics[i].updatePosition(time);
  }

};
