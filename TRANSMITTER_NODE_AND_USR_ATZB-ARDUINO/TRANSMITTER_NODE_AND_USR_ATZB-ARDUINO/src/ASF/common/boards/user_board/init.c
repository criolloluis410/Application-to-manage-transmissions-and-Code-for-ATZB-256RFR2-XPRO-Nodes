#include <asf.h>
#include <board.h>
#include <periodic_timer.h>
#include <sio2host.h>



/**
 * \brief Initializes the board components
 */
void board_init(void)
{
	// INICIALIZACION DE PUERTOS
	//Para el pulsador.
	ioport_configure_pin(GPIO_PUSH_BUTTON_0,IOPORT_DIR_INPUT | IOPORT_PULL_UP);
	//Para diodos led.
	ioport_configure_pin(LED0, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	ioport_configure_pin(LED1, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	ioport_configure_pin(LED2, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	
}

/**
 * \brief Initializes the modules added by the Project Wizard
 */
void modules_init(void)
{
	    app_timers_init();
sio2host_init();


}