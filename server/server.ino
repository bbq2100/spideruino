/* Create a WIFI access point and provide a REST API for controlling the hexapod. */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Servo.h>

const char *ssid = "The cake is a lie.";

ESP8266WebServer server(80);

Servo servoLeft, servoMid, servoRight;

long posLeft = 90, posMid = 90, posRight = 90;

int forward[4][3] = {{70, 95, 60}, {110, 93, 100}, {110, 83, 100}, {80, 85, 70}};
int backward[4][3] = {{70, 95, 60}, {80, 85, 70}, {110, 83, 100}, {110, 93, 100}};
int left[4][3] = {{115, 95, 60}, {100, 98, 100}, {100, 80, 100}, {115, 90, 60}};
int right[4][3] = {{115, 82, 70}, {90, 82, 110}, {90, 95, 110}, {116, 97, 75}};

void handleRoot() {
	server.send(200, "text/html", "<h1>You are connected</h1>");
}

void setup() {
  Serial.begin(115200);

  servoLeft.attach(13);
  servoMid.attach(12);
  servoRight.attach(14);

	Serial.println();
	Serial.print("Configuring access point...");

	WiFi.softAP(ssid);

	IPAddress myIP = WiFi.softAPIP();
	Serial.print("AP IP address: ");
	Serial.println(myIP);
	server.on("/", handleRoot);
  server.on("/position", handlePosition);
  server.on("/forward", handleForward);
  server.on("/backward", handleBackward);
  server.on("/left", handleLeft);
  server.on("/right", handleRight);
  server.on("/reset", handleReset);
	server.begin();
	Serial.println("HTTP server started. Waiting for client requests...");
}

void loop() {
	server.handleClient();
}

void run(int left, int mid, int right) {
  servoLeft.write(left);
  servoMid.write(mid);
  servoRight.write(right);
}

void resetPosition() {
  run(90, 90, 90);
  delay(150);
}

void handlePosition() {
  int left = server.arg("left").toInt();
  int mid = server.arg("mid").toInt();
  int right = server.arg("right").toInt();

  run(left, mid, right);

  responseOK();
}

void handleForward() {
  handleMovement(forward);
}

void handleBackward() {
  handleMovement(backward);
}

void handleLeft() {
  handleMovement(left);
}

void handleRight() {
  handleMovement(right);
}

void handleReset() {
  resetPosition();
  server.send(200, "text/plain", "all good");
}

void handleMovement(int coords[4][3]) {
  resetPosition();

  for (int i = 0; i <= 3; i++) {
    int left = coords[i][0];
    int mid = coords[i][1];
    int right = coords[i][2];

    run(left, mid, right);

    delay(150);
  }
  responseOK();
}

void responseOK() {
  server.send(200, "text/plain", "all good");
}
