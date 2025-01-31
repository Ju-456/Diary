// 1 infrared sensor, 0 for "empty" and 1 else

#define SENSOR_PIN 14  

void setup() {
    Serial.begin(115200);
}

void loop() {
    int sensorValue = analogRead(SENSOR_PIN) > 4000 ? 0 : 1;

    /* print real value if we need to test
    int sensorValue = analogRead(SENSOR_PIN); 
    */

    Serial.println(sensorValue);

    delay(100);  
}
