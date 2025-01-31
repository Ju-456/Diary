// 3 infrared sensors, 0 for "empty" and 1 else

#define SENSOR_PIN_1 14  
#define SENSOR_PIN_2 27 
#define SENSOR_PIN_3 26  

void setup() {
    Serial.begin(115200);
}

void loop() {
    int sensorValue1 = analogRead(SENSOR_PIN_1) > 4000 ? 0 : 1;
    int sensorValue2 = analogRead(SENSOR_PIN_2) > 4000 ? 0 : 1;
    int sensorValue3 = analogRead(SENSOR_PIN_3) > 4000 ? 0 : 1;

    /* print reals values if we need to test
    int sensorValue1 = analogRead(SENSOR_PIN_1); 
    int sensorValue2 = analogRead(SENSOR_PIN_2); 
    int sensorValue3 = analogRead(SENSOR_PIN_3);
    */

    Serial.print(sensorValue1);
    Serial.print("\t");
    Serial.print(sensorValue2);
    Serial.print("\t");
    Serial.println(sensorValue3);

    delay(100);  
}