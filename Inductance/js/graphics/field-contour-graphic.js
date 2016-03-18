/***
 * Bach Field Contour Graphic
 */

'use strict';

Bach = Bach || {};

/***
 * Create a contour line where the contour is defined by a set of field line points all emitted at the same moment.
 * @param config
 * @param {Bach.ChargedParticle} config.particle - The particle.
 * @param {Number} config.index - The index of the contour's field points.
 * @param {THREE.Object3D} config.parent3d - The parent object.
 * @param {Number} config.characteristicLength - Used to scale the size of objects.
 * @constructor
 */
Bach.FieldContourGraphic = function(config) {
  this.config = _.clone(config);
  var particle = this.config.particle;
  var index = this.config.index;

  this.fieldPoints = [];

  for(var i=0; i<particle.fieldLines.length; i++) {
    var point = particle.fieldLines[i].points[index];
    this.fieldPoints.push(point);
  }

  this.fieldContourSpline = new Bach.DynamicSpline({
    initialNumPoints: this.fieldPoints.length * 3,
    parent3d: this.config.parent3d,
    color: 0xFFFFff
  });

};

Bach.FieldContourGraphic.prototype.updatePosition = function(time) {
  var positions = [];
  for(var i=0; i<this.fieldPoints.length; i++) {
    var position = this.fieldPoints[i].getPosition(time);
    if(position) {
      positions.push(position);
    }
  }

  if(positions.length > 1) {
    this.fieldContourSpline.curve.visible = true;
    this.fieldContourSpline.updatePoints(positions);
  } else {
    this.fieldContourSpline.curve.visible = false;
  }
};
