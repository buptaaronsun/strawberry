#include <Wire.h>
#include <JY901.h>

/**********************************BMP180预编译S*******************************************/
#define BMP180ADD 0x77  // I2C address of BMP180     
unsigned char OSS;                            
int ac1;           // 0xAA     0xAB
int ac2;           // 0xAC     0xAD
int ac3;           // 0xAE     0xAE
unsigned int ac4;  // 0xB0     0xB1
unsigned int ac5;  // 0xB2     0xB3
unsigned int ac6;  // 0xB4     0xB5
int b1;            // 0xB6     0xB7
int b2;            // 0xB8     0xB9
int mb;            // 0xBA     0xBB
int mc;            // 0xBC     0xBD
int md;            // 0xBE     0xBF
float temperature;  
double pressure;   
double pressure2;
long b5;          
double altitude;  
 
/*********************************BMP180预编译F********************************************/
/**********************************JY901预编译S*******************************************/
double accx,accy,accz,gyrox,gyroy,gyroz,anglex,angley,anglez,magx,magy,magz;
/**********************************JY901预编译F*******************************************/
/**********************************WIFI预编译*********************************************/
#define SSID        "wifi" //改为你的Wi-Fi名称
#define PASSWORD    "11111111"//Wi-Fi密码
#define HOST_NAME   "api.heclouds.com"
#define DEVICEID    "505471488" //OneNet上的设备ID
#define PROJECTID   "194032" //OneNet上的产品ID
#define HOST_PORT   (80)
String apiKey="XI1JnYA=XTTieVb6tDoWZv9BG20=";//与你的设备绑定的APIKey

#define INTERVAL_SENSOR   17000             //定义传感器采样时间间隔  597000
#define INTERVAL_NET      17000             //定义发送时间
//传感器部分================================   
#include <Wire.h>                                  //调用库  
#include <ESP8266.h>
#include <I2Cdev.h>                                //调用库  
/*******温湿度*******/
#include <Microduino_SHT2x.h>
/*******光照*******/
#define  sensorPin_1  A0
#define IDLE_TIMEOUT_MS  3000      // Amount of time to wait (in milliseconds) with no data 
                                   // received before closing the connection.  If you know the server
                                   // you're accessing is quick to respond, you can reduce this value.

//WEBSITE     
char buf[10];

#define INTERVAL_sensor 2000
unsigned long sensorlastTime = millis();

float tempOLED, humiOLED, lightnessOLED;

#define INTERVAL_OLED 1000

String mCottenData;
String jsonToSend;

//3,传感器值的设置 
float sensor_tem, sensor_hum, sensor_lux;                    //传感器温度、湿度、光照   
char  sensor_tem_c[7], sensor_hum_c[7], sensor_lux_c[7] ;    //换成char数组传输
#include <SoftwareSerial.h>
#define EspSerial mySerial
#define UARTSPEED  9600
SoftwareSerial mySerial(2, 3); /* RX:D3, TX:D2 */
ESP8266 wifi(&EspSerial);
//ESP8266 wifi(Serial1);                                      //定义一个ESP8266（wifi）的对象
unsigned long net_time1 = millis();                          //数据上传服务器时间
unsigned long sensor_time = millis();                        //传感器采样时间计时器

//int SensorData;                                   //用于存储传感器数据
String postString;                                //用于存储发送数据的字符串
//String jsonToSend;                                //用于存储发送的json格式参数

Tem_Hum_S2 TempMonitor;
 /**********************************WIFI预编译*********************************************/
void setup()
{
   Serial.begin(115200);
  /*************************************BMP180****************************************/
  Wire.begin();
  
  OSS = 2;  // Oversampling Setting           0: single    1: 2 times    2: 4 times   3: 8 times 
  BMP180start();
  
 
/*************************************BMP180****************************************/
/**********************************WIFI*********************************************/
 //初始化串口波特率  
   
    while (!Serial); // wait for Leonardo enumeration, others continue immediately
    Serial.print(F("setup begin\r\n"));
    delay(100);
    pinMode(sensorPin_1, INPUT);

  WifiInit(EspSerial, UARTSPEED);

  Serial.print(F("FW Version:"));
  Serial.println(wifi.getVersion().c_str());

  if (wifi.setOprToStationSoftAP()) {
    Serial.print(F("to station + softap ok\r\n"));
  } else {
    Serial.print(F("to station + softap err\r\n"));
  }

  if (wifi.joinAP(SSID, PASSWORD)) {
    Serial.print(F("Join AP success\r\n"));

    Serial.print(F("IP:"));
    Serial.println( wifi.getLocalIP().c_str());
  } else {
    Serial.print(F("Join AP failure\r\n"));
  }

  if (wifi.disableMUX()) {
    Serial.print(F("single ok\r\n"));
  } else {
    Serial.print(F("single err\r\n"));
  }

  Serial.print(F("setup end\r\n"));
 /**********************************WIFI*********************************************/
 

/*************************************JY901****************************************/
 JY901.StartIIC();
/*************************************JY901****************************************/
 
}
 
void loop()
{
  /*************************************BMP180****************************************/
  calculate();
  show();
  
  /*************************************BMP180****************************************/
  /*************************************JY901****************************************/
  JY901.GetAcc();//陀螺仪 
  accx=(float)JY901.stcAcc.a[0]/32768*16;
  accy=(float)JY901.stcAcc.a[1]/32768*16;
  accz=(float)JY901.stcAcc.a[2]/32768*16;
  JY901.GetGyro();
  gyrox=(float)JY901.stcGyro.w[0]/32768*2000;
  gyroy=(float)JY901.stcGyro.w[1]/32768*2000;
  gyroz=(float)JY901.stcGyro.w[2]/32768*2000;
  JY901.GetAngle();
  anglex=(float)JY901.stcAngle.Angle[0]/32768*180;
  angley=(float)JY901.stcAngle.Angle[1]/32768*180;
  anglez=(float)JY901.stcAngle.Angle[2]/32768*180;
  JY901.GetMag();
  magx=JY901.stcMag.h[0];
  magy=JY901.stcMag.h[1];
  magz=JY901.stcMag.h[2];//陀螺仪
  /*************************************JY901****************************************/
  delay(3000);
   /**********************************WIFI*********************************************/
if (sensor_time > millis())  sensor_time = millis();  
    
  if(millis() - sensor_time > INTERVAL_SENSOR)              //传感器采样时间间隔  
  {  
    getSensorData();                                        //读串口中的传感器数据
    sensor_time = millis();
  }  

    
  if (net_time1 > millis())  net_time1 = millis();
  
  if (millis() - net_time1 > INTERVAL_NET)                  //发送数据时间间隔
  {                
    updateSensorData1();
    //updateSensorData2();//将数据上传到服务器的函数
    net_time1 = millis();
  }

 /**********************************WIFI*********************************************/

    delay(1000);
}
 
/*************************************BMP180函数****************************************/
void calculate()
{
  temperature = bmp180GetTemperature(bmp180ReadUT());
  temperature = temperature*0.1;
  pressure = bmp180GetPressure(bmp180ReadUP());
  pressure2 = pressure/101325;
  pressure2 = pow(pressure2,0.19029496);
  altitude = 44330*(1-pressure2)+275;                            //altitude = 44330*(1-(pressure/101325)^0.19029496);
}
 
/** print reslut **/
void show()
{
  Serial.print("Temperature: ");
  Serial.print(temperature, 1);                            //10 hexadecimal
  Serial.println(" C");
  Serial.print("Pressure: ");
  Serial.print(pressure, 0);                               //10 hexadecimal
  Serial.println(" Pa");
  Serial.print("altitude:");
  Serial.print(altitude);
  Serial.println("m");
}
 
/**BMP180 satrt program**/
void BMP180start()
{                     /*MSB*/
  ac1 = bmp180ReadDate(0xAA);                      //get full data
  ac2 = bmp180ReadDate(0xAC);  
  ac3 = bmp180ReadDate(0xAE);  
  ac4 = bmp180ReadDate(0xB0);  
  ac5 = bmp180ReadDate(0xB2);  
  ac6 = bmp180ReadDate(0xB4);  
  b1  = bmp180ReadDate(0xB6);  
  b2  = bmp180ReadDate(0xB8);  
  mb  = bmp180ReadDate(0xBA);  
  mc  = bmp180ReadDate(0xBC);  
  md  = bmp180ReadDate(0xBE);
}
 
/***BMP180 temperature Calculate***/
short bmp180GetTemperature(unsigned int ut)
{
  long x1, x2;
  x1 = (((long)ut - (long)ac6)*(long)ac5) >> 15;  //x1=((ut-ac6)*ac5)/(2^15)
  x2 = ((long)mc << 11)/(x1 + md);                //x2=(mc*2^11)/(x1+md)
  b5 = x1 + x2;                                   //b5=x1+x2
  return ((b5 + 8)>>4);                           //t=(b5+8)/(2^4)
}
 
/***BMP180 pressure Calculate***/
 
