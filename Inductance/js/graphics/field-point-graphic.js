/***
 * Bach Field Point Graphic
 */

'use strict';

Bach = Bach || {};

/***
 * Build the 3D graphics for a field point.
 * @param config
 * @param {Bach.FieldPoint} config.fieldPoint - The field point object.
 * @param {Number} config.characteristicLength - Used to scale the size of objects.
 * @constructor
 */
Bach.FieldPointGraphic = function(config) {
  this.config = _.clone(config);
  this.fieldPoint = this.config.fieldPoint;

  var geometry = new THREE.SphereGeometry(this.config.characteristicLength * 0.005, 10, 10);
  var material = new THREE.MeshBasicMaterial({ color: 0x00ff00 });
  this.mesh = new THREE.Mesh(geometry, material);
  this.config.parent3d.add(this.mesh);

  this.updatePosition(this.config.time);
};

Bach.FieldPointGraphic.prototype.updatePosition = function(time) {
  var position = this.fieldPoint.getPosition(time);
  if(!position) {
    this.mesh.visible = false;
  } else {
    this.mesh.visible = true;
    this.mesh.position.copy(position);
  }
};
