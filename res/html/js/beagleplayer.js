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

     /* $( volSlider ).slider('option', 'value',TrackVolume);
      $( volSlider ).slider('option','slide')
             .call($( volSlider ),null,{ handle: $('.ui-slider-handle', $( volSlider )), value: TrackVolume }); */
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
