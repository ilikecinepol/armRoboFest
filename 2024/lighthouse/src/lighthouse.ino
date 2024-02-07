#include <PIDController.h>
#include <Ultrasonic.h>


volatile long int encoder_pos = 0;
PIDController pos_pid;
int motor_value = 255;
unsigned int integerValue = 0;  // Max value is 65535
char incomingByte;
#define PIN_POT A0
int ena = 5;
int in1 = 4;
int in2 = 6;
int hall1 = 2;
int hall2 = 3;
int trigPin = 9;
int echoPin = 8;
int mode = 0;
int count = 0;
int openPos = 15000;
int closePos = 0;
Ultrasonic ultrasonic(trigPin, echoPin);

void setup() {

  Serial.begin(9600);
  pinMode(hall1, INPUT);
  pinMode(hall2, INPUT);

  pinMode(ena, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  // pinMode(trigPin, OUTPUT);
  // pinMode(echoPin, INPUT);


  attachInterrupt(digitalPinToInterrupt(2), encoder, RISING);

  pos_pid.begin();
  pos_pid.tune(150, 0, 2000);
  pos_pid.limit(-255, 255);
}

void loop() {
  int dist = ultrasonic.read();
  if (analogRead(PIN_POT) < 500) {
    mode = 0;  // автоматический режим
  } else {
    mode = 1;  // ручной режим
  }
  switch (mode) {
    case 0:
      Serial.print(dist);  // выводим расстояние в сантиметрах
      Serial.println(" cm");
      analogWrite(ena, 0);
      if (dist < 10) {
        pos_pid.setpoint(openPos);
        while ((abs(encoder_pos - openPos)) > 10) {
          motor_value = pos_pid.compute(encoder_pos);
          if (motor_value > 0) {
            MotorCounterClockwise(motor_value);
          } else {
            MotorClockwise(abs(motor_value));
          }
          Serial.println(encoder_pos);
          delay(10);
        }
      }
      break;
    case 1:
      pos_pid.setpoint(closePos);
      motor_value = pos_pid.compute(encoder_pos);
      if (motor_value > 0) {
        MotorCounterClockwise(motor_value);
      } else {
        MotorClockwise(abs(motor_value));
      }
      Serial.println(encoder_pos);
      delay(10);

      break;
  }



  //Serial.println(mode);
  /*
  if (Serial.available() > 0) {
    integerValue = 0;
    while (1) {
      incomingByte = Serial.read();
      if (incomingByte == '\n') break;
      if (incomingByte == -1) continue;
      integerValue *= 10;
      integerValue = ((incomingByte - 48) + integerValue);
      pos_pid.setpoint(integerValue);
    }
  }

  motor_value = pos_pid.compute(encoder_pos);
  if (motor_value > 0) {
    MotorCounterClockwise(motor_value);
  } else {
    MotorClockwise(abs(motor_value));
  }
  Serial.println(encoder_pos);
  delay(10);

  int rotat;
  rotat = analogRead(PIN_POT) / 4;
  */
}
void MotorCounterClockwise(int power) {
  if (power > 100) {
    digitalWrite(in2, LOW);
    digitalWrite(in1, HIGH);
    analogWrite(ena, power);
  } else {
    digitalWrite(ena, LOW);
  }
}

void MotorClockwise(int power) {
  if (power > 200) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    analogWrite(ena, power);
  } else {
    digitalWrite(ena, LOW);
  }
}

void encoder() {

  if (digitalRead(hall2) == LOW) {
    encoder_pos++;
  } else {
    encoder_pos--;
  }
}
