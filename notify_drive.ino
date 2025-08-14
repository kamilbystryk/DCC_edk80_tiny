

//produkcja

void notifyDccMsg( DCC_MSG * Msg)
{
  byte c = 0;

  for (uint8_t i = 0; i < Msg->Size; i++)
  {
    if (Msg->Data[i] == 63)
      c = i;
    //Serial.print(Msg->Data[i], HEX);
    //Serial.write(' ');
  }

  if (c > 1 && bitRead(number53, 7) == 1  && locoH > 192)
  {
    //if (Msg->Data[c-2] == 193 && Msg->Data[c-1] == 178)
    //Serial.println("loco reading");

    if (Msg->Data[c - 2] == locoH && Msg->Data[c - 1] == locoL)
    {
      //Serial.println("locoH");
      if (Msg->Data[c + 1] < locoS || (Msg->Data[c + 1] > 127 && Msg->Data[c + 1] < (127 + locoS)))
      {
        digitalWrite(FunctionPin20, ACT);
        digitalWrite(FunctionPin21, ACT);
      }
      else
      {
        digitalWrite(FunctionPin20, NACT);
        digitalWrite(FunctionPin21, NACT);
      }
    }
  }
  else if (c == 1 && bitRead(number53, 7) == 1  && locoH == 192)
  {
    //if (Msg->Data[c-2] == 193 && Msg->Data[c-1] == 178)
    //Serial.println("loco reading");

    if (Msg->Data[c - 1] == locoL)
    {
      //Serial.print("locoL ");
      if (Msg->Data[c + 1] < locoS || (Msg->Data[c + 1] > 127 && Msg->Data[c + 1] < (127 + locoS)))
      {
        //Serial.println("set");
        digitalWrite(FunctionPin20, ACT);
        digitalWrite(FunctionPin21, ACT);
      }
      else
      {
        //Serial.println("clear");
        digitalWrite(FunctionPin20, NACT);
        digitalWrite(FunctionPin21, NACT);
      }
    }
  }
}

