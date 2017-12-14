/****************************
The MIT License (MIT)
Copyright (c) 2015 by bbx10node@gmail.com
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************/

/**** Following adapted from initial code by bbx10node@gmail.com with included license above 
for LED Weather Cube *****/

#include <ESP8266WiFi.h>
#ifndef min
#define min(x,y) (((x)<(y))?(x):(y))
#endif
#ifndef max
#define max(x,y) (((x)>(y))?(x):(y))
#endif
#include <ArduinoJson.h>

//WiFi SSID and Password, change for access to different networks
const char SSID[]     = "iPhone"; //your type of interent connection
const char PASSWORD[] = ""; //your password

//API key from http://www.wunderground.com/weather/api/
#define WU_API_KEY "6b6c4a265b7eb243"

//Initial 30 min delay between update checks, no longer active
#define DELAY_NORMAL    (30*60*1000)
// 20 minute delay between updates after an error
#define DELAY_ERROR     (20*60*1000)

//Host definition for Weather Underground API
#define WUNDERGROUND "api.wunderground.com"

//HTTP request
const char WUNDERGROUND_REQ[] =
    "GET /api/6b6c4a265b7eb243/conditions/q/CO/Boulder.json HTTP/1.1\r\n"
    "User-Agent: ESP8266/0.1\r\n"
    "Accept: */*\r\n"
    "Host: " WUNDERGROUND "\r\n"
    "Connection: close\r\n"
    "\r\n";

//LED Pin Variables
int sunPin = 16;
int cloudPin = 12;
int rainPin = 13;
int tstormPin = 4;
int snowPin = 0;

//Initial variable to hold weather condition data
const char *w = " ";

//String version of above weather data, for string compare
String weather = " ";

//Boolean switch for calling API, true at first and then in 20 min intervals
bool fetch = true;

//Boolean switch for setting up WiFi, true when turned on, otherwise false
bool wifi = true;

void setup(){
  
  Serial.begin(9600);

  //LED Pin Set Up
  pinMode(sunPin, OUTPUT);
  pinMode(cloudPin, OUTPUT);
  pinMode(rainPin, OUTPUT);
  pinMode(tstormPin, OUTPUT);
  pinMode(snowPin, OUTPUT);

  //set up WiFi if not been set up yet
  if (wifi){
  //Connect to WiFi Network
  Serial.println();
  Serial.println();
  Serial.print(F("Connecting to "));
  Serial.println(SSID);

  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }

  Serial.println(wifi);
  Serial.println();
  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());

  wifi = false;
  Serial.println(wifi);
  }
}

//Variable for holding JSON data
static char respBuf[4096];

