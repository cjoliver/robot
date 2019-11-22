#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
//#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <ESP8266WiFi.h>
//#include <PubSubClient.h>
//#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <ESP8266HTTPUpdateServer.h>
//#include <Servo.h>
//#include <WiFiClientSecure.h>
//#include <ArduinoJson.h>
const char* ssid = "wificar";
const char* password = "password";

//Set Wifi ssid and password
const char* host = "car";
const char joystick_html[] PROGMEM="<html>\n\t<head>\n\t\t<meta charset=\"utf-8\">\n\t\t<meta name=\"viewport\" content=\"width=device-width, user-scalable=no, minimum-scale=1.0, maximum-scale=1.0\">\t\t\n\t\t<style>\n\t\tbody {\n\t\t\toverflow\t: hidden;\n\t\t\tpadding\t\t: 0;\n\t\t\tmargin\t\t: 0;\n\t\t\tbackground-color: #BBB;\n\t\t}\n\t\t#info {\n\t\t\tposition\t: absolute;\n\t\t\ttop\t\t: 0px;\n\t\t\twidth\t\t: 100%;\n\t\t\tpadding\t\t: 5px;\n\t\t\ttext-align\t: center;\n\t\t}\n\t\t#info a {\n\t\t\tcolor\t\t: #66F;\n\t\t\ttext-decoration\t: none;\n\t\t}\n\t\t#info a:hover {\n\t\t\ttext-decoration\t: underline;\n\t\t}\n\t\t#container {\n\t\t\twidth\t\t: 100%;\n\t\t\theight\t\t: 100%;\n\t\t\toverflow\t: hidden;\n\t\t\tpadding\t\t: 0;\n\t\t\tmargin\t\t: 0;\n\t\t\t-webkit-user-select\t: none;\n\t\t\t-moz-user-select\t: none;\n\t\t}\n\t\t</style>\n\n\t</head>\n\t<body>\n\n\t\n\t\t<div id=\"container\"></div>\n\t\t<div id=\"info\">\n\t\t\tTouch the screen and move <a href=\"http://192.168.4.2:8080/\" target=\"_blank\">Open camera</a>\n\t\t\t<br/>\n\t\t\t<span id=\"result\"></span>\n\t\t</div> \n\t\t<script src=\"./virtualjoystick.js\"></script>\n\t\t<script>\n\t\t\tconsole.log(\"touchscreen is\", VirtualJoystick.touchScreenAvailable() ? \"available\" : \"not available\");\n\t\n\t\t\tvar joystick\t= new VirtualJoystick({\n\t\t\t\tcontainer\t: document.getElementById('container'),\n\t\t\t\tmouseSupport\t: true,\n\t\t\t\tlimitStickTravel\t: true,\n\t\t\t});\n\t\t\tjoystick.addEventListener('touchStart', function(){\n\t\t\t\tconsole.log('down')\n\t\t\t})\n\t\t\tjoystick.addEventListener('touchEnd', function(){\n\t\t\t\tconsole.log('up')\n\t\t\t})\n\t\t\tvar prevX = 0;\n\t\t\tvar prevY = 0;\n\t\t\tvar newX = 0;\n\t\t\tvar newY = 0;\n\t\t\tsetInterval(function(){\n\t\t\t\tvar outputEl\t= document.getElementById('result');\n\t\t\t\tnewX = Math.round(joystick.deltaX());\n\t\t\t\tnewY = Math.round(joystick.deltaY()) * -1;\n\t\t\t\toutputEl.innerHTML\t= '<b>Position:</b> '\n\t\t\t\t\t+ ' X:'+newX\n\t\t\t\t\t+ ' Y:'+newY;\n\t\t\t\tif ( newX != prevX || newY != prevY || newX!=0 || newY!=0){\n\t\t\t\t\tvar xhr = new XMLHttpRequest();\n\t\t\t\t\txhr.open('PUT', \"./jsData.html?x=\"+newX+\"&y=\"+newY)\n\t\t\t\t\txhr.send();\n\t\t\t\t\tsetTimeout(function(){xhr.abort();},100);\n\t\t\t\t}\n\t\t\t\tprevX = newX;\n\t\t\t\tprevY = newY;\n\t\t\t}, 150);\n\t\t</script>\n\t</body>\n</html>";
const char virtualjoystick_js[] PROGMEM="var VirtualJoystick\t= function(opts)\n{\n\topts\t\t\t= opts\t\t\t|| {};\n\tthis._container\t\t= opts.container\t|| document.body;\n\tthis._strokeStyle\t= opts.strokeStyle\t|| 'cyan';\n\tthis._stickEl\t\t= opts.stickElement\t|| this._buildJoystickStick();\n\tthis._baseEl\t\t= opts.baseElement\t|| this._buildJoystickBase();\n\tthis._mouseSupport\t= opts.mouseSupport !== undefined ? opts.mouseSupport : false;\n\tthis._stationaryBase\t= opts.stationaryBase || false;\n\tthis._baseX\t\t= this._stickX = opts.baseX || 0\n\tthis._baseY\t\t= this._stickY = opts.baseY || 0\n\tthis._limitStickTravel\t= opts.limitStickTravel || false\n\tthis._stickRadius\t= opts.stickRadius !== undefined ? opts.stickRadius : 100\n\tthis._useCssTransform\t= opts.useCssTransform !== undefined ? opts.useCssTransform : false\n\n\tthis._container.style.position\t= \"relative\"\n\n\tthis._container.appendChild(this._baseEl)\n\tthis._baseEl.style.position\t= \"absolute\"\n\tthis._baseEl.style.display\t= \"none\"\n\tthis._container.appendChild(this._stickEl)\n\tthis._stickEl.style.position\t= \"absolute\"\n\tthis._stickEl.style.display\t= \"none\"\n\n\tthis._pressed\t= false;\n\tthis._touchIdx\t= null;\n\t\n\tif(this._stationaryBase === true){\n\t\tthis._baseEl.style.display\t= \"\";\n\t\tthis._baseEl.style.left\t\t= (this._baseX - this._baseEl.width /2)+\"px\";\n\t\tthis._baseEl.style.top\t\t= (this._baseY - this._baseEl.height/2)+\"px\";\n\t}\n    \n\tthis._transform\t= this._useCssTransform ? this._getTransformProperty() : false;\n\tthis._has3d\t= this._check3D();\n\t\n\tvar __bind\t= function(fn, me){ return function(){ return fn.apply(me, arguments); }; };\n\tthis._$onTouchStart\t= __bind(this._onTouchStart\t, this);\n\tthis._$onTouchEnd\t= __bind(this._onTouchEnd\t, this);\n\tthis._$onTouchMove\t= __bind(this._onTouchMove\t, this);\n\tthis._container.addEventListener( 'touchstart'\t, this._$onTouchStart\t, false );\n\tthis._container.addEventListener( 'touchend'\t, this._$onTouchEnd\t, false );\n\tthis._container.addEventListener( 'touchmove'\t, this._$onTouchMove\t, false );\n\tif( this._mouseSupport ){\n\t\tthis._$onMouseDown\t= __bind(this._onMouseDown\t, this);\n\t\tthis._$onMouseUp\t= __bind(this._onMouseUp\t, this);\n\t\tthis._$onMouseMove\t= __bind(this._onMouseMove\t, this);\n\t\tthis._container.addEventListener( 'mousedown'\t, this._$onMouseDown\t, false );\n\t\tthis._container.addEventListener( 'mouseup'\t, this._$onMouseUp\t, false );\n\t\tthis._container.addEventListener( 'mousemove'\t, this._$onMouseMove\t, false );\n\t}\n}\n\nVirtualJoystick.prototype.destroy\t= function()\n{\n\tthis._container.removeChild(this._baseEl);\n\tthis._container.removeChild(this._stickEl);\n\n\tthis._container.removeEventListener( 'touchstart'\t, this._$onTouchStart\t, false );\n\tthis._container.removeEventListener( 'touchend'\t\t, this._$onTouchEnd\t, false );\n\tthis._container.removeEventListener( 'touchmove'\t, this._$onTouchMove\t, false );\n\tif( this._mouseSupport ){\n\t\tthis._container.removeEventListener( 'mouseup'\t\t, this._$onMouseUp\t, false );\n\t\tthis._container.removeEventListener( 'mousedown'\t, this._$onMouseDown\t, false );\n\t\tthis._container.removeEventListener( 'mousemove'\t, this._$onMouseMove\t, false );\n\t}\n}\n\n/**\n * @returns {Boolean} true if touchscreen is currently available, false otherwise\n*/\nVirtualJoystick.touchScreenAvailable\t= function()\n{\n\treturn 'createTouch' in document ? true : false;\n}\n\n/**\n * microevents.js - https://github.com/jeromeetienne/microevent.js\n*/\n;(function(destObj){\n\tdestObj.addEventListener\t= function(event, fct){\n\t\tif(this._events === undefined) \tthis._events\t= {};\n\t\tthis._events[event] = this._events[event]\t|| [];\n\t\tthis._events[event].push(fct);\n\t\treturn fct;\n\t};\n\tdestObj.removeEventListener\t= function(event, fct){\n\t\tif(this._events === undefined) \tthis._events\t= {};\n\t\tif( event in this._events === false  )\treturn;\n\t\tthis._events[event].splice(this._events[event].indexOf(fct), 1);\n\t};\n\tdestObj.dispatchEvent\t\t= function(event /* , args... */){\n\t\tif(this._events === undefined) \tthis._events\t= {};\n\t\tif( this._events[event] === undefined )\treturn;\n\t\tvar tmpArray\t= this._events[event].slice(); \n\t\tfor(var i = 0; i < tmpArray.length; i++){\n\t\t\tvar result\t= tmpArray[i].apply(this, Array.prototype.slice.call(arguments, 1))\n\t\t\tif( result !== undefined )\treturn result;\n\t\t}\n\t\treturn undefined\n\t};\n})(VirtualJoystick.prototype);\n\n//////////////////////////////////////////////////////////////////////////////////\n//\t\t\t\t\t\t\t\t\t\t//\n//////////////////////////////////////////////////////////////////////////////////\n\nVirtualJoystick.prototype.deltaX\t= function(){ return this._stickX - this._baseX;\t}\nVirtualJoystick.prototype.deltaY\t= function(){ return this._stickY - this._baseY;\t}\n\nVirtualJoystick.prototype.up\t= function(){\n\tif( this._pressed === false )\treturn false;\n\tvar deltaX\t= this.deltaX();\n\tvar deltaY\t= this.deltaY();\n\tif( deltaY >= 0 )\t\t\t\treturn false;\n\tif( Math.abs(deltaX) > 2*Math.abs(deltaY) )\treturn false;\n\treturn true;\n}\nVirtualJoystick.prototype.down\t= function(){\n\tif( this._pressed === false )\treturn false;\n\tvar deltaX\t= this.deltaX();\n\tvar deltaY\t= this.deltaY();\n\tif( deltaY <= 0 )\t\t\t\treturn false;\n\tif( Math.abs(deltaX) > 2*Math.abs(deltaY) )\treturn false;\n\treturn true;\t\n}\nVirtualJoystick.prototype.right\t= function(){\n\tif( this._pressed === false )\treturn false;\n\tvar deltaX\t= this.deltaX();\n\tvar deltaY\t= this.deltaY();\n\tif( deltaX <= 0 )\t\t\t\treturn false;\n\tif( Math.abs(deltaY) > 2*Math.abs(deltaX) )\treturn false;\n\treturn true;\t\n}\nVirtualJoystick.prototype.left\t= function(){\n\tif( this._pressed === false )\treturn false;\n\tvar deltaX\t= this.deltaX();\n\tvar deltaY\t= this.deltaY();\n\tif( deltaX >= 0 )\t\t\t\treturn false;\n\tif( Math.abs(deltaY) > 2*Math.abs(deltaX) )\treturn false;\n\treturn true;\t\n}\n\n//////////////////////////////////////////////////////////////////////////////////\n//\t\t\t\t\t\t\t\t\t\t//\n//////////////////////////////////////////////////////////////////////////////////\n\nVirtualJoystick.prototype._onUp\t= function()\n{\n\tthis._pressed\t= false; \n\tthis._stickEl.style.display\t= \"none\";\n\t\n\tif(this._stationaryBase == false){\t\n\t\tthis._baseEl.style.display\t= \"none\";\n\t\n\t\tthis._baseX\t= this._baseY\t= 0;\n\t\tthis._stickX\t= this._stickY\t= 0;\n\t}\n}\n\nVirtualJoystick.prototype._onDown\t= function(x, y)\n{\n\tthis._pressed\t= true; \n\tif(this._stationaryBase == false){\n\t\tthis._baseX\t= x;\n\t\tthis._baseY\t= y;\n\t\tthis._baseEl.style.display\t= \"\";\n\t\tthis._move(this._baseEl.style, (this._baseX - this._baseEl.width /2), (this._baseY - this._baseEl.height/2));\n\t}\n\t\n\tthis._stickX\t= x;\n\tthis._stickY\t= y;\n\t\n\tif(this._limitStickTravel === true){\n\t\tvar deltaX\t= this.deltaX();\n\t\tvar deltaY\t= this.deltaY();\n\t\tvar stickDistance = Math.sqrt( (deltaX * deltaX) + (deltaY * deltaY) );\n\t\tif(stickDistance > this._stickRadius){\n\t\t\tvar stickNormalizedX = deltaX / stickDistance;\n\t\t\tvar stickNormalizedY = deltaY / stickDistance;\n\t\t\t\n\t\t\tthis._stickX = stickNormalizedX * this._stickRadius + this._baseX;\n\t\t\tthis._stickY = stickNormalizedY * this._stickRadius + this._baseY;\n\t\t} \t\n\t}\n\t\n\tthis._stickEl.style.display\t= \"\";\n\tthis._move(this._stickEl.style, (this._stickX - this._stickEl.width /2), (this._stickY - this._stickEl.height/2));\t\n}\n\nVirtualJoystick.prototype._onMove\t= function(x, y)\n{\n\tif( this._pressed === true ){\n\t\tthis._stickX\t= x;\n\t\tthis._stickY\t= y;\n\t\t\n\t\tif(this._limitStickTravel === true){\n\t\t\tvar deltaX\t= this.deltaX();\n\t\t\tvar deltaY\t= this.deltaY();\n\t\t\tvar stickDistance = Math.sqrt( (deltaX * deltaX) + (deltaY * deltaY) );\n\t\t\tif(stickDistance > this._stickRadius){\n\t\t\t\tvar stickNormalizedX = deltaX / stickDistance;\n\t\t\t\tvar stickNormalizedY = deltaY / stickDistance;\n\t\t\t\n\t\t\t\tthis._stickX = stickNormalizedX * this._stickRadius + this._baseX;\n\t\t\t\tthis._stickY = stickNormalizedY * this._stickRadius + this._baseY;\n\t\t\t} \t\t\n\t\t}\n\t\t\n        \tthis._move(this._stickEl.style, (this._stickX - this._stickEl.width /2), (this._stickY - this._stickEl.height/2));\t\n\t}\t\n}\n\n\n//////////////////////////////////////////////////////////////////////////////////\n//\t\tbind touch events (and mouse events for debug)\t\t\t//\n//////////////////////////////////////////////////////////////////////////////////\n\nVirtualJoystick.prototype._onMouseUp\t= function(event)\n{\n\treturn this._onUp();\n}\n\nVirtualJoystick.prototype._onMouseDown\t= function(event)\n{\n\tevent.preventDefault();\n\tvar x\t= event.clientX;\n\tvar y\t= event.clientY;\n\treturn this._onDown(x, y);\n}\n\nVirtualJoystick.prototype._onMouseMove\t= function(event)\n{\n\tvar x\t= event.clientX;\n\tvar y\t= event.clientY;\n\treturn this._onMove(x, y);\n}\n\n//////////////////////////////////////////////////////////////////////////////////\n//\t\tcomment\t\t\t\t\t\t\t\t//\n//////////////////////////////////////////////////////////////////////////////////\n\nVirtualJoystick.prototype._onTouchStart\t= function(event)\n{\n\t// if there is already a touch inprogress do nothing\n\tif( this._touchIdx !== null )\treturn;\n\n\t// notify event for validation\n\tvar isValid\t= this.dispatchEvent('touchStartValidation', event);\n\tif( isValid === false )\treturn;\n\t\n\t// dispatch touchStart\n\tthis.dispatchEvent('touchStart', event);\n\n\tevent.preventDefault();\n\t// get the first who changed\n\tvar touch\t= event.changedTouches[0];\n\t// set the touchIdx of this joystick\n\tthis._touchIdx\t= touch.identifier;\n\n\t// forward the action\n\tvar x\t\t= touch.pageX;\n\tvar y\t\t= touch.pageY;\n\treturn this._onDown(x, y)\n}\n\nVirtualJoystick.prototype._onTouchEnd\t= function(event)\n{\n\t// if there is no touch in progress, do nothing\n\tif( this._touchIdx === null )\treturn;\n\n\t// dispatch touchEnd\n\tthis.dispatchEvent('touchEnd', event);\n\n\t// try to find our touch event\n\tvar touchList\t= event.changedTouches;\n\tfor(var i = 0; i < touchList.length && touchList[i].identifier !== this._touchIdx; i++);\n\t// if touch event isnt found, \n\tif( i === touchList.length)\treturn;\n\n\t// reset touchIdx - mark it as no-touch-in-progress\n\tthis._touchIdx\t= null;\n\n//??????\n// no preventDefault to get click event on ios\nevent.preventDefault();\n\n\treturn this._onUp()\n}\n\nVirtualJoystick.prototype._onTouchMove\t= function(event)\n{\n\t// if there is no touch in progress, do nothing\n\tif( this._touchIdx === null )\treturn;\n\n\t// try to find our touch event\n\tvar touchList\t= event.changedTouches;\n\tfor(var i = 0; i < touchList.length && touchList[i].identifier !== this._touchIdx; i++ );\n\t// if touch event with the proper identifier isnt found, do nothing\n\tif( i === touchList.length)\treturn;\n\tvar touch\t= touchList[i];\n\n\tevent.preventDefault();\n\n\tvar x\t\t= touch.pageX;\n\tvar y\t\t= touch.pageY;\n\treturn this._onMove(x, y)\n}\n\n\n//////////////////////////////////////////////////////////////////////////////////\n//\t\tbuild default stickEl and baseEl\t\t\t\t//\n//////////////////////////////////////////////////////////////////////////////////\n\n/**\n * build the canvas for joystick base\n */\nVirtualJoystick.prototype._buildJoystickBase\t= function()\n{\n\tvar canvas\t= document.createElement( 'canvas' );\n\tcanvas.width\t= 126;\n\tcanvas.height\t= 126;\n\t\n\tvar ctx\t\t= canvas.getContext('2d');\n\tctx.beginPath(); \n\tctx.strokeStyle = this._strokeStyle; \n\tctx.lineWidth\t= 6; \n\tctx.arc( canvas.width/2, canvas.width/2, 40, 0, Math.PI*2, true); \n\tctx.stroke();\t\n\n\tctx.beginPath(); \n\tctx.strokeStyle\t= this._strokeStyle; \n\tctx.lineWidth\t= 2; \n\tctx.arc( canvas.width/2, canvas.width/2, 60, 0, Math.PI*2, true); \n\tctx.stroke();\n\t\n\treturn canvas;\n}\n\n/**\n * build the canvas for joystick stick\n */\nVirtualJoystick.prototype._buildJoystickStick\t= function()\n{\n\tvar canvas\t= document.createElement( 'canvas' );\n\tcanvas.width\t= 86;\n\tcanvas.height\t= 86;\n\tvar ctx\t\t= canvas.getContext('2d');\n\tctx.beginPath(); \n\tctx.strokeStyle\t= this._strokeStyle; \n\tctx.lineWidth\t= 6; \n\tctx.arc( canvas.width/2, canvas.width/2, 40, 0, Math.PI*2, true); \n\tctx.stroke();\n\treturn canvas;\n}\n\n//////////////////////////////////////////////////////////////////////////////////\n//\t\tmove using translate3d method with fallback to translate > 'top' and 'left'\t\t\n//      modified from https://github.com/component/translate and dependents\n//////////////////////////////////////////////////////////////////////////////////\n\nVirtualJoystick.prototype._move = function(style, x, y)\n{\n\tif (this._transform) {\n\t\tif (this._has3d) {\n\t\t\tstyle[this._transform] = 'translate3d(' + x + 'px,' + y + 'px, 0)';\n\t\t} else {\n\t\t\tstyle[this._transform] = 'translate(' + x + 'px,' + y + 'px)';\n\t\t}\n\t} else {\n\t\tstyle.left = x + 'px';\n\t\tstyle.top = y + 'px';\n\t}\n}\n\nVirtualJoystick.prototype._getTransformProperty = function() \n{\n\tvar styles = [\n\t\t'webkitTransform',\n\t\t'MozTransform',\n\t\t'msTransform',\n\t\t'OTransform',\n\t\t'transform'\n\t];\n\n\tvar el = document.createElement('p');\n\tvar style;\n\n\tfor (var i = 0; i < styles.length; i++) {\n\t\tstyle = styles[i];\n\t\tif (null != el.style[style]) {\n\t\t\treturn style;\n\t\t}\n\t}         \n}\n  \nVirtualJoystick.prototype._check3D = function() \n{        \n\tvar prop = this._getTransformProperty();\n\t// IE8<= doesn't have `getComputedStyle`\n\tif (!prop || !window.getComputedStyle) return module.exports = false;\n\n\tvar map = {\n\t\twebkitTransform: '-webkit-transform',\n\t\tOTransform: '-o-transform',\n\t\tmsTransform: '-ms-transform',\n\t\tMozTransform: '-moz-transform',\n\t\ttransform: 'transform'\n\t};\n\n\t// from: https://gist.github.com/lorenzopolidori/3794226\n\tvar el = document.createElement('div');\n\tel.style[prop] = 'translate3d(1px,1px,1px)';\n\tdocument.body.insertBefore(el, null);\n\tvar val = getComputedStyle(el).getPropertyValue(map[prop]);\n\tdocument.body.removeChild(el);\n\tvar exports = null != val && val.length && 'none' != val;\n\treturn exports;\n}\n";
//Define the pins
//int STBY = 12; //standby
//Motor B
int PWMB = D4; //Speed control-d0
int BIN1 = D5; //Direction-d1
int BIN2 = D6; //Direction-d2
//Motor A
int PWMA = D3; //Speed control -D4
int AIN1 = D7; //Direction -D5
int AIN2 = D8; //Direction-D6
const int trigPin = 13;//d7 trig
const int echoPin = 15;//d8 echo
long duration;
int distance;

