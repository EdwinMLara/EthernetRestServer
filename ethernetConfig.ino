void ethernetConfiguration(){
  byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
  byte ip[] = {10, 0, 0, 208};
  byte gateway[] = {10, 0, 0, 1};
  byte subnet[] = {255, 255, 255, 0};

  //Ethernet.begin(mac, ip, gateway, subnet);
  Ethernet.begin(mac);

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
