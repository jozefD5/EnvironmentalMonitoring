/*
 * serialcommands.h
 *
 *  Created on: 19 Oct 2022
 *      Author: jozef.duda
 */

#ifndef ENVCORE_SERIALCOM_SERIALCOMMANDS_H_
#define ENVCORE_SERIALCOM_SERIALCOMMANDS_H_




//Serial commands, system is using '\t' to indicate end of the message. Escape character '\n' is not used as many high level frameworks
//automatically add new line. This prevents system from miss understanding commands
//RX commands
#define    ENV_SC_ACTIVATE         "env_aenvm\t"            //activate environment monitoring
#define    ENV_SC_DEACTIVATE       "env_deaenvm\t"          //deactivate environment monitoring

#define    ENV_SC_TEMP_ACTIVE      "env_temp_act\t"         //activate temperature monitoring
#define    ENV_SC_TEMP_DEACTIVE    "env_temp_dact\t"        //deactivate temperature monitoring

#define    ENV_SC_PRES_ACTIVE      "env_pres_act\t"         //activate temperature monitoring
#define    ENV_SC_PRES_DEACTIVE    "env_pres_dact\t"        //deactivate temperature monitoring

#define    ENV_SC_RSTATUS          "env_rstatus\t"          //read monitoring status



//TX commands
#define    ENV_SC_STATUS           "env_status"             //monitoring status
#define    ENV_SC_TEMP_DATA        "env_temp"               //temperature in deg C
#define    ENV_SC_PRES_DATA        "env_pres"               //pressure in psi





#endif /* ENVCORE_SERIALCOM_SERIALCOMMANDS_H_ */
