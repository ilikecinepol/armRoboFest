
/*
 Выводы, отвечающие за скорость вращения двигателей
Цифровой вывод 11- DC Мотор №1 / Шаговый №1
Цифровой вывод 3- DC Мотор №2 / Шаговый №1
Цифровой вывод 5- DC Мотор №3 / Шаговый №2
Цифровой вывод 6- DC Мотор №4 / Шаговый №2

Выводы, отвечающие за выбор направления вращения двигателей:
Цифровые выводы 4, 7, 8 и 12

Выводы для управления сервоприводами (выведены на штырьки на краю платы):
Цифровой вывод 9- Сервопривод №1
Цифровой вывод 10- Сервопривод №2


 5-9-10-2-13-gnd
В итоге незадействованными цифровыми выводами остаются только пины 2, 13 и пины интерфейса UART- 0, 1. 
Однако есть выход из данной ситуации. У нас остались незадействованные аналоговые входы A0-A6, их можно использовать как цифровые. В коде они будут записываться как цифровые с 14 по 19.
 * */


#include <PS2X_lib.h>  //for v1.6
#include <Servo.h>
#include <Wire.h>

#include <AFMotor.h>   // подключаем библиотеку для шилда
AF_DCMotor motor1(1);  // подключаем мотор к клеммникам M1
AF_DCMotor motor2(2);  // подключаем мотор к клеммникам M2
AF_DCMotor motor3(3);  // подключаем мотор к клеммникам M4
AF_DCMotor motor4(4);  // подключаем мотор к клеммникам M4

//PS2手柄引脚；
#define PS2_DAT 14
#define PS2_CMD 15
#define PS2_SEL 16
#define PS2_CLK 17

#define servoPin 9
#define in1 2
#define in2 13

#define closeAngle 90
#define openAngle 0
#define flagTime 10

Servo servo;

#define pressures true
//#define pressures false
#define rumble true
//#define rumble false

PS2X ps2x;  // create PS2 Controller Class

int error = 0;
byte type = 0;
byte vibrate = 1;
int speed;  //小车速度




