#ifndef bsp_h
#define bsp_h

//---------------------------------------------
typedef enum salida_estado_tag
{
    PWM_APAGADO=0,
    PWM_ARRIBA_PRIMERO,
    PWM_ABAJO_PRIMERO,
}Salida_estado;

typedef struct pwm_salida_tag
{
	uint32_t pin_sup;
	uint32_t pin_inf;
	uint32_t puerto;
	uint32_t pol_bit;
	Salida_estado update;
}Pwm_salida;

//-------------------------------------------
#define BSP_TICKS_PER_SEC    (100U)

#define BSP_CONV_MS_2_TICKS(time_ms)	((BSP_TICKS_PER_SEC*time_ms) / (1000U))

//Del PWM-----------------------------
#define FREC_CLOCK_PWM 		(25000000U)			//frecuencia de clock del módulo de pwm, se define con el prescaler
#define FREC_CLOCK_PWM_MHZ 	(25U)

#define FREC_CONMUTACION (3U)  					//frecuencia de conmutación de los transistores, en hercios

#define MATCH_MAX (FREC_CLOCK_PWM / FREC_CONMUTACION)	//valor de match de reset (más uno)
#define MATCH_VALOR(a) (((a)*MATCH_MAX)/100)				//convierte de porcentual a valor de match

//#define DEAD_TIME	15		//cantidad de ticks del timer de pwm de tiempo muerto (con 25MHz, 1 son 40ns)
#define DEAD_TIME	(1023U)	//para el módulo de motor son 10 bits, máximo 1023 (~40 us con 25MHz de clock)

#define PWM_CICLOS_OFF	(3U)	//cantidad de ciclos de pwm que se mantiene apagado entre actualizaciones

//Dependiente de la plataforma
#define PUERTO_SUP1		1
#define PUERTO_INF1		1
#define PUERTO_SUP2		1
#define PUERTO_INF2		1
#define PUERTO_SUP3		1
#define PUERTO_INF3		1
#define PIN_SUP1		26
#define PIN_INF1		25
#define PIN_SUP2		12
#define PIN_INF2		13
#define PIN_SUP3		14
#define PIN_INF3		15

#define CANAL_PAR1		1		//canal del módulo de control de motor del par SUP1-INF1 (COHERENTE CON PINES Y PUERTO)
#define CANAL_PAR2		0		//canal del módulo de control de motor del par SUP2-INF2
#define CANAL_PAR3		2		//canal del módulo de control de motor del par SUP3-INF3

#define POLARIDAD_PAR1	10		//bit que controla la polaridad del canal del par SUP1-INF1	(COHERENTE CON "CANAL_PAR1")
#define POLARIDAD_PAR2	2		//bit que controla la polaridad del canal del par SUP2-INF2
#define POLARIDAD_PAR3	18		//bit que controla la polaridad del canal del par SUP3-INF3

//Del ADC-----------------------
#define ADC_FREC_MAX	(200000UL)		//frecuencia máxima del ADC en Hz (hardware)
#define ADC_FREC_MIN	(2000UL)		//frecuencia mínima del ADC en Hz (hardware)

#define ADC_MAX		(1<<12)			//cantidad de valores que puede representar (2^12)

#define ADC_TIEMPO_MEDICION_MS		(4U)	//tiempo de actualización de las mediciones de todas las celdas
											//(cada cuanto se tiene una nueva medición promediada de todas las celdas)

#define ADC_CANT_PROMEDIO	(30U)		//cantidad de lecturas que promedia por celda
#define ADC_FRECUENCIA		((ADC_CANT_PROMEDIO * CANT_CELDAS * (1000UL)) / ADC_TIEMPO_MEDICION_MS)
						//frecuencia de muestreo del ADC (en Hz) (menor a 200kHz), pero mayor a... 1k, creo
						//cantidad de muestras (por mil) sobre el tiempo (en ms)

#if (ADC_FRECUENCIA < ADC_FREC_MIN)
#error	Frecuencia menor a la minima
#endif

#if (ADC_FRECUENCIA > ADC_FREC_MAX)
#error	Frecuencia mayor a la maxima
#endif

#if (ADC_CANT_PROMEDIO > ((0xFFFFFFFFUL) / ADC_MAX))
#error	Cantidad a promediar muy grande (desborde acumulador)
#endif

#define ADC_VCELDAMAX_mV		(4300U)	//tensión de celda correspondiente a la máxima que mide el ADC
#define ADC_VREF_mV				(3300U)	//tensión de referencia del ADC en mV
#define CONV_ADC2VOLT_mV(a)		(((a)*ADC_VREF_mV)/ADC_MAX)	//convierte de cuentas a mV
#define CONV_ADC2VCELDA_mV(a)	((CONV_ADC2VOLT_mV(a)*ADC_VCELDAMAX_mV)/ADC_VREF_mV)

//Dependientes de la plataforma
#define ADC_CANAL_MIN	0		//número de canal mínimo de la plataforma
#define ADC_CANAL_MAX	7		//número de canal máximo de la plataforma (supone que los valores intermedios son válidos)

#define ADC_CANAL_NOCEL	8

#define ADC_CANAL_CEL1	2		//número de canal del ADC para medir la celda 1
#define ADC_CANAL_CEL2	3		//número de canal del ADC para medir la celda 2
#define ADC_CANAL_CEL3	4		//número de canal del ADC para medir la celda 3
#define ADC_CANAL_CEL4	5		//número de canal del ADC para medir la celda 4

#define ADC_INT_CEL1	ADC_ADINTEN2	//enum para habilitar la interrupción del canal elegido
#define ADC_INT_CEL2	ADC_ADINTEN3
#define ADC_INT_CEL3	ADC_ADINTEN4
#define ADC_INT_CEL4	ADC_ADINTEN5

#define ADC_CANAL_PIN1 	PINSEL_PIN_25		//enum del pin para pinsel
#define ADC_CANAL_PIN2 	PINSEL_PIN_26		//enum del pin para pinsel
#define ADC_CANAL_PIN3 	PINSEL_PIN_30		//enum del pin para pinsel
#define ADC_CANAL_PIN4 	PINSEL_PIN_31		//enum del pin para pinsel

//-----------------------------------------------
void BSP_init(void);
void BSP_terminate(int16_t result);
void BSP_config_adc(void);
void BSP_config_pwm(void);
void BSP_pwm_int_off(void);
void BSP_pwm_int_on(void);
void BSP_pwm_update(Salida_estado *salidas_est);
void BSP_pwm_apagar(void);
void BSP_pwm_activar(Pwm_salida *pwm_salidas);
void BSP_adc_stop(void);
void BSP_adc_start(void);
void BSP_adc_convert(void);
void BSP_adc_sel_celda(uint8_t celda);
uint8_t BSP_adc_canal(uint8_t celda);
uint8_t BSP_adc_celda(uint8_t canal);
void BSP_adc_int_off(uint8_t celda);
void BSP_adc_int_on(uint8_t celda);

void BSP_led_toggle(void);
void BSP_led_on(uint32_t puerto, uint32_t led);
void BSP_led_off(uint32_t puerto, uint32_t led);

#endif /* bsp_h */
