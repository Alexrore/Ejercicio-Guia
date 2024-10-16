#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

void *AtenderCliente (void *scoket)
{
	int sock_conn, ret;
	int *s;
	s =(int *) socket;
	sock_conn = *s;
	char peticion[512];
	char respuesta[512];
	
	int terminar =0;
	
	while (terminar ==0)
	{
		ret = read(sock_conn,peticion, sizeof(peticion));
		printf("Recibido\n");
		peticion[ret]='\0';
		
		//escribimos el nombre en la consola
		
		printf("Peticion: %s\n", peticion);
		//vemos que quieren
		char *p = strtok(peticion, "/");
		int codigo = atoi(p);
		p=strtok( NULL, "/");
		char nombre[50];
		strcpy(nombre, p);
		printf("Codigo: %d, Nombre: %s\n", codigo, nombre);
		
		if (codigo==0)
			terminar=1;
		else if (codigo==1)
		{
			sprintf(respuesta, "%d",strlen (nombre));
		}
		else if (codigo == 2)
		{
			if((nombre[0]=='M') || (nombre[0] == 'S'))
				strcpy (respuesta, "SI");
			else 
				strcpy (respuesta, "NO");
		}
		else if( codigo ==3)
		{
			p=strtok( NULL, "/");
			float altura = atof(p);
			if (altura > 1.7)
				sprintf(respuesta, "%s: eres alto", nombre);
			else 
				sprintf(respuesta, "%s: no eres alto", nombre);
		}
		printf ("Respuesta: %s\n", respuesta);
		
		write (sock_conn,respuesta, strlen(respuesta));
		close(sock_conn);
	}
}
int main(int argc, char *argv[]) {
	int sock_conn, sock_listen, ret;
	struct sockaddr_in serv_adr;
	
	//Abrimos el socket
	if ((sock_listen = socket(AF_INET, SOCK_STREAM, 0))<0)
	{
		printf("Error creando el socket");
	}
	//Hacemos Bind en el puerto
	
	memset(&serv_adr, 0, sizeof(serv_adr)); //Inicia a zero el serv_adr
	serv_adr.sin_family = AF_INET;	
	
	//asocia el socket a cualquier ip de la maquina
	//htonl formatea el numero que recibe al formato necesario
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	//escuchamos en el puerto 9050
	serv_adr.sin_port = htons(9050);
	if (bind(sock_listen,(struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0)
		printf("Error al bind");
	if (listen(sock_listen, 3)< 0)
		printf("Error en el Listen");
	int sockets[100];
	pthread_t thread;
	int i =0;
	for (;;)
	{
		
		printf("Escuchando\n");
		sock_conn = accept(sock_listen, NULL, NULL);
		printf("He recibido conexion\n");
		//sock_conn es el socket que usaremos para este cliente
		sockets[i] = sock_conn;
		
		
		pthread_create(&thread, NULL, AtenderCliente, &sockets[i]);
		
		i++;
		
	}

	
	
	return 0;
}


