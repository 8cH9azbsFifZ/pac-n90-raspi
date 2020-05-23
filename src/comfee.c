// alle bei 24c
#define off	"110100001000000100100011111111111111111111101101011010111101111110110111000000000000000000000100101"
#define on	"110100001100000100100011111111111111111110101101011010111100111110110111000000000000000000010100101"
#define swing	"110100010000000011111111111111111111111110111110011010111011111111000000000000000000000000010000011"

// bei 30°
#define modeauto // siehe unten
#define modecool 	"110100001100010000100110111111111111111110101110111010111100111011110110010000000000000000010100010"
#define modedry		"110100001100000010100110111111111111111110101001011010111100111111010110010000000000000000010101101"
#define modeheat	"110100001100010110100110111111111111111110101111111010111100111010010110010000000000000000010100000"
#define mdoefan		"110100001100011000101111011111111111111110100101111010111100111001110100001000000000000000010110100"


// cool, 30
#define cool30cfanauto_off	"110100001001000000100110111111111111111111110001111010111101101111110110010000000000000000000011100"
#define cool30cfanauto_on	  "110100001101000000100110111111111111111110110001111010111100101111110110010000000000000000010011100"

#define cool30cfanlow		"110100001100010000100110111111111111111110101110111010111100111011110110010000000000000000010100010"
#define cool30cfanmid		"110100001100100000100110111111111111111110100001111010111100110111110110010000000000000000010111100"
#define cool30cfanhi		"110100001100110000100110111111111111111110100110111010111100110011110110010000000000000000010110010"
#define cool30cfanauto		"110100001101000000100110111111111111111110110001111010111100101111110110010000000000000000010011100"


// heat, 30
#define heat30cfanauto_off	"110100001001000110100110111111111111111111110000011010111101101110010110010000000000000000000011111"
#define heat30cfanauto_on	"110100001101000110100110111111111111111110110000011010111100101110010110010000000000000000010011111"
#define heat30cfanlow		"110100001100010110100110111111111111111110101111111010111100111010010110010000000000000000010100000"
#define heat30cfanmid		"110100001100100110100110111111111111111110100000011010111100110110010110010000000000000000010111111"
#define heat30cfanhigh		"110100001100110110100110111111111111111110100111111010111100110010010110010000000000000000010110000"
#define heat30cfanauto		"110100001101000110100110111111111111111110110000011010111100101110010110010000000000000000010011111"

// fan only (30)
#define fanauto_on		"110100001101001000101111011111111111111110111101111010111100101101110100001000000000000000010000100"
#define fanauto_off		"110100001001001000101111011111111111111111111101111010111101101101110100001000000000000000000000100"
#define fanlow			"110100001100011000101111011111111111111110100101111010111100111001110100001000000000000000010110100"
#define fanmid			"110100001100101000101111011111111111111110101101111010111100110101110100001000000000000000010100100"
#define fanhigh			"110100001100111000101111011111111111111110101001111010111100110001110100001000000000000000010101100"
#define fanauto			"110100001101001000101111011111111111111110111101111010111100101101110100001000000000000000010000100"

#define led	"110100010000010001111111111111111111111110111010111010111011111011100000000000000000000000010001010" 


