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

#if !defined FAN_NAME  
#define FAN_NAME                  "Honeywell Fan"            // i.e. the name of the ac
#endif

// IR Pin on Raspi
#define OUT_PIN                   24

// MQTT Connection Configuration
#define CLIENTID                  "<<clientId>>" // FIXME
#define CLIENT_USERNAME           "<<tenant_ID>>/<<username>>"
#define CLIENT_PASSWORD           "<<password>>"

// Define topics
#define MQTT_TOPIC_TOGGLE_POWER         MQTT_TOPIC_ROOT "fan/toggle/power"
#define MQTT_TOPIC_TOGGLE_WAVE          MQTT_TOPIC_ROOT "fan/toggle/wave"
#define MQTT_TOPIC_TOGGLE_FAN           MQTT_TOPIC_ROOT "fan/toggle/fan"
#define MQTT_TOPIC_TOGGLE_TURN          MQTT_TOPIC_ROOT "fan/toggle/turn"
#define MQTT_TOPIC_TOGGLE_TIMER         MQTT_TOPIC_ROOT "fan/toggle/timer"

#define MQTT_TOPIC_POWER                MQTT_TOPIC_ROOT "fan/power"
#define MQTT_TOPIC_TURN                 MQTT_TOPIC_ROOT "fan/turn"
#define MQTT_TOPIC_WAVE                 MQTT_TOPIC_ROOT "fan/wave"
#define MQTT_TOPIC_FAN                  MQTT_TOPIC_ROOT "fan/fan"
#define MQTT_TOPIC_TIMER                MQTT_TOPIC_ROOT "fan/timer"


#define UPDATE_INTERVAL               3       // Update MQTT every N seconds

// Define modes
#define WAVE_WOOD                     2
#define WAVE_MOON                     3
#define WAVE_NONE                     1
#define WAVE_WOOD_NAME                "Wood"
#define WAVE_MOON_NAME                "Moon"
#define WAVE_NONE_NAME                "None"

#define FAN_LOW                       1
#define FAN_MID                       2 
#define FAN_HIGH                      3
#define FAN_LOW_NAME                  "Low"
#define FAN_MID_NAME                  "Mid"
#define FAN_HIGH_NAME                 "High"

#define TIMER_30                      2
#define TIMER_60                      3
#define TIMER_120                     4
#define TIMER_240                     5
#define TIMER_NONE                    1

#define TIMER_30_NAME                      "30"
#define TIMER_60_NAME                      "60"
#define TIMER_120_NAME                     "120"
#define TIMER_240_NAME                     "240"
#define TIMER_NONE_NAME                    "0"

typedef struct {
  bool on;
  bool turn;
  int timer;
  int wave;
  int fan;
} dl_aircon_msg_t;


dl_aircon_msg_t msg;


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


