#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	valor = config_get_string_value(config, "CLAVE");

	log_info(logger, "IP: %s", ip);
	log_info(logger, "PUERTO: %s", puerto);
	log_info(logger, "CLAVE: %s", valor);


	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor, conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
	return 0;
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create("tp0.log", "CLIENT", true, LOG_LEVEL_INFO);

	if(nuevo_logger == NULL){
		printf("No se pudo crear el logger\n");
		abort();
	}

	log_info(nuevo_logger, "Hola! Soy un log");

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config = config_create("/home/utnso/tp0/client/cliente.config");

	if(nuevo_config == NULL){
		printf("No se pudo crear el config");
		abort();
	}

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido = NULL;

	while(1){
		leido = readline("> ");

		if(strcmp(leido, "") == 0){
			free(leido);
			break;
		}

		log_info(logger, "%s", leido);
		free(leido);
	}
	
}

void paquete(int conexion)
{
	char* leido;
	t_paquete* paquete = crear_paquete();

	while(1){
		leido = readline("> ");

		if(strcmp(leido, "") == 0){
			free(leido);
			break;
		}

		agregar_a_paquete(paquete, leido, strlen(leido) + 1);
		free(leido);
	}

	enviar_paquete(paquete, conexion);
	eliminar_paquete(paquete);

}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	if(config != NULL){
		config_destroy(config);
	}

	if(logger != NULL){
		log_destroy(logger);
	}

	liberar_conexion(conexion);
}
