#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "MQTTClient.h"

#include "ir-slinger/irslinger.h" // FIXME: dir

#define ADDRESS     "t20:1883"
#define CLIENTID    "<<clientId>>" // FIXME

typedef struct {
  bool on;
  bool timer;
  bool unitF;
  int temperature;
  int timer_value;
  int mode;
  int fan;
} dl_aircon_msg_t;

unsigned long dl_assemble_msg(dl_aircon_msg_t* msg);
bool dl_decode_msg(dl_aircon_msg_t* msg);
void dl_print_msg(dl_aircon_msg_t *msg);
unsigned char bit_reverse( unsigned char x );

/*
 * DeLonghi PAC N81 IR Commands
 * 
 * 32 Bit Value
 * NEC Encoding
 * 
 * +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 * |   0x01    |   0x02    |  |Lo|Md|Hi|Ac|  |Dh|Bl|
 * +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 * | Timer     |  |CF|T |ON|    Temperature        |
 * +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 * 
 * 0x01 and 0x02 is a fixed prefix
 * Lo / Md / Hi are bits for the fan setting
 * Cl / Dh / Bl are mode switches
 *    Ac  Airconditioning
 *    Dh  Dehumidify
 *    Bl  Blow (Fan Mode)
 * Timer is the set value for timer control.     
 *    Set to 1 even if not active via the bit
 * CF   Celsius / Fahrenheit Setting. Low C, High F
 * T    Timer Low Off, High On
 * On   On/Off Switch
 * 
 * Temperature    8-Bit Temperature Value
 * 
 * 0-16 means 16-32°C
 * 61-89 means 61-89°F
 * 
 * Both the temperature and timer have their bit-order reversed.
 */

#define MIN(X, Y)  ((X) < (Y) ? (X) : (Y))
#define MAX(X, Y)  ((X) > (Y) ? (X) : (Y))
#define constrain(X,Y,Z) (MIN(MAX(X,Y),Z))


unsigned long dl_assemble_msg(dl_aircon_msg_t* msg){
  unsigned long buf = 0x12000000;

  if (!msg->unitF){
    msg->temperature = constrain(msg->temperature, 16, 32);
    buf |= bit_reverse(msg->temperature-16);
  }else{
    msg->temperature = constrain(msg->temperature, 61, 89);
    buf |= bit_reverse(msg->temperature);
  }

  buf |= bit_reverse(msg->timer_value) << 8;

  if (msg->on) buf |= 0x1 << 8;
  if (msg->timer) buf |= 0x1 << 9;
  if (msg->unitF) buf |= 0x1 << 10;

  if (msg->mode == 8 || msg->mode == 2 || msg->mode == 1){
    buf |= msg->mode << 16;
  }else{
    buf |= 0x8 << 16;
  }

  if (msg->fan == 4 || msg->fan == 2 || msg->fan == 1){
    buf |= msg->fan << 20;
  }else{
    buf |= 0x2 << 20;
  }

  return buf;
}

/*
bool dl_decode_msg(unsigned long code, dl_aircon_msg_t* msg){
  
  msg->on = ( (code) >> 8 ) & 0x01;
  msg->timer = ( (code) >> 9 ) & 0x01;
  msg->unitF = ( (code) >> 10 ) & 0x01;

  msg->temperature = bit_reverse(code & 0xFF);
  if (!msg->unitF) { //degC has a -16 offset
      msg->temperature += 16;
  }

  msg->timer_value = bit_reverse((code >> 12) & 0xF) >> 4;
  msg->mode = (code >> 16) & 0xF;
  msg->fan = (code >> 20) & 0xF;
}

*/

// Reverse the order of bits in a byte. 
// I.e. MSB is swapped with LSB, etc. 
unsigned char bit_reverse( unsigned char x ) 
{ 
   x = ((x >> 1) & 0x55) | ((x << 1) & 0xaa); 
   x = ((x >> 2) & 0x33) | ((x << 2) & 0xcc); 
   x = ((x >> 4) & 0x0f) | ((x << 4) & 0xf0); 
   return x;    
}