int send_power(void)
{
	uint32_t outPin = OUT_PIN;            // The Broadcom pin number the signal will be sent on
	int frequency = 38000;          // The frequency of the IR signal in Hz
	double dutyCycle = 0.5;         // The duty cycle of the IR signal. 0.5 means for every cycle,
	                                // the LED will turn on for half the cycle time, and off the other half

	int codes[] = { 1258 , 383 , 1293 , 390 , 454 , 1226 , 1292 , 390 , 1293 , 396 , 460 , 1234 , 459 , 1238 , 456 , 1237 , 457 , 1236 , 461 , 1234 , 456 , 1238 , 1293 , 7173 , 1293 , 387 , 1288 , 391 , 457 , 1228 , 1294 , 389 , 1286 , 397 , 459 , 1236 , 459 , 1234 , 458 , 1238 , 456 , 1237 , 459 , 1235 , 460 , 1234 , 1293 , 7213 , 1294 , 384 , 1293 , 383 , 461 , 1227 , 1292 , 396 , 1283 , 400 , 458 , 1238 , 456 , 1235 , 458 , 1238 , 459 , 1236 , 458 , 1234 , 463 , 1229 , 1293 , 7189 , 1292 , 387 , 1288 , 392 , 457 , 1227 , 1292 , 388 , 1289 , 399 , 457 , 1236 , 457 , 1240 , 455 , 1243 , 453 , 1240 , 454 , 1242 , 451 , 1235 , 1295 , 7188 , 1291 , 389 , 1288 , 389 , 459 , 1227 , 1290 , 392 , 1286 , 399 , 458 , 1235 , 458 , 1243 , 452 , 1241 , 453 , 1242 , 451 , 1243 , 458 , 1230 , 1298 };

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

	int codes[] = { 1257 , 385 , 1296 , 384 , 457 , 1230 , 1290 , 386 , 1295 , 394 , 462 , 1235 , 460 , 1232 , 465 , 1230 , 461 , 1232 , 465 , 1232 , 1290 , 399 , 456 , 8018 , 1292 , 385 , 1294 , 382 , 465 , 1222 , 1295 , 388 , 1294 , 393 , 459 , 1238 , 459 , 1235 , 456 , 1239 , 455 , 1238 , 456 , 1234 , 1297 , 393 , 463 , 8054 , 1295 , 384 , 1293 , 383 , 459 , 1227 , 1294 , 385 , 1296 , 394 , 461 , 1231 , 463 , 1232 , 462 , 1235 , 461 , 1238 , 456 , 1232 , 1296 , 395 , 456 , 8033 , 1291 , 386 , 1295 , 381 , 465 , 1220 , 1298 , 384 , 1298 , 394 , 463 , 1230 , 465 , 1230 , 461 , 1231 , 463 , 1239 , 456 , 1231 , 1297 , 395 , 461 , 8025 , 1293 , 385 , 1291 , 385 , 464 , 1222 , 1296 , 387 , 1295 , 394 , 461 , 1238 , 458 , 1234 , 460 , 1234 , 460 , 1236 , 457 , 1236 , 1294 , 393 , 462 };

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

	int codes[] = { 1283 , 385 , 1297 , 385 , 455 , 1228 , 1292 , 390 , 1293 , 393 , 460 , 1236 , 467 , 1222 , 1297 , 396 , 455 , 1237 , 459 , 1236 , 462 , 1237 , 459 , 8008 , 1292 , 385 , 1295 , 380 , 462 , 1224 , 1297 , 384 , 1298 , 391 , 460 , 1236 , 458 , 1232 , 1296 , 393 , 461 , 1234 , 463 , 1231 , 463 , 1234 , 461 , 8044 , 1299 , 380 , 1296 , 384 , 460 , 1227 , 1292 , 388 , 1292 , 395 , 462 , 1234 , 458 , 1233 , 1298 , 391 , 461 , 1235 , 462 , 1233 , 463 , 1230 , 463 , 8018 , 1296 , 384 , 1293 , 383 , 463 , 1222 , 1299 , 382 , 1296 , 394 , 458 , 1235 , 463 , 1229 , 1295 , 396 , 463 , 1229 , 465 , 1229 , 463 , 1237 , 457 , 8024 , 1294 , 381 , 1296 , 384 , 458 , 1231 , 1291 , 389 , 1292 , 392 , 464 , 1230 , 462 , 1234 , 1296 , 394 , 458 , 1234 , 463 , 1233 , 461 , 1230 , 463 , 8014 , 1291 , 387 , 1291 , 383 , 460 , 1228 , 1291 , 390 , 1290 , 399 , 459 , 1238 , 454 , 1238 , 1291 , 397 , 453 , 1242 , 452 , 1242 , 454 , 1240 , 459 };


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

	int codes[] = { 1283 , 384 , 1291 , 388 , 458 , 1227 , 1294 , 390 , 1291 , 394 , 463 , 1235 , 459 , 1235 , 456 , 1239 , 456 , 1236 , 1293 , 398 , 458 , 1237 , 458 , 7985 , 1294 , 386 , 1288 , 385 , 460 , 1227 , 1293 , 390 , 1291 , 400 , 457 , 1233 , 460 , 1237 , 458 , 1236 , 458 , 1237 , 1290 , 399 , 457 , 1236 , 459 , 8021 , 1293 , 386 , 1291 , 385 , 462 , 1225 , 1295 , 385 , 1293 , 397 , 458 , 1237 , 457 , 1236 , 458 , 1235 , 459 , 1238 , 1293 , 393 , 463 , 1233 , 459 , 7993 , 1295 , 386 , 1291 , 387 , 455 , 1228 , 1298 , 384 , 1294 , 397 , 456 , 1239 , 455 , 1236 , 459 , 1238 , 456 , 1232 , 1297 , 394 , 461 , 1234 , 461 , 7995 , 1299 , 379 , 1296 , 383 , 462 , 1223 , 1295 , 385 , 1297 , 394 , 461 , 1233 , 483 , 1210 , 463 , 1231 , 463 , 1231 , 1294 , 392 , 468 , 1227 , 462 , 7982 , 1298 , 378 , 1299 , 383 , 458 , 1229 , 1290 , 390 , 1293 , 392 , 462 , 1235 , 459 , 1237 , 457 , 1239 , 455 , 1233 , 1296 , 394 , 462 , 1231 , 462 , 8018 , 1299 , 385 , 1292 , 384 , 457 , 1228 , 1290 , 393 , 1290 , 396 , 459 , 1238 , 455 , 1238 , 458 , 1237 , 456 , 1236 , 1305 , 386 , 453 , 1242 , 455 };


	int result = irSlingRaw(
		outPin,
		frequency,
		dutyCycle,
		codes,
		sizeof(codes) / sizeof(int));
	
	return result;

}


