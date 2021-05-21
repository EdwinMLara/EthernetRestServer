#include <string.h>
#include <SPI.h>
#include <Ethernet.h>  

EthernetServer server(80);

struct Control{
  uint8_t *relevadoresC;
  bool state;
};

uint8_t auxC1[2] = {2,7};
uint8_t auxC2[2] = {8,9};
Control c1 = {auxC1,false};
Control c2 = {auxC2,false};

bool banderaRe = false;
unsigned long startmillis = 0;

bool banderasCT[4];
bool banderaTempo = false;

unsigned long confTempo[4] = {0,0,0,0};
unsigned long tempoStartMillis = 0;
unsigned long timeCount = 0;
unsigned long desfase = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);   
 
  Serial.println("Servidor Ethernet.");
  
  pinMode(2,OUTPUT);
  digitalWrite(2,LOW);
  
  pinMode(7,OUTPUT);
  digitalWrite(7,LOW);

  pinMode(8,OUTPUT);
  digitalWrite(8,LOW);
  
  pinMode(9,OUTPUT);
  digitalWrite(9,LOW);

  pinMode(4,OUTPUT);
  digitalWrite(4,HIGH);
  ethernetConfiguration();
}

void loop() {
   EthernetClient client = server.available();
   if(client){
    char response[100] = "";
    char *aux,aux2;
    
    while(client.connected()){
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
              
              if(strstr(request,"POST") != NULL){
                Serial.println("POST");
                int ppr = processPostRequest(request);
                if(ppr){
                  strcpy(response, "{\"status\":200,\"response\":\"updated\"}");
                }else{
                  strcpy(response, "{\"status\":500,\"response\":\"error\"}");
                }
              }

              if(strstr(request,"GET") != NULL){
                Serial.println("GET");
                int auxSeconds = timeCount/1000;
                char aux[15];
                String(c1.state).toCharArray(aux,15);
                strcpy(response, "{\"status\":200,\"response\":{\"control1\":");
                strcat(response,aux);
                strcat(response,",\"control2\":");
                String(c2.state).toCharArray(aux,15);
                strcat(response,aux);
                strcat(response,",\"tempo\":{\"state\":");
                String(banderaTempo).toCharArray(aux,15);
                strcat(response,aux);
                strcat(response,",\"time\":");
                String(auxSeconds).toCharArray(aux,15);
                strcat(response,aux);
                strcat(response,"}}}");
              }
              
              client.println("HTTP/1.1 200 OK");
              client.println(); 
              client.print(response);
              delay(5);   
              break;   
        }   
      }
      delay(20);
      client.flush();
      client.stop();
  }
     
  if((((millis() - startmillis) >= 10000) && banderaRe)){
    apagar_rele();
    banderaRe = false;
    Serial.println("Apagando relevadores internos");
  }

  if(banderaTempo){
    timeCount = (millis() - tempoStartMillis) + desfase;
    temporizador(timeCount);
    delay(500);
  }
}
