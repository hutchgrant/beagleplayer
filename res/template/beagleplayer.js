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
    var TrackState = "idle";


    var hours = 0, minutes= 0, seconds = 0;
    var totalHours= 0, totalMinutes= 0, totalSeconds= 0;
    var min= 0;

    var seekSlider = "#seek_slider", seekVal = "#seek_amount";
    var volSlider = "#vol_slider", volVal = "#vol_amount";
    var timerInterval;

    var trackTitle = document.getElementById("track_title");
    var seekTime = document.getElementById("seek_time");
    var seekRange = document.getElementById("seek_range");

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
      if(TrackName != ""){
          TrackState = "play";
      }

      TrackPos = detached.getSeekPos();
      $( seekSlider ).slider( "value", TrackPos );

      TrackRange = detached.getRange();    // { min: 0, max : 100 }; // Get Track Min/Max Range Time
      $( seekSlider ).slider( "option", "max", TrackRange );

      TrackVolume = detached.getVolume();
       $( volSlider ).slider( "value", TrackVolume );

      if(TrackState == "play"){
          calcMinMax();
          calcRange();
      }
  }

    function addEvents(){
        prevButton.addEventListener('click', function() {
            sendRemoteCmd(5);
        }, false);

        pauseButton.addEventListener('click', function() {
            TrackState = "pause";
            sendRemoteCmd(4);
        }, false);

        stopButton.addEventListener('click', function() {
            TrackState = "stop";
            sendRemoteCmd(2);
        }, false);

        playButton.addEventListener('click', function() {
            TrackState = "play";
            sendRemoteCmd(1);
        }, false);

        nextButton.addEventListener('click', function() {
            sendRemoteCmd(4);
        }, false);
    }
    function sendRemoteCmd(cmd){
        detached.remoteCmd(cmd)
    }

    function calcRange(){
        console.log("Total Range: " + TrackRange);
        totalMinutes = parseInt(TrackRange / 60);
        console.log("Total Minutes: " + totalMinutes);

        totalHours = parseInt(totalMinutes / 60);
        console.log("Total hours: " + totalHours);

        totalMinutes = parseInt(totalMinutes - (totalHours * 60));
        console.log("Total Minutes after hours subtracted: " + totalMinutes);

        totalSeconds = parseInt(TrackRange - (totalMinutes * 60));
        console.log("Total Seconds After minutes and hours subtracted: " + totalSeconds);

        seekRange.innerHTML = totalHours + ":" + totalMinutes + ":" + totalSeconds
    }

    function calcMinMax(){

        /// we need to call every second, iterates every 0.5 for slider visuals
        if(timerInterval){
            timerInterval = false;
        }else{
            if(TrackPos > seconds){
                seconds = TrackPos
            }
            seconds++;
            min = seconds;
            if(seconds % 60 == 0){
                 seconds = 0;
                 minutes++;
            }
            if(min % 60 == 0){
                minutes = 0 ;
                hours++;
            }
            seekTime.innerHTML = hours + ":" + minutes + ":" + seconds
            timerInterval = true;
        }
    }

    /* INIT */
    TrackName = detached.getTrack();
    trackTitle.innerHTML = TrackName;
    TrackRange = detached.getRange();
    TrackVolume = detached.getVolume();

    addEvents();
    createSlider(0,0,TrackRange,seekSlider, seekVal);
    createSlider(TrackVolume,0,100,volSlider, volVal);

    var timer=setInterval(function () {checkWidget()}, 500);

});
