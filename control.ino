void apagar_rele(){
 digitalWrite(2,LOW);
 delay(1);
 digitalWrite(7,LOW);
 delay(1);
 digitalWrite(8,LOW);
 delay(1);
 digitalWrite(12,LOW);
 delay(1);
}

void lunchPulse(uint8_t rele){
  digitalWrite(rele,HIGH);
  Serial.println("Encendiendo relevadores internos");
}

void controlFunction (struct Control *c,int state){
  banderaRe = true;
  startmillis = millis();
  if(state){
    c->state = true;
    lunchPulse(c->relevadoresC[0]);
  }else{
    c->state = false;
    lunchPulse(c->relevadoresC[1]);
  }
}
