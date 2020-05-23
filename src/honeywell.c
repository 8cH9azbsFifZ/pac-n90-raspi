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
#define MQTT_TOPIC_ROOT           "honeywell/"            // i.e. the name of the ac
#endif

#if !defined PAC_NAME
#define PAC_NAME                  "Honeywell Fan"            // i.e. the name of the ac
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

#define SIGNAL_POWER 	"00100111111100100111111100100111111100100111111100100111111"
#define SIGNAL_FAN    "00100111110100100111110100100111110100100111110100100111110100100111110"
#define SIGNAL_TURN	  "00100110111100100110111100100110111100100110111100100110111100100110111"
#define SIGNAL_WAVE	  "00100111101100100111101100100111101100100111101100100111101100100111101"

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
  int leadingPulseDuration = 0; //9102; // The duration of the beginning pulse in microseconds
  int leadingGapDuration = 0;//4450;   // The duration of the gap in microseconds after the leading pulse
  int onePulse = 560;              // The duration of a pulse in microseconds when sending a logical 1
  int zeroPulse = 560;             // The duration of a pulse in microseconds when sending a logical 0
  int oneGap = 1687; //1600;               // The duration of the gap in microseconds when sending a logical 1
  int zeroGap = 560;               // The duration of the gap in microseconds when sending a logical 0
  int sendTrailingPulse = 0;       // 1 = Send a trailing pulse with duration equal to "onePulse"
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


int send_power(void)
{
	uint32_t outPin = OUT_PIN;            // The Broadcom pin number the signal will be sent on
	int frequency = 38000;          // The frequency of the IR signal in Hz
	double dutyCycle = 0.5;         // The duty cycle of the IR signal. 0.5 means for every cycle,
	                                // the LED will turn on for half the cycle time, and off the other half

	int codes[] = {
1258 , 
383 , 
1293 , 
390 , 
454 , 
1226 , 
1292 , 
390 , 
1293 , 
396 , 
460 , 
1234 , 
459 , 
1238 , 
456 , 
1237 , 
457 , 
1236 , 
461 , 
1234 , 
456 , 
1238 , 
1293 , 
7173 , 
1293 , 
387 , 
1288 , 
391 , 
457 , 
1228 , 
1294 , 
389 , 
1286 , 
397 , 
459 , 
1236 , 
459 , 
1234 , 
458 , 
1238 , 
456 , 
1237 , 
459 , 
1235 , 
460 , 
1234 , 
1293 , 
7213 , 
1294 , 
384 , 
1293 , 
383 , 
461 , 
1227 , 
1292 , 
396 , 
1283 , 
400 , 
458 , 
1238 , 
456 , 
1235 , 
458 , 
1238 , 
459 , 
1236 , 
458 , 
1234 , 
463 , 
1229 , 
1293 , 
7189 , 
1292 , 
387 , 
1288 , 
392 , 
457 , 
1227 , 
1292 , 
388 , 
1289 , 
399 , 
457 , 
1236 , 
457 , 
1240 , 
455 , 
1243 , 
453 , 
1240 , 
454 , 
1242 , 
451 , 
1235 , 
1295 , 
7188 , 
1291 , 
389 , 
1288 , 
389 , 
459 , 
1227 , 
1290 , 
392 , 
1286 , 
399 , 
458 , 
1235 , 
458 , 
1243 , 
452 , 
1241 , 
453 , 
1242 , 
451 , 
1243 , 
458 , 
1230 , 
1298 
};

	int result = irSlingRaw(
		outPin,
		frequency,
		dutyCycle,
		codes,
		sizeof(codes) / sizeof(int));
	
	return result;
}



