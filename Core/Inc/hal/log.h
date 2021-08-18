/*
 * log.h
 *
 *  Created on: 2021/01/30
 *      Author: shohe
 */

#ifndef INC_LOG_H_
#define INC_LOG_H_

#ifdef _VSCODE
#include "Inc/glob.h"
#include "Inc/hal/sen_dist.h"
#include "Inc/hal/CTRL.h"
#else
#include "glob.h"
#include "hal/sen_dist.h"
#include "hal/CTRL.h"
#endif

float templog1,templog2 ; 

void log_in2( 	float log1,float log2,
			float log3,float log4,
			float log5)/*,float log6,
			float log7,float log8,
			float log9,float log10,
			float log11,float log12)
*/;
void log_interrupt ( void );
void log_flag_on(void);
void log_flag_off(void);
void log_read2(void);

#endif /* INC_LOG_H_ */
