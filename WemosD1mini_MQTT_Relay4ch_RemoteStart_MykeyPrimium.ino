//Wemos D1 Mini with 4ch Relay and MQTT
//Edit by Diego Na <- it's me

#include <PubSubClient.h> // MQTT library
#include <ESP8266WiFi.h> // ESP8266 WIFI library

//EDIT THESE LINES TO MATCH YOUR SETUP

#define wifi_ssid "your lte router's SSID"
#define wifi_pass "your lte router's Password"

#define mqtt_server "your mqtt server address"  //MQTT IP Address
#define mqtt_user "your mqtt id" 
#define mqtt_password "your mqtt password"
#define mqtt_port "your mqtt port --> 1883"
#define mqtt_clientID "your mqtt clientID - any code or text"
#define switchTopic "/sonata/remote/"

#define switchPin1 D1
#define switchPin2 D3
#define switchPin3 D5
#define switchPin4 D7

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(wifi_ssid, wifi_pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {                 //initialize the switch as an output and set to LOW (off)
  pinMode(switchPin1, OUTPUT); // Relay Switch 1
  pinMode(switchPin2, OUTPUT); // Relay Switch 2
  pinMode(switchPin3, OUTPUT); // Relay Switch 3
  pinMode(switchPin4, OUTPUT); // Relay Switch 4
    
  digitalWrite(switchPin1, HIGH);
  digitalWrite(switchPin2, HIGH);
  digitalWrite(switchPin3, HIGH);
  digitalWrite(switchPin4, HIGH);

  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

}

void loop(){
  if (!client.connected()) {
    reconnect();
  }

  client.loop(); //maintain MQTT connection

  delay(10);   //MUST delay to allow ESP8266 WIFI functions to run
}


void callback(char* topic, byte* payload, unsigned int length) {
 
//  Serial.println("Callback update.");
//  Serial.print("Topic : ");
//  Serial.println(topicStr);

   if (topic, switchTopic) 
    {

     if (payload[0] == 'R'){
       client.publish("/sonata/notify/", "모든 릴레이 끄는중");
       digitalWrite(switchPin1, HIGH);
       digitalWrite(switchPin2, HIGH);
       digitalWrite(switchPin3, HIGH);
       digitalWrite(switchPin4, HIGH);
       delay(2000);
       client.publish("/sonata/notify/", "모든 릴레이가 꺼졌습니다");
       }
       
     else if(payload[0] == 'L'){
       client.publish("/sonata/notify/", "도어 잠금중");
       digitalWrite(switchPin1, LOW); // 배터리 전원인가
       delay(3000);
       digitalWrite(switchPin2, LOW); // 락버튼 1회 누름
       delay(1000);
       digitalWrite(switchPin2, HIGH); // 락버튼 뗌
       delay(4000);
       digitalWrite(switchPin1, HIGH); // 배터리 전원끊음
       delay(1000);
       client.publish("/sonata/notify/", "도어잠금 상태입니다.");
       }
       
     else if(payload[0] == 'U'){
       client.publish("/sonata/notify/", "도어 잠금해제중");
       digitalWrite(switchPin1, LOW); // 배터리 전원인가
       delay(3000);
       digitalWrite(switchPin3, LOW); // 언락버튼 1회 누름
       delay(1000);
       digitalWrite(switchPin3, HIGH); // 언락버튼 뗌
       delay(4000);
       digitalWrite(switchPin1, HIGH); // 배터리 전원끊음
       delay(1000);
       client.publish("/sonata/notify/", "도어잠금 해제 상태입니다.");
       }

     else if(payload[0] == 'T'){
       client.publish("/sonata/notify/", "트렁크 여는중");
       digitalWrite(switchPin1, LOW); // 배터리 전원인가
       delay(3000);
       digitalWrite(switchPin4, LOW); // 트렁크버튼 1회 길게누름
       delay(5000);
       digitalWrite(switchPin4, HIGH); // 트렁크버튼 뗌
       delay(4000);
       digitalWrite(switchPin1, HIGH); // 배터리 전원끊음
       delay(1000);
       client.publish("/sonata/notify/", "트렁크 열렸습니다.");
       }

      else if(payload[0] == 'S'){
       client.publish("/sonata/notify/", "원격시동중");
       digitalWrite(switchPin1, LOW); // 배터리 전원인가
       delay(3000);
       digitalWrite(switchPin2, LOW); // 락버튼 1회 누름
       delay(1000);
       digitalWrite(switchPin2, HIGH); // 락버튼 뗌
       delay(1000);
       digitalWrite(switchPin2, LOW); // 락버튼 1회 누름
       delay(1000);
       digitalWrite(switchPin2, HIGH); // 락버튼 뗌
       delay(4000);
       digitalWrite(switchPin1, HIGH); // 배터리 전원끊음
       delay(1000);
       client.publish("/sonata/notify/", "원격시동 완료");
       }
       
     else if (payload[0] == 'V'){  
       client.publish("/sonata/notify/", "전압측정 시작");
       int sensorValue = analogRead(A0);   // read the input on analog pin 0 
       float voltage = sensorValue * (18.2 / 1024 * 1);   // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 18.2V)
       //Serial.print("차량전압 : ");
       //Serial.print(voltage);   // print out the value you read
       //Serial.println("V");
       delay(1000);
       client.publish("/sonata/notify/",  (char*) String(voltage,1).c_str());
       }
       
     else {
    //   Serial.println("뭐하냐");
       client.publish("/sonata/notify/", "누구냐?");
      }
   }
}


void reconnect() {

    while (!client.connected()) { // Loop until we're reconnected to the MQTT server
      Serial.println("Attempting MQTT connection...");

      if (client.connect(mqtt_clientID, mqtt_user, mqtt_password)) {
        Serial.println("MQTT Connected");
        client.publish("/sonata/alarm/", "MQTT Connected");
        client.subscribe(switchTopic);
        }
        
      else {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        delay(5000);  // Wait 5 seconds before retrying
        }
    }
}
