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

  var _this = this;
  $root.find('.pause-play-button').on('click', function(event) {
    if(_this.runParent.state === "playing") {
      _this.validateAndConfigure();
      _this.runParent.pause();
      $(event.target).text('Run');
    } else {
      _this.runParent.play();
      $(event.target).text('Pause');
    }
    return false;
  });

};

Shear.Controls.prototype.validateAndConfigure = function() {
  var velocity = this.$root.find('#velocity').val();
  localStorage.setItem('lastVelocity', velocity);

  var config = {
    "velocity": velocity
  };
  this.runParent.configureRunParameters(config);
};
