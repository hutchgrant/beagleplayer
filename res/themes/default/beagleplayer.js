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
    var pastPaused = false, originalColor = "", screenMode = false, toggleFull = false;
    var displayGUI = false, volumeShowing = false, ignoreRange = false;
    var playlistShowing = false, directoriesShowing = false;
    var playbackRetry = false, DEBUG = false;

    var seekSlider = "#seek_slider", seekVal = "#seek_amount";
    var volSlider = "#vol_slider", volVal = "#vol_amount";
    var timerInterval = false;

    var trackVideo = document.getElementById("video");
    var trackSource = document.getElementById("source");

    var trackTitle = document.getElementById("track_title");
    var trackButtons = document.getElementById("track_buttons");
    var trackVolume = document.getElementById("track_volume");
    var trackSeek = document.getElementById("track_seek");

    var seekTime = document.getElementById("seek_time");
    var seekRange = document.getElementById("seek_range");
    var playBody = document.getElementsByTagName('body')[0];

    var player = document.getElementById('player');
    var playlist = document.getElementsByClassName('playlist')[0];
    var playlistList = document.getElementById('playlist_table');
    var directory = document.getElementsByClassName('directory')[0];
    var directoryList = document.getElementById('directory_table');



     /*
      * Create seek+volume sliders
     */
    function createSlider(init, min, max, sliderName, sliderVal){
        var orientation = "horizontal";
      if(sliderName === "#vol_slider"){
          orientation = "vertical";
     }
     $( sliderName ).slider({
        range: "min",
        value: init,
        min: min,
        max: max,
        orientation: orientation,
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
          $('#playlist_table > tbody').html("");
           $('#directory_table > tbody').html("");
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
                displayPlaylist();
                displayDirectories();
          }else{
              if(pastPaused){
                  trackVideo.play();
                  pastPaused = false;
              }

              if(parseInt(TrackPos) - trackVideo.currentTime > 2 && trackVideo.currentTime > 0){
                  moveSlider = true;
              }else if(parseInt(TrackPos) - trackVideo.currentTime < -2  && trackVideo.currentTime > 0){
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
        ignoreRange = false;
        trackVideo.setAttribute("src",TrackPath);
        if(parseInt(TrackMode) === 0 || parseInt(TrackMode) === 2){
            trackVideo.setAttribute("type","audio/mp3");
            showGUI();
            if(TrackMode === 2){
                ignoreRange = true;
            }
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
        $('#prev').click(function() {
            videoStarted = false;  // reload media src
            sendRemoteCmd(5);
        });
        $('#pause').click(function() {
             sendRemoteCmd(2);
        });
        $('#play').click(function() {
            sendRemoteCmd(1);
        });
        $('#next').click(function() {
            videoStarted = false;  // reload media src
            sendRemoteCmd(4);
        });
        $('#full').click(function() {
            if(!screenMode){
                screenMode = true;
            }else{
                screenMode = false;
            }
            toggleFullScreen(true);
        });
        $('#video').click(function() {
               showGUI();
        });
        $('#open').click(function() {
            toggleDirectories();
        });
        $('#vol_button').click(function() {
            toggleVolume();
        });
        $('#playlist').click(function() {
            togglePlaylist();
        });

        trackVideo.addEventListener("durationchange", function() {
            TrackRange = trackVideo.duration;
             detached.remoteRange(trackVideo.duration);
        }, false);

        if(DEBUG){
            debugging();
        }
    }
    /*
      *  Show Controls Button GUI
      */
    function showGUI(){
        if(!displayGUI){
            trackTitle.style.display = "block";
            trackButtons.style.display = "block";
            trackVolume.style.display = "block";
            trackSeek.style.display = "block";
            displayGUI = true;
        }else{
            trackTitle.style.display = "none";
            trackButtons.style.display = "none";
            trackVolume.style.display = "none";
            trackSeek.style.display = "none";
            displayGUI = false;
        }
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
    function toggleFullScreen(sendRemote){
        if(screenMode || toggleFull){
            trackVideo.setAttribute("width", 2000);
            trackVideo.setAttribute("height", 1200);
            trackVideo.style.marginLeft = "-2%";
            trackVideo.style.marginTop = "10px";
            originalColor = playBody.style.backgroundColor;
            playBody.style.backgroundColor = "black";
            if(sendRemote){
                detached.remoteScreen(screenMode);
            }
        }else{
            trackVideo.setAttribute("height", 800);
            trackVideo.setAttribute("width", 1024);
            trackVideo.style.marginLeft = "auto";
            trackVideo.style.marginTop = "10px";
            playBody.style.backgroundColor = originalColor;
            if(sendRemote){
                detached.remoteScreen(screenMode);
            }
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
            trackVideo.style.display = "block";
            playerVisible = false;
        }
    }
    /*
     * Toggle Playlist display
     */
    function togglePlaylist(){
        if(!playlistShowing){
            playlist.style.display = "inline";
            playlistShowing = true;
        }else{
            playlist.style.display = "none";
            playlistShowing = false;
        }
    }
    /*
     * Toggle Directory display
     */
    function toggleDirectories(){
        if(!directoriesShowing){
             directory.style.display = "inline";
            directoriesShowing = true;

        }else{
             directory.style.display = "none";
            directoriesShowing = false;
        }
    }
    /*
      * Toggle Volume display
      */
    function toggleVolume(){
        if(!volumeShowing){
            trackVolume.style.display = "block";
            volumeShowing = true;
        }else{
            trackVolume.style.display = "none";
             volumeShowing = false;
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
            totalSeconds = (parseInt(pos - (parseInt(totalMinutes) * 60)) - ((parseInt(totalHours) *60)*60));

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
            controlPlaylist();
        }
    }
    /*
    * Control the playlist, prev/next
    */
    function controlPlaylist(){
        if(trackVideo.currentTime >= TrackRange && TrackRange > 1 && !ignoreRange){
            // seek to the next song, if their is a range, if range isn't being ignored
            videoStarted = false;  // reload media src
            sendRemoteCmd(4);
        }
    }
    /*
    * Fill in Playlist
    */
    function displayPlaylist(){
        var row = [], cell1 = [];
        var track = "";
        var counter = 0;
        for(var x=0; x<fileobj.getSize(); x++){
            if(parseInt(fileobj.getPar(x)) === parseInt(detached.getSelectedDirID())){
                track = "<p id='track_" +counter + "'>" +fileobj.getQStrName(x) + "</p>";
                row[counter] = playlistList.insertRow(0);
                cell1[counter] = row[counter].insertCell(0);
                cell1[counter].innerHTML = track;
                cell1[counter].setAttribute("class", fileobj.getID(x));
                cell1[counter].setAttribute("id", counter);
                cell1[counter].onclick=function(){playlistSelection(this)};
                counter++;
                if(DEBUG){
                    console.log(fileobj.getQStrName(x) + "  index = " + counter);
                }
            }
        }
    }
    /*
    * Fill in Directories
    */
    function displayDirectories(){
        var row = [], cell1 = [];
        var track = "";
        var counter = 0;
        for(var x=0; x<dirobj.getSize(); x++){
                track = "<p id='dir_" +counter + "'>" +dirobj.getQStrName(x) + "</p>";
                 row[counter] =   directoryList.insertRow(0)
                cell1[counter] = row[counter].insertCell(0);
                cell1[counter].innerHTML = track;
                cell1[counter].setAttribute("class", dirobj.getID(x));
                cell1[counter].setAttribute("id", counter);
                cell1[counter].onclick=function(){directorySelection(this)};
                counter++;
                console.log(dirobj.getQStrName(x) + "  index = " + counter);
        }
    }
    /*
     * Determine which playlist item was selected, send signal to player
     */
    function playlistSelection(x){
        var trackID = x.getAttribute("class");
        var trackNum = x.getAttribute("id");
        var trackName = x.textContent;

        detached.remoteTrack(trackNum);
        togglePlaylist();
    }
    /*
     * Determine which directory item was selected, send signal to player
     */
    function directorySelection(x){
        var trackID = x.getAttribute("class");
        var trackNum = x.getAttribute("id");
        var trackName = x.textContent;

        detached.remoteDirectory(trackNum);
        $('#playlist_table > tbody').html("");
           displayPlaylist();
      ///  togglePlaylist();
    }
    /*
      * Briefly pause playback then try reload if playback interrupted or src not found
      */
    function retryPlayback(){
        if(!playbackRetry){
             sendRemoteCmd(2);  // pause playback
            var delay=5000;//1 seconds
             setTimeout(function(){
                 sendRemoteCmd(1);  // play playback
             },delay);
            playbackRetry = true;
        }else{
             sendRemoteCmd(2);  // pause playback
            playbackRetry = false;
        }
    }
    /*
      * Debugging Playback
      */
    function debugging(){
            trackVideo.addEventListener("stalled", function() {
                console.log("TRACK STALLED!");
            }, false);
            trackVideo.addEventListener("abort", function() {
                console.log("TRACK aborted!");
            }, false);
            trackVideo.addEventListener("error", function() {
                console.log("TRACK Error!");
            }, false);
            trackVideo.addEventListener("suspend", function() {
                console.log("TRACK paused!");
            }, false);
            trackVideo.addEventListener("ended", function() {
                console.log("TRACK ended!");
            }, false);
            trackVideo.addEventListener("waiting", function() {
                console.log("TRACK waiting!");
            }, false);
            trackVideo.addEventListener("canplaythrough", function() {
                console.log("TRACK doesn't need buffering!");
            }, false);
            trackVideo.addEventListener("progress", function() {
                console.log("TRACK progress!");
            }, false);
    }

    /* Debugging - Enable for verbose events */
    DEBUG = false;
    /* INIT */
    TrackPath = detached.getPath();
    TrackName = detached.getTrack();
    trackTitle.innerHTML = TrackName;
    TrackVolume = detached.getVolume();
    screenMode = detached.getScreenMode();
    toggleFullScreen(false);

    addEvents();
    createSlider(0,0,TrackRange,seekSlider, seekVal);
    createSlider(TrackVolume,0,100,volSlider, volVal);

    var timer=setInterval(function () {checkWidget()}, 500);
});
