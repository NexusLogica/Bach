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

if(QUnit && QUnit.assert) {
  QUnit.assert.near = function (actual, expected, eps, message) {
    var hasEps = (arguments.length > 2 && typeof eps !== "string");
    var actualMessage = (hasEps ? message : eps);
    var tolerance = 1.0e-6;
    if (hasEps) {
      tolerance = eps;
    }
    var diff = Math.abs(actual - expected);
    QUnit.push(diff < tolerance, actual, expected, actualMessage);
  };
}
