#include <Wire.h> 
#define VSENSE A2
#define PWM 3
float V_MAX = 20;
#define MUESTRAS 50
int RawValue= 0;
int ADCvRef = 0;
int i = 0;
int valorPWM = 0;
double volts = 0;
double tensionMedida = 0;
float tensionSetPoint = 0;
float tension = 0;
float atenuacion = 1.0;
float desvioTension = 0;
//-----------------------------------------------------------
//Funcion de calculo de tension de alimentacion
//-----------------------------------------------------------
int vRefADC(){
   long result;
   ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
   delay(2);
   ADCSRA |= _BV(ADSC);
   while (bit_is_set(ADCSRA,ADSC));
   result = ADCL;
   result |= ADCH<<8;
   result = 1125300L / result;
   return result;
}
//-----------------------------------------------------------
//Funcion Setup de Configuracion
//-----------------------------------------------------------
void setup(){
   Serial.begin(115200);
   ADCvRef = vRefADC();
   Serial.print("Tension Verf:  ");
   Serial.print(ADCvRef);
   Serial.println(" mV");
   delay(2000);
   pinMode(PWM, OUTPUT);
   valorPWM=128;
   analogWrite(PWM, valorPWM);
   delay(2000);
}
//-----------------------------------------------------------
//Funcion de medicion de SetPoint
//-----------------------------------------------------------
float mideSetPoint(){
   tensionSetPoint = 15;
   delay(1);
   return (tensionSetPoint/1000)*3.7;
}
//-----------------------------------------------------------
//Funcion de medicion de Tension
//-----------------------------------------------------------
float mideTension(){
   volts = 0;
   for(i=0;i<MUESTRAS;i++){
      vRefADC();
      RawValue = analogRead(VSENSE);
      tensionMedida = (RawValue / 1023.0) * ADCvRef;
      volts = volts + tensionMedida;
      delay(1);
   }
   tension = ((((volts / MUESTRAS) / 1000) / atenuacion) - desvioTension)*3.7;
   return tension;
}
//-----------------------------------------------------------
//Controla el PWM
//-----------------------------------------------------------
void ajustaPWM(){
   if(tension < V_MAX){
      if(valorPWM>0)
         valorPWM--;
      delay(1);
      analogWrite(PWM, valorPWM);
      Serial.print("PWM:");
      Serial.println(valorPWM); 
   }else{
      if(valorPWM<255)
         valorPWM++;
      delay(1);
      analogWrite(PWM, valorPWM);
      Serial.print("PWM:");
      Serial.println(valorPWM); 
   }
}   
//-----------------------------------------------------------
//Funcion Repetitiva
//-----------------------------------------------------------
void loop(){
   V_MAX = mideSetPoint();
   mideTension();
   ajustaPWM();
}
