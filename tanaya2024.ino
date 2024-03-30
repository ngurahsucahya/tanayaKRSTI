/*
  KRSTI Tanaya Created on April 20th, 2023
  modified March 30th, 2024 by Sucahya

  Akses peta ID servo:
  ....

  Catatan: 
  1. Code untuk menggerakkan servo AX12A
     ax12a.moveSpeed(ID, Angle, Speed);
     ax12a.moveSpeed(24, 512, 75);
*/


#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <AX12A.h>


// deklarasi servo AX12A
#define DirectionPin  (10u)
#define BaudRate      (1000000ul)
#define ID            (1u)

const int SERVO_KAKI_KIRI_ID[] = {1, 2, 3, 4, 5};
const int SERVO_KAKI_KANAN_ID[] = {6, 7, 8, 9, 10};

// Deklarasi objek MPU6050 dan inisialisasi alamat
Adafruit_MPU6050 mpu;
int gyro_x, gyro_y;

// Deklarasi fungsi untuk gerakin servo ke posisi tertentu dengan kecepatan
void moveServo(int id, int position, int speed) {
  ax12a.moveSpeedRW(id, position, speed);
}

void setup() {
  Serial.begin(1000000);
  ax12a.begin(BaudRate, DirectionPin, &Serial);
  Serial.begin(9600);
  while (!Serial);

  // Mulai komunikasi dengan MPU6050
  Wire.begin();

  // Mengatur semua servo ke posisi awal (harusnya lebih kompleks lagi ngatur sudutnya, tpi ini nyoba dulu aja)
  for (int i = 0; i < 5; i++) {
    moveServo(SERVO_KAKI_KIRI_ID[i], 512, 75);
    moveServo(SERVO_KAKI_KANAN_ID[i], 512, 150);
  }

}

void loop() {

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Baca data gyro pada sumbu x dan y dari MPU6050
  gyro_x = g.gyro.x;
  gyro_y = g.gyro.y;

  // Cetak semua nilai gyro
  Serial.print("Gyro X: ");
  Serial.print(gyro_x);
  Serial.print("   Gyro Y: ");
  Serial.println(gyro_y);

  // Penentuan gerakan berdasarkan nilai gyro
  if (gyro_x > 1000) {  // Miring ke kanan
    for (int i = 0; i < 5; i++) {
      moveServo(SERVO_KAKI_KIRI_ID[i], map(i, 0, 4, 200, 1000), 150);  // Gerakan kaki kiri maju
      moveServo(SERVO_KAKI_KANAN_ID[i], 512, 150);  // Gerakan kaki kanan diam
    }
  } else if (gyro_x < -1000) {  // Miring ke kiri
    for (int i = 0; i < 5; i++) {
      moveServo(SERVO_KAKI_KIRI_ID[i], 512, 150);  // Gerakan kaki kiri diam
      moveServo(SERVO_KAKI_KANAN_ID[i], map(i, 0, 4, 200, 1000), 150);  // Gerakan kaki kanan maju
    }
  } else if (gyro_y > 1000) {  // Miring ke depan
    // belum tau hehe
  } else if (gyro_y < -1000) {  // Miring ke belakang
    // belum tau hehe

  } else {  // Stady
    for (int i = 0; i < 5; i++) {
      moveServo(SERVO_KAKI_KIRI_ID[i], 512, 512);  // Gerakan kaki kiri diam (harusnya lebih kompleks lagi sih hehe)
      moveServo(SERVO_KAKI_KANAN_ID[i], 512, 512);  // Gerakan kaki kanan diam
    }
  }

  delay(100); 
}