void loop(){

  //Call to API if not been called yet or if over 20 minutes since last call
  if(fetch){
    Serial.println("Fetching weather...");
    getWeather();
  }

  //Print out weather
   Serial.print("weather is: ");
   Serial.println(weather);

   //Option 1: sun
  if (weather == "Clear"){
    Serial.println("sun");
    //Turn off other LEDs
    digitalWrite(cloudPin, LOW);
    digitalWrite(rainPin, LOW);
    digitalWrite(tstormPin, LOW);
    digitalWrite(snowPin, LOW);

   //Turn on correct LED
    digitalWrite(sunPin, HIGH);
    delay(1200000);
    fetch = true;
  }

  //Option 2: cloudy
  else if (weather == "Light Mist" || weather == "Heavy Mist" || weather == "Mist" || weather == "Light Fog" || weather == "Heavy Fog" || weather == "Fog" || weather == "Light Fog Patches" || weather == "Heavy Fog Patches" || weather == "Fog Patches" || weather == "Light Smoke" || weather == "Heavy Smoke" || weather == "Smoke" || weather == "Light Volcanic Ash" || weather == "Heavy Volcanic Ash" || weather == "Volcanic Ash" || weather == "Light Widespread Dust" || weather == "Heavy Widespread Dust" || weather == "Widespread Dust" || weather == "Light Sand" || weather == "Heavy Sand" || weather == "Sand" || weather == "Light Haze" || weather == "Heavy Haze" || weather == "Haze" || weather == "Light Dust Whirls" || weather == "Heavy Dust Whirls" || weather == "Dust Whirls" || weather == "Light Sandstorm" || weather == "Heavy Sandstorm" || weather == "Sandstorm" || weather == "Light Low Drifting Widespread Dust" || weather == "Heavy Low Drifting Widespread Dust" || weather == "Low Drifting Widespread Dust" || weather == "Light Low Drifting Sand" || weather == "Heavy Low Drifting Sand" || weather == "Low Drifting Sand" || weather == "Light Blowing Widespread Dust" || weather == "Heavy Blowing Widespread Dust" || weather == "Blowing Widespread Dust" || weather == "Light Blowing Sand" || weather == "Heavy Blowing Sand" || weather == "Blowing Sand" || weather == "Light Freezing Fog" || weather == "Heavy Freezing Fog" || weather == "Freezing Fog" || weather == "Patches of Fog" || weather == "Shallow Fog" || weather == "Partial Fog" || weather == "Overcast" || weather == "Partly Cloudy" || weather == "Mostly Cloudy" || weather == "Scattered Clouds" || weather == "Funnel Cloud"){
    Serial.println("cloud");
    //Turn off other LEDs
    digitalWrite(sunPin, LOW);
    digitalWrite(rainPin, LOW);
    digitalWrite(tstormPin, LOW);
    digitalWrite(snowPin, LOW);

    //Turn on correct LED
    digitalWrite(cloudPin, HIGH);
    delay(1200000);
    fetch = true;
  }
  //Option 3: rain
  else if (weather == "Light Drizzle" || weather == "Heavy Drizzle" || weather == "Drizzle" || weather == "Light Rain" || weather == "Heavy Rain" || weather == "Rain" || weather == "Light Rain Mist" || weather == "Heavy Rain Mist" || weather == "Rain Mist" || weather == "Light Rain Showers" || weather == "Heavy Rain Showers" || weather == "Rain Showers" || weather == "Light Snow Showers" || weather == "Heavy Snow Showers" || weather == "Snow Showers" || weather == "Light Hail Showers" || weather == "Heavy Hail Showers" || weather == "Hail Showers" || weather == "Light Small Hail Showers" || weather == "Heavy Small Hail Showers" || weather == "Small Hail Showers" || weather == "Light Ice Pellet Showers" || weather == "Heavy Ice Pellet Showers" || weather == "Ice Pellet Showers" || weather == "Light Freezing Drizzle" || weather == "Heavy Freezing Drizzle" || weather == "Freezing Drizzle" || weather == "Light Freezing Rain" || weather == "Heavy Freezing Rain" || weather == "Freezing Rain" || weather == "Small Hail" || weather == "Squalls" || weather == "Unknown Precipitation" || weather == "Light Spray" || weather == "Heavy Spray" || weather == "Spray"){
    Serial.println("rain");
    //Turn off other LEDs
    digitalWrite(sunPin, LOW);
    digitalWrite(cloudPin, LOW);
    digitalWrite(tstormPin, LOW);
    digitalWrite(snowPin, LOW);

   //Turn on correct LED
    digitalWrite(rainPin, HIGH);
    delay(1200000);
    fetch = true;
  }
  //Option 4: thunderstorm
  else if (weather == "Light Thunderstorm" || weather == "Heavy Thunderstorm" || weather == "Thunderstorm" || weather == "Light Thunderstorms and Rain" || weather == "Heavy Thunderstorms and Rain" || weather == "Thunderstorms and Rain" || weather == "Light Thunderstorms and Snow" || weather == "Heavy Thunderstorms and Snow" || weather == "Thunderstorms and Snow" || weather == "Light Thunderstorms and Ice Pellets" || weather == "Heavy Thunderstorms and Ice Pellets" || weather == "Thunderstorms and Ice Pellets" || weather == "Light Thunderstorms with Hail" || weather == "Heavy Thunderstorms with Hail" || weather == "Thunderstorms with Hail" || weather == "Light Thunderstorms with Small Hail" || weather == "Heavy Thunderstorms with Small Hail" || weather == "Thunderstorms with Small Hail"){
    Serial.println("tstorm");
    //Turn off other LEDs
    digitalWrite(sunPin, LOW);
    digitalWrite(cloudPin, LOW);
    digitalWrite(rainPin, LOW);
    digitalWrite(snowPin, LOW);

   //Turn on correct LED
    digitalWrite(tstormPin, HIGH);
    delay(1200000);
    fetch = true;
  }
  //Option 5: snow
  else if (weather == "Light Snow" || weather == "Heavy Snow" || weather == "Snow" || weather == "Light Snow Grains" || weather == "Heavy Snow Grains" || weather == "Snow Grains" || weather == "Light Ice Crystals" || weather == "Heavy Ice Crystals" || weather == "Ice Crystals" || weather == "Light Ice Pellets" || weather == "Heavy Ice Pellets" || weather == "Ice Pellets" || weather == "Light Hail" || weather == "Heavy Hail" || weather == "Hail" || weather == "Light Low Drifting Snow" || weather == "Heavy Low Drifting Snow" || weather == "Low Drifting Snow" || weather == "Light Blowing Snow" || weather == "Heavy Blowing Snow" || weather == "Blowing Snow" || weather == "Light Snow Blowing Snow Mist" || weather == "Heavy Snow Blowing Snow Mist" || weather == "Snow Blowing Snow Mist"){
    Serial.println("snow");
    //Turn off other LEDs
    digitalWrite(sunPin, LOW);
    digitalWrite(cloudPin, LOW);
    digitalWrite(rainPin, LOW);
    digitalWrite(tstormPin, LOW);

   //Turn on correct LED
    digitalWrite(snowPin, HIGH);
    delay(1200000);
    fetch = true;
  }
}

