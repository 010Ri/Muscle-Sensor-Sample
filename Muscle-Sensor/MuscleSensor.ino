/* Input のための Pin を宣言 (Analog Input Pin) */
int AIP = A0;

/* Muscle Sensor の値を保持するための変数 sensor_value と temp_max と temp_min を宣言 */
int sensor_value = 0;
int temp_max = 0;
int temp_min = 0;

/* 最大値を保持する変数 MAX と最小値を保持する変数 MIN を宣言 */
int MAX = 0;
int MIN = 1000;

/* 1ブロックの大きさを保持する変数 delta と、しきい値 threshold を宣言 */
int delta = 0;
int threshold = 0;

/* InputPin, OutputPin の初期設定 */
void setup()
{
   /* InputPin の初期設定 */
   pinMode(AIP, INPUT);
   
   Serial.begin(9600);
   Serial.println("Watch Serial");
}

/* mainの処理 */
void loop()
{
  /* シリアルモニタに入力された値を読む */
  if (Serial.available() > 0)
  {
    char mode = Serial.read();
     Serial.print("Mode "); 
     Serial.println(mode);

    /* シリアルモニタに入力された値によってモードを選択・処理を開始 */
    switch(mode)
    {
      case '1' : /* 最大値と最小値を取り、それらの値間を 100 ブロックに分ける（delta を算出する）*/
        while(!Serial.available())
        {
          sensor_value = analogRead(AIP);
          Serial.println(sensor_value);
          temp_max = sensor_value;
          MAX = max(MAX, temp_max);
          temp_min = sensor_value;
          MIN = min(MIN, temp_min);
          
          if(Serial.available() > 0)
          {
            char temp = Serial.read();
            Serial.println(temp); 
            Serial.println("STOP MEASUREMENT");

            Serial.print("MAX ");
            Serial.println(MAX);
            Serial.print("MIN ");
            Serial.println(MIN); 
          }
        }
        delta = (MAX - MIN) / 100;
        Serial.print("delta "); 
        Serial.println(delta);
        break;
        
      case '2' : /* 80ブロックを超えたときに PC に値を送信 */
        if(delta < 1){
          Serial.println("Check the sensor connection.");
          break;
        }
        while(1){
          char temp = Serial.read();
          Serial.println(temp); 
          threshold = MIN + delta * 80;
          sensor_value = analogRead(AIP);
           Serial.println(sensor_value); 
          if(sensor_value > threshold){
            Serial.print("To PC ");
            Serial.write("GO");
          }
          if(Serial.available() > 1){
             Serial.println("STOP MEASUREMENT"); 
            break;
          }
        }
        break;
    }
  }
}