// Uncomment the #define beNACT to print all Function Packets
#define NOTIFY_DCC_FUNC
#ifdef  NOTIFY_DCC_FUNC
void notifyDccFunc(uint16_t Addr, DCC_ADDR_TYPE AddrType, FN_GROUP FuncGrp, uint8_t FuncState)
{
  /*if (test)
    {
    Serial.print("notifyDccFunc: Addr: ");
      Serial.print(Addr, DEC);
      Serial.print( (AddrType == DCC_ADDR_SHORT) ? 'S' : 'L' );
      Serial.print("  Function Group: ");
      Serial.println(FuncGrp, DEC);
      Serial.println(Dcc.getAddr());
    }*/
  if (DCactive)
  {
    DCactive = false;
    DCCactive = true;
    DriveAll(false, false);
    if (Dcc.getCV(62) == 0) Dcc.setCV(62, 255);
  }


  if ((Addr == myAddress && AddrType == DCC_ADDR_SHORT && !isAddressLong) || (Addr == myAddressLong && AddrType == DCC_ADDR_LONG && isAddressLong)) {

    if (!DCCactive) DCCactive = true;
    if (DCCdefaultSchema)
    {
      DCCdefaultSchema = false;
      DriveAll(false, false);
      digitalWrite( FunctionPin11, ACT );
      digitalWrite( FunctionPin12, ACT );
    }
    switch ( FuncGrp )
    {
#ifdef NMRA_DCC_ENABLE_14_SPEED_STEP_MODE
      case FN_0:
        //Serial.print(" FN0: ");
        //Serial.println((FuncState & FN_BIT_00) ? "1  " : "0  ");
        break;
#endif

      case FN_0_4:
        if (Dcc.getCV(CV_29_CONFIG) & CV29_F0_LOCATION) // Only process Function 0 in this packet if we're not in Speed Step 14 Mode
        {
          //Serial.print(" FN 0: ");
          //Serial.println((FuncState & FN_BIT_00) ? "1  " : "0  ");
        }

        /*Serial.print(" FN 1-4: ");
          Serial.print((FuncState & FN_BIT_01) ? "1  " : "0  ");
          Serial.print((FuncState & FN_BIT_02) ? "1  " : "0  ");
          Serial.print((FuncState & FN_BIT_03) ? "1  " : "0  ");
          Serial.println((FuncState & FN_BIT_04) ? "1  " : "0  ");*/
        if (FuncState & FN_BIT_00)
        {

          out1 = true;
          if (!f0)
          {
            //Serial.println("F0 +");
            if (bitRead(cv120, 0) == 1) analogWrite( FunctionPin0, PWMvalue);
            if (!konend)
            {
              if (bitRead(cv120, 1) == 1) analogWrite( FunctionPin11, PWMvalue2);
              if (bitRead(cv120, 2) == 1) analogWrite( FunctionPin12, PWMvalue3);
            }
          }
          f0 = true;
        }
        else
        {
          if (f0)
          {
            //Serial.println("F0 -");
            if (bitRead(cv120, 0) == 1) analogWrite( FunctionPin0, 0 );
            if (!konend)
            {
              if (bitRead(cv120, 1) == 1) analogWrite( FunctionPin11, 0);
              if (bitRead(cv120, 2) == 1) analogWrite( FunctionPin12, 0);
            }
            f0 = false;
          }
        }

        if (FuncState & FN_BIT_01)
        {
          if (!f1)
          {
            //Serial.println("F1 +");
            if (bitRead(cv121, 0) == 1) analogWrite( FunctionPin0, PWMvalue);
            if (!konend)
            {
              if (bitRead(cv121, 1) == 1) analogWrite( FunctionPin11, PWMvalue2);
              if (bitRead(cv121, 2) == 1) analogWrite( FunctionPin12, PWMvalue3);
            }
            f1 = true;
          }
        }
        else
        {
          if (f1)
          {
            //Serial.println("F1 -");
            if (bitRead(cv121, 0) == 1) analogWrite( FunctionPin0, 0 );
            if (!konend)
            {
              if (bitRead(cv121, 1) == 1) analogWrite( FunctionPin11, 0);
              if (bitRead(cv121, 2) == 1) analogWrite( FunctionPin12, 0);
            }
            f1 = false;
          }

        }
        if (FuncState & FN_BIT_02)
        {
          if (!f2)
          {
            //Serial.println("F2 +");
            if (bitRead(cv122, 0) == 1) analogWrite( FunctionPin0, PWMvalue);
            if (!konend)
            {
              if (bitRead(cv122, 1) == 1) analogWrite( FunctionPin11, PWMvalue2);
              if (bitRead(cv122, 2) == 1) analogWrite( FunctionPin12, PWMvalue3);
            }
            f2 = true;
          }
        }
        else
        {
          if (f2)
          {
            //Serial.println("F2 -");
            if (bitRead(cv122, 0) == 1) analogWrite( FunctionPin0, 0);
            if (!konend)
            {
              if (bitRead(cv122, 1) == 1) analogWrite( FunctionPin11, 0);
              if (bitRead(cv122, 2) == 1) analogWrite( FunctionPin12, 0);
            }
            f2 = false;
          }
        }
        if (FuncState & FN_BIT_03)
        {
          if (!f3)
          {
            //Serial.println("F3 +");
            if (bitRead(cv123, 0) == 1) analogWrite( FunctionPin0, PWMvalue);
            if (!konend)
            {
              if (bitRead(cv123, 1) == 1) analogWrite( FunctionPin11, PWMvalue2);
              if (bitRead(cv123, 2) == 1) analogWrite( FunctionPin12, PWMvalue3);
            }
            f3 = true;
          }
        }
        else
        {
          if (f3)
          {
            //Serial.println("F3 -");
            if (bitRead(cv123, 0) == 1) analogWrite( FunctionPin0, 0);
            if (!konend)
            {
              if (bitRead(cv123, 1) == 1) analogWrite( FunctionPin11, 0);
              if (bitRead(cv123, 2) == 1) analogWrite( FunctionPin12, 0);
            }
            f3 = false;
          }
        }
        if (FuncState & FN_BIT_04)
        {
          if (!f4)
          {
            //Serial.println("F4 +");
            if (bitRead(cv124, 0) == 1) analogWrite( FunctionPin0, PWMvalue);
            if (!konend)
            {
              if (bitRead(cv124, 1) == 1) analogWrite( FunctionPin11, PWMvalue2);
              if (bitRead(cv124, 2) == 1) analogWrite( FunctionPin12, PWMvalue3);
            }
            f4 = true;
          }
        }
        else
        {
          if (f4)
          {
            //Serial.println("F4 -");
            if (bitRead(cv124, 0) == 1) analogWrite( FunctionPin0, 0);
            if (!konend)
            {
              if (bitRead(cv124, 1) == 1) analogWrite( FunctionPin11, 0);
              if (bitRead(cv124, 2) == 1) analogWrite( FunctionPin12, 0);
            }
            f4 = false;
          }
        }
        break;
      case FN_5_8:
        if (FuncState & FN_BIT_05)
        {
          if (!f5)
          {
            //Serial.println("F5 +");
            if (bitRead(cv125, 0) == 1) analogWrite( FunctionPin0, PWMvalue);
            if (!konend)
            {
              if (bitRead(cv125, 1) == 1) analogWrite( FunctionPin11, PWMvalue2);
              if (bitRead(cv125, 2) == 1) analogWrite( FunctionPin12, PWMvalue3);
            }
            f5 = true;
          }
        }
        else
        {
          if (f5)
          {
            //Serial.println("F5 -");
            if (bitRead(cv125, 0) == 1) analogWrite( FunctionPin0, 0);
            if (!konend)
            {
              if (bitRead(cv125, 1) == 1) analogWrite( FunctionPin11, 0);
              if (bitRead(cv125, 2) == 1) analogWrite( FunctionPin12, 0);
            }
            f5 = false;
          }
        }
        if (FuncState & FN_BIT_06)
        {
          if (!f6)
          {
            if (bitRead(cv126, 0) == 1) analogWrite( FunctionPin0, PWMvalue);
            if (!konend)
            {
              if (bitRead(cv126, 1) == 1) analogWrite( FunctionPin11, PWMvalue2);
              if (bitRead(cv126, 2) == 1) analogWrite( FunctionPin12, PWMvalue3);
            }
            f6 = true;
          }
        }
        else
        {
          if (f6)
          {
            if (bitRead(cv126, 0) == 1) analogWrite( FunctionPin0, 0);
            if (!konend)
            {
              if (bitRead(cv126, 1) == 1) analogWrite( FunctionPin11, 0);
              if (bitRead(cv126, 2) == 1) analogWrite( FunctionPin12, 0);
            }
            f6 = false;
          }
        }
        if (FuncState & FN_BIT_07)
        {
          if (!f7)
          {
            if (bitRead(cv127, 0) == 1) analogWrite( FunctionPin0, PWMvalue);
            if (!konend)
            {
              if (bitRead(cv127, 1) == 1) analogWrite( FunctionPin11, PWMvalue2);
              if (bitRead(cv127, 2) == 1) analogWrite( FunctionPin12, PWMvalue3);
            }
            f7 = true;
          }
        }
        else
        {
          if (f7)
          {
            if (bitRead(cv127, 0) == 1) analogWrite( FunctionPin0, 0);
            if (!konend)
            {
              if (bitRead(cv127, 1) == 1) analogWrite( FunctionPin11, 0);
              if (bitRead(cv127, 2) == 1) analogWrite( FunctionPin12, 0);
            }
            f7 = false;
          }
        }
        if (FuncState & FN_BIT_08)
        {
          if (!f8)
          {
            if (bitRead(cv128, 0) == 1) analogWrite( FunctionPin0, PWMvalue);
            if (!konend)
            {
              if (bitRead(cv128, 1) == 1) analogWrite( FunctionPin11, PWMvalue2);
              if (bitRead(cv128, 2) == 1) analogWrite( FunctionPin12, PWMvalue3);
            }
            f8 = true;
          }
        }
        else
        {
          if (f8)
          {
            if (bitRead(cv128, 0) == 1) analogWrite( FunctionPin0, 0);
            if (!konend)
            {
              if (bitRead(cv128, 1) == 1) analogWrite( FunctionPin11, 0);
              if (bitRead(cv128, 2) == 1) analogWrite( FunctionPin12, 0);
            }
            f8 = false;
          }
        }
        break;
      case FN_9_12:
        if (FuncState & FN_BIT_09)
        {
          if (!f9)
          {
            if (bitRead(cv129, 0) == 1) analogWrite( FunctionPin0, PWMvalue);
            if (!konend)
            {
              if (bitRead(cv129, 1) == 1) analogWrite( FunctionPin11, PWMvalue2);
              if (bitRead(cv129, 2) == 1) analogWrite( FunctionPin12, PWMvalue3);
            }
            f9 = true;
          }
        }
        else
        {
          if (f9)
          {
            if (bitRead(cv129, 0) == 1) analogWrite( FunctionPin0, 0);
            if (!konend)
            {
              if (bitRead(cv129, 1) == 1) analogWrite( FunctionPin11, 0);
              if (bitRead(cv129, 2) == 1) analogWrite( FunctionPin12, 0);
            }
            f9 = false;
          }
        }
        if (FuncState & FN_BIT_10)
        {
          if (!f10)
          {
            if (bitRead(cv130, 0) == 1) analogWrite( FunctionPin0, PWMvalue);
            if (!konend)
            {
              if (bitRead(cv130, 1) == 1) analogWrite( FunctionPin11, PWMvalue2);
              if (bitRead(cv130, 2) == 1) analogWrite( FunctionPin12, PWMvalue3);
            }
            f10 = true;
          }
        }
        else
        {
          if (f10)
          {
            if (bitRead(cv130, 0) == 1) analogWrite( FunctionPin0, 0);
            if (!konend)
            {
              if (bitRead(cv130, 1) == 1) analogWrite( FunctionPin11, 0);
              if (bitRead(cv130, 2) == 1) analogWrite( FunctionPin12, 0);
            }
            f10 = false;
          }
        }
        if (FuncState & FN_BIT_11)
        {
          if (!f11)
          {
            if (bitRead(cv131, 0) == 1) analogWrite( FunctionPin0, PWMvalue);
            if (!konend)
            {
              if (bitRead(cv131, 1) == 1) analogWrite( FunctionPin11, PWMvalue2);
              if (bitRead(cv131, 2) == 1) analogWrite( FunctionPin12, PWMvalue3);
            }
            f11 = true;
          }
        }
        else
        {
          if (f11)
          {
            if (bitRead(cv131, 0) == 1) analogWrite( FunctionPin0, 0);
            if (!konend)
            {
              if (bitRead(cv131, 1) == 1) analogWrite( FunctionPin11, 0);
              if (bitRead(cv131, 2) == 1) analogWrite( FunctionPin12, 0);
            }
            f11 = false;
          }
        }
        if (FuncState & FN_BIT_12)
        {
          if (!f12)
          {
            if (bitRead(cv132, 0) == 1) analogWrite( FunctionPin0, PWMvalue);
            if (!konend)
            {
              if (bitRead(cv132, 1) == 1) analogWrite( FunctionPin11, PWMvalue2);
              if (bitRead(cv132, 2) == 1) analogWrite( FunctionPin12, PWMvalue3);
            }
            f12 = true;
          }
        }
        else
        {
          if (f12)
          {
            if (bitRead(cv132, 0) == 1) analogWrite( FunctionPin0, 0);
            if (!konend)
            {
              if (bitRead(cv132, 1) == 1) analogWrite( FunctionPin11, 0);
              if (bitRead(cv132, 2) == 1) analogWrite( FunctionPin12, 0);
            }
            f12 = false;
          }
        }
        break;
      case FN_13_20:
        if (FuncState & FN_BIT_13)
        {
          if (!f13)
          {
            if (bitRead(cv133, 0) == 1) analogWrite( FunctionPin0, PWMvalue);
            if (!konend)
            {
              if (bitRead(cv133, 1) == 1) analogWrite( FunctionPin11, PWMvalue2);
              if (bitRead(cv133, 2) == 1) analogWrite( FunctionPin12, PWMvalue3);
            }
            f13 = true;
          }
        }
        else
        {
          if (f13)
          {
            if (bitRead(cv133, 0) == 1) analogWrite( FunctionPin0, 0);
            if (!konend)
            {
              if (bitRead(cv133, 1) == 1) analogWrite( FunctionPin11, 0);
              if (bitRead(cv133, 2) == 1) analogWrite( FunctionPin12, 0);
            }
            f13 = false;
          }
        }

        if (FuncState & FN_BIT_17)
        {
          if (cv53_1)
          {
            if (konend)
            {
              konend = false;
            }
          }
        }
        else
        {

        }
        if (FuncState & FN_BIT_20)
        {
          if (bitRead(cv140, 0) == 1) analogWrite( FunctionPin0, PWMvalue);
          if (!konend)
          {
            if (bitRead(cv140, 1) == 1) analogWrite( FunctionPin11, PWMvalue2);
            if (bitRead(cv140, 2) == 1) analogWrite( FunctionPin12, PWMvalue3);
          }
        }
        else
        {
          if (bitRead(cv140, 0) == 1) analogWrite( FunctionPin0, 0 );
          if (!konend)
          {
            if (bitRead(cv140, 1) == 1) analogWrite( FunctionPin11, 0);
            if (bitRead(cv140, 2) == 1) analogWrite( FunctionPin12, 0);
          }
        }
        break;
      case FN_21_28:
        if (FuncState & FN_BIT_21)
        {
          if (bitRead(cv141, 0) == 1) analogWrite( FunctionPin0, PWMvalue);
          if (!konend)
          {
            if (bitRead(cv141, 1) == 1) analogWrite( FunctionPin11, PWMvalue2);
            if (bitRead(cv141, 2) == 1) analogWrite( FunctionPin12, PWMvalue3);
          }
        }
        else
        {
          if (bitRead(cv141, 0) == 1) analogWrite( FunctionPin0, 0 );
          if (!konend)
          {
            if (bitRead(cv141, 1) == 1) analogWrite( FunctionPin11, 0);
            if (bitRead(cv141, 2) == 1) analogWrite( FunctionPin12, 0);
          }
        }
        if (FuncState & FN_BIT_22)
        {
          if (bitRead(cv142, 0) == 1) analogWrite( FunctionPin0, PWMvalue);
          if (!konend)
          {
            if (bitRead(cv142, 1) == 1) analogWrite( FunctionPin11, PWMvalue2);
            if (bitRead(cv142, 2) == 1) analogWrite( FunctionPin12, PWMvalue3);
          }
        }
        else
        {
          if (bitRead(cv142, 0) == 1) analogWrite( FunctionPin0, 0 );
          if (!konend)
          {
            if (bitRead(cv142, 1) == 1) analogWrite( FunctionPin11, 0);
            if (bitRead(cv142, 2) == 1) analogWrite( FunctionPin12, 0);
          }
        }
        /*Serial.print(" FN 21-28: ");
          Serial.print((FuncState & FN_BIT_21) ? "1  ": "0  ");
          Serial.print((FuncState & FN_BIT_22) ? "1  ": "0  ");
          Serial.print((FuncState & FN_BIT_23) ? "1  ": "0  ");
          Serial.print((FuncState & FN_BIT_24) ? "1  ": "0  ");
          Serial.print((FuncState & FN_BIT_25) ? "1  ": "0  ");
          Serial.print((FuncState & FN_BIT_26) ? "1  ": "0  ");
          Serial.print((FuncState & FN_BIT_27) ? "1  ": "0  ");
          Serial.println((FuncState & FN_BIT_28) ? "1  ": "0  ");*/
        break;

    }
  }
}
#endif



