#include <SPI.h>
#include <Ethernet.h>
#include <C:\Users\Utilizador135\Desktop\httppost2\ArduinoBot\httppost2\Max6675.h>

byte mac[] = {  
  0x90, 0xA2, 0xDA, 0x0D, 0x11, 0xF7 };

Max6675 ts(8, 9, 10);
char serverName[] = "atec.marionobre.com";
int serverPort = 80;
char pageName[] = "/listener.php";
EthernetClient client;
int totalCount = 0; 
char params[32]; //array de parametros em char em que enviamos os parametros


#define delayMillis 100000UL //tempo

unsigned long presentTime = 0;
unsigned long lastTime = 0;



void setup() {
  Serial.begin(9600);

  // disable SD SPI
  pinMode(4,OUTPUT);
  digitalWrite(4,HIGH);

  Serial.print(F("Iniciar a placa ethernet..."));
  if(!Ethernet.begin(mac)) 
  Serial.println(F("Falhou"));
  else 
  Serial.println(Ethernet.localIP());

  delay(2000);
  Serial.println(F("Ready"));
  randomSeed(analogRead(0)); //seed de random 
  ts.setOffset(0);
}

void loop()
{
  
  Ethernet.maintain(); //renova o dhcp - mesmo ip

  presentTime = millis(); //tempo desde o programa começou em milisegundos

  if(presentTime - lastTime > delayMillis)
  {
    lastTime = presentTime;

    float r = (float)rand() / (float)RAND_MAX; //random values em floats
    float randomph=7 + r * (7.5 - 7);
    float randomclr=0.5 + r * (2.5 - 0.5);
    float temp = ts.getCelsius();
    
    String p = "ph="+String(randomph)+"&clr="+String(randomclr)+"&temp="+String(temp)+"&mac=1";
    const char* pfinal=p.c_str();
    
    //envia os parametros para o array de chars 
    sprintf(params, pfinal, totalCount);
    Serial.print(params); 
        
    if(!postPage(serverName,serverPort,pageName,params)) 
    Serial.print(F("Fail "));
    else 
    Serial.print(F("Pass"));
    totalCount++;
    Serial.println(totalCount,DEC);
  }    
}


byte postPage(char* domainBuffer,int thisPort,char* page,char* thisData)
{
  int inChar;
  char outBuf[64];

  Serial.print(F("connecting..."));

  if(client.connect(domainBuffer,thisPort) == 1)
  {
    Serial.println(F("connected"));

    
    sprintf(outBuf,"POST %s HTTP/1.1",page);
    client.println(outBuf);
    sprintf(outBuf,"Host: %s",domainBuffer);
    client.println(outBuf);
    client.println(F("Connection: close\r\nContent-Type: application/x-www-form-urlencoded"));
    sprintf(outBuf,"Content-Length: %u\r\n",strlen(thisData));
    client.println(outBuf);

    client.print(thisData);
  } 
  else
  {
    Serial.println(F("failed"));
    return 0;
  }

  int connectLoop = 0;

  while(client.connected())
  {
    while(client.available())
    {
      inChar = client.read();
      Serial.write(inChar);
      connectLoop = 0;
    }

    delay(1);
    connectLoop++;
    if(connectLoop > 10000)
    {
      Serial.println();
      Serial.println(F("Timeout"));
      client.stop();
    }
  }

  Serial.println();
  Serial.println(F("disconnecting."));
  client.stop();
  return 1;
}