int send_timer(void)
{
  uint32_t outPin = OUT_PIN;            // The Broadcom pin number the signal will be sent on
	int frequency = 38000;          // The frequency of the IR signal in Hz
	double dutyCycle = 0.5;         // The duty cycle of the IR signal. 0.5 means for every cycle,
	                                // the LED will turn on for half the cycle time, and off the other half

  int codes[] = {1293 , 381 , 1298 , 386 , 455 , 1232 , 1292 , 389 , 1303 , 394 , 458 , 1241 , 456 , 1235 , 459 , 1239 , 1294 , 395 , 461 , 1236 , 457 , 1237 , 460 , 7986 , 1295 , 387 , 1296 , 383 , 459 , 1233 , 1292 , 391 , 1293 , 397 , 459 , 1240 , 456 , 1237 , 459 , 1238 , 1300 , 397 , 458 , 1237 , 458 , 1240 , 460 , 8018 , 1305 , 391 , 1309 , 387 , 461 , 1230 , 1301 , 391 , 1298 , 399 , 460 , 1239 , 456 , 1242 , 458 , 1235 , 1301 , 400 , 457 , 1240 , 459 , 1243 , 457 , 7994 , 1301 , 387 , 1297 , 389 , 459 , 1225 , 1306 , 391 , 1299 , 397 , 459 , 1241 , 455 , 1238 , 459 , 1239 , 1298 , 396 , 460 , 1234 , 459 , 1240 , 459 , 8003 , 1294 , 391 , 1298 , 387 , 459 , 1228 , 1295 , 394 , 1296 , 396 , 459 , 1241 , 465 , 1228 , 457 , 1240 , 1296 , 400 , 460 , 1235 , 458 , 1241 , 456 , 7994 , 1294 , 391 , 1294 , 390 , 457 , 1228 , 1300 , 391 , 1295 , 401 , 459 , 1237 , 456 , 1241 , 458 , 1233 , 1297 , 401 , 458 , 1238 , 455 , 1241 , 460 };

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
    bool state = true;
    if(strcmp(payload,"on")==0) 
    { state=true; }
    if(strcmp(payload,"off")==0) 
    { state=false; }
    if (state != msg.on) { msg.on = state; send_power(); }
  }

  if (strcmp(topicName,MQTT_TOPIC_TOGGLE_WAVE)==0)
  {
    int wave=0;
    if (strcmp(payload,WAVE_MOON_NAME)==0)
    { wave=WAVE_MOON; }
    if (strcmp(payload,WAVE_WOOD_NAME)==0)
    { wave=WAVE_WOOD; }
    if (strcmp(payload,WAVE_NONE_NAME)==0)
    { wave=WAVE_NONE; }
    if (wave != 0) 
    {
      int i;
      while (msg.wave != wave)
      {
        msg.wave++;
        if (msg.wave>3) {msg.wave=1;}
        send_wave();
      }
    }
  }

  if (strcmp(topicName,MQTT_TOPIC_TOGGLE_FAN)==0)
  {
    int state=0;
    if (strcmp(payload,FAN_HIGH_NAME) == 0)
    { msg.fan=FAN_HIGH; }
    if (strcmp(payload,FAN_MID_NAME) == 0)
    { msg.fan=FAN_MID; }
    if (strcmp(payload,FAN_LOW_NAME) == 0)
    { msg.fan=FAN_LOW; }
    if (state != 0)
    {
      while (state != msg.fan)
      {
        msg.fan++;
        if (msg.fan>3) {msg.fan=1;}
        send_fan();
      }
    }
  }

  if (strcmp(topicName,MQTT_TOPIC_TOGGLE_TURN)==0)
  {
    msg.turn = !msg.turn;
    send_turn();
  }

  // FIXME: timer

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
  publish(client, "fan/name", FAN_NAME); 

  //listen for operation
  MQTTClient_subscribe(client, MQTT_TOPIC_TOGGLE_POWER, 0);
  MQTTClient_subscribe(client, MQTT_TOPIC_TOGGLE_WAVE, 0);
  MQTTClient_subscribe(client, MQTT_TOPIC_TOGGLE_FAN, 0);
  MQTTClient_subscribe(client, MQTT_TOPIC_TOGGLE_TURN, 0);
  MQTTClient_subscribe(client, MQTT_TOPIC_TOGGLE_TIMER, 0);

  //Default settings
  msg.on = false;
  msg.turn = false;
  msg.timer = TIMER_NONE;
  msg.wave=WAVE_NONE;
  msg.fan=FAN_LOW;

  char timer[8];
  
  for (;;) {
    //send temperature measurement
    if (msg.on == true) { publish(client, MQTT_TOPIC_POWER, "on"); } else { publish(client, MQTT_TOPIC_POWER, "off"); }
    if (msg.turn == true) { publish(client, MQTT_TOPIC_TIMER, "on");  } else { publish(client, MQTT_TOPIC_TIMER, "off");  }
    if (msg.wave == WAVE_NONE) { publish(client, MQTT_TOPIC_WAVE, WAVE_MOON_NAME); }
    else if (msg.wave == WAVE_WOOD) { publish(client, MQTT_TOPIC_WAVE, WAVE_WOOD_NAME); }
    else if (msg.wave == WAVE_MOON)       { publish(client, MQTT_TOPIC_WAVE, WAVE_NONE_NAME); }

    if (msg.fan == FAN_LOW)       { publish(client, MQTT_TOPIC_FAN, FAN_LOW_NAME); }
    else if (msg.fan == FAN_MID)  { publish(client, MQTT_TOPIC_FAN, FAN_MID_NAME);  }
    else if (msg.fan == FAN_HIGH) { publish(client, MQTT_TOPIC_FAN, FAN_HIGH_NAME); }

    sprintf(timer, "%d", msg.timer);
    publish(client, MQTT_TOPIC_TIMER, timer); 

    sleep(UPDATE_INTERVAL);
  }


  MQTTClient_disconnect(client, 1000);
  MQTTClient_destroy(&client);

  return rc;
}
