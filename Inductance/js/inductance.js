/***
 * Bach.Inductance is a namespace for the Inductance app.
 *
 * Copyright (C) 2015 Lawrence Gunn - All Rights Reserved
 *
 * @file inductance.js
 * @class Bach.Inductance
 */
'use strict';

var Bach = Bach || {};
Bach.Inductance = Bach.Inductance || {};
Bach.Inductance3D = Bach.Inductance3D || {};

Bach.Inductance3D.FollowUniversal = 1;
Bach.Inductance3D.FollowRelativistic = 2;
Bach.Inductance3D.SizeRealPositionRelative = 3;

if(window.QUnit && window.QUnit.assert) {
  QUnit.assert.near = function (actual, expected, eps, message) {
    var hasEps = (arguments.length > 2 && typeof eps !== "string");
    var actualMessage = (hasEps ? message : eps);
    var tolerance = 1.0e-6;
    if (hasEps) {
      tolerance = Math.abs(eps);
    }
    var diff = Math.abs(actual - expected);
    QUnit.push(diff < tolerance, actual, expected, actualMessage);
  };

  QUnit.assert.lessThan = function (actual, expected, message) {
    QUnit.push(actual < expected, actual, expected, message);
  };

  QUnit.assert.vectorNear = function (actual, expected, eps, message) {
    var hasEps = (arguments.length > 2 && typeof eps !== "string");
    var actualMessage = (hasEps ? message : eps);
    var tolerance = 1.0e-6;
    if (hasEps) {
      tolerance = Math.abs(eps);
    }
    var diffX = Math.abs(actual.x - expected.x);
    var diffY = Math.abs(actual.y - expected.y);
    var diffZ = Math.abs(actual.z - expected.z);

    var err = "z";
    var diffMax = diffZ;
    if(diffX > diffY && diffX > diffZ) {
      err = "x";
      diffMax = diffX;
    } else if(diffY > diffX && diffY >   diffZ) {
      err = "y";
      diffMax = diffY;
    }
    QUnit.push(diffMax < tolerance, actual[err], expected[err], actualMessage+": Max is "+err);
  };

}
