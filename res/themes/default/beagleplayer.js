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
    var TrackMode = 0;
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

     /*
      * Create seek+volume sliders
     */
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
  /*
   * Repeatedly check widgets, to see if any of our var have changed
  */
  function checkWidget(){
      TrackMode = detached.getMode();
      TrackState = detached.getState();
      TrackChange = detached.getPlaylistMove();
      if(TrackChange == true){
          defaultRange();
          videoStarted = false;
      }

      TrackName = detached.getTrack();
      trackTitle.innerHTML = TrackName;
      TrackPath = detached.getPath();
      TrackPos = detached.getSeekPos();
      TrackVolume = detached.getVolume();

      $( seekSlider ).slider( "value", TrackPos );
      $( seekSlider ).slider( "option", "max", TrackRange );
       $( volSlider ).slider( "value", TrackVolume );

      if(parseInt(TrackState) === 1){
          var moveSlider = false;
          if(!videoStarted){
                loadAndStart();
          }else{
              if(pastPaused){
                  trackVideo.play();
                  pastPaused = false;
              }
              console.log( TrackPos + " " +trackVideo.currentTime);

              if(parseInt(TrackPos) - trackVideo.currentTime >= 2){
                  moveSlider = true;
              }else if(parseInt(TrackPos) - trackVideo.currentTime <= -2){
                  moveSlider = true;
              }
          }
          calcRange(TrackPos, seekTime, moveSlider);
          calcRange(TrackRange, seekRange, false);
      }else if(parseInt(TrackState) === 3){
          videoStarted = false;
          TrackName = "";
          TrackPath = "";
          TrackPos = 0;
          TrackRange = 0;
          TrackVolume = 0;
          TrackState = 5;
          defaultRange();
          trackVideo.pause();
          pastPaused = true;
      }else if(parseInt(TrackState) === 2){
          trackVideo.pause();
          pastPaused = true;
      }else{
          defaultRange();
      }
  }
    /*
     * Load the media file into the <video> tag, initialize player
     */
    function loadAndStart(){
        trackVideo.setAttribute("src",TrackPath);
        if(parseInt(TrackMode) === 0){
            trackVideo.setAttribute("type","audio/mp3");
        }else{
            trackVideo.setAttribute("type","video/mp4");
            playerVisible = false;
            togglePlayer();
        }
        // TrackRange = trackVideo.duration;
        trackVideo.load();
        videoStarted = true;
        pastPaused = false;
        trackVideo.play();
    }
    /*
     * Initialize media timer with default range
     */
    function defaultRange(){
        seekTime.innerHTML = "00" + ":" + "00" + ":" + "00";
        seekRange.innerHTML = "00" + ":" + "00" + ":" + "00";
    }
    /*
     * Add event listeners to our buttons and player
     */
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
            TrackRange = trackVideo.duration;
             detached.remoteRange(trackVideo.duration);
        }, false);
    }
    /*
     * Send Remote signals to qt widget
     */
    function sendRemoteCmd(cmd){
        detached.remoteCmd(cmd);
    }
    /*
     * Toggle <video> size
     */
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
    /*
     * Toggle <video> display visibility
     */
    function togglePlayer(){
        if(!playerVisible){
            trackVideo.style.display = "block";
            playerVisible = true;
        }else{
            trackVideo.style.display = "none";
            playerVisible = false;
        }
    }
     /*
      * Calculate a timer's range
      */
    function calcRange(pos, range, moveSeek){
        /// we need to call every second, iterates every 0.5 for slider visuals
        if(timerInterval){
            timerInterval = false;
        }else{
            if(moveSeek){
                trackVideo.currentTime = pos;
            }

            trackVideo.volume = TrackVolume/100;

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
    TrackVolume = detached.getVolume();

    addEvents();
    createSlider(0,0,TrackRange,seekSlider, seekVal);
    createSlider(TrackVolume,0,100,volSlider, volVal);

    var timer=setInterval(function () {checkWidget()}, 500);
});
