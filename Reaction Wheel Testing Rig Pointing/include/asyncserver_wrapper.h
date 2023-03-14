#pragma once

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

// https://randomnerdtutorials.com/esp32-async-web-server-espasyncwebserver-library/
// https://www.w3schools.com/jsref/event_onsubmit.asp
// https://developer.mozilla.org/en-US/docs/Web/API/XMLHttpRequest
// https://developer.mozilla.org/en-US/docs/Web/API/HTMLFormElement/elements

struct GainStruct
{
	float P;
	float I;
	float D;
};

GainStruct gains;

String outputState(int output);

// Replace with your network credentials
const char *ssid = "ESP32-Access-Point";
const char *password = "VIP-2022";

const char *PARAM_INPUT_1 = "P";
const char *PARAM_INPUT_2 = "I";
const char *PARAM_INPUT_3 = "D";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 3.0rem;}
    p {font-size: 3.0rem;}
    body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
    .switch {position: relative; display: inline-block; width: 120px; height: 68px} 
    .switch input {display: none}
    .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 6px}
    .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 3px}
    input:checked+.slider {background-color: #b30000}
    input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
  </style>
</head>
<body>
  <h2>ESP Web Server</h2>
  %BUTTONPLACEHOLDER%
<script>function handleSubmit(element) {
  inputs = element.elements;
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/update?P="+inputs["P"].value+"&I="+inputs["I"].value+"&D="+inputs["D"].value+"/", true)
  xhr.send();
}
</script>
</body>
</html>
)rawliteral";

// Replaces placeholder with button section in your web page
String processor(const String &var)
{
	// Serial.println(var);
	if (var == "BUTTONPLACEHOLDER")
	{
		String form = "";
		form += "<form onsubmit=\"handleSubmit(this)\">";
		form += "<h4>P - Gain</h4><input type=\"text\" name=\"P\" value=\"" + String(gains.P, 4) + "\">";
		form += "<h4>I - Gain</h4><input type=\"text\" name=\"I\" value=\"" + String(gains.I, 4) + "\">";
		form += "<h4>D - Gain</h4><input type=\"text\" name=\"D\" value=\"" + String(gains.D, 4) + "\">";
		form += "<br><input type=\"submit\"></form>";
		return form;
	}
	return String();
}

namespace asyncserver
{
	bool receivedData = false;

	void setup()
	{
		// Set access point
		Serial.print("Setting AP (Access Point)...");
		WiFi.softAP(ssid, password);

		// Print IP address
		IPAddress IP = WiFi.softAPIP();
		Serial.print("AP IP address: ");
		Serial.println(IP);

		// Route for root / web page
		server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
				  { request->send_P(200, "text/html", index_html, processor); });

		// Send a GET request to <ESP_IP>/update?...
		server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request)
				  {
    		String P;
   			String I;
			String D;
   			// GET input values on <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
			if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2) && request->hasParam(PARAM_INPUT_3)) {
			P = request->getParam(PARAM_INPUT_1)->value();
			I = request->getParam(PARAM_INPUT_2)->value();
			D = request->getParam(PARAM_INPUT_3)->value();
			gains.P = P.toFloat();
			gains.I = I.toFloat();
			gains.D = D.toFloat();

			// indicate that new data received
			receivedData = true;

			}
			else {
			Serial.println("No data received");
			}
			request->send(200, "text/plain", "OK"); }); // Start server

		server.begin();
	}

	bool hasData()
	{
		if(receivedData)
		{
			receivedData = false;
			return true;
		}
		return false;
	}

	float getP()
	{
		return gains.P;
	}

	float getI()
	{
		return gains.I;
	}

	float getD()
	{
		return gains.D;
	}

	void printGains()
	{
		Serial.println(String(gains.P) + " " + String(gains.I) + " " + String(gains.D));
	}

};