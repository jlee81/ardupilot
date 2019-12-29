// user defined variables

static union {
   float value;
   char bytes[4];
} _roll, _pitch, _roll_output, _pitch_output, ai_roll_in, ai_pitch_in;
 
char Rx_success = 0;
static uint8_t _ai_enable = 0;
 
static unsigned char tx_pck[20] = {0,};
 
static uint8_t ucStatus = 0;
static uint8_t ucCnt = 0;
static uint8_t RxChecksum = 0x00;
static char RxDataBuf[8] = {0,};


// example variables used in Wii camera testing - replace with your own
// variables
#ifdef USERHOOK_VARIABLES

#if WII_CAMERA == 1
WiiCamera           ircam;
int                 WiiRange=0;
int                 WiiRotation=0;
int                 WiiDisplacementX=0;
int                 WiiDisplacementY=0;
#endif  // WII_CAMERA

#endif  // USERHOOK_VARIABLES