void setup() {
  pinMode(PS2_DAT, OUTPUT);
  pinMode(PS2_CMD, OUTPUT);
  pinMode(PS2_SEL, OUTPUT);
  pinMode(PS2_CLK, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  Serial.begin(57600);
  servo.attach(servoPin);
  delay(300);

  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);

  if (error == 0) {
    Serial.print("Found Controller, configured successful ");
    Serial.print("pressures = ");
    if (pressures)
      Serial.println("true ");
    else
      Serial.println("false");
    Serial.print("rumble = ");
    if (rumble)
      Serial.println("true)");
    else
      Serial.println("false");
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
    Serial.println("holding L1 or R1 will print out the analog stick values.");
    Serial.println("Note: Go to www.billporter.info for updates and to report bugs.");
  } else if (error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");

  else if (error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

  else if (error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");

  // Serial.print(ps2x.Analog(1), HEX);

  type = ps2x.readType();
  switch (type) {
    case 0:
      Serial.print("Unknown Controller type found ");
      break;
    case 1:
      Serial.print("DualShock Controller found ");
      break;
    case 2:
      Serial.print("GuitarHero Controller found ");
      break;
    case 3:
      Serial.print("Wireless Sony DualShock Controller found ");
      break;

      error = 0;

      type = 1;
  }
}

//定义小车运动方式

void turnLeft(int speed)  //左转
{
  motor1.setSpeed(speed);
  motor1.run(BACKWARD);
  motor2.setSpeed(speed);
  motor2.run(FORWARD);
  motor3.setSpeed(speed);
  motor3.run(BACKWARD);
  motor4.setSpeed(speed);
  motor4.run(FORWARD);
}

void turnRight(int speed)  //右转
{
  motor1.setSpeed(speed);
  motor1.run(FORWARD);
  motor2.setSpeed(speed);
  motor2.run(BACKWARD);
  motor3.setSpeed(speed);
  motor3.run(FORWARD);
  motor4.setSpeed(speed);
  motor4.run(BACKWARD);
}

void forward(int speed)  //前进
{
  motor1.setSpeed(speed);
  motor1.run(FORWARD);
  motor2.setSpeed(speed);
  motor2.run(FORWARD);
  motor3.setSpeed(speed);
  motor3.run(FORWARD);
  motor4.setSpeed(speed);
  motor4.run(FORWARD);
}


void back(int speed)  //后退
{
  motor1.setSpeed(speed);
  motor1.run(BACKWARD);
  motor2.setSpeed(speed);
  motor2.run(BACKWARD);
  motor3.setSpeed(speed);
  motor3.run(BACKWARD);
  motor4.setSpeed(speed);
  motor4.run(BACKWARD);
}

void stop()  // 停止；
{
  motor1.run(RELEASE);  // останавливаем мотор M1
  motor2.run(RELEASE);  // останавливаем мотор M2
  motor3.run(RELEASE);
  motor4.run(RELEASE);
  delay(20);
}

void open()  // 停止；
{
  servo.write(openAngle);
  delay(20);
}

void close()  // 停止；
{
  servo.write(closeAngle);
  delay(20);
}

void flagUp()  // 停止；
{
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  delay(flagTime);
}

void flagDown()  // 停止；
{
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  delay(flagTime);
}

void left(int speed) {
  motor1.setSpeed(speed);
  motor1.run(FORWARD);
  motor2.setSpeed(speed);
  motor2.run(FORWARD);
  motor3.setSpeed(speed);
  motor3.run(BACKWARD);
  motor4.setSpeed(speed);
  motor4.run(BACKWARD);
}
void right(int speed) {
  motor1.setSpeed(speed);
  motor1.run(BACKWARD);
  motor2.setSpeed(speed);
  motor2.run(BACKWARD);
  motor3.setSpeed(speed);
  motor3.run(FORWARD);
  motor4.setSpeed(speed);
  motor4.run(FORWARD);
}

void loop() {

  if (type == 2) {  //Guitar Hero Controller
    return;
  } else {                              //DualShock Controller
    ps2x.read_gamepad(false, vibrate);  //read controller and set large motor to spin at 'vibrate' speed

    //start 开始运行，电机初PWM为100；
    if (ps2x.Button(PSB_START)) {
      Serial.println("Start is being held");
      speed = 10;
      forward(speed);
    }
    // 电机正转；
    if (ps2x.Button(PSB_PAD_UP)) {
      Serial.println("Up held this hard: ");
      speed = 200;
      forward(speed);
    }

    // 电机反转；
    if (ps2x.Button(PSB_PAD_DOWN)) {
      Serial.print("Down held this hard: ");
      speed = 150;
      back(speed);
    }

    //左转；
    if (ps2x.Button(PSB_PAD_LEFT)) {
      Serial.println("turn left ");
      speed = 100;
      turnLeft(speed);
    }

    //右转；
    if (ps2x.Button(PSB_PAD_RIGHT)) {
      Serial.println("turn right");
      speed = 100;
      turnRight(speed);
    }
    // Stop
    if (ps2x.Button(PSB_SELECT)) {
      Serial.println("stop");
      stop();
    }
    delay(20);
  }
  if (ps2x.Button(PSS_RY)) {
    left(speed);
  }
  if (ps2x.Button(PSS_RX)) {
    right(speed);
  }
  if (ps2x.Button(PSB_GREEN)) {
    open();
  }
  if (ps2x.Button(PSB_RED)) {
    close();
  }
   if (ps2x.Button(PSB_L1)) {
    flagUp();
  }
   if (ps2x.Button(PSB_L2)) {
    flagDown();
  }
  if (ps2x.Button(PSB_R2)) {  //print stick values if either is TRUE(摇杆)
    Serial.print("Stick Values:");
    Serial.print(ps2x.Analog(PSS_LY), DEC);  //Left stick, Y axis. Other options: LX, RY, RX
    Serial.print(",");
    Serial.print(ps2x.Analog(PSS_LX), DEC);
    Serial.print(",");
    Serial.print(ps2x.Analog(PSS_RY), DEC);
    Serial.print(",");
    Serial.println(ps2x.Analog(PSS_RX), DEC);



    int LY = ps2x.Analog(PSS_LY);
    int LX = ps2x.Analog(PSS_LX);

    int RY = ps2x.Analog(PSS_RY);
    int RX = ps2x.Analog(PSS_RX);


    if (LY < 125)  //前进
    {
      speed = 2 * (127 - LY);
      forward(255);
      delay(20);
    }

    if (LY > 128)  //后退
    {
      speed = 2 * (LY - 128);
      back(255);
      delay(20);
    }

    if (LX < 125)  //左转
    {
      speed = (127 - LX);
      turnLeft(255);

      delay(20);
    }

    if (LX > 128)  //右转
    {
      speed = (LX - 128);
      turnRight(255);
      delay(20);
    }

    if (LY >= 125 && LY <= 128 && LX >= 125 && LX <= 128)  //如果摇杆居中
    {
      stop();
      delay(20);
    }

    //******************
    if (RX < 125)  //前进
    {
      speed = 2 * (127 - RX);
      left(speed);
      delay(20);
    }

    if (RX > 128)  //后退
    {
      speed = 2 * (RX - 128);
      right(speed);
      delay(20);
    }
  }
}
/*
void loop(){
  servo.write(0);
  delay(1000);
  servo.write(90);
  delay(1000);
  
}*/
