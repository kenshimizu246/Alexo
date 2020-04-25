import React, { useState, useEffect } from 'react';
import ReactDOM from 'react-dom';


function ServoSlider(handler, id, conn, min, max, step, initVal, width, height){
  const [value, setValue] = useState(0);
  const [clicking, setClicking] = useState(false);

  const handleId = 'handle'+id;
  const boxId = 'box'+id;

  useEffect(() => {
    conn.on('MSG',onMessage.bind(this));
    console.log('useEffect...');

    return () => {conn.removeEventListener('MSG', onMessage)};
  }, []);

  function onMessage(msg){
    var s = JSON.stringify(msg);
    if(msg.header.source === id){
      if(0 < msg.body.value){
        setValue(msg.body.value);
      }
    }
  }

  function handleMouseDown(e) {
    var hdl = document.getElementById(handleId);
    console.log('handle.MouseDown:'+e.target.id
              +':'+e.clientX
              );
    setClicking(true);
  }

  function pos2val() {
    var box = document.getElementById(boxId);
    var hdl = document.getElementById(handleId);

    var hdlPos = hdl.offsetLeft;
    var boxPos = box.offsetLeft;
    var hdlWidth = hdl.offsetWidth;
    var boxWidth = box.offsetWidth;

    var vr = max - min;
    var p = hdlPos - (boxPos + 1);
    var r = boxWidth - (hdlWidth + 2);
    var vu = vr / r;

    return (vu * p) + min;
  }

  function val2pos(val, boxWidth, handleWidth) {
    var vr = max - min;
    var r = boxWidth - handleWidth - 2; // 2 is both side borders.
    var u = r / vr;

    return (u * val) + 11; // 11 must be box.offsetLeft (10) + 1px border.
  }

  function handleMouseUp(e) {
    var box = document.getElementById(boxId);
    var hdl = document.getElementById(handleId);

    var boxWidth = box.offsetWidth;
    var hdlWidth = hdl.offsetWidth;

    var range = boxWidth - (hdlWidth + 2);
    var val = pos2val();
    console.log('handle.mouseUp'
              +':v:'+val
              +':'+e.clientX
              +':'+hdlWidth
              +':'+boxWidth
              );
    setValue(val);
    setClicking(false);
    handler(val);
  }

  function handleMouseMove(e) {
    if(e.buttons == 1 && clicking){
      var box = document.getElementById(boxId);
      var hdl = document.getElementById(handleId);

      var boxPos = box.offsetLeft;
      var hdlPos = hdl.offsetLeft;
      var boxWidth = box.offsetWidth;
      var hdlWidth = hdl.offsetWidth;
      var movement = e.movementX;

      console.log('handle.mouseMove:'
              +e.target.id
              +':'+e.button
              +':'+e.buttons
              +':'+e.clientX
              +':'+e.movementX
              +':'+clicking
              +':'+box
              +':'+hdl
              );

      var limL = boxPos;
      var limR = boxPos + boxWidth - hdlWidth;
      var x = hdlPos + movement;
      if(limL < x && x < limR){
        hdl.style.position = 'absolute';
        hdl.style.left = x + 'px';
      }
    }
  }

  var p = val2pos(initVal, 322, 20) + 'px';
  var hdlstyle = {
        position: 'absolute',
        left: p,
  };
  var w = width + 'px';
  var h = height + 'px';
  var boxstyle = {
    width: w,
    height: h,
    border: 'solid 1px'
  };

  return (
    <div className='sliderBox' id={boxId}
        onMouseMove={handleMouseMove}
    >
      <div className='sliderHandle'
        id={handleId}
        style={hdlstyle}
        onMouseDown={handleMouseDown}
        onMouseUp={handleMouseUp}
      />
    </div>
  );
}

export default ServoSlider;
