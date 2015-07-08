'use strict';

var Bach = Bach || {};

Bach.AcceleratingCharge = function(data) {
  var d = data || {};
  this.charge = d.charge || -1.0;




};

Bach.AcceleratingCharge.prototype.forceAtPoint = function(t, chargePos) {
  var pos = this.getPosition(t);
  return
};