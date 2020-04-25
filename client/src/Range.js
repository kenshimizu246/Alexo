import React, { useState, useEffect } from 'react';



function Range(handlerOnClick, minVal, maxVal, stepVal, initVal, flip){
  const [value, setValue] = useState(initVal);
  const range = maxVal - minVal;
  const center = range/2;

  function convert(value) {
    if(flip){
      value = range - (value - minVal) + minVal
    }
    return value;
  }

  function handlerWrapper(){
    console.log('handlerWrapper');
    if(handlerOnClick){
      handlerOnClick(convert(value));
    }
  }

  return (
    <div>
      <input type="range"
        min={minVal} max={maxVal} step={stepVal} defaultValue={initVal}
        onClick={() => {handlerWrapper();}}
        onChange={(e) => {setValue(e.target.value);}}
        onInput={(e) => {setValue(e.target.value);}}
      />
    </div>
  );
}

export default Range;
