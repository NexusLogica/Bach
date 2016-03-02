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

  this.fieldLineSpline = new Bach.DynamicSpline({
    initialNumPoints: this.fieldPointGraphics.length * 3,
    parent3d: this.config.parent3d,
    color: 0x6666ff
  });

};

Bach.FieldLineGraphic.prototype.updatePosition = function(time, particlePosition) {
  //var state = this.particle.getStateAtTime(time);
  //this.mesh.position.copy(state.position);

  var positions = [];
  for(var i=0; i<this.fieldPointGraphics.length; i++) {
    var position = this.fieldPointGraphics[i].updatePosition(time);
    if(position) {
      positions.push(position);
    }
  }

  positions.push(particlePosition);

  if(positions.length > 1) {
    this.fieldLineSpline.curve.visible = true;
    this.fieldLineSpline.updatePoints(positions);
  } else {
    this.fieldLineSpline.curve.visible = false;
  }
};
