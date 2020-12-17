#include <SPI.h>
#include <Ethernet.h> 
#include <ArduinoJson.h>
#include <string.h>

EthernetServer server(80);

uint8_t encender = 2;
uint8_t apagar = 7;
bool status_sistem = false;

void ethernetConfiguration(){
  byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
  byte ip[] = {10, 0, 0, 208};
  byte gateway[] = {10, 0, 0, 1};
  byte subnet[] = {255, 255, 255, 0};

  Ethernet.begin(mac, ip, gateway, subnet);

  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("No se Encontrado el puerto Ethernet.");
    while (true) {
      delay(1);
    }
  }

  
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("El cable Ethernet no esta conectado.");
  }
}

void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  Serial.println("Servidor Ethernet.");
  
  pinMode(encender,OUTPUT);
  digitalWrite(encender,LOW);
  pinMode(apagar,OUTPUT);
  digitalWrite(apagar,LOW);

  pinMode(4,OUTPUT);
  digitalWrite(4,HIGH);

  ethernetConfiguration();
  
  server.begin();
  Serial.print("Server inicio en: ");
  Serial.println(Ethernet.localIP());
}

char* substr(const char *src, int m, int n)
{
    int len = n - m;
    char *dest = (char*)malloc(sizeof(char) * (len + 1));
    for (int i = m; i < n && (*(src + i) != '\0'); i++){
        *dest = *(src + i);
        dest++;
    }
    *dest = '\0';
    return dest - len;
}

int* getBodyIndexPositions(char request[]){
  static int positions[2] = {0,0};
  int i=0;
  while(request[i] != '\0'){
      switch(request[i]){
        case '{':
          positions[0] = i;
          break;
        case '}':
          positions[1] = i;
          break;
     }
    i++;
  }
  return positions;
}

void lunchPulse(uint8_t rele){
  status_sistem = !status_sistem;
  digitalWrite(rele,HIGH);
  delay(5000);
  digitalWrite(rele,LOW);
}

uint8_t getTypeRequest(char request[]){
  char *ret;
  uint8_t tam = strlen(request),typeRequest = 0;
  char auxRequest[tam+1];
  
  strcpy(auxRequest,request);
  
  const char divide[2] = " ";
  if(strchr(auxRequest,' ')){
    strtok(auxRequest,divide);
  }

  Serial.println(auxRequest);
  
  if(strcmp(auxRequest,"POST") == 0){   
      int *indexs;
      indexs = getBodyIndexPositions(request);
      uint8_t bodyLength = (indexs[1] - indexs[0]) + 3;
      char body[bodyLength];
      memcpy(body,&request[indexs[0]],bodyLength);
      Serial.println(body);

      const int capacity = JSON_OBJECT_SIZE(3);
      StaticJsonDocument<capacity> jsonStringRequest;
      
      DeserializationError error = deserializeJson(jsonStringRequest, body);
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        typeRequest = 0;
      }else{
        typeRequest = 1;
      }
      
      uint8_t encender_val,apagar_val;
      encender_val = (uint8_t)jsonStringRequest["encender"];
      apagar_val = (uint8_t)jsonStringRequest["apagar"];
      Serial.print(encender_val);
      Serial.print(" ");
      Serial.print(apagar_val);
      Serial.println();
      
      
  }else if(strcmp(auxRequest,"GET") == 0){
    typeRequest = 2;
  }
  
  return typeRequest;
}

void loop() {
   EthernetClient client = server.available();
   if(client){
    String header;
    bool bandera = true;
    Serial.println("Nuevo cliente conectado");
    delay(5);
      while(client.connected() && bandera){
          if(client.available()){
              StaticJsonDocument<200>doc;
              JsonObject obj=doc.to<JsonObject>();
              
              uint8_t typeRequest = 0,count = 0;
              char request[500],*aux;
              
              while(client.available() && count < 500){
                char c;
                c = client.read();
                request[count] = c;
                count++;
              }
              request[count] = '\0';
              aux = request;

              typeRequest = getTypeRequest(aux);
              Serial.println(typeRequest);
              
              switch(typeRequest){
                case 1: 
                  obj["status"] = 200;
                  obj["result"] = "request succes";
                  break;
                case 2:
                  obj["status_sistem"] = status_sistem ;
                  break;
                default:
                  obj["status"] = 200;
                  obj["result"] = "error request";
              }

              client.println("HTTP/1.1 200 OK"); 
              client.println("Access-Control-Allow-Origin:*");
              client.print("Content-Length: ");
              client.println(measureJson(doc));
              client.println("Content-Type: application/json");
              client.println();
              serializeJson(doc, client);
              break;   
          }      
      }
      
      client.stop();
      client.flush();
      delay(5);
      Serial.println("");
      Serial.println("Cliente desconectado");
  }
}
