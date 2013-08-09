#include "sckstream.inl"

#include "strbas.h"
#include "asrtbas.h"
#include "memory.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

static const int TAM_BUFFER = 4096;
static const int MAX_COLA = 5;
static const char *i_TEXTO_CONFIRMACION = "CONFIRMADO";

//---------------------------------------------------------

static struct sockaddr_in *i_crea_direccion_servidor(int puerto)
{
	struct sockaddr_in *direccion;

	direccion = MALLOC (struct sockaddr_in);

	bzero(direccion, sizeof(struct sockaddr_in));
	direccion->sin_family = AF_INET;
	direccion->sin_addr.s_addr = INADDR_ANY;
	direccion->sin_port = htons (puerto);

	return direccion;
}

//---------------------------------------------------------

static struct sockaddr_in *i_crea_direccion_cliente(int puerto, const struct hostent *hostname_servidor)
{
	struct sockaddr_in *direccion;
	const struct in_addr *direccion_servidor;

	assert_no_null (hostname_servidor);

	direccion_servidor = (const struct in_addr *)(hostname_servidor->h_addr);

	direccion = MALLOC(struct sockaddr_in);

	bzero(direccion, sizeof(struct sockaddr_in));
	direccion->sin_family = AF_INET;
	direccion->sin_addr.s_addr = htonl (INADDR_ANY);
	direccion->sin_addr.s_addr = direccion_servidor->s_addr;
	direccion->sin_port = htons (puerto);

	return direccion;
}

//---------------------------------------------------------

static int i_abre_socket_stream(void)
{
	int hand_socket, protocolo;

	protocolo = 0;
	hand_socket = socket(AF_INET, SOCK_STREAM, protocolo);
	assert (hand_socket != -1);

	return hand_socket;
}

//---------------------------------------------------------

int sckstream_abrir_servidor(int puerto)
{
	struct sockaddr_in *direccion_servidor;
	int hand_socket, ret;

	direccion_servidor = i_crea_direccion_servidor(puerto);

	hand_socket = i_abre_socket_stream();
	ret = bind (hand_socket, (struct sockaddr *)direccion_servidor, sizeof(struct sockaddr_in));
	assert (ret != -1);

	ret = listen(hand_socket, MAX_COLA);
	assert(ret != -1);

	FREE_T(&direccion_servidor, struct sockaddr_in);

	return hand_socket;
}

//---------------------------------------------------------

static bool i_es_host_correcto(const char *nombre_host, struct hostent **host_servidor)
{
	assert_no_null(nombre_host);
	assert_no_null(host_servidor);
	
	*host_servidor = gethostbyname(nombre_host);

	if (*host_servidor != NULL)
		return true;
	else
		return false;
}

//---------------------------------------------------------

bool sckstream_abrir_cliente(
					int puerto, const char *hostname_servidor, 
					int *hand_socket_cliente, 
					char **mensaje_error)
{
	int ret;
	struct hostent *host_servidor;
	bool resp;

	assert_no_null(hand_socket_cliente);

	resp = i_es_host_correcto(hostname_servidor, &host_servidor);
	
	if (resp == true)
	{
		struct sockaddr_in *direccion_servidor;
			
		direccion_servidor = i_crea_direccion_cliente(puerto, host_servidor);

		*hand_socket_cliente = i_abre_socket_stream();

		ret = connect(
					*hand_socket_cliente, 
					(const struct sockaddr *) direccion_servidor, 
					sizeof(struct sockaddr_in));
					
		assert (ret != -1);

		FREE_T(&direccion_servidor, struct sockaddr_in);
	}
	else
	{
		if (mensaje_error != NULL)
			*mensaje_error = strbas_printf("Error al resolver el nombre del servidor %s", hostname_servidor);
	}
	
	return resp;
}

//---------------------------------------------------------

void sckstream_close(int hand_socket)
{
	int ret;

	ret = close (hand_socket);
	assert (ret != -1);
}

//---------------------------------------------------------

static char *i_obtener_host_cliente_de_mensaje(const struct sockaddr_in *mensaje_aceptado)
{
	struct hostent *host_cliente;
	
	assert_no_null(mensaje_aceptado);
	host_cliente = gethostbyaddr((char *)&(mensaje_aceptado->sin_addr), sizeof(struct in_addr), AF_INET);
	assert_no_null(host_cliente);
	
	return strbas_printf("%s", inet_ntoa((struct in_addr)mensaje_aceptado->sin_addr));
}

//---------------------------------------------------------

int sckstream_aceptar_peticion(int hand_socket_servidor, char **ip_cliente)
{
	int hand_socket_aceptado;
	struct sockaddr_in mensaje_aceptado;
	socklen_t bytesLeidos;
		
	hand_socket_aceptado = accept(hand_socket_servidor, (struct sockaddr *)&mensaje_aceptado, &bytesLeidos);
	assert(hand_socket_aceptado >= 0);
	
	if (ip_cliente != NULL)
		*ip_cliente = i_obtener_host_cliente_de_mensaje(&mensaje_aceptado);
	
	return hand_socket_aceptado;
}

//---------------------------------------------------------

void sckstream_enviar_cadena(int hand_socket, const char *buffer)
{
	int resp;
	int flags;

	flags = 0;
	resp = send(hand_socket, buffer, strbas_length(buffer) + 1, flags);
	assert(resp >= 0);
}

//---------------------------------------------------------

char *sckstream_recibir_cadena(int hand_socket)
{
	char buffer[TAM_BUFFER];
	int numeroBytesRecibidos, flags;

	flags = 0;
	numeroBytesRecibidos = recv (hand_socket, buffer, TAM_BUFFER, flags);
	assert(numeroBytesRecibidos >= 0);
	buffer[numeroBytesRecibidos - 1] = '\0';

	return strbas_copyString(buffer);
}

//---------------------------------------------------------

void sckstream_enviar_confirmacion(int hand_socket)
{
    int flags, resp;
    
    flags = 0;
    resp = send(hand_socket, i_TEXTO_CONFIRMACION, strbas_length(i_TEXTO_CONFIRMACION) + 1, flags);
    assert(resp);
}

//---------------------------------------------------------

void sckstream_recibir_confirmacion(int hand_socket)
{
	char buffer[100];
	int numeroBytesRecibidos, flags;

	flags = 0;
	numeroBytesRecibidos = recv(hand_socket, buffer, 100, flags);
	assert(numeroBytesRecibidos >= 0);
	buffer[numeroBytesRecibidos - 1] = '\0';
    
    assert(strbas_equalsStringWithoutMatchCase(buffer, i_TEXTO_CONFIRMACION) == true);
}
