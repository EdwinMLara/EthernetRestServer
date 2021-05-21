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


int* numIntruccionsPos(char body[],int cant){
  /*
   * [0]->control
   * [1]->cT
   * [2]->des
  */
  static int *posinst = (int*) malloc(cant * sizeof(int));
  uint8_t count = 0,i=0;
  while(body[i] != '\0'){
        if(body[i] == ':'){
          posinst[count] = i;
          count++;
        }
        if(count == cant ){
          break;
        }
        i++;
     }
   return posinst;
}

int* str2instruccions(char *body,int pos,int cant){
  /*[0]->who
    [1]->state
    [2}->temporizador state
    [3]->desfase*/
  static int *inst = (int*) malloc(cant * sizeof(int));
  pos += 1;
  uint8_t i;
  char c[2];
  for(i=1;i<=cant;i++){
    c[0] = body[pos+i];
    c[1] = '\0'; 
    int aux = atoi(c);
    inst[i-1] = aux;
  }
  return inst;
  
}

int getDefase(char body[],int pos){
  char c[3] = {'0','0','\0'};
  c[0] = body[pos+1];
  c[1] = body[pos+2];
  return atoi(c);;
}

uint8_t processPostRequest(const char request[]){
    uint8_t result = 0;
    int *indexs;
    indexs = getBodyIndexPositions(request);
 
    int bodyLength = (indexs[1] - indexs[0]) + 3;
    char body[bodyLength];
    memcpy(body,&request[indexs[0]],bodyLength);
    int* posI = numIntruccionsPos(body,3);
    
    int* insR = str2instruccions(body,posI[0],4);
    
    if(insR[2] != 0){
      int* insRCT = str2instruccions(body,posI[1],4);
      uint8_t i;
      unsigned long sum = 0;
      for(i=0;i<4;i++){
        sum += insRCT[i]*600000;
        confTempo[i] = sum;
      }
      iniciarBanderasTempo();
      controlFunction(&c1,1);
      controlFunction(&c2,1);
      tempoStartMillis = millis();
      banderaTempo = true;
      Serial.println("Iniciando Tempo");
      result = 3;
    }else{
      
      if(insR[3]){
        int des = getDefase(body,posI[2]);
        Serial.println(des);
        desFaseTime(des);
        Serial.println("Desfazando");
        return 4;
      }
       
      switch(insR[0]){
        case 1:
          Serial.println("Control 1");
          banderaTempo = false;
          controlFunction(&c1,insR[1]);
          result = 1;
          break;
        case 2:
          Serial.println("Control 2");
          banderaTempo = false;
          controlFunction(&c2,insR[1]);
          result = 2;
          break;
        default:
          Serial.println("Error Control");
          result = 0;
      }
     }
    
  return result;
}

void print_vector(int *p,int t){
  int i;
  for(i=0;i<t;i++){
    Serial.print(i);
    Serial.print(",");
    Serial.println(p[i]);
  }
}
