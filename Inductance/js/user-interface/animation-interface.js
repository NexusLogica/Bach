/***
 *
 */
'use strict';

$(function() {
  $('.animation-play-pause').on('click', function() {
    if($(this).hasClass('play')) {
      window.simulation.startAnimation();
      $(this).addClass('pause').removeClass('play');
    } else {
      window.simulation.stopAnimation();
      $(this).addClass('play').removeClass('pause');
    }
  });
});