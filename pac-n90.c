#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "MQTTClient.h"

#include "ir-slinger/irslinger.h" 

#define ADDRESS     "t20:1883"
#define CLIENTID    "<<clientId>>" // FIXME
#define OUT_PIN     24
#define UPDATE_INTERVAL   3 // Update MQTT every N seconds

// Define topics
#define MQTT_TOPIC_TOGGLE_POWER "pac/toggle/power"
#define MQTT_TOPIC_TOGGLE_MODE "pac/toggle/mode"
#define MQTT_TOPIC_TOGGLE_FAN "pac/toggle/fan"
#define MQTT_TOPIC_TOGGLE_TEMPERATURE "pac/toggle/temperature"

#define MQTT_TOPIC_POWER "pac/power"
#define MQTT_TOPIC_TEMPERATURE "pac/temperature"
#define MQTT_TOPIC_UNITF "pac/unitF"
#define MQTT_TOPIC_TIMER "pac/timer"
#define MQTT_TOPIC_TIMER_VALUE "pac/timer_value"
#define MQTT_TOPIC_MODE "pac/mode"
#define MQTT_TOPIC_FAN "pac/fan"


// Define modes
#define MODE_AIRCONDITIONING 8
#define MODE_DEHUMIDIFY 2
#define MODE_BLOW 1
#define MODE_AIRCONDITIONING_NAME "Airconditioning"
#define MODE_DEHUMIDIFY_NAME "Dehumidify"
#define MODE_BLOW_NAME "Blow"

#define FAN_LOW 4
#define FAN_MID 2 
#define FAN_HIGH 1
#define FAN_LOW_NAME "Low"
#define FAN_MID_NAME "Mid"
#define FAN_HIGH_NAME "High"


typedef struct {
  bool on;
  bool timer;
  bool unitF;
  int temperature;
  int timer_value;
  int mode;
  int fan;
} dl_aircon_msg_t;


dl_aircon_msg_t msg;

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

#define TEMPERATURE_MIN 16
#define TEMPERATURE_MAX 32
#define TEMPERATURE_F_MIN 61
#define TEMPERATURE_F_MAX 89