int send_fan (void)
{

	uint32_t outPin = OUT_PIN;            // The Broadcom pin number the signal will be sent on
	int frequency = 38000;          // The frequency of the IR signal in Hz
	double dutyCycle = 0.5;         // The duty cycle of the IR signal. 0.5 means for every cycle,
	                                // the LED will turn on for half the cycle time, and off the other half

	int codes[] = {
1257 , 
385 , 
1296 , 
384 , 
457 , 
1230 , 
1290 , 
386 , 
1295 , 
394 , 
462 , 
1235 , 
460 , 
1232 , 
465 , 
1230 , 
461 , 
1232 , 
465 , 
1232 , 
1290 , 
399 , 
456 , 
8018 , 
1292 , 
385 , 
1294 , 
382 , 
465 , 
1222 , 
1295 , 
388 , 
1294 , 
393 , 
459 , 
1238 , 
459 , 
1235 , 
456 , 
1239 , 
455 , 
1238 , 
456 , 
1234 , 
1297 , 
393 , 
463 , 
8054 , 
1295 , 
384 , 
1293 , 
383 , 
459 , 
1227 , 
1294 , 
385 , 
1296 , 
394 , 
461 , 
1231 , 
463 , 
1232 , 
462 , 
1235 , 
461 , 
1238 , 
456 , 
1232 , 
1296 , 
395 , 
456 , 
8033 , 
1291 , 
386 , 
1295 , 
381 , 
465 , 
1220 , 
1298 , 
384 , 
1298 , 
394 , 
463 , 
1230 , 
465 , 
1230 , 
461 , 
1231 , 
463 , 
1239 , 
456 , 
1231 , 
1297 , 
395 , 
461 , 
8025 , 
1293 , 
385 , 
1291 , 
385 , 
464 , 
1222 , 
1296 , 
387 , 
1295 , 
394 , 
461 , 
1238 , 
458 , 
1234 , 
460 , 
1234 , 
460 , 
1236 , 
457 , 
1236 , 
1294 , 
393 , 
462 };

	int result = irSlingRaw(
		outPin,
		frequency,
		dutyCycle,
		codes,
		sizeof(codes) / sizeof(int));
	
	return result;

}

int send_turn(void)
{
  	uint32_t outPin = OUT_PIN;            // The Broadcom pin number the signal will be sent on
	int frequency = 38000;          // The frequency of the IR signal in Hz
	double dutyCycle = 0.5;         // The duty cycle of the IR signal. 0.5 means for every cycle,
	                                // the LED will turn on for half the cycle time, and off the other half

	int codes[] = {

  1283 , 
385 , 
1297 , 
385 , 
455 , 
1228 , 
1292 , 
390 , 
1293 , 
393 , 
460 , 
1236 , 
467 , 
1222 , 
1297 , 
396 , 
455 , 
1237 , 
459 , 
1236 , 
462 , 
1237 , 
459 , 
8008 , 
1292 , 
385 , 
1295 , 
380 , 
462 , 
1224 , 
1297 , 
384 , 
1298 , 
391 , 
460 , 
1236 , 
458 , 
1232 , 
1296 , 
393 , 
461 , 
1234 , 
463 , 
1231 , 
463 , 
1234 , 
461 , 
8044 , 
1299 , 
380 , 
1296 , 
384 , 
460 , 
1227 , 
1292 , 
388 , 
1292 , 
395 , 
462 , 
1234 , 
458 , 
1233 , 
1298 , 
391 , 
461 , 
1235 , 
462 , 
1233 , 
463 , 
1230 , 
463 , 
8018 , 
1296 , 
384 , 
1293 , 
383 , 
463 , 
1222 , 
1299 , 
382 , 
1296 , 
394 , 
458 , 
1235 , 
463 , 
1229 , 
1295 , 
396 , 
463 , 
1229 , 
465 , 
1229 , 
463 , 
1237 , 
457 , 
8024 , 
1294 , 
381 , 
1296 , 
384 , 
458 , 
1231 , 
1291 , 
389 , 
1292 , 
392 , 
464 , 
1230 , 
462 , 
1234 , 
1296 , 
394 , 
458 , 
1234 , 
463 , 
1233 , 
461 , 
1230 , 
463 , 
8014 , 
1291 , 
387 , 
1291 , 
383 , 
460 , 
1228 , 
1291 , 
390 , 
1290 , 
399 , 
459 , 
1238 , 
454 , 
1238 , 
1291 , 
397 , 
453 , 
1242 , 
452 , 
1242 , 
454 , 
1240 , 
459 };


	int result = irSlingRaw(
		outPin,
		frequency,
		dutyCycle,
		codes,
		sizeof(codes) / sizeof(int));
	
	return result;



}