ESP8266WebServer server (80);
ESP8266HTTPUpdateServer httpUpdater;



void handleVirtualJoystickJS(){
  server.send_P(200,"application/javascript",virtualjoystick_js);
}
void handleJoystickHtml()
{
 server.send_P(200,"text/html",joystick_html); 
}
//This function takes the parameters passed in the URL(the x and y coordinates of the joystick)
//and sets the motor speed based on those parameters. 
  boolean yDir;
void handleJSData() {

  int x = map(server.arg(0).toInt(), -100, 100, -127, 127);
  int y = map(server.arg(1).toInt(), -100, 100, -127, 127);

  int     nMotMixL;           // Motor (left)  mixed output           (-128..+127)
  int     nMotMixR;  
  float fPivYLimit = 32.0;
      
  // TEMP VARIABLES
  float   nMotPremixL;    // Motor (left)  premixed output        (-128..+127)
  float   nMotPremixR;    // Motor (right) premixed output        (-128..+127)
  int     nPivSpeed;      // Pivot Speed                          (-128..+127)
  float   fPivScale;      // Balance scale b/w drive and pivot    (   0..1   )
  
  
  // Calculate Drive Turn output due to Joystick X input
  if (y >= 0) {
    // Forward
    nMotPremixL = (x>=0)? 127.0 : (127.0 + x);
    nMotPremixR = (x>=0)? (127.0 - x) : 127.0;
  } else {
    // Reverse
    nMotPremixL = (x>=0)? (127.0 - x) : 127.0;
    nMotPremixR = (x>=0)? 127.0 : (127.0 + x);
  }
  
  // Scale Drive output due to Joystick Y input (throttle)
  nMotPremixL = nMotPremixL * y/128.0;
  nMotPremixR = nMotPremixR * y/128.0;
  
  // Now calculate pivot amount
  // - Strength of pivot (nPivSpeed) based on Joystick X input
  // - Blending of pivot vs drive (fPivScale) based on Joystick Y input
  nPivSpeed = x;
  fPivScale = (abs(y)>fPivYLimit)? 0.0 : (1.0 - abs(y)/fPivYLimit);
  
  // Calculate final mix of Drive and Pivot
  nMotMixL = (1.0-fPivScale)*nMotPremixL + fPivScale*( nPivSpeed);
  nMotMixR = (1.0-fPivScale)*nMotPremixR + fPivScale*(-nPivSpeed);

    Serial.println();
  Serial.print("First: " );
  Serial.print(nMotMixL);
  Serial.print(" Second: ");
  Serial.print(nMotMixR);

  int finalLeft = map(nMotMixL, -127, 127, -1024, 1024);
  int finalRight = map(nMotMixR, -127, 127, -1024, 1024);

  //MotorA
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(D3, finalLeft);
  //MotorB
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(D4, finalRight);

  //return an HTTP 200
  server.send(200, "text/plain", "");   
}

void setup()
{  Serial.begin(115200);

  delay(100);

  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  delay(100);

  //set the pins as outputs
  //pinMode(STBY, OUTPUT);

  pinMode(D3, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);

  pinMode(D4, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);  
  pinMode(trigPin, OUTPUT);//Sets the trigPin as an Output
  pinMode(echoPin, INPUT);
  digitalWrite(trigPin, LOW);
  analogWriteFreq(30);

  server.on("/jsData.html", handleJSData);  
  server.on("/joystick.html",handleJoystickHtml);
  server.on("/virtualjoystick.js",handleVirtualJoystickJS);
  server.on("/", handleJoystickHtml);
  httpUpdater.setup(&server);
  server.begin();
  MDNS.begin(host);
  delay(500);    
}

void loop()
{  
  server.handleClient();  
  
}