unsigned long dl_assemble_msg(dl_aircon_msg_t* msg){
  unsigned long buf = 0x12000000;

  if (!msg->unitF){
    msg->temperature = constrain(msg->temperature, TEMPERATURE_MIN, TEMPERATURE_MAX); 
    buf |= bit_reverse(msg->temperature-TEMPERATURE_MIN);
  }else{
    msg->temperature = constrain(msg->temperature, TEMPERATURE_F_MIN, TEMPERATURE_F_MAX); 
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


int send_ir (char* msg)
{
  uint32_t outPin = OUT_PIN;            // The Broadcom pin number the signal will be sent on
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


int on_message(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
  char* payload = message->payload;
  printf("Received operation %s\n", payload);

  unsigned long data;
  char *result;
 
  if (strcmp(topicName,MQTT_TOPIC_TOGGLE_POWER)==0)
  {
    if(strcmp(payload,"on")==0) 
    { msg.on=true; }
    if(strcmp(payload,"off")==0) 
    { msg.on=false; }
  }

  if (strcmp(topicName,MQTT_TOPIC_TOGGLE_MODE)==0)
  {
    if (atoi(payload) == MODE_AIRCONDITIONING)
    { msg.mode=MODE_AIRCONDITIONING; }
    if (strcmp(payload,"airconditioning")==0)
    { msg.mode=MODE_AIRCONDITIONING; }
    if (atoi(payload) == MODE_DEHUMIDIFY)
    { msg.mode=MODE_DEHUMIDIFY; }
    if (strcmp(payload,"dehumidify")==0)
    { msg.mode=MODE_DEHUMIDIFY; }
    if (atoi(payload) == MODE_BLOW)
    { msg.mode=MODE_BLOW; }
    if (strcmp(payload,"blow")==0)
    { msg.mode=MODE_BLOW; }
  }

  if (strcmp(topicName,MQTT_TOPIC_TOGGLE_FAN)==0)
  {
    if (atoi(payload) == FAN_LOW)
    { msg.fan=FAN_LOW; }
    if (atoi(payload) == FAN_MID)
    { msg.fan=FAN_MID; }
    if (atoi(payload) == FAN_HIGH)
    { msg.fan=FAN_HIGH; }
    if (strcmp(payload,"high") == 0)
    { msg.fan=FAN_HIGH; }
    if (strcmp(payload,"mid") == 0)
    { msg.fan=FAN_MID; }
    if (strcmp(payload,"low") == 0)
    { msg.fan=FAN_LOW; }
  }

  if (strcmp(topicName,MQTT_TOPIC_TOGGLE_TEMPERATURE)==0)
  {
    msg.temperature = constrain(atoi(payload), TEMPERATURE_MIN, TEMPERATURE_MAX); 
  }

  data = dl_assemble_msg(&msg);
  result = returnBits(sizeof(data), &data);
  send_ir(result);
 

  MQTTClient_freeMessage(&message);
  MQTTClient_free(topicName);

  return 1;
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
  printf(MQTT_TOPIC_TOGGLE_POWER);
  MQTTClient_subscribe(client, MQTT_TOPIC_TOGGLE_POWER, 0);
  MQTTClient_subscribe(client, MQTT_TOPIC_TOGGLE_MODE, 0);
  MQTTClient_subscribe(client, MQTT_TOPIC_TOGGLE_FAN, 0);
  MQTTClient_subscribe(client, MQTT_TOPIC_TOGGLE_TEMPERATURE, 0);

  //Default settings
  msg.on = false;
  msg.temperature = 24;
  msg.unitF = false;
  msg.timer = false;
  msg.timer_value = 1;
  msg.mode=MODE_AIRCONDITIONING;
  msg.fan=FAN_MID;

  char temperature[8];
  char unitF[8];
  char timer_value[8];

  for (;;) {
    //send temperature measurement
    if (msg.on == true) { publish(client, MQTT_TOPIC_POWER, "on"); } else { publish(client, MQTT_TOPIC_POWER, "off"); }
    sprintf(temperature, "%d", msg.temperature);
    if (msg.unitF == true) { sprintf(unitF, "°F"); } else { sprintf(unitF, "°C"); }
    if (msg.timer == true) { publish(client, MQTT_TOPIC_TIMER, "on");  } else { publish(client, MQTT_TOPIC_TIMER, "off");  }
    sprintf(timer_value, "%d", msg.timer_value);
    if (msg.mode == MODE_AIRCONDITIONING) { publish(client, MQTT_TOPIC_MODE, MODE_AIRCONDITIONING_NAME); }
    else if (msg.mode == MODE_DEHUMIDIFY) { publish(client, MQTT_TOPIC_MODE, MODE_DEHUMIDIFY_NAME); }
    else if (msg.mode == MODE_BLOW)       { publish(client, MQTT_TOPIC_MODE, MODE_BLOW_NAME); }

    if (msg.fan == FAN_LOW)       { publish(client, MQTT_TOPIC_FAN, FAN_LOW_NAME); }
    else if (msg.fan == FAN_MID)  { publish(client, MQTT_TOPIC_FAN, FAN_MID_NAME);  }
    else if (msg.fan == FAN_HIGH) { publish(client, MQTT_TOPIC_FAN, FAN_HIGH_NAME); }

    publish(client, MQTT_TOPIC_TEMPERATURE, temperature); 
    publish(client, MQTT_TOPIC_UNITF, unitF); 
    
    publish(client, MQTT_TOPIC_TIMER_VALUE, timer_value); 
     

    sleep(UPDATE_INTERVAL);
  }


  MQTTClient_disconnect(client, 1000);
  MQTTClient_destroy(&client);

  return rc;
}
