#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

/* codul de eroare returnat de anumite apeluri */
extern int errno;

/* portul de conectare la server*/
int port;

/* programul */
int
main (int argc, char *argv[])
{
  int sd;			/* descriptorul de socket */
  struct sockaddr_in server;	/* structura folosita pentru conectare */
  char buffer[100];		/* mesajul trimis */
  char kilo[100];
  char height[100];
  char result[100];

  /* exista toate argumentele in linia de comanda? */
  if (argc != 3)
    {
      printf ("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
      return -1;
    }

  /* stabilim portul */
  port = atoi (argv[2]);

  /* cream socketul */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("Eroare la socket().\n");
      return errno;
    }

  /* umplem structura folosita pentru realizarea conexiunii cu serverul */
  server.sin_family = AF_INET;
  		/* familia socket-ului */
  server.sin_addr.s_addr = inet_addr(argv[1]);
		/* adresa IP a serverului */
  server.sin_port = htons (port);
  		/* portul de conectare */

  /* ne conectam la server */
  if (connect (sd, (struct sockaddr *) &server,
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

  if (write (sd, kilo, 100) <= 0)
    {
      perror ("Eroare la write() spre server.\n");
      return errno;
    }
  /* citirea inaltimii si trimiterea catre server */
  bzero (height, 100);
  printf ("Introduceti inaltime: ");
  fflush (stdout);
  read (0, height, 100);

  if (write (sd, height, 100) <= 0)
    {
      perror ("Eroare la write() spre server.\n");
      return errno;
    }

  /* citirea raspunsului dat de server 
     (ne blocam pina cind serverul raspunde) */
  if (read (sd, result, 100) < 0)
    {
      perror ("Eroare la read() de la server.\n");
      return errno;
    }
  /* afisam mesajul primit */
  printf ("Mesajul primit este: `%s`.\n", result);

  /* inchidem conexiunea, am terminat */
  close (sd);
}