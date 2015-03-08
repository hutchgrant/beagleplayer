/*
 *  Written By: Grant Hutchinson
 *  License: GPLv3.
 *  h.g.utchinson@gmail.com
 *  Copyright (C) 2015 by Grant Hutchinson
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
jQuery( document ).ready(function($) {

    var TrackName = "";
    var TrackPos = 0;
    var TrackRange = 0;
    var TrackVolume = 0;

    var seekSlider = "#seek_slider", seekVal = "#seek_amount";
    var volSlider = "#vol_slider", volVal = "#vol_amount";
    var firstRun;

    var trackTitle = document.getElementById("track_title");

    var prevButton = document.getElementById('prev');
    var stopButton = document.getElementById('stop');
    var pauseButton = document.getElementById('pause');
    var playButton = document.getElementById('play');
    var nextButton = document.getElementById('next');

    function createSlider(init, min, max, sliderName, sliderVal){
      $( sliderName ).slider({
        range: "min",
        value: init,
        min: min,
        max: max,
        slide: function( event, ui ) {
            if(sliderName === "#vol_slider"){
                detached.remoteVol(ui.value);
            }else if(sliderName === "#seek_slider"){
                detached.remoteSeek(ui.value);
            }
          $( sliderVal ).val( ui.value );
        }
      });
      $( sliderVal ).val( $( sliderName ).slider( "value" ) );
    }

  function checkWidget(){

      TrackName = detached.getTrack();
      trackTitle.innerHTML = TrackName;

      TrackPos = detached.getSeekPos();
      $( seekSlider ).slider( "value", TrackPos );
      $( seekSlider ).slider('option', 'value',TrackPos);
       $( seekSlider ).slider('option','slide')
                   .call($( seekSlider ),null,{ handle: $('.ui-slider-handle', $( seekSlider )), value: TrackPos });

      TrackRange = detached.getRange();    // { min: 0, max : 100 }; // Get Track Min/Max Range Time
      $( seekSlider ).slider( "option", "max", TrackRange );

      TrackVolume = detached.getVolume();
       $( volSlider ).slider( "value", TrackVolume );
  }

    function addEvents(){
        prevButton.addEventListener('click', function() {
            sendRemoteCmd(5);
        }, false);

        pauseButton.addEventListener('click', function() {
            sendRemoteCmd(4);
        }, false);

        stopButton.addEventListener('click', function() {
            sendRemoteCmd(2);
        }, false);

        playButton.addEventListener('click', function() {
            sendRemoteCmd(1);
        }, false);

        nextButton.addEventListener('click', function() {
            sendRemoteCmd(4);
        }, false);
    }
    function sendRemoteCmd(cmd){
        detached.remoteCmd(cmd)
    }

    /* INIT */
    TrackName = detached.getTrack();
    trackTitle.innerHTML = TrackName;
    TrackRange = detached.getRange();
    TrackVolume = detached.getVolume();

    addEvents();

    createSlider(0,0,100,seekSlider, seekVal);
    createSlider(TrackVolume,0,100,volSlider, volVal);

    var timer=setInterval(function () {checkWidget()}, 500);

});