//Function to call API with request defined earlier
void getWeather(){

  //Set fetch switch off
  fetch = false;
  Serial.print("In getWeather function... ");
  Serial.println(fetch);
  
  //Open socket to WU server port 80
  Serial.print(F("Connecting to "));
  Serial.println(WUNDERGROUND);

  //Use WiFiClient class to create TCP connections
  WiFiClient httpclient;
  const int httpPort = 80;
  if (!httpclient.connect(WUNDERGROUND, httpPort)) {
    Serial.println(F("connection failed"));
    delay(DELAY_ERROR);
    return;
  }

  //This will send the http request to the server
  Serial.print(WUNDERGROUND_REQ);
  Serial.print("Sending http request to server...");
  httpclient.print(WUNDERGROUND_REQ);
  httpclient.flush();

  //Collect http response headers and content from Weather Underground
  //HTTP headers are discarded.
  //The content is formatted in JSON and is left in respBuf.
  int respLen = 0;
  bool skip_headers = true;
  while (httpclient.connected() || httpclient.available()) {
    if (skip_headers) {
      String aLine = httpclient.readStringUntil('\n');
      //Serial.println(aLine);
      // Blank line denotes end of headers
      if (aLine.length() <= 1) {
        skip_headers = false;
      }
    }
    else {
      int bytesIn;
      bytesIn = httpclient.read((uint8_t *)&respBuf[respLen], sizeof(respBuf) - respLen);
      Serial.print(F("bytesIn ")); 
      Serial.println(bytesIn);
      if (bytesIn > 0) {
        respLen += bytesIn;
        if (respLen > sizeof(respBuf)) respLen = sizeof(respBuf);
      }
      else if (bytesIn < 0) {
        Serial.print(F("read error "));
        Serial.println(bytesIn);
      }
    }
    delay(1);
  }
  httpclient.stop();

  if (respLen >= sizeof(respBuf)) {
    //Serial.print(F("respBuf overflow "));
    //Serial.println(respLen);
    delay(DELAY_ERROR);
    return;
  }
  // Terminate the C string
  respBuf[respLen++] = '\0';
  //Serial.print(F("respLen "));
  //Serial.println(respLen);
  //Serial.println(respBuf);

  //Call function to parse data
  showWeather(respBuf);
}

//Function for parsing and categorizing JSON data from API call
bool showWeather(char *json){
  Serial.print("In showWeather function...");
  StaticJsonBuffer<3*1024> jsonBuffer;

  //Skip characters until first '{' found
  //Ignore chunked length, if present
  char *jsonstart = strchr(json, '{');
  //Serial.print(F("jsonstart ")); Serial.println(jsonstart);
  if (jsonstart == NULL) {
    Serial.print("jsonstart = NULL");
    Serial.println(F("JSON data missing"));
    return false;
  }
  json = jsonstart;

  //Parse JSON
  JsonObject& root = jsonBuffer.parseObject(json);
  if (!root.success()) {
    Serial.println(F("jsonBuffer.parseObject() failed"));
    return false;
  }

  //Extract weather info from parsed JSON
  JsonObject& current = root["current_observation"];
  const float temp_f = current["temp_f"];
  Serial.print(temp_f, 1); Serial.print(F(" F, "));
  const float temp_c = current["temp_c"];
  Serial.print(temp_c, 1); Serial.print(F(" C, "));
  const char *humi = current[F("relative_humidity")];
  Serial.print(humi);   Serial.println(F(" RH"));
  //First assign weather value to w
  w = current["weather"];
  //Print w to make it a string
  Serial.println(w);
  //Save string to weather
  weather = w;
  const char *pressure_mb = current["pressure_mb"];
  Serial.println(pressure_mb);
  const char *observation_time = current["observation_time_rfc822"];
  Serial.println(observation_time);

  //Extract local timezone fields
  const char *local_tz_short = current["local_tz_short"];
  Serial.println(local_tz_short);
  const char *local_tz_long = current["local_tz_long"];
  Serial.println(local_tz_long);
  const char *local_tz_offset = current["local_tz_offset"];
  Serial.println(local_tz_offset);
  return true;
}