int send_wave (void)
{
    	uint32_t outPin = OUT_PIN;            // The Broadcom pin number the signal will be sent on
	int frequency = 38000;          // The frequency of the IR signal in Hz
	double dutyCycle = 0.5;         // The duty cycle of the IR signal. 0.5 means for every cycle,
	                                // the LED will turn on for half the cycle time, and off the other half

	int codes[] = {
    1283 , 
384 , 
1291 , 
388 , 
458 , 
1227 , 
1294 , 
390 , 
1291 , 
394 , 
463 , 
1235 , 
459 , 
1235 , 
456 , 
1239 , 
456 , 
1236 , 
1293 , 
398 , 
458 , 
1237 , 
458 , 
7985 , 
1294 , 
386 , 
1288 , 
385 , 
460 , 
1227 , 
1293 , 
390 , 
1291 , 
400 , 
457 , 
1233 , 
460 , 
1237 , 
458 , 
1236 , 
458 , 
1237 , 
1290 , 
399 , 
457 , 
1236 , 
459 , 
8021 , 
1293 , 
386 , 
1291 , 
385 , 
462 , 
1225 , 
1295 , 
385 , 
1293 , 
397 , 
458 , 
1237 , 
457 , 
1236 , 
458 , 
1235 , 
459 , 
1238 , 
1293 , 
393 , 
463 , 
1233 , 
459 , 
7993 , 
1295 , 
386 , 
1291 , 
387 , 
455 , 
1228 , 
1298 , 
384 , 
1294 , 
397 , 
456 , 
1239 , 
455 , 
1236 , 
459 , 
1238 , 
456 , 
1232 , 
1297 , 
394 , 
461 , 
1234 , 
461 , 
7995 , 
1299 , 
379 , 
1296 , 
383 , 
462 , 
1223 , 
1295 , 
385 , 
1297 , 
394 , 
461 , 
1233 , 
483 , 
1210 , 
463 , 
1231 , 
463 , 
1231 , 
1294 , 
392 , 
468 , 
1227 , 
462 , 
7982 , 
1298 , 
378 , 
1299 , 
383 , 
458 , 
1229 , 
1290 , 
390 , 
1293 , 
392 , 
462 , 
1235 , 
459 , 
1237 , 
457 , 
1239 , 
455 , 
1233 , 
1296 , 
394 , 
462 , 
1231 , 
462 , 
8018 , 
1299 , 
385 , 
1292 , 
384 , 
457 , 
1228 , 
1290 , 
393 , 
1290 , 
396 , 
459 , 
1238 , 
455 , 
1238 , 
458 , 
1237 , 
456 , 
1236 , 
1305 , 
386 , 
453 , 
1242 , 
455 
   };


	int result = irSlingRaw(
		outPin,
		frequency,
		dutyCycle,
		codes,
		sizeof(codes) / sizeof(int));
	
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
  //publish(client, "pac/name", PAC_NAME); 

  //listen for operation
  //MQTTClient_subscribe(client, MQTT_TOPIC_TOGGLE_POWER, 0);
  //MQTTClient_subscribe(client, MQTT_TOPIC_TOGGLE_MODE, 0);
  //MQTTClient_subscribe(client, MQTT_TOPIC_TOGGLE_FAN, 0);
  //MQTTClient_subscribe(client, MQTT_TOPIC_TOGGLE_TEMPERATURE, 0);

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
send_power();
sleep (5);
send_fan();
sleep (3);
send_turn();
sleep (3);
send_wave();
sleep (3);
send_power();
return 0;

  send_ir(SIGNAL_POWER); 
  send_ir(SIGNAL_POWER); 
  send_ir(SIGNAL_POWER); 

//#define SIGNAL_POWER 	"00100111111100100111111100100111111100100111111100100111111100100111111"
//#define SIGNAL_FAN    "00100111110100100111110100100111110100100111110100100111110100100111110"
//#define SIGNAL_TURN	  "00100110111100100110111100100110111100100110111100100110111100100110111"
//#define SIGNAL_WAVE	  "00100111101100100111101100100111101100100111101100100111101100100111101"
  return 0;
  
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
