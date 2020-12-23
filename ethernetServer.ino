#include <ArduinoJson.h>
#include <Ethernet.h>  
#include <SPI.h>
#include <string.h>

EthernetServer server(80);

uint8_t encender = 2;
uint8_t apagar = 7;
bool status_sistem = false;
bool banderaPulse = false;
boolean banderasTemporizador[4];

unsigned long startmillis;
unsigned long startmillisTemporizador;

unsigned long desFase = 0;
DynamicJsonDocument doc(200);
DynamicJsonDocument doc2(200);
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

  iniciar_banderas(banderasTemporizador,4);
  
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

void iniciar_banderas(boolean *banderas, uint8_t tam){
  uint8_t i;
  for(i=0;i<=tam-1;i++){
    banderas[i] = true;
  }
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
  banderaPulse = true;
  Serial.println("Encendiendo relevadores internos");
  startmillis =  millis();
}

void pulseControl(unsigned long countTime,uint8_t seconds){
  unsigned long tiempo_encendido = seconds*1000;
    if(countTime >= tiempo_encendido){
      digitalWrite(encender,LOW);
      digitalWrite(apagar,LOW);
      banderaPulse = false;
      Serial.println("Apagando relevadores internos");
    }
}

void temporizador(unsigned long countTime,uint8_t te,uint8_t ta,uint8_t te2,uint8_t ta2){
   if((countTime >= te) && banderasTemporizador[0]){
    Serial.println("Entro en 1");
    banderasTemporizador[0] = false;
  }
  if((countTime >= (te + ta)) && banderasTemporizador[1]){
    Serial.println("Entro en 2");
    banderasTemporizador[1] = false;
  }
  if((countTime >= (te + ta + te2)) && banderasTemporizador[2]){
    Serial.println("Entro en 3");
    banderasTemporizador[2] = false;
  }
  if((countTime >= (te + ta + te2 + ta2)) && banderasTemporizador[3]){
    banderasTemporizador[3] = false;
    iniciar_banderas(banderasTemporizador,4);
    desFase = 0;
    Serial.println("Reinicio"); 
  }

}

uint8_t getTypeRequest(const char request[]){
  uint8_t typeRequest = 0;
  char *token;
  int tam = strlen(request);
  char auxRequest[tam+1]; 
  strcpy(auxRequest,request);
  token = strtok(auxRequest," ");
  Serial.println(auxRequest);
  if(strcmp(token,"POST") == 0){   
      int *indexs;
      indexs = getBodyIndexPositions(request);
   
      int bodyLength = (indexs[1] - indexs[0]) + 3;
      char body[bodyLength];
      memcpy(body,&request[indexs[0]],bodyLength);
      DeserializationError error = deserializeJson(doc2,body);
      
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return 0;
      }

      uint8_t encender_val = doc2["encender"];
      uint8_t apagar_val = doc2["apagar"];
      uint8_t tempo = doc2["tempo"];
      uint8_t desfase = doc2["desfase"];

      Serial.print(encender_val);
      Serial.print(" ");
      Serial.print(apagar_val);
      Serial.print(" ");
      Serial.print(tempo);
      Serial.print(" ");
      Serial.println(desfase);

      if(encender_val)
        lunchPulse(encender);

      if(apagar_val)
        lunchPulse(apagar);

      doc2.clear();
      typeRequest = 1; 
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
              
              switch(typeRequest){
                case 1: 
                  doc["status"] = 200;
                  doc["result"] = "request succes";
                  break;
                case 2:
                  doc["status_sistem"] = status_sistem ;
                  break;
                default:
                  doc["status"] = 200;
                  doc["result"] = "error request";
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

      doc.clear();
      client.stop();
      client.flush();
      delay(5);
      Serial.println("");
      Serial.println("Cliente desconectado");
  }

  if(banderaPulse){
    unsigned long countTime; 
    countTime = (millis() - startmillis);
    pulseControl(countTime,5);
  }
}
