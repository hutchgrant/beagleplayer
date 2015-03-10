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
    var TrackPath = "";
    var TrackPos = 0;
    var TrackRange = 0;
    var TrackVolume = 0;
    var TrackState = 5;
    var TrackChange = false;

    var hours = 0, minutes= 0, seconds = 0;
    var totalHours= 0, totalMinutes= 0, totalSeconds= 0;
    var videoStarted = false, fullScreened = false, playerVisible = false;
    var pastPaused = false;

    var seekSlider = "#seek_slider", seekVal = "#seek_amount";
    var volSlider = "#vol_slider", volVal = "#vol_amount";
    var timerInterval = false;

    var trackVideo = document.getElementById("video");
    var trackSource = document.getElementById("source");

    var trackTitle = document.getElementById("track_title");
    var seekTime = document.getElementById("seek_time");
    var seekRange = document.getElementById("seek_range");

    var prevButton = document.getElementById('prev');
    var stopButton = document.getElementById('stop');
    var pauseButton = document.getElementById('pause');
    var playButton = document.getElementById('play');
    var nextButton = document.getElementById('next');
    var fullScreen = document.getElementById('full');
    var openButton = document.getElementById('open');

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
      TrackState = detached.getState();
      TrackName = detached.getTrack();
      trackTitle.innerHTML = TrackName;
      TrackPath = detached.getPath();
      TrackChange = detached.getPlaylistMove();
      if(TrackChange){
          defaultRange();
      }

      TrackPos = detached.getSeekPos();
      $( seekSlider ).slider( "value", TrackPos );

      TrackRange = detached.getRange();    // { min: 0, max : 100 }; // Get Track Min/Max Range Time
      $( seekSlider ).slider( "option", "max", TrackRange );

      TrackVolume = detached.getVolume();
       $( volSlider ).slider( "value", TrackVolume );
      if(parseInt(TrackState) === 1){
          calcRange(TrackPos, seekTime);
          calcRange(TrackRange, seekRange);
          if(!videoStarted){
              trackVideo.setAttribute("src",TrackPath);
              trackVideo.setAttribute("type","video/mp4");
              trackVideo.load();
              videoStarted = true;
              pastPaused = false;
              togglePlayer();
              trackVideo.play();
          }else{
              if(pastPaused){
                  trackVideo.play();
                  pastPaused = false;
              }
          }
      }else if(parseInt(TrackState) === 3){
          videoStarted = false;
          TrackName = "";
          TrackPath = "";
          TrackPos = 0;
          TrackRange = 0;
          TrackVolume = 0;
          TrackState = 5;
          defaultRange();
          html5();
          pastPaused = true;
      }else if(parseInt(TrackState) === 2){
          trackVideo.pause();
          pastPaused = true;
      }else{
          defaultRange();
      }
  }
    function defaultRange(){
        seekTime.innerHTML = "00" + ":" + "00" + ":" + "00";
        seekRange.innerHTML = "00" + ":" + "00" + ":" + "00";
    }

    function addEvents(){
        prevButton.addEventListener('click', function() {
            videoStarted = false;  // reload media src
            sendRemoteCmd(5);
        }, false);

        pauseButton.addEventListener('click', function() {
            sendRemoteCmd(2);
        }, false);

        stopButton.addEventListener('click', function() {
            sendRemoteCmd(3);
        }, false);

        playButton.addEventListener('click', function() {
            sendRemoteCmd(1);
        }, false);

        nextButton.addEventListener('click', function() {
            videoStarted = false;  // reload media src
            sendRemoteCmd(4);
        }, false);

        fullScreen.addEventListener("loadedmetadata", goFullscreen, false);
        fullScreen.addEventListener('click', function() {
            toggleFullScreen();
        }, false);
        openButton.addEventListener('click', function() {
            togglePlayer();
        }, false);

        trackVideo.addEventListener("pause", function() {
           ///  sendRemoteCmd(2);
        }, false);
        trackVideo.addEventListener("durationchange", function() {
             detached.remoteRange(trackVideo.duration);
        }, false);
    }
    function sendRemoteCmd(cmd){
        detached.remoteCmd(cmd);
    }
    function toggleFullScreen(){
        if(!fullScreened){
            trackVideo.setAttribute("width", 2000);
            trackVideo.setAttribute("height", 1000);
            fullScreened = true;
        }else{
            trackVideo.setAttribute("height", 700);
            trackVideo.setAttribute("width", 1024);
            fullScreened = false;
        }
    }
    function togglePlayer(){
        if(!playerVisible){
            trackVideo.style.display = "block";
            playerVisible = true;
        }else{
            trackVideo.style.display = "none";
            playerVisible = false;
        }
    }

    function calcRange(pos, range){
        /// we need to call every second, iterates every 0.5 for slider visuals
        if(timerInterval){
            timerInterval = false;
        }else{
            totalMinutes = parseInt(pos) / 60;
            totalHours = (parseInt(totalMinutes) / 60);

            totalMinutes = parseInt(totalMinutes - (parseInt(totalHours) * 60));
            totalSeconds = parseInt(pos - (parseInt(totalMinutes) * 60));

            var strSec = "", strMin = "", strHr = "";
            if(parseInt(totalSeconds) < 10){
                strSec = "0" + parseInt(totalSeconds);
            }else{
                strSec = parseInt(totalSeconds);
            }
            if(parseInt(totalMinutes) < 10){
                strMin = "0" + parseInt(totalMinutes);
            }else{
                strMin = parseInt(totalMinutes);
            }
            if(parseInt(totalHours) < 10){
                strHr = "0" + parseInt(totalHours);
            }else{
                strHr = parseInt(totalHours);
            }
            range.innerHTML = strHr + ":" + strMin + ":" + strSec;
        }
    }
    function goFullscreen() {
        trackVideo.webkitEnterFullscreen();
    }
    /* INIT */
    TrackPath = detached.getPath();
    TrackName = detached.getTrack();
    trackTitle.innerHTML = TrackName;
    TrackRange = detached.getRange();
    TrackVolume = detached.getVolume();

    addEvents();
    createSlider(0,0,TrackRange,seekSlider, seekVal);
    createSlider(TrackVolume,0,100,volSlider, volVal);

    var timer=setInterval(function () {checkWidget()}, 500);

});
