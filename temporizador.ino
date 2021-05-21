
void iniciarBanderasTempo(){
  uint8_t i;
  for(i=0;i<4;i++){
    banderasCT[i] = true;
  }
}


void temporizador(unsigned long countTime){
  Serial.println(countTime);
  if((countTime >= confTempo[0]) && banderasCT[0]){
    Serial.println("Entro en 1");
    banderasCT[0] = false;
    controlFunction(&c1,0);
  }
  if((countTime >= confTempo[1]) && banderasCT[1]){
    Serial.println("Entro en 2");
    banderasCT[1] = false;
    controlFunction(&c1,1);
  }
  if((countTime >= confTempo[2]) && banderasCT[2]){
    Serial.println("Entro en 3");
    banderasCT[2] = false;
    controlFunction(&c1,0);
  }
  if((countTime >= confTempo[3]) && banderasCT[3]){
    Serial.println("Reinicio"); 
    banderasCT[3] = false;
    controlFunction(&c1,1);
    iniciarBanderasTempo();
    desfase = 0;
    tempoStartMillis = millis();
  }

}

void desFaseTime(int desfaseaux){
  unsigned long aux = desfaseaux * 60000;
  desfase += aux;
}
