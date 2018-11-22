#include "sim_bat.h"

//----------------------------------------------------


//-------------------------------------------------
static Celda *bateria=NULL;
static uint8_t l_cant_celdas = 0;
static int32_t capa_celda = 20000;
static uint8_t l_init = 0;

//-------------------------------------------------
int8_t sim_bat_init(uint8_t cant_celdas)
{
	int8_t retorno;
	uint8_t i;

	bateria = (Celda *)malloc(cant_celdas * sizeof(Celda));

	if(bateria == NULL)
	{
		retorno = -1;
		l_init = 0;
	}
	else
	{
		for(i=0; i<cant_celdas; i++)
		{
			bateria[i].tension = 0;
			bateria[i].corriente = 0;
			bateria[i].n_celda = i;
		}
		
		l_cant_celdas = cant_celdas;		
		retorno = 1;
		l_init = 1;
	}

	return retorno;
}
//---------------------------------------------------
void sim_bat_free(void)
{
	l_init = 0;

	if(bateria)
	{
		free(bateria);
	}
}
//------------------------------------------------------
uint16_t sim_bat_get_tension(uint8_t ind_celda)
{
	uint16_t retorno = 0;

	if(ind_celda < l_cant_celdas && l_init != 0)
	{
		retorno = bateria[ind_celda].tension;
	}

	return retorno;
}
//----------------------------------------------------------
int16_t sim_bat_get_corriente(uint8_t ind_celda)
{
	int16_t retorno = 0;

	if(ind_celda < l_cant_celdas && l_init != 0)
	{
		retorno = bateria[ind_celda].corriente;
	}

	return retorno;
}
//-----------------------------------------------------------
void sim_bat_set_tension(uint8_t ind_celda, uint16_t tension)
{
	if(ind_celda < l_cant_celdas && l_init != 0)
	{
		bateria[ind_celda].tension = tension;
	}
}
//-----------------------------------------------------------
void sim_bat_set_corriente(uint8_t ind_celda, int16_t corriente)
{
	if(ind_celda < l_cant_celdas && l_init != 0)
	{
		bateria[ind_celda].corriente = corriente;
	}
}
//-----------------------------------------------------------
void sim_bat_update(uint32_t dt_ms)
{
	uint8_t i;

	if(l_init != 0)
	{
		for(i=0; i<l_cant_celdas; i++)
		{
			bateria[i].tension = (int32_t)bateria[i].tension + (int32_t)(bateria[i].corriente*dt_ms)/capa_celda;
		}
	}
}