// alle in mode auto
#define on_30c	"110100001100000100100110111111111111111110101000111010111100111110110110010000000000000000010101110"
#define on_29c	"110100001100000100100110011111111111111110101000011010111100111110110110011000000000000000010101111"
#define on_28c	"110100001100000100100101111111111111111110101011011010111100111110110110100000000000000000010101001"
#define on_27c	"110100001100000100100101011111111111111110101011111010111100111110110110101000000000000000010101000"
#define on_26c	"110100001100000100100100111111111111111110101010111010111100111110110110110000000000000000010101010"
#define on_25c	"110100001100000100100100011111111111111110101010011010111100111110110110111000000000000000010101011"
#define on_24c  "110100001100000100100011111111111111111110101101011010111100111110110111000000000000000000010100101"
#define on_23c	"110100001100000100100011011111111111111110101101111010111100111110110111001000000000000000010100100"
#define on_22c  "110100001100000100100010111111111111111110101100111010111100111110110111010000000000000000010100110"
#define on_21c	"110100001100000100100010011111111111111110101100011010111100111110110111011000000000000000010100111"
#define on_20c	"110100001100000100100001111111111111111110101111011010111100111110110111100000000000000000010100001"
#define on_19c	"110100001100000100100001011111111111111110101111111010111100111110110111101000000000000000010100000"
#define on_18c	"110100001100000100100000111111111111111110101110111010111100111110110111110000000000000000010100010"
#define on_17c	"110100001100000100100000011111111111111110101110011010111100111110110111111000000000000000010100011"
#define off30c	"110100001000000100100110111111111111111111101000111010111101111110110110010000000000000000000101110"
#define off29c 	"110100001000000100100110011111111111111111101000011010111101111110110110011000000000000000000101111"
#define off28c	"110100001000000100100101111111111111111111101011011010111101111110110110100000000000000000000101001"
#define off27c	"110100001000000100100101011111111111111111101011111010111101111110110110101000000000000000000101000"
#define off26c	"110100001000000100100100111111111111111111101010111010111101111110110110110000000000000000000101010"
#define off25c	"110100001000000100100100011111111111111111101010011010111101111110110110111000000000000000000101011"
#define off24c  "110100001000000100100011111111111111111111101101011010111101111110110111000000000000000000000100101"
#define off23c	"110100001000000100100011011111111111111111101101111010111101111110110111001000000000000000000100100"
#define off22c	"110100001000000100100010111111111111111111101100111010111101111110110111010000000000000000000100110"
#define off21c	"110100001000000100100010011111111111111111101100011010111101111110110111011000000000000000000100111"
#define off20c	"110100001000000100100001111111111111111111101111011010111101111110110111100000000000000000000100001"
#define off19c	"110100001000000100100001011111111111111111101111111010111101111110110111101000000000000000000100000"
#define off18c	"110100001100000100100000111111111111111110101110111010111100111110110111110000000000000000010100010"
#define off17c	"110100001000000100100000011111111111111111101110011010111101111110110111111000000000000000000100011"










#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "MQTTClient.h"

#include "ir-slinger/irslinger.h" 

#if !defined MQTT_ADDRESS
#define MQTT_ADDRESS              "t20:1883"      // Address of the mqtt broker
#endif

#if !defined MQTT_TOPIC_ROOT
#define MQTT_TOPIC_ROOT           "n90/"            // i.e. the name of the ac
#endif

#if !defined PAC_NAME
#define PAC_NAME                  "DeLonghi PAC N90 Eco"            // i.e. the name of the ac
#endif

// IR Pin on Raspi
#define OUT_PIN                   24

// MQTT Connection Configuration
#define CLIENTID                  "<<clientId>>" // FIXME
#define CLIENT_USERNAME           "<<tenant_ID>>/<<username>>"
#define CLIENT_PASSWORD           "<<password>>"

// Define topics
#define MQTT_TOPIC_TOGGLE_POWER         MQTT_TOPIC_ROOT "pac/toggle/power"
#define MQTT_TOPIC_TOGGLE_MODE          MQTT_TOPIC_ROOT "pac/toggle/mode"
#define MQTT_TOPIC_TOGGLE_FAN           MQTT_TOPIC_ROOT "pac/toggle/fan"
#define MQTT_TOPIC_TOGGLE_TEMPERATURE   MQTT_TOPIC_ROOT "pac/toggle/temperature"

#define MQTT_TOPIC_POWER                MQTT_TOPIC_ROOT "pac/power"
#define MQTT_TOPIC_TEMPERATURE          MQTT_TOPIC_ROOT "pac/temperature"
#define MQTT_TOPIC_UNITF                MQTT_TOPIC_ROOT "pac/unitF"
#define MQTT_TOPIC_TIMER                MQTT_TOPIC_ROOT "pac/timer"
#define MQTT_TOPIC_TIMER_VALUE          MQTT_TOPIC_ROOT "pac/timer_value"
#define MQTT_TOPIC_MODE                 MQTT_TOPIC_ROOT "pac/mode"
#define MQTT_TOPIC_FAN                  MQTT_TOPIC_ROOT "pac/fan"


