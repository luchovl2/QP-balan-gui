/*
===============================================================================
 Name        : qp_balan.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

//#include <cr_section_macros.h>

#include "qpc.h"
#include "balan.h"
#include "bsp.h"

//-------------------------------------
int main()
{
    static QEvt const *medicionQueueSto[5];
    static QEvt const *controlQueueSto[5];
    static QSubscrList subscrSto[MAX_PUB_SIG];
    static QF_MPOOL_EL(VceldasEvt)smlPoolSto[2*CANT_CELDAS]; /* small pool */

    Medicion_ctor(); 	/* instantiate all Philosopher active objects */
    Control_ctor(); 	/* instantiate the Table active object */

    QF_init();    /* initialize the framework and the underlying RT kernel */
    BSP_init();   /* initialize the Board Support Package */

    /* initialize publish-subscribe... */
    QF_psInit(subscrSto, Q_DIM(subscrSto));

    /* initialize event pools... */
    QF_poolInit(smlPoolSto, sizeof(smlPoolSto), sizeof(smlPoolSto[0]));

    /* start the active objects... */
	QACTIVE_START(AO_Control,            /* AO to start */
				  (uint_fast8_t)(2),	 /* QP priority of the AO */
				  controlQueueSto,      /* event queue storage */
				  Q_DIM(controlQueueSto), /* queue length [events] */
				  (void *)0,             /* stack storage (not used) */
				  0U,                    /* size of the stack [bytes] */
				 (QEvt *)0);             /* initialization event */

    QACTIVE_START(AO_Medicion,               /* AO to start */
                  (uint_fast8_t)(1),		 /* QP priority of the AO */
                  medicionQueueSto,          /* event queue storage */
                  Q_DIM(medicionQueueSto),   /* queue length [events] */
                  (void *)0,                 /* stack storage (not used) */
                  0U,                        /* size of the stack [bytes] */
                  (QEvt *)0);                /* initialization event */

    return QF_run(); /* run the QF application */
}
