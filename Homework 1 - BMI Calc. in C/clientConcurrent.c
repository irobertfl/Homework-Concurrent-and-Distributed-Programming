#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

extern int errno;

int port;

int
main (int argc, char *argv[])
{
  int socketDesc;			
  struct sockaddr_in server;
  char buffer[100];
  char kilo[100];
  char height[100];
  char result[100];

  if (argc != 3)
    {
      printf ("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
      return -1;
    }

  port = atoi (argv[2]);

  if ((socketDesc = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("Eroare la socket().\n");
      return errno;
    }

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(argv[1]);
  server.sin_port = htons (port);

  if (connect (socketDesc, (struct sockaddr *) &server,
	       sizeof (struct sockaddr)) == -1)
    {
      perror ("Eroare la connect().\n");
      return errno;
    }

  /* citirea kilogramelor si trimiterea catre server */
  bzero (kilo, 100);
  printf ("Introduceti Kilogramele: ");
  fflush (stdout);
  read (0, kilo, 100);

  if (write (socketDesc, kilo, 100) <= 0)
    {
      perror ("Eroare la write() spre server.\n");
      return errno;
    }
  /* citirea inaltimii si trimiterea catre server */
  bzero (height, 100);
  printf ("Introduceti inaltime: ");
  fflush (stdout);
  read (0, height, 100);

  if (write (socketDesc, height, 100) <= 0)
    {
      perror ("Eroare la write() spre server.\n");
      return errno;
    }

  /* citirea raspunsului dat de server */
  if (read (socketDesc, result, 100) < 0)
    {
      perror ("Eroare la read() de la server.\n");
      return errno;
    }
  /* afisare mesaj primit */
  printf ("Mesajul primit este: `%s`.\n", result);

  close (socketDesc);
}