long bmp180GetPressure(unsigned long up)
{
  long x1, x2, x3, b3, b6, p;
  unsigned long b4, b7;
  
  b6 = b5 - 4000;
 
  x1 = (b2 * (b6 * b6)>>12)>>11;
  x2 = (ac2 * b6)>>11;
  x3 = x1 + x2;
  b3 = (((((long)ac1)*4 + x3)<<OSS) + 2)>>2;
  
  x1 = (ac3 * b6)>>13;
  x2 = (b1 * ((b6 * b6)>>12))>>16;
  x3 = ((x1 + x2) + 2)>>2;
  b4 = (ac4 * (unsigned long)(x3 + 32768))>>15;
  
  b7 = ((unsigned long)(up - b3) * (50000>>OSS));
  if (b7 < 0x80000000)
    p = (b7<<1)/b4;
  else
    p = (b7/b4)<<1;
    
  x1 = (p>>8) * (p>>8);
  x1 = (x1 * 3038)>>16;
  x2 = (-7357 * p)>>16;
  p += (x1 + x2 + 3791)>>4;
  
  return p;
}
 
 
/*** Read 1 bytes from the BMP180  ***/
 
int bmp180Read(unsigned char address)
{
  unsigned char data;
  
  Wire.beginTransmission(BMP180ADD);
  Wire.write(address);
  Wire.endTransmission();
  
  Wire.requestFrom(BMP180ADD, 1);
  while(!Wire.available());
    
  return Wire.read();
}
 
/*** Read 2 bytes from the BMP180 ***/
int bmp180ReadDate(unsigned char address)
{
  unsigned char msb, lsb;
  Wire.beginTransmission(BMP180ADD);
  Wire.write(address);
  Wire.endTransmission();
  Wire.requestFrom(BMP180ADD, 2);
  while(Wire.available()<2);
  msb = Wire.read();
  lsb = Wire.read();
  return (int) msb<<8 | lsb;
}
 
/*** read uncompensated temperature value ***/
unsigned int bmp180ReadUT()
{
  unsigned int ut;
  Wire.beginTransmission(BMP180ADD);
  Wire.write(0xF4);                       // Write 0x2E into Register 0xF4
  Wire.write(0x2E);                       // This requests a temperature reading
  Wire.endTransmission();  
  delay(5);                               // Wait at least 4.5ms
  ut = bmp180ReadDate(0xF6);               // read MSB from 0xF6 read LSB from (16 bit)
  return ut;
}
 
