jQuery( document ).ready(function($) {

    console.log("TESTING BEAGLEPLAYER JS");

    var TrackName = "";
    var TrackPos = 0;
    var TrackRange = 0;
    var TrackVolume = 0;

    var seekSlider = "#seek_slider", seekVal = "#seek_amount";
    var volSlider = "#vol_slider", volVal = "#vol_amount";
    var firstRun;

    TrackName = detached.getTrack();
    document.getElementById("track_title").innerHTML = TrackName;
    TrackRange = detached.getRange();
    TrackVolume = detached.getVolume();
    console.log(TrackVolume)
    console.log(TrackRange);

    function createSlider(init, min, max, sliderName, sliderVal){
      $( sliderName ).slider({
        range: "min",
        value: init,
        min: min,
        max: max,
        slide: function( event, ui ) {
          $( sliderVal ).val( ui.value );
        }
      });
      $( sliderVal ).val( $( sliderName ).slider( "value" ) );
    }

  var timer=setInterval(function () {checkWidget()}, 500);
  function checkWidget(){

      TrackName = detached.getTrack();
      document.getElementById("track_title").innerHTML = TrackName;

      TrackPos = detached.getSeekPos();
      $( seekSlider ).slider( "value", TrackPos );

      TrackRange = detached.getRange();    // { min: 0, max : 100 }; // Get Track Min/Max Range Time
      $( seekSlider ).slider( "option", "max", TrackRange );

      TrackVolume = detached.getVolume();
      console.log(TrackVolume);
       $( volSlider ).slider( "value", TrackVolume );

  }

  function sendRemoteCmd(cmd){
      detached.remoteCmd(cmd)
  }

    createSlider(0,0,TrackRange,seekSlider, seekVal)
    createSlider(TrackVolume,0,100,volSlider, volVal)
});
