void notifyCVChange( uint16_t CV, uint8_t Value)
{
  if (test)
  {
    Serial.print("set CV: ");
    Serial.println(CV);
    Serial.print("Value: ");
    Serial.println(Value);
  }
  if (CV == 1 || CV == 55)
  {
    myAddress = Value;
    Dcc.setCV(1, Value);
    digitalWrite( DccAckPin, HIGH );
    delay( 8 );
    digitalWrite( DccAckPin, LOW );
    delay( 100);

    resetFunc();
  }
  /*if (CV == 8)
    {
      if (test)
    {
    Serial.println("FACTORY RESET!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    }

    //FactoryReset
    Dcc.setCV(7, Version);
    Dcc.setCV(47, 55);
    Dcc.setCV(48, 55);
    Dcc.setCV(49, 170);
    Dcc.setCV(50, 170);
    Dcc.setCV(51, 51);
    Dcc.setCV(52, 51);
    Dcc.setCV(53, 0);

    }*/

  if (CV == 47)
  {
    f13cv47L = Value;
  }
  if (CV == 48)
  {
    f13cv48H = Value;
  }
  if (CV == 49)
  {
    f14cv49L = Value;
  }
  if (CV == 50)
  {
    f14cv50H = Value;
  }
  if (CV == 51)
  {
    f15cv51L = Value;
  }
  if (CV == 52)
  {
    f15cv52H = Value;
  }
  if (CV == 53)
  {
    int number53 = Value;
    if (bitRead(number53, 0) == 1)
    {
      cv53_0 = true;
    }
    else
    {
      cv53_0 = false;
    }
    if (bitRead(number53, 1) == 1)
    {
      cv53_1 = true;
    }
    else
    {
      cv53_1 = false;
    }
    if (bitRead(number53, 5) == 1)
    {
      reedLight = true;
    }
  }
  if (CV == 54)
  {
    PWMvalue = 255-Value;
  }
  if (CV == 56)
  {
    Dcc.setCV(17, Value);
  }
  
  if (CV == 57)
  {
    Dcc.setCV(18, Value);
  }
  if (CV == 120)
  {
    cv120 = Value;
  }
  if (CV == 121)
  {
    cv121 = Value;
  }
  if (CV == 122)
  {
    cv122 = Value;
  }
  if (CV == 123)
  {
    cv123 = Value;
  }
  if (CV == 124)
  {
    cv124 = Value;
  }
  if (CV == 125)
  {
    cv125 = Value;
  }
  if (CV == 126)
  {
    cv126 = Value;
  }
  if (CV == 127)
  {
    cv127 = Value;
  }
  if (CV == 128)
  {
    cv128 = Value;
  }
  if (CV == 129)
  {
    cv129 = Value;
  }
  if (CV == 130)
  {
    cv130 = Value;
  }
  if (CV == 131)
  {
    cv131 = Value;
  }
  if (CV == 132)
  {
    cv132 = Value;
  }
  if (CV == 133)
  {
    cv133 = Value;
  }
  if (CV == 140)
  {
    cv140 = Value;
  }
  if (CV == 141)
  {
    cv141 = Value;
  }
  if (CV == 142)
  {
    cv142 = Value;
  }
  if (CV == 230) locoH = Value;
  if (CV == 231) locoL = Value;
  if (CV == 232) locoS = Value;
  if (CV == 233) PWMvalue2 = 255-Value;
  if (CV == 234) PWMvalue3 = 255-Value;
}

void CVinit()
{
  if (Dcc.getCV(1) == 255)
  {
    FactoryDefaultCVIndex = sizeof(FactoryDefaultCVs) / sizeof(CVPair);
  }
  if (test)
  {
    Serial.print("CV7=");
    Serial.println(Dcc.getCV(7));
    Serial.print("CV17=");
    Serial.println(Dcc.getCV(17));
    Serial.print("CV18=");
    Serial.println(Dcc.getCV(18));
    Serial.print("CV29=");
    Serial.println(Dcc.getCV(29));
    Serial.print("CV53=");
    Serial.println(Dcc.getCV(53));
    Serial.print("CV1=");
    Serial.println(Dcc.getCV(1));
    Serial.print("address=");
    Serial.println(Dcc.getAddr());
  }
  int number53 = Dcc.getCV(53);

  if (bitRead(number53, 0) == 1)
  {
    cv53_0 = true;
  }
  else
  {
    cv53_0 = false;
  }
  //aktywacja kontaktronu
  if (bitRead(number53, 1) == 1)
  {
    cv53_1 = true;
  }
  else
  {
    cv53_1 = false;
  }
  if (bitRead(number53, 5) == 1)
  {
    reedLight = true;
  }

  PWMvalue = Dcc.getCV(54);
  PWMvalue2 = 255-Dcc.getCV(233);
  PWMvalue3 = 255-Dcc.getCV(234);
  myAddress = Dcc.getCV(1);
  if (Dcc.getCV(29) & 0b00100000)
  {
    myAddressLong = ((Dcc.getCV(CV_MULTIFUNCTION_EXTENDED_ADDRESS_MSB) - 192) << 8) | Dcc.getCV(CV_MULTIFUNCTION_EXTENDED_ADDRESS_LSB);
    isAddressLong = true;
    if (test)
    {
      Serial.print("myAddressLong=");
      Serial.println(myAddressLong);
    }
  }
  f13cv47L = Dcc.getCV(47);
  f13cv48H = Dcc.getCV(48);
  f14cv49L = Dcc.getCV(49);
  f14cv50H = Dcc.getCV(50);
  f15cv51L = Dcc.getCV(51);
  f15cv52H = Dcc.getCV(52);
  cv120 = Dcc.getCV(120);
  cv121 = Dcc.getCV(121);
  cv122 = Dcc.getCV(122);
  cv123 = Dcc.getCV(123);
  cv124 = Dcc.getCV(124);
  cv125 = Dcc.getCV(125);
  cv126 = Dcc.getCV(126);
  cv127 = Dcc.getCV(127);
  cv128 = Dcc.getCV(128);
  cv129 = Dcc.getCV(129);
  cv130 = Dcc.getCV(130);
  cv131 = Dcc.getCV(131);
  cv132 = Dcc.getCV(132);
  cv133 = Dcc.getCV(133);
  cv140 = Dcc.getCV(140);
  cv141 = Dcc.getCV(141);
  cv142 = Dcc.getCV(142);
}

void notifyCVAck(void)
{
  //Serial.println("notifyCVAck") ;

  digitalWrite( DccAckPin, HIGH );
  delay( 8 );
  digitalWrite( DccAckPin, LOW );
}
