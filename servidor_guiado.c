#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

void *AtenderCliente(void *socket)
{
	int sock_conn, ret;
	int *s;
	s = (int *)socket;
	sock_conn = *s;
	char peticion[512];
	char respuesta[512];
	
	int terminar = 0;
	
	while (terminar == 0)
	{
		ret = read(sock_conn, peticion, sizeof(peticion));
		if (ret <= 0) {
			// Si ret <= 0, significa que hubo un error o que el cliente cerró la conexión
			terminar = 1;
			continue;
		}
		
		printf("Recibido\n");
		peticion[ret] = '\0';
		
		// Escribimos la petición en la consola
		printf("Peticion: %s\n", peticion);
		
		// Vemos qué quieren
		char *p = strtok(peticion, "/");
		int codigo = atoi(p);
		p = strtok(NULL, "/");
		char nombre[50];
		strcpy(nombre, p);
		printf("Codigo: %d, Nombre: %s\n", codigo, nombre);
		
		if (codigo == 0)
			terminar = 1;
		else if (codigo == 1)
		{
			sprintf(respuesta, "%d", strlen(nombre));
		}
		else if (codigo == 2)
		{
			if ((nombre[0] == 'M') || (nombre[0] == 'S'))
				strcpy(respuesta, "SI");
			else
				strcpy(respuesta, "NO");
		}
		else if (codigo == 3)
		{
			p = strtok(NULL, "/");
			float altura = atof(p);
			if (altura > 1.7)
				sprintf(respuesta, "%s: eres alto", nombre);
			else
				sprintf(respuesta, "%s: no eres alto", nombre);
		}
		printf("Respuesta: %s\n", respuesta);
		
		write(sock_conn, respuesta, strlen(respuesta));
	}
	
	// Cerrar el socket y liberar memoria
	close(sock_conn);
	free(s);
	
	return NULL;
}

int main(int argc, char *argv[])
{
	int sock_conn, sock_listen, ret;
	struct sockaddr_in serv_adr;
	
	// Abrimos el socket
	if ((sock_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("Error creando el socket\n");
		return -1;
	}
	
	// Hacemos Bind en el puerto
	memset(&serv_adr, 0, sizeof(serv_adr));  // Inicializamos a cero serv_adr
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);  // Escuchar en cualquier IP local
	serv_adr.sin_port = htons(9050);  // Escuchar en el puerto 9050
	
	if (bind(sock_listen, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) < 0)
	{
		printf("Error en el bind\n");
		return -1;
	}
	
	if (listen(sock_listen, 3) < 0)
	{
		printf("Error en el listen\n");
		return -1;
	}
	
	pthread_t thread;
	
	// Bucle principal para aceptar conexiones de clientes
	for (;;)
	{
		printf("Escuchando\n");
		sock_conn = accept(sock_listen, NULL, NULL);
		if (sock_conn < 0)
		{
			printf("Error al aceptar la conexión\n");
			continue;
		}
		printf("He recibido conexion\n");
		
		// Crear un nuevo hilo para atender al cliente
		int *p_sock_conn = malloc(sizeof(int));
		*p_sock_conn = sock_conn;
		pthread_create(&thread, NULL, AtenderCliente, p_sock_conn);
		pthread_detach(thread);  // Desacopla el hilo
	}
	
	return 0;
}


