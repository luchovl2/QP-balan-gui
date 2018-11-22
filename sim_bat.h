#ifndef SIM_BAT_H
#define SIM_BAT_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define SIM_BAT_CORRIENTE_mA	(150)
#define SIM_BAT_UPDATE_mS		(200U)

//---------------------------------------------
typedef struct celda_tag
{
	uint16_t tension;
	int16_t corriente;
	uint8_t n_celda;
}Celda;

//---------------------------------------------
int8_t sim_bat_init(uint8_t cant_celdas);
void sim_bat_free(void);
uint16_t sim_bat_get_tension(uint8_t ind_celda);
int16_t sim_bat_get_corriente(uint8_t ind_celda);
void sim_bat_set_tension(uint8_t ind_celda, uint16_t tension);
void sim_bat_set_corriente(uint8_t ind_celda, int16_t corriente);
void sim_bat_update(uint32_t dt_ms);

#endif /* SIM_BAT_H */
