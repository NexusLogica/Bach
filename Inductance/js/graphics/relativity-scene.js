/***
 * Bach.Inductance is a namespace for the Inductance app.
 *
 * Copyright (C) 2015 Lawrence Gunn - All Rights Reserved
 *
 * @file inductance.js
 * @class Bach.Inductance
 */
'use strict';

Bach.RelativityScene = function() {
  this.scale = 1.0;
};

/***
 *
 * @method create
 * @param config
 * @param {DomElement} config.containerElement - canvas container
 */
Bach.RelativityScene.prototype.create = function(config) {
  this.scene =  new THREE.Scene();
  if(config.hasOwnProperty('scale')) {
    this.scale = config.scale;
  }

  this.beforeRenderSignal = new signals.Signal();

  this.renderer = new THREE.WebGLRenderer({ antialias: true });
  this.width = config.containerElement.innerWidth();
  this.height = config.containerElement.innerHeight();
  this.renderer.setViewport(0, 0, this.width, this.height);
  this.renderer.setSize(this.width, this.height);
  this.renderer.setClearColor(0xff0000, 0.0);

  config.containerElement.append(this.renderer.domElement);

  this.camera = new THREE.PerspectiveCamera(60, this.width / this.height, 0.001, 1000);
  this.camera.translateZ(-0.8*this.scale);
  this.camera.translateY(-0.4*this.scale);
  this.addDefaultLighting(this.scene);
  this.addAxisMarker();
  this.createDebugCloud();

  this.showGrid(config.showGrid);
  this.enablePanAndRotate(true);
  this.stopRendering = false;
};

/***
 * Adds a function that is called before each render.
 * @param {Function} onBefore - Called with 'this' as the only argument.
 */
Bach.RelativityScene.prototype.addBeforeRenderListener = function(onBefore) {
  this.beforeRenderSignal.add(onBefore);
};


/***
 * Removed a previously added function that is called before each render.
 * @param {Function} onBefore
 */
Bach.RelativityScene.prototype.removeBeforeRenderListener = function(onBefore) {
  this.beforeRenderSignal.remove(onBefore);
};


/***
 * @method renderContinuously
 */
Bach.RelativityScene.prototype.renderContinuously = function(onRenderFunction) {
  var _this = this;
  this.stopRendering = false;
  var rf = function() {
    if(onRenderFunction) {
      onRenderFunction();
    }
    _this.render();
    if(!_this.stopRendering) {
      requestAnimationFrame(rf);
    }
  };

  requestAnimationFrame(rf);
};

/***
 * Do the actual render.
 */
Bach.RelativityScene.prototype.render = function() {
  this.beforeRenderSignal.dispatch(this);
  this.renderer.render(this.scene, this.camera);
};

Bach.RelativityScene.prototype.showGrid = function(show) {
  if(show) {
    if(!this.gridHelper) {
      var gridHelperWidth = 1.0*this.scale;
      this.gridHelper = new THREE.GridHelper(Math.floor(gridHelperWidth / 0.025*this.scale) * 0.025*this.scale, 0.025*this.scale);
      this.gridHelper.setColors(0x3662B2, 0x7b99c4);
      this.scene.add(this.gridHelper);
    }
    this.gridHelper.visible = true;
  } else if(this.gridHelper) {
    this.gridHelper.visible = false;
  }
};

Bach.RelativityScene.prototype.addDefaultLighting = function (object3D) {
  // Add default ambient light.
  //var light = new THREE.AmbientLight(0xffffff);
  var light = new THREE.AmbientLight(0x106060);
  light.name = "default-ambientLight";
  light.serialisable = false;
  light.cannotBeEdited = true;
  object3D.add(light);

  // Add a directional light.
  light = new THREE.DirectionalLight(0xffffff, 1.0);
  light.target.name = 'default-directional-light-target';
  light.target.serialisable = false;
  light.target.cannotBeEdited = true;
  object3D.add(light.target);

  // Add (another) directional light.
  light.name = 'default-directional-light';
  light.position.set(-1.0, -1.0, -1.5);

  light.serialisable = false;
  light.cannotBeEdited = true;
  object3D.add(light);
};

Bach.RelativityScene.prototype.addAxisMarker = function() {
  var axisHelper = new THREE.AxisHelper(0.25);
  this.scene.add(axisHelper);
};

Bach.RelativityScene.prototype.enablePanAndRotate = function(enable) {
  if(enable) {
    if(!this.panAndRotateControl) {
      this.panAndRotateControl = new THREE.OrbitControls(this.camera, this.renderer.domElement);
      var _this = this;
      this.panAndRotateControl.addEventListener('change', function () {
        _this.render();
      });
    }
    this.panAndRotateControl.enabled = true;
  } else {
    if(this.panAndRotateControl) {
      this.panAndRotateControl.enabled = false;
    }
  }
};

Bach.RelativityScene.prototype.createDebugCloud = function() {
  var geometry = new THREE.BoxGeometry(0.02*this.scale, 0.02*this.scale, 0.02*this.scale);
  var material = new THREE.MeshBasicMaterial( {color: 0xF2995F} );
  material.wireframe = true;
  //var geometry = new THREE.CylinderGeometry(0, 10, 30, 4, 1);
  //var material = new THREE.MeshLambertMaterial({color: 0xffffff, shading: THREE.FlatShading});

  var spread = 1.0*this.scale;

  for (var i = 0; i < 200; i++) {
    var mesh = new THREE.Mesh(geometry, material);
    mesh.position.x = ( Math.random() - 0.5 ) * spread;
    mesh.position.y = ( Math.random() - 0.5 ) * spread;
    mesh.position.z = ( Math.random() - 0.5 ) * spread;
    mesh.updateMatrix();
    mesh.matrixAutoUpdate = false;
    this.scene.add(mesh);
  }
};
