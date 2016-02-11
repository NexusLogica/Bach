/***
 * Bach Straight Particle Simulation
 */

'use strict';

Bach = Bach || {};

Bach.StraightParticleSimulation = function(config) {
  this.config = $.extend({}, config);
  this.configureRenderer();
};

Bach.StraightParticleSimulation.prototype.configureRenderer = function() {
  this.renderer = new Bach.RelativityScene($.extend({

    },
    this.config
  ));

  this.renderer.renderContinuously(this.onRender.bind(this));

};

Bach.StraightParticleSimulation.prototype.onRender = function() {

};