// Define modes
#define UPDATE_INTERVAL               3       // Update MQTT every N seconds
#define MODE_AIRCONDITIONING          8
#define MODE_DEHUMIDIFY               2
#define MODE_BLOW                     1
#define MODE_AIRCONDITIONING_NAME     "Airconditioning"
#define MODE_DEHUMIDIFY_NAME          "Dehumidify"
#define MODE_BLOW_NAME                "Blow"

#define FAN_LOW                       4
#define FAN_MID                       2 
#define FAN_HIGH                      1
#define FAN_LOW_NAME                  "Low"
#define FAN_MID_NAME                  "Mid"
#define FAN_HIGH_NAME                 "High"


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

#define TEMPERATURE_MIN       16
#define TEMPERATURE_MAX       32
#define TEMPERATURE_F_MIN     61
#define TEMPERATURE_F_MAX     89

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
    if (strcmp(payload,MODE_AIRCONDITIONING_NAME)==0)
    { msg.mode=MODE_AIRCONDITIONING; }
    if (atoi(payload) == MODE_DEHUMIDIFY)
    { msg.mode=MODE_DEHUMIDIFY; }
    if (strcmp(payload,MODE_DEHUMIDIFY_NAME)==0)
    { msg.mode=MODE_DEHUMIDIFY; }
    if (atoi(payload) == MODE_BLOW)
    { msg.mode=MODE_BLOW; }
    if (strcmp(payload,MODE_BLOW_NAME)==0)
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
    if (strcmp(payload,FAN_HIGH_NAME) == 0)
    { msg.fan=FAN_HIGH; }
    if (strcmp(payload,FAN_MID_NAME) == 0)
    { msg.fan=FAN_MID; }
    if (strcmp(payload,FAN_LOW_NAME) == 0)
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
  MQTTClient_create(&client, MQTT_ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
  MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
  conn_opts.username = CLIENT_USERNAME; 
  conn_opts.password = CLIENT_PASSWORD;

  MQTTClient_setCallbacks(client, NULL, NULL, on_message, NULL);

  int rc;
  if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
      printf("Failed to connect, return code %d\n", rc);
      exit(-1);
  }

  //create device
  publish(client, "pac/name", PAC_NAME); 

  //listen for operation
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
  msg.mode=MODE_BLOW;//MODE_AIRCONDITIONING;
  msg.fan=FAN_LOW;

  char temperature[8];
  char timer_value[8];

  for (;;) {
    //send temperature measurement
    if (msg.on == true) { publish(client, MQTT_TOPIC_POWER, "on"); } else { publish(client, MQTT_TOPIC_POWER, "off"); }
    if (msg.unitF == true) { publish(client, MQTT_TOPIC_UNITF, "°F"); } else { publish(client, MQTT_TOPIC_UNITF, "°C"); }
    if (msg.timer == true) { publish(client, MQTT_TOPIC_TIMER, "on");  } else { publish(client, MQTT_TOPIC_TIMER, "off");  }
    if (msg.mode == MODE_AIRCONDITIONING) { publish(client, MQTT_TOPIC_MODE, MODE_AIRCONDITIONING_NAME); }
    else if (msg.mode == MODE_DEHUMIDIFY) { publish(client, MQTT_TOPIC_MODE, MODE_DEHUMIDIFY_NAME); }
    else if (msg.mode == MODE_BLOW)       { publish(client, MQTT_TOPIC_MODE, MODE_BLOW_NAME); }

    if (msg.fan == FAN_LOW)       { publish(client, MQTT_TOPIC_FAN, FAN_LOW_NAME); }
    else if (msg.fan == FAN_MID)  { publish(client, MQTT_TOPIC_FAN, FAN_MID_NAME);  }
    else if (msg.fan == FAN_HIGH) { publish(client, MQTT_TOPIC_FAN, FAN_HIGH_NAME); }

    sprintf(temperature, "%d", msg.temperature);
    publish(client, MQTT_TOPIC_TEMPERATURE, temperature); 

    sprintf(timer_value, "%d", msg.timer_value);
    publish(client, MQTT_TOPIC_TIMER_VALUE, timer_value); 

    sleep(UPDATE_INTERVAL);
  }


  MQTTClient_disconnect(client, 1000);
  MQTTClient_destroy(&client);

  return rc;
}
