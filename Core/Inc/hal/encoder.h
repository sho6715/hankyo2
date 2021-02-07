/*
 * encoder.h
 *
 *  Created on: Jan 13, 2021
 *      Author: shohe
 */

#ifndef INC_ENCODER_H_
#define INC_ENCODER_H_

#ifdef _VSCODE
#include "Inc/glob.h"
#include "Inc/gpio.h"
#include "Inc/hal/CTRL.h"
#include "Inc/hal/run.h"
#else
#include "glob.h"
#include "gpio.h"
#include "hal/CTRL.h"
#include "hal/run.h"
#endif

typedef enum{
    enL=0,
    enR,
    endir_MAX
}en_endir;

en_endir encoderdir;

void ENC_GetDiv( int32_t* p_r, int32_t* p_l );
void ENC_setref(void);
void ENC_print(void);
void recv_spi_encoder(void);

#endif /* INC_ENCODER_H_ */
