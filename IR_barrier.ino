/* IR Barrier 0.1 coded by Jónatan Rojas
Trigger for EOS 450D, with wire connection. It is triggered when an IR beam is crossed.
All the "Serial" lines can be commented or deleted, ar just for monitoring.
Spanish version: Disparador de cámara EOS 450D con conexión mediante cable al cortarse la señal entre un emisor y un receptor IR
Todas las lineas con "Serial" se pueden comentar o borrar, son solo para hacerle el seguimiento al programa.

*/

const int photoResPin = A0;  //define a pin for Photo resistor || pin para la fotoresistencia.
const int ledPinAlign=11;     //define a pin for LED indicator for align || pin para mostrar que el ir está alineado.
const int potPin = A2;    // select the input pin for the potentiometer || pin para leer información del potenciómetro.
const int onPin = 3; //define a pin for Button. || pin para el pulsador
const int activeLedPin = 4; // select a pin for LED when the trigger is on. || pin para el led indicador de que el disparador está activo.
const int triggerPin = 9; //select a pin for triggering the camera. || pin para accionar la cámara.

int sensorValue = 0;  // variable to store the value coming from the sensor || variable para guardar el valor del potenciómetro.
int lastStateButton = 0; // variable to store the state of the button (ON/OFF). || Variable para guardar el status del sistema (ON/OFF)
int stateButton = 0; // variable to store the actual state of the button. || Variable donde guardaremos el valor del botón en cada ciclo.
void setup()
{
    Serial.begin(9600);  //Begin serial communication || Iniciamos la comunicación. 
    pinMode( ledPinAlign, OUTPUT);
    pinMode( activeLedPin, OUTPUT);
    pinMode( triggerPin, OUTPUT);
    pinMode( onPin, INPUT); 
    digitalWrite(triggerPin, LOW);
}

void loop()
{
   stateButton = digitalRead(onPin); // read if the button is pushed. || Guardamos en la variable el valor del pulsador (para saber si lo hemos pulsado).
   sensorValue = analogRead(potPin);    // read the value from the sensor || Guardamos el valor del potenciómetro.
   Serial.println("Este es el valor del sensor, que es la mitad del delay:");
   Serial.println(sensorValue); //Write the value of potenciometer to the serial monitor. || Muestra el valor del potenciómetro.
   Serial.println("Valor de la fotoresistencia de IR");
   Serial.println(analogRead(photoResPin)); //Write the value of the photoresistor to the serial monitor. || Muestra el valor de la fotoresistencia. 
   Serial.println("Valor del button");
   Serial.println(stateButton);
    if (stateButton == 1 && lastStateButton == 0){ //If button pushed and the status was OFF, active the trigger and the indicator Pin, change status to ON. || Si se pulsa el botón y el estado anterior era OFF, activa el disparador y el led indicativo. Cambia el status a ON
      digitalWrite(activeLedPin, HIGH);
      lastStateButton = 1;
      Serial.println("State cambiado a 1");
    }
    else if (stateButton == 1 && lastStateButton == 1) { //If button pushed and the status was ON, deactive the trigger and the indicator Pin, change status to OFF. || Desactiva el disparador, el led indicativo y cambia el estado a OFF.
      digitalWrite(activeLedPin, LOW);
      lastStateButton = 0;
      Serial.println("State cambiado a 0");
    }
   
    if (analogRead(photoResPin) != 0) {
       analogWrite(ledPinAlign, analogRead(photoResPin)*6);  //send the value to the ledPin if it is aligned || ilumina el led si está alineado, cambiando la intensidad depende de la cercanía de este.
       Serial.println("laser alineado");
   }
    else {
      delay(sensorValue*2); //short delay for faster response to light. || Cuanto menor sea el valor del potenciómetro, mayor será el retardo entre el evento y el disparo de la cámara.
      digitalWrite(ledPinAlign, LOW); 
       Serial.println("laser roto");
      if (lastStateButton == 1) { // if state is ON (and the IR beam is cutted), trigger the camera || Si el disparador está activo, y el infrarrojo está cortado dispara la cámara.
        digitalWrite(triggerPin, HIGH);
        Serial.println("Dispara");
        delay(60); //Needed to let the camera know that has to be triggered. || Necesario para que le de tiempo a la cámara de saber que tiene que disparar.
        digitalWrite(triggerPin, LOW);
        delay(800); //To prevent the camera to shot multiple pics too fast || Añadir para que no se active demasiado rápido el disparador entre foto y foto.
      }
    }

}

