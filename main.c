#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <softPwm.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define PI  3.14159265

int main (void)
{
int fd;

int adressMPU6050=0x68;
printf("Praca studentów: 241210, 241795 -12.05.2020\r\n");

int dane[2];


if(wiringPiSetup() == -1)
exit(1);

if((fd=wiringPiI2CSetup(adressMPU6050)) == -1){
printf("error initialize I2C");
exit(1);
}
printf("I2C modul MPU6050\r\n");

//Uruchamia pomiary
int regPWR=0x6C; // adres rejestru PWR_MGMT_1
wiringPiI2CWriteReg8(fd, regPWR, 0); // zapisanie wartosci 0 (1 bajt) w rejestrze PWR_MGMT_1

//zad 1
int regWho=0x75;// adres rejestru WHO_AM_I

dane[0] = wiringPiI2CReadReg8(fd,regWho); //odczytanie bajtu danych z rejestru WHO_AM_I
printf("I am: %d\r\n",dane[0]); //wyswietlenie odczytanych danych

//zad 2
int regTempH = 0x41; // adres rejestru TEMP_OUT_H
int regTempL = 0x42; // adres rejestru TEMP_OUT_L
uint8_t temp[2];
temp[0] = wiringPiI2CReadReg8(fd,regTempH); //odczytanie bajtu danych z rejestru TEMP_OUT_L
temp[1] = wiringPiI2CReadReg8(fd,regTempL); //odczytanie bajtu danych z rejestru TEMP_OUT_H
int16_t tempOut= temp[0]<<8|temp[1]; //przeliczenie odczytanych danych na bitowa wartosc temperatury
float celsius = tempOut/340.0 + 36.53; // przeliczenie temperatury na stopnie celsjusza wedlug wzoru z dokumentacji
printf("Temperature: %f C\r\n", celsius); // wyswietlenie wyniku

//zad 3
//przypisanie adresow zyroskopu i akcelerometru
int regAccX_H = 0x3B;
int regAccX_L = 0x3C;
int regAccY_H = 0x3D;
int regAccY_L = 0x3E;
int regAccZ_H = 0x3F;
int regAccZ_L = 0x40;

int regGyrX_H = 0x43;
int regGyrX_L = 0x44;
int regGyrY_H = 0x45;
int regGyrY_L = 0x46;
int regGyrZ_H = 0x47;
int regGyrZ_L = 0x48;

//odczytanie danych
int accX[2];
int accY[2];
int accZ[2];
int gyrX[2];
int gyrY[2];
int gyrZ[2];

//test ustawien zyroskopu i akcelerometru
int regAccTest = 0x1C;
int regGyroTest = 0x1B;

uint8_t accTest = wiringPiI2CReadReg8(fd, regAccTest);
uint8_t gyrTest = wiringPiI2CReadReg8(fd, regGyroTest);

printf("Test Gyro: %d\r\nTest Accelerator: %d\r\n", gyrTest, accTest);

accX[0] = wiringPiI2CReadReg8(fd,regAccX_H);
accX[1] = wiringPiI2CReadReg8(fd,regAccX_L);
accY[0] = wiringPiI2CReadReg8(fd,regAccY_H);
accY[1] = wiringPiI2CReadReg8(fd,regAccY_L);
accZ[0] = wiringPiI2CReadReg8(fd,regAccZ_H);
accZ[1] = wiringPiI2CReadReg8(fd,regAccZ_L);

gyrX[0] = wiringPiI2CReadReg8(fd,regGyrX_H);
gyrX[1] = wiringPiI2CReadReg8(fd,regGyrX_L);
gyrY[0] = wiringPiI2CReadReg8(fd,regGyrY_H);
gyrY[1] = wiringPiI2CReadReg8(fd,regGyrY_L);
gyrZ[0] = wiringPiI2CReadReg8(fd,regGyrZ_H);
gyrZ[1] = wiringPiI2CReadReg8(fd,regGyrZ_L);

//przeliczenie na int16_t
int16_t accX_Out = accX[0] << 8 | accX[1];
int16_t accY_Out = accY[0] << 8 | accY[1];
int16_t accZ_Out = accZ[0] << 8 | accZ[1];

int16_t gyrX_Out = gyrX[0] << 8 | gyrX[1];
int16_t gyrY_Out = gyrY[0] << 8 | gyrY[1];
int16_t gyrZ_Out = gyrZ[0] << 8 | gyrZ[1];

//przeliczenie na odpowiednie jednostki
float accX_final = accX_Out/16384.0;
float accY_final = accY_Out/16384.0;
float accZ_final = accZ_Out/16384.0;

float gyrX_final = gyrX_Out/131.0;
float gyrY_final = gyrY_Out/131.0;
float gyrZ_final = gyrZ_Out/131.0;

//wyswietlenie obliczonych parametrow
printf("Accelerator X: %f\r\n",accX_final);
printf("Accelerator Y: %f\r\n",accY_final);
printf("Accelerator Z: %f\r\n",accZ_final);

printf("Gyro X: %f\r\n",gyrX_final);
printf("Gyro Y: %f\r\n",gyrY_final);
printf("Gyro Z: %f\r\n",gyrZ_final);



//zad 4
float poziom = atan2(accX_final, accZ_final) * (180.0/PI); //obliczenie kata na podstawie wskazan akcelerometru
printf("Poziom: %f\r\n", poziom);
return 0;

}
