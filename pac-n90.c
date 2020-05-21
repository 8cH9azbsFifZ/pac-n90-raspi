#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

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

int main (void)
{
  dl_aircon_msg_t msg;

  //Default settings
  msg.on = false;
  msg.temperature = 24;
  msg.unitF = false;
  msg.timer = false;
  msg.timer_value = 1;
  msg.mode=8;
  msg.fan=2;

  unsigned long data = dl_assemble_msg(&msg);
  char *result = returnBits(sizeof(data), &data);
  printf ("%s",result);
}
