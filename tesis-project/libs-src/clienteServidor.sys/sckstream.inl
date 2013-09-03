// Interface sobre socket ocultando los detalles de sockets.

int sckstream_abrir_servidor(int puerto);

bool sckstream_abrir_cliente(
					int puerto, const char *hostname_servidor, 
					int *hand_socket_cliente, 
					char **mensaje_error);

void sckstream_close (int hand_socket);

int sckstream_aceptar_peticion(int hand_socket_servidor, char **ip_cliente);

void sckstream_enviar_cadena(int hand_socket, const char *buffer);

char *sckstream_recibir_cadena(int hand_socket);

void sckstream_enviar_confirmacion(int hand_socket);

void sckstream_recibir_confirmacion(int hand_socket);


