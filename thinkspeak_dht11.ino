#include <DHT.h>
#include <ESP8266WiFi.h>

String apiKey = "xxxxx";      //  ThingSpeak API key

const char *ssid =  "xxxxxx"; // Wifi ssid and password
const char *pass =  "xxxxxx";
const char* server = "api.thingspeak.com";

#define DHTPIN 0              //pin where the dht11 is connected
DHT dht(DHTPIN, DHT11);

WiFiClient client;

void setup()
{
      Serial.begin(115200);
      delay(10);
      dht.begin();

      Serial.println("Connecting to ");
      Serial.println(ssid);


      WiFi.begin(ssid, pass);
      while (WiFi.status() != WL_CONNECTED)
      {
            delay(500);
            Serial.print(".");
      }
      Serial.println("");
      Serial.println("WiFi connected");
      pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
      float h = dht.readHumidity();
      float t = dht.readTemperature();

      if (isnan(h) || isnan(t)){
            Serial.println("Failed to read from DHT sensor!");
            return;
      }

      // "184.106.153.149" or api.thingspeak.com
      if (client.connect(server,80)){
            String postStr = apiKey;
            postStr +="&field1=";
            postStr += String(t);
            postStr +="&field2=";
            postStr += String(h);
            postStr += "\r\n\r\n";

            client.print("POST /update HTTP/1.1\n");
            client.print("Host: api.thingspeak.com\n");
            client.print("Connection: close\n");
            client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
            client.print("Content-Type: application/x-www-form-urlencoded\n");
            client.print("Content-Length: ");
            client.print(postStr.length());
            client.print("\n\n");
            client.print(postStr);

            Serial.print("Temperature: ");
            Serial.print(t);
            Serial.print(" degrees Celcius, Humidity: ");
            Serial.print(h);
            Serial.println("%. Send to Thingspeak.");
            digitalWrite(LED_BUILTIN, HIGH);
      }

      client.stop();
      Serial.println("Waiting...");
      delay(10000*2); // delay for 60 sec (thingspeak needs minimum 15 sec)
      digitalWrite(LED_BUILTIN, LOW);
}
