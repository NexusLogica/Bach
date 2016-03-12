/***
 * Bach Field Contour Graphics
 */

'use strict';

Bach = Bach || {};

/***
 * Create a contour line where the contour is defined by a set of field line points all emitted at the same moment.
 * @param config
 * @param {Bach.ChargedParticle} config.particle - The particle.
 * @param {THREE.Object3D} config.parent3d - The parent object.
 * @param {Number} config.characteristicLength - Used to scale the size of objects.
 * @param {Number} config.time - The current time, used for the first rendered frame.
 * @constructor
 */
Bach.FieldContourGraphics = function(config) {
  this.config = _.clone(config);
  var particle = this.config.particle;

  this.fieldContours = [];

  // We need at least two field lines for a contour.
  if(particle.fieldLines.length > 1) {

    // Use the first one for to determine the size of them all.
    var firstLinePoints = particle.fieldLines[0].points;
    for (var i = 0; i < firstLinePoints.length; i++) {

      var contour = new Bach.FieldContourGraphic({
        particle: particle,
        index: i,
        parent3d: this.config.parent3d,
        characteristicLength: this.config.characteristicLength
      });

      this.fieldContours.push(contour);
    }
  }

  this.updatePosition(this.config.time);
};

Bach.FieldContourGraphics.prototype.updatePosition = function(time) {
  for(var i=0; i<this.fieldContours.length; i++) {
    this.fieldContours[i].updatePosition(time);
  }
};
