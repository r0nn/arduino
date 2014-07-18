#include <dht.h>
#include <LiquidCrystal.h>

#define LM35 A0
#define DHT11 7

LiquidCrystal lcd(12,11,5,4,3,2);			//构造一个LiquidCrystal的类成员。使用数字IO ，12,11,5,4,3,2
dht DHT;

int val = 0;				//存放AD变量值
float temp = 0;			        //存放温度值的10倍
int chk = 0;

void setup()
{
  lcd.begin(16,2);		//初始化LCD1602
  lcd.print("Welcome to use!");		//液晶显示Welcome to use！
  delay(1000);				//延时1000ms
  lcd.clear();				//液晶清屏
}

void loop()
{
  // LM35
  val = analogRead(LM35);		//读取AD值
  temp = val * 4.8876;			//转换为温度值的10倍，有利于液晶显示
  lcd.setCursor(0,0);				//设置液晶开始显示的指针位置
  lcd.print("T: ");	//液晶显示“LM35 temp =”
  lcd.print((int)temp/10);	//液晶显示温度整数值
  lcd.print(".");						//液晶显示小数点
  lcd.print((int)temp%10);	//液晶显示温度小数值
  lcd.print((char)223);			//液晶显示“°”
  lcd.print("C");						//液晶显示“C”
  
  // DHT11
  chk = DHT.read11(DHT11);
  lcd.setCursor(0,1);
  switch(chk)
  {
     case DHTLIB_OK:
         lcd.print("H: ");
         lcd.print(DHT.humidity);
         lcd.print(" T: ");
         lcd.print(DHT.temperature);
         break;
      case DHTLIB_ERROR_CHECKSUM: 
	 lcd.print("Checksum error."); 
	 break;
      case DHTLIB_ERROR_TIMEOUT: 
	 lcd.print("Time out error."); 
	 break;
      default:
         lcd.print("Unknown error..");
         break;
  }
  
  delay(1000);							//延时1000ms
}
