#include "Copter.h"
#include "UserVariables.h"

void Copter::ai_control1()
{
//  hal.uartE->printf("ahrs.roll = %f - tele4_400hz\n", ahrs.roll);  
  control_in_rx();
  attitude_control->set_ai_roll_in(ai_roll_in.value);
  attitude_control->set_ai_pitch_in(ai_pitch_in.value);
}

void Copter::ai_control2()
{
  _roll.value = ahrs.roll;
  _pitch.value = ahrs.pitch;
  _roll_output.value = attitude_control->get_output_roll();
  _pitch_output.value = attitude_control->get_output_pitch();
  _ai_enable = (uint8_t)flag_AI_reset * 0x10 + (uint8_t)flag_RC_User;
  // _dt = attitude_control->get_dt()

  if(!flag_AI_reset)
  {
    control_out_tx();
  }  
}


void Copter::control_out_tx()
{
  memset(tx_pck, 0, sizeof(tx_pck));
  tx_pck[0] = 0xAA;
  tx_pck[1] = 0x55;
  for(int i=0;i<4;i++) {
    tx_pck[2+i] = _roll_output.bytes[i];  
  }

  for(int i=0;i<4;i++) {
    tx_pck[6+i] = _pitch_output.bytes[i];  
  }

  for(int i=0;i<4;i++) {
    tx_pck[10+i] = _roll.bytes[i];  
  }

  for(int i=0;i<4;i++) {
    tx_pck[14+i] = _pitch.bytes[i];  
  }

  tx_pck[18] = _ai_enable;  // enable

  for(int i=2;i<19;i++) {
    tx_pck[19] += tx_pck[i];
  }

  hal.uartE->write(&tx_pck[0], 20);
}


void Copter::control_in_rx()
{
  uint16_t nbytes = hal.uartE->available();     // serial4 = uartE, serial5 = uartF

  while(nbytes-- > 0)
  {
    unsigned char ch = hal.uartE->read();

    switch( ucStatus )
    {
      case 0:
        if(ch == 0x55) {
          ucStatus = 1;
        }
      break;
      case 1:
        if(ch == 0xAA) {
          ucStatus = 2;
        }
        else {
          ucStatus = 0;
        }
      break;

      case 2:
      case 3:
      case 4:
      case 5:

      case 6:
      case 7:
      case 8:
      case 9:
        RxDataBuf[ucCnt++] = ch;
        RxChecksum += ch;
        ucStatus++;
      break;

      case 10:
        if(RxChecksum == ch)
        {
          ai_roll_in.bytes[0] = RxDataBuf[0];
          ai_roll_in.bytes[1] = RxDataBuf[1];
          ai_roll_in.bytes[2] = RxDataBuf[2];
          ai_roll_in.bytes[3] = RxDataBuf[3];
          ai_pitch_in.bytes[0] = RxDataBuf[4];
          ai_pitch_in.bytes[1] = RxDataBuf[5];
          ai_pitch_in.bytes[2] = RxDataBuf[6];
          ai_pitch_in.bytes[3] = RxDataBuf[7];
          memset(RxDataBuf, 0, 8);
          Rxxx_checksum_cnt++;
          errorcnt = 0;
        }
        else {
          errorcnt++;
          if(errorcnt>5){
            ai_roll_in.value = 0;
            ai_pitch_in.value = 0;
            errorcnt = 0;
          }
          
        }
        
        ucStatus = 0;
        ucCnt = 0;
        RxChecksum = 0;
      break;

      default:
        ucStatus = 0;
        ucCnt = 0;
        RxChecksum = 0;
      break;
    }
    
    
  }

}

