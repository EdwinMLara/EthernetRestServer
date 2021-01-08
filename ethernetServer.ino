#include <ArduinoJson.h>
#include <SPI.h>
#include <Ethernet.h>  
#include <string.h>

EthernetServer server(80);

uint8_t encender = 2;
uint8_t apagar = 7;

bool status_sistem = false;
bool banderaPulse = false;
bool banderaTempo = false;

boolean banderasTemporizador[4];

unsigned long startmillis;
unsigned long startmillisTemporizador;

unsigned long teG1;
unsigned long taG1;
unsigned long teG2;
unsigned long taG2;

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

void configuracionTemporizador(uint8_t te1,uint8_t ta1,uint8_t te2,uint8_t ta2){
  iniciar_banderas(banderasTemporizador,4);
  teG1 = te1*60*1000;
  taG1 = teG1 + ta1*60*1000;
  teG2 = teG1 + taG1 + te2*60*1000;
  taG2 = teG1 + taG1 + teG2 + ta2*60*1000;
}

char* Seguntos2Time(unsigned long tiempo){
    char str[20] = "";
    char aux[20] = "";
    unsigned long reciduo,hr,minu,seg;
    seg = tiempo % 60;
    reciduo = tiempo / 60;
    minu = reciduo % 60;
    reciduo = reciduo / 60;
    hr = reciduo%60;
    itoa(hr,aux,10);
    strcat(str,aux);
    strcat(str," : ");
    itoa(minu,aux,10);
    strcat(str,aux);
    strcat(str," : ");
    itoa(seg,aux,10);
    strcat(str,aux);
    Serial.println("Adentro ");
    Serial.println(str);

    char *strReturn = malloc(20);
    strcpy(strReturn, str);
    return strReturn;
}

void temporizador(unsigned long countTime){
  if((countTime >= teG1) && banderasTemporizador[0]){
    Serial.println("Entro en 1");
    banderasTemporizador[0] = false;
  }
  if((countTime >= taG1) && banderasTemporizador[1]){
    Serial.println("Entro en 2");
    banderasTemporizador[1] = false;
  }
  if((countTime >= teG2) && banderasTemporizador[2]){
    Serial.println("Entro en 3");
    banderasTemporizador[2] = false;
  }
  if((countTime >= taG2) && banderasTemporizador[3]){
    banderasTemporizador[3] = false;
    iniciar_banderas(banderasTemporizador,4);
    desFase = 0;
    Serial.println("Reinicio"); 
  }

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

void desFaseTime(uint8_t desfase){
  unsigned long *aux = &desFase;
  Serial.print(*aux);
  Serial.print(" ");
  *aux += desfase*60*1000;
  Serial.println(desFase);
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

      Serial.println(body);
      delay(20);
      DeserializationError error = deserializeJson(doc2,body);
      
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return 0;
      }

      delay(10);

      if(doc.containsKey("tempoConfig")){
        Serial.println("Configurando Temporizador");
      }else{
        uint8_t encender_val = doc2["encender"];
        uint8_t apagar_val = doc2["apagar"];
        uint8_t tempo = doc2["tempo"];
        uint8_t desfase = doc2["desfase"];

        if(encender_val)
          lunchPulse(encender);
  
        if(apagar_val)
          lunchPulse(apagar);
  
        if(tempo){
          Serial.println("Se encendio el Temporizador");
        }
        
        if(desfase){
          Serial.println("defasamiento en la señal");  
        }
      }
       
      doc2.clear();
      typeRequest = 1; 
  }else if(strcmp(auxRequest,"GET") == 0){
      typeRequest = 2;
  }
  
  return typeRequest;
}

void setup() {
  Serial.begin(9600);
  while (!Serial);

  startmillis = 0;
  startmillisTemporizador = 0;
  configuracionTemporizador(4,2,5,13);

  Serial.println("Configuracion Temporizador: ");
  Serial.println(teG1);
  Serial.println(taG1);
  Serial.println(teG2);
  Serial.println(taG2);
  
  Serial.println("Servidor Ethernet.");
  
  pinMode(encender,OUTPUT);
  digitalWrite(encender,LOW);
  
  pinMode(apagar,OUTPUT);
  digitalWrite(apagar,LOW);

  pinMode(4,OUTPUT);
  digitalWrite(4,HIGH);

  ethernetConfiguration();
}

void loop() {
   EthernetClient client = server.available();
   if(client){
    String header;
    bool bandera = true;
    char *aux,aux2;
    Serial.println("Nuevo cliente conectado");
      while(client.connected() && bandera){
          if(client.available()){
              
              uint8_t typeRequest = 0,count = 0;
              char request[500];
              
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
                  doc["status_request"].set(200);
                  doc["result"].set("request success");
                  break;
                case 2:
                  doc["status_sistem"].set(status_sistem);
                  doc["status_Tempo"].set(banderaTempo);
                  doc["desfase"].set(desFase);
                  /*if(banderaTempo){
                    unsigned long currentmillis,countTime2;
                    currentmillis = millis(); 
                    countTime2 = (currentmillis - startmillisTemporizador) + desFase;
                    aux2 = Seguntos2Time(countTime2/1000); 
                    Serial.println(aux2);
                  }*/
                  break;
                default:
                  doc["status"].set(200);
                  doc["result"].set("error request");
              }
              
              char output[80]="";
              serializeJson(doc,output);
              Serial.println(output);
              
              delay(10);
              client.println("HTTP/1.1 200 OK"); 
              client.print("Content-Length: ");
              client.println(measureJson(doc));
              client.println("Content-Type: application/json");
              client.println();
              
              client.println(output);
              break;   
          }   
      }
      delay(10);
      client.stop();
      client.flush();
      doc.clear();
      Serial.println("");
      Serial.println("Cliente desconectado");
  }
  

  if(banderaPulse){
    unsigned long countTime; 
    countTime = millis() - startmillis;
    pulseControl(countTime,5);
  }

  if(banderaTempo){
    unsigned long countTime; 
    countTime = (millis() - startmillisTemporizador) + desFase;
    temporizador(countTime);
  }

  delay(10);
}
