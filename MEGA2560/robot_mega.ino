#include <SoftwareSerial.h>

const byte rxPin = 10;
const byte txPin = 11;

// Set up a new SoftwareSerial object, pin MEGA (5,6,7 ==> OFF, the rest are ON)
SoftwareSerial mySerial (rxPin, txPin);


#define MSG_BUFFER_SIZE	(50)
const int maxDataLength = 20; 

unsigned long previousMillis = 0;  
const long interval = 50; 
int target_dof1, actual_dof1, target_dof2, actual_dof2, target_dof3, actual_dof3, target_dof4, actual_dof4, target_dof5, actual_dof5; 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  mySerial.begin(115200);
  Serial1.begin(115200);
  Serial2.begin(115200);
  Serial3.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
   //Check to see if anything is available in the serial receive buffer
  while (Serial3.available() > 0)
  {
    //Create a place to hold the incoming message
    static char message[MSG_BUFFER_SIZE];
    static unsigned int message_pos = 0;
    //Read the next available byte in the serial receive buffer
    char inByte = Serial3.read();
    //Message coming in (check not terminating character) and guard for over message size
    if ( inByte != '\n' && (message_pos < MSG_BUFFER_SIZE - 1) )
    {
      //Add the incoming byte to our message
      message[message_pos] = inByte;
      message_pos++;
    }
    //Full message received...
    else
    {
      //Add null character to string
      message[message_pos] = '\0';
      //Print the message (or do other things)
      Serial.println(message);  
      parseData(message);
      //Reset for the next message
      message_pos = 0;
    }
  }

  while (Serial1.available() > 0)
  {
    static char message1[MSG_BUFFER_SIZE];
    static unsigned int message_pos1 = 0;
    char inByte1 = Serial1.read();
    if ( inByte1 != '\n' && (message_pos1 < MSG_BUFFER_SIZE - 1) )
    {
      message1[message_pos1] = inByte1;
      message_pos1++;
    }
    else
    {
      message1[message_pos1] = '\0';
      
      int data[maxDataLength];
      int dataLength = 0;
      splitData(message1, data, &dataLength); // Panggil fungsi splitData

      // Pastikan ada setidaknya dua elemen dalam array
      if (dataLength >= 4) {
        target_dof1 = data[0]/(61.2*2);
        actual_dof1 = data[1]/(61.2*2);  
        target_dof2 = data[2]/111.3;
        actual_dof2 = data[3]/111.3;        
      }

      //Serial.println(message1);
      message_pos1 = 0;
    }
  }

  while (Serial2.available() > 0)
  {
    static char message2[MSG_BUFFER_SIZE];
    static unsigned int message_pos2 = 0;
    char inByte2 = Serial2.read();
    if ( inByte2 != '\n' && (message_pos2 < MSG_BUFFER_SIZE - 1) )
    {
      message2[message_pos2] = inByte2;
      message_pos2++;
    }
    else
    {
      message2[message_pos2] = '\0';

      int data[maxDataLength];
      int dataLength = 0;
      splitData(message2, data, &dataLength); // Panggil fungsi splitData

      // Pastikan ada setidaknya dua elemen dalam array
      if (dataLength >= 2) {
        target_dof3 = data[0]/(48.4*2);
        actual_dof3 = data[1]/(48.4*2);         
      }

      //Serial.println(message2);
      message_pos2 = 0;
    }
  }

  while (mySerial.available() > 0)
  {
    static char message3[MSG_BUFFER_SIZE];
    static unsigned int message_pos3 = 0;
    char inByte3 = mySerial.read();
    if ( inByte3 != '\n' && (message_pos3 < MSG_BUFFER_SIZE - 1) )
    {
      message3[message_pos3] = inByte3;
      message_pos3++;
    }
    else
    {
      message3[message_pos3] = '\0';
      //Serial.println(message3);
      int data[maxDataLength];
      int dataLength = 0;
      splitData(message3, data, &dataLength); // Panggil fungsi splitData

      // Pastikan ada setidaknya dua elemen dalam array
      if (dataLength >= 4) {
        target_dof4 = data[0]/(24.5*2);
        actual_dof4 = data[1]/(24.5*2);  
        target_dof5 = data[2]/(24.5*2);
        actual_dof5 = data[3]/(24.5*2);    
      }
      message_pos3 = 0;
    }
  }


  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;    
    sendDataToSerial3(target_dof1, actual_dof1, target_dof2, actual_dof2, target_dof3, actual_dof3, target_dof4, actual_dof4, target_dof5, actual_dof5);
  }
}

void sendDataToSerial3(int target_dof1, int actual_dof1, int target_dof2, int actual_dof2, int target_dof3, int actual_dof3, int target_dof4, int actual_dof4, int target_dof5, int actual_dof5) {
    // Gabungkan semua data menjadi satu string dengan pemisah ';'
    String data = String(target_dof1) + ";" + String(actual_dof1) + ";" + String(target_dof2) + ";" + String(actual_dof2) + ";" + String(target_dof3) + ";" + String(actual_dof3) + ";" + String(target_dof4) + ";" + String(actual_dof4) + ";" + String(target_dof5) + ";" + String(actual_dof5);
    
    // Kirim data tersebut melalui Serial3
    Serial3.println(data);
}


void parseData(String input) {
  input = input.substring(1, input.length() - 1); // Remove '#' and '>'
  String joinValues[5]; // Assuming 5 values

  int index = 0;
  int lastIndex = 0;

  while (index < input.length()) {
    int separatorIndex = input.indexOf(';', index);
    if (separatorIndex == -1) {
      separatorIndex = input.length();
    }

    String valueStr = input.substring(index, separatorIndex);
    joinValues[lastIndex] = valueStr;
    lastIndex = lastIndex + 1;

    index = separatorIndex + 1;
  }

  // Print the parsed values
  for (int i = 0; i < 5; i++) {
    Serial.print("Join ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.println(joinValues[i]);
  }

  //Uno1
  String dataUno1 = joinValues[0] + ";" + joinValues[1] + ">";
  Serial1.println(dataUno1);
  Serial.println(dataUno1);
  //Uno2
  String dataUno2 = joinValues[2] + ";" + "0" + ">";
  Serial2.println(dataUno2);
  Serial.println(dataUno2);
  //Uno3
  String dataUno3 = joinValues[3] + ";" + joinValues[4] + ">";
  mySerial.println(dataUno3);
  Serial.println(dataUno3);
}


void splitData(char* data, int* outputArray, int* outputLength) {
  char* token = strtok(data, ",");
  int count = 0;
  while (token != NULL && count < maxDataLength) {
    outputArray[count] = atoi(token); // Konversi ke integer dan simpan ke array
    count++;
    token = strtok(NULL, ",");
  }
  *outputLength = count; // Simpan panjang data yang dihasilkan
}