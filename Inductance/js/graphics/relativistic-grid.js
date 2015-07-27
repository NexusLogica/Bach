/***
 * Bach.Inductance is a namespace for the Inductance app.
 *
 * Copyright (C) 2015 Lawrence Gunn - All Rights Reserved
 *
 * @file relativistic-grid.js
 * @class Bach.Inductance
 */
'use strict';

Bach.Inductance3D.RelativisticGrid = function() {
};

/***
 *
 * @method create
 * @param config
 * @param {Bach.RelativisticPoints} config.points - size and position based on Bach.Inductance3D's FollowActual, FollowRelativistic, or SizeRealPositionRelative.
 * @param {Number} follow - size and position based on Bach.Inductance3D's FollowActual, FollowRelativistic, or SizeRealPositionRelative.
 * @param {Array} config.connections - vertex numbers to connect.
 */
Bach.Inductance3D.RelativisticGrid.prototype.create = function(config) {
};

/***
 * @method buildGridGraphics
 */
Bach.Inductance3D.RelativisticGrid.prototype.buildGridGraphics = function() {
  var _this = this;
  this.stopRendering = false;
  var rf = function() {
    _this.render();
    if(!_this.stopRendering) {
      requestAnimationFrame(rf);
    }
  };

  requestAnimationFrame(rf);
};

/***
 * @method updateOnRender
 */
Bach.Inductance3D.RelativisticGrid.prototype.updateOnRender = function() {

};