// Refs: 
// https://stackoverflow.com/questions/111928/is-there-a-printf-converter-to-print-in-binary-format
// https://stackoverflow.com/questions/41384262/convert-string-to-binary-in-c
void printBits(size_t const size, void const * const ptr, char *result)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;

    for (i=size-1;i>=0;i--)
    {
        for (j=7;j>=0;j--)
        {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    puts("");
}

char* returnBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;
    char *binary = malloc(size*8+1); // each char is one byte (8 bits) and + 1 at the end for null terminator
    binary[0] = '\0';

    for (i=size-1;i>=0;i--)
    {
        for (j=7;j>=0;j--)
        {
            byte = (b[i] >> j) & 1;
            if (byte) { 
		strcat (binary,"1");
	    }else{
		strcat(binary,"0");
	    }
        }
    }
    return binary;
}




void publish(MQTTClient client, char* topic, char* payload) {
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    pubmsg.payload = payload;
    pubmsg.payloadlen = strlen(pubmsg.payload);
    pubmsg.qos = 2;
    pubmsg.retained = 0;
    MQTTClient_deliveryToken token;
    MQTTClient_publishMessage(client, topic, &pubmsg, &token);
    MQTTClient_waitForCompletion(client, token, 1000L);
    printf("Message '%s' with delivery token %d delivered\n", payload, token);
}

int on_message(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    char* payload = message->payload;
    printf("Received operation %s\n", payload);
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}


int send_ir (char* msg)
{

        uint32_t outPin = 24;            // The Broadcom pin number the signal will be sent on
        int frequency = 38000;           // The frequency of the IR signal in Hz
        double dutyCycle = 0.5;          // The duty cycle of the IR signal. 0.5 means for every cycle,
                                         // the LED will turn on for half the cycle time, and off the other half
        int leadingPulseDuration = 9000; //9102; // The duration of the beginning pulse in microseconds
        int leadingGapDuration = 4500;//4450;   // The duration of the gap in microseconds after the leading pulse
        int onePulse = 560;              // The duration of a pulse in microseconds when sending a logical 1
        int zeroPulse = 560;             // The duration of a pulse in microseconds when sending a logical 0
        int oneGap = 1687; //1600;               // The duration of the gap in microseconds when sending a logical 1
        int zeroGap = 560;               // The duration of the gap in microseconds when sending a logical 0
        int sendTrailingPulse = 1;       // 1 = Send a trailing pulse with duration equal to "onePulse"
                                         // 0 = Don't send a trailing pulse

        int result = irSling(
                outPin,
                frequency,
                dutyCycle,
                leadingPulseDuration,
                leadingGapDuration,
                onePulse,
                zeroPulse,
                oneGap,
                zeroGap,
                sendTrailingPulse,
                msg);
   return result;
}


int main (void)
{
    MQTTClient client;
    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    conn_opts.username = "<<tenant_ID>>/<<username>>"; // FIXME
    conn_opts.password = "<<password>>";

    MQTTClient_setCallbacks(client, NULL, NULL, on_message, NULL);

    int rc;
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        exit(-1);
    }
    //create device
    publish(client, "pac/name", "DeLonghi PAC N90 Eco"); 
    //listen for operation
    MQTTClient_subscribe(client, "pac/on", 0);

 
  dl_aircon_msg_t msg;

  //Default settings
  msg.on = true; //false;
  msg.temperature = 24;
  msg.unitF = false;
  msg.timer = false;
  msg.timer_value = 1;
  msg.mode=8;
  msg.fan=2;

  unsigned long data = dl_assemble_msg(&msg);
  char *result = returnBits(sizeof(data), &data);

  char power[8];
  char temperature[8];
  char unitF[8];
  char timer[8];
  char timer_value[8];
  char mode[8];
  char fan[8];

  for (;;) {
        //send temperature measurement
        sprintf(power, "%d", msg.on);
        sprintf(temperature, "%d", msg.temperature);
        sprintf(unitF, "%d", msg.unitF);
        sprintf(timer, "%d", msg.timer);
        sprintf(timer_value, "%d", msg.timer_value);
        sprintf(mode, "%d", msg.mode);
        sprintf(fan, "%d", msg.fan);
        publish(client, "pac/power", power); 
        publish(client, "pac/temperature", temperature); 
        publish(client, "pac/unitF", unitF); 
        publish(client, "pac/timer", timer); 
        publish(client, "pac/timer_value", timer_value); 
        publish(client, "pac/mode", mode); 
        publish(client, "pac/fan", fan); 
        sleep(3);
  }

  send_ir(result);



    MQTTClient_disconnect(client, 1000);
    MQTTClient_destroy(&client);
    return rc;

}
