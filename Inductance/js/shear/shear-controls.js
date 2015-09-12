/***
 */
'use strict';

var Shear = Shear || {};

Shear.Controls = function() {
};

Shear.Controls.prototype.initialize = function(runParent, $root) {
  this.runParent = runParent;
  this.$root = $root;

  var velocity = localStorage.getItem('lastVelocity') || 0.0;
  this.$root.find('#velocity').val(velocity);

  var acceleration = localStorage.getItem('lastAcceleration') || 0.0;
  this.$root.find('#acceleration').val(acceleration);

  var _this = this;
  $root.find('.pause-play-button').on('click', function(event) {
    if(_this.runParent.state === "playing") {
      _this.runParent.pause();
      $(event.target).text('Run');
    } else {
      _this.validateAndConfigure();
      _this.runParent.play();
      $(event.target).text('Pause');
    }
    return false;
  });

};

Shear.Controls.prototype.validateAndConfigure = function() {
  var velocity = this.$root.find('#velocity').val();
  localStorage.setItem('lastVelocity', velocity);

  var acceleration = this.$root.find('#acceleration').val();
  localStorage.setItem('lastAcceleration', acceleration);

  var config = {
    "initialVelocity": velocity,
    "acceleration": acceleration,
    "pathType": "straight"
  };
  this.runParent.configureRunParameters(config);
};