/*** Read uncompensated pressure value from BMP180 ***/
unsigned long bmp180ReadUP()
{
  unsigned char msb, lsb, xlsb;
  unsigned long up = 0;
  
  Wire.beginTransmission(BMP180ADD);
  Wire.write(0xF4);                        // Write 0x34+(OSS<<6) into register 0xF4
  Wire.write(0x34 + (OSS<<6));             // 0x34+oss*64
  Wire.endTransmission(); 
  delay(2 + (3<<OSS));                     // Wait for conversion, delay time dependent on OSS
  
  Wire.beginTransmission(BMP180ADD);
  Wire.write(0xF6);                        // Read register 0xF6 (MSB), 0xF7 (LSB), and 0xF8 (XLSB)
  Wire.endTransmission();
  
  Wire.requestFrom(BMP180ADD, 3); 
  while(Wire.available() < 3);             // Wait for data to become available
  msb = Wire.read();
  lsb = Wire.read();
  xlsb = Wire.read();
  up = (((unsigned long) msb << 16) | ((unsigned long) lsb << 8) | (unsigned long) xlsb) >> (8-OSS);//16 to 19 bit
  return up;
}
/*************************************BMP180函数****************************************/
/*************************************WIFI函数****************************************/
void getSensorData(){  
    sensor_tem = TempMonitor.getTemperature();  
    sensor_hum = TempMonitor.getHumidity();   
    //获取光照
    sensor_lux = analogRead(A0);    
    delay(1000);
    dtostrf(sensor_tem, 2, 1, sensor_tem_c);
    dtostrf(sensor_hum, 2, 1, sensor_hum_c);
    dtostrf(sensor_lux, 3, 1, sensor_lux_c);
}
void updateSensorData1() {
  if (wifi.createTCP(HOST_NAME, HOST_PORT)) { //建立TCP连接，如果失败，不能发送该数据
    Serial.print("create tcp ok\r\n");

    jsonToSend="{\"temp\":";
    dtostrf(temperature,1,2,buf);
    jsonToSend+="\""+String(buf)+"\"";
    jsonToSend+=",\"hum\":";
    dtostrf(altitude,1,2,buf);
    jsonToSend+="\""+String(buf)+"\"";
    jsonToSend+=",\"press\":";
    dtostrf(pressure,1,2,buf);
    jsonToSend+="\""+String(buf)+"\"";
        
        
        
        jsonToSend+=",\"anglex\":";
    dtostrf(anglex,1,2,buf);
    jsonToSend+="\""+String(buf)+"\"";
        jsonToSend+=",\"angley\":";
    dtostrf(angley,1,2,buf);
    jsonToSend+="\""+String(buf)+"\"";
        jsonToSend+=",\"anglez\":";
    dtostrf(anglez,1,2,buf);
    jsonToSend+="\""+String(buf)+"\"";


        
     
        
  /*
            jsonToSend+=",\"ax\":";
    dtostrf(accx,1,2,buf);
    jsonToSend+="\""+String(buf)+"\"";
        jsonToSend+=",\"ay\":";
    dtostrf(accy,1,2,buf);
    jsonToSend+="\""+String(buf)+"\"";
        jsonToSend+=",\"az\":";
    dtostrf(accz,1,2,buf);
    jsonToSend+="\""+String(buf)+"\"";


//角速度
jsonToSend+=",\"wx\":";
    dtostrf(gyrox,1,2,buf);
    jsonToSend+="\""+String(buf)+"\"";
        jsonToSend+=",\"wy\":";
    dtostrf(gyroy,1,2,buf);
    jsonToSend+="\""+String(buf)+"\"";
        jsonToSend+=",\"wz\":";
    dtostrf(gyroz,1,2,buf);
    jsonToSend+="\""+String(buf)+"\"";


    //磁场强度
  
   jsonToSend+=",\"Bx\":";
    dtostrf(magx,1,2,buf);
    jsonToSend+="\""+String(buf)+"\"";
        jsonToSend+=",\"Bz\":";
    dtostrf(magz,1,2,buf);
    jsonToSend+="\""+String(buf)+"\"";
        jsonToSend+=",\"Byy\":";
    dtostrf(magy,1,2,buf);
    jsonToSend+="\""+String(buf)+"\"";
    */



    jsonToSend+="}";



    postString="POST /devices/";
    postString+=DEVICEID;
    postString+="/datapoints?type=3 HTTP/1.1";
    postString+="\r\n";
    postString+="api-key:";
    postString+=apiKey;
    postString+="\r\n";
    postString+="Host:api.heclouds.com\r\n";
    postString+="Connection:close\r\n";
    postString+="Content-Length:";
    postString+=jsonToSend.length();
    postString+="\r\n";
    postString+="\r\n";
    postString+=jsonToSend;
    postString+="\r\n";
    postString+="\r\n";
    postString+="\r\n";

  const char *postArray = postString.c_str();                 //将str转化为char数组
  Serial.println(postArray);
  wifi.send((const uint8_t*)postArray, strlen(postArray));    //send发送命令，参数必须是这两种格式，尤其是(const uint8_t*)
  Serial.println("send success");   
     if (wifi.releaseTCP()) {                                 //释放TCP连接
        Serial.print("release tcp ok\r\n");
        } 
     else {
        Serial.print("release tcp err\r\n");
        }
      postArray = NULL;                                       //清空数组，等待下次传输数据
  
  } else {
    Serial.print("create tcp err\r\n");
  }
}

/*
 void updateSensorData2() {
  if (wifi.createTCP(HOST_NAME, HOST_PORT)) { //建立TCP连接，如果失败，不能发送该数据
    Serial.print("create tcp ok\r\n");

     
        
        jsonToSend="{\"ax\":";
    dtostrf(accx,1,2,buf);
    jsonToSend+="\""+String(buf)+"\"";
        jsonToSend+=",\"ay\":";
    dtostrf(accy,1,2,buf);
    jsonToSend+="\""+String(buf)+"\"";
        jsonToSend+=",\"az\":";
    dtostrf(accz,1,2,buf);
    jsonToSend+="\""+String(buf)+"\"";
    
    jsonToSend+="}";



    postString="POST /devices/";
    postString+=DEVICEID;
    postString+="/datapoints?type=3 HTTP/1.1";
    postString+="\r\n";
    postString+="api-key:";
    postString+=apiKey;
    postString+="\r\n";
    postString+="Host:api.heclouds.com\r\n";
    postString+="Connection:close\r\n";
    postString+="Content-Length:";
    postString+=jsonToSend.length();
    postString+="\r\n";
    postString+="\r\n";
    postString+=jsonToSend;
    postString+="\r\n";
    postString+="\r\n";
    postString+="\r\n";

  const char *postArray = postString.c_str();                 //将str转化为char数组
  Serial.println(postArray);
  wifi.send((const uint8_t*)postArray, strlen(postArray));    //send发送命令，参数必须是这两种格式，尤其是(const uint8_t*)
  Serial.println("send success");   
     if (wifi.releaseTCP()) {                                 //释放TCP连接
        Serial.print("release tcp ok\r\n");
        } 
     else {
        Serial.print("release tcp err\r\n");
        }
      postArray = NULL;                                       //清空数组，等待下次传输数据
  
  } else {
    Serial.print("create tcp err\r\n");
  }
}
/*************************************WIFI函数****************************************/