void ProceedReedSwitch()
{
  Serial.println("ProceedReedSwitch");
  {
    change(true);
  }
}

void change(bool add)
{
  konend = true;
  savekontaktron2 = true;

  if (add)
  {
    mod++;
  }
  if (mod == 0 || mod > 6)
  {
    mod = 1;
  }
  if (test)
  {
    Serial.print("change: ");
    Serial.println(mod);
  }
  if (WithEnds)
  {
    if (mod == 1)
    {
      if (reedLight)
      {
        analogWrite( FunctionPin0, 0);
        f0 = false;
      }
      digitalWrite(FunctionPin11, ACT);
      digitalWrite(FunctionPin12, ACT);
      //light = false;
      kon2 = false;
      kon3 = false;
    }
    if (mod == 2)
    {
      if (reedLight)
      {
        analogWrite( FunctionPin0, 0);
        f0 = false;
      }
      digitalWrite(FunctionPin11, NACT);
      digitalWrite(FunctionPin12, ACT);
      //light = false;
      kon2 = true;
      kon3 = false;
    }
    if (mod == 3)
    {
      if (reedLight)
      {
        analogWrite( FunctionPin0, 0);
        f0 = false;
      }
      digitalWrite(FunctionPin11, ACT);
      digitalWrite(FunctionPin12, NACT);
      //light = false;
      kon2 = false;
      kon3 = true;
    }
    if (mod == 4)
    {
      if (reedLight)
      {
        analogWrite( FunctionPin0, PWMvalue);
        f0 = true;
      }
      digitalWrite(FunctionPin11, ACT);
      digitalWrite(FunctionPin12, ACT);
      //light = false;
      kon2 = false;
      kon3 = false;
    }
    if (mod == 5)
    {
      if (reedLight)
      {
        analogWrite( FunctionPin0, PWMvalue);
        f0 = true;
      }
      digitalWrite(FunctionPin11, NACT);
      digitalWrite(FunctionPin12, ACT);
      //light = false;
      kon2 = true;
      kon3 = false;
    }
    if (mod == 6)
    {
      if (reedLight)
      {
        analogWrite( FunctionPin0, PWMvalue);
        f0 = true;
      }

      digitalWrite(FunctionPin11, ACT);
      digitalWrite(FunctionPin12, NACT);
      //light = false;
      kon2 = false;
      kon3 = true;
    }
  }
  else
  {
    if (mod == 1 || mod == 3 || mod == 5)
    {
      if (reedLight)
      {
        analogWrite( FunctionPin0, PWMvalue);
        f0 = true;
      }
    }
    if (mod == 2 || mod == 4 || mod == 6)
    {
      if (reedLight)
      {
        analogWrite( FunctionPin0, 0);
        f0 = false;
      }
    }
  }
}

void DriveAll(bool value, bool ending)
{
  if (value)
  {
    analogWrite( FunctionPin0, 255);
    if (ending)
    {
      digitalWrite(FunctionPin11, ACT);
      digitalWrite(FunctionPin12, ACT);
    }
  }
  else
  {
    analogWrite( FunctionPin0, 0);
    digitalWrite(FunctionPin11, NACT);
    digitalWrite(FunctionPin12, NACT);
  }
}
