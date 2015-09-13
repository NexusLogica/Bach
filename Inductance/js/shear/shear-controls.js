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

  var radius = localStorage.getItem('lastRadius') || 1.0;
  this.$root.find('#radius').val(radius);

  var useRadius = (localStorage.getItem('lastUseRadius') === 'true');
  this.$root.find('#use-radius').prop('checked', useRadius);
  if(!useRadius) { this.$root.find('#radius').prop('disabled', true); }

  var _this = this;
  this.$root.find('.pause-play-button').on('click', function(event) {
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

  this.$root.find('#use-radius').on('change', function() {
    var checked = _this.$root.find('#use-radius').prop('checked');
    _this.$root.find('#radius').prop('disabled', !checked);
  });
};

Shear.Controls.prototype.validateAndConfigure = function() {
  var velocity = this.$root.find('#velocity').val();
  localStorage.setItem('lastVelocity', velocity);

  var acceleration = this.$root.find('#acceleration').val();
  localStorage.setItem('lastAcceleration', acceleration);

  var config = {
    "initialVelocity": parseFloat(velocity),
    "acceleration": parseFloat(acceleration)
  };

  var useRadius = this.$root.find('#use-radius').prop('checked');
  if(useRadius) {
    config.pathType = 'circular';
    config.radius = parseFloat(this.$root.find('#radius').val());

    localStorage.setItem('lastRadius', config.radius);
    localStorage.setItem('lastUseRadius', true);
  } else {
    config.pathType = 'straight';
    localStorage.setItem('lastUseRadius', false);
  }

  this.runParent.configureRunParameters(config);
};
