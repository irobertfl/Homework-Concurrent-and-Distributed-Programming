#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>

/* portul folosit */

#define PORT 23235

extern int errno;		/* eroarea returnata de unele apeluri */

/* functie de convertire a adresei IP a clientului in sir de caractere */
char *
conv_addr (struct sockaddr_in address)
{
  static char str[25];
  char port[7];

  /* adresa IP a clientului */
  strcpy (str, inet_ntoa (address.sin_addr));	
  /* portul utilizat de client */
  bzero (port, 7);
  sprintf (port, ":%d", ntohs (address.sin_port));	
  strcat (str, port);
  return (str);
}

/* programul */
int
main ()
{
  struct sockaddr_in server;	/* structurile pentru server si clienti */
  struct sockaddr_in from;
  fd_set readfds;		/* multimea descriptorilor de citire */
  fd_set actfds;		/* multimea descriptorilor activi */
  struct timeval tv;		/* structura de timp pentru select() */
  int sd, client;		/* descriptori de socket */
  int fd;			/* descriptor folosit pentru 
				   parcurgerea listelor de descriptori */
  int nfds;			/* numarul maxim de descriptori */
  int len;			/* lungimea structurii sockaddr_in */

  /* creare socket */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("Eroare la socket().\n");
      return errno;
    }
  /* pregatim structurile de date */
  bzero (&server, sizeof (server));

  /* umplem structura folosita de server */
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = htonl (INADDR_ANY);
  server.sin_port = htons (PORT);

  /* atasam socketul */
  if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
      perror ("Eroare la bind().\n");
      return errno;
    }

  /* punem serverul sa asculte daca vin clienti sa se conecteze */
  if (listen (sd, 5) == -1)
    {
      perror ("Eroare la listen().\n");
      return errno;
    }
  
  /* completam multimea de descriptori de citire */
  FD_ZERO (&actfds);		/* initial, multimea este vida */
  FD_SET (sd, &actfds);		/* includem in multime socketul creat */

  tv.tv_sec = 1;		/* se va astepta un timp de 1 sec. */
  tv.tv_usec = 0;
  
  /* valoarea maxima a descriptorilor folositi */
  nfds = sd;

  printf ("Asteptam la portul %d...\n", PORT);
  fflush (stdout);
        
  /* servim in mod concurent clientii... */
  while (1)
    {
      /* ajustam multimea descriptorilor activi (efectiv utilizati) */
      bcopy ((char *) &actfds, (char *) &readfds, sizeof (readfds));

      /* apelul select() */
      if (select (nfds+1, &readfds, NULL, NULL, &tv) < 0)
	{
	  perror ("Eroare la select().\n");
	  return errno;
	}
      /* vedem daca e pregatit socketul pentru a-i accepta pe clienti */
      if (FD_ISSET (sd, &readfds))
	{
	  /* pregatirea structurii client */
	  len = sizeof (from);
	  bzero (&from, sizeof (from));

	  /* a venit un client, acceptam conexiunea */
	  client = accept (sd, (struct sockaddr *) &from, &len);

	  /* eroare la acceptarea conexiunii de la un client */
	  if (client < 0)
	    {
	      perror ("Eroare la accept().\n");
	      continue;
	    }

          if (nfds < client) /* ajusteaza valoarea maximului */
            nfds = client;
            
	  /* includem in lista de descriptori activi si acest socket */
	  FD_SET (client, &actfds);

	  printf
	    ("S-a conectat clientul cu descriptorul %d, de la adresa %s.\n",
	     client, conv_addr (from));
	  fflush (stdout);
	}
      /* vedem daca e pregatit vreun socket client pentru a face echo */
      for (fd = 0; fd <= nfds; fd++)	/* parcurgem multimea de descriptori */
	{
	  /* este un socket de citire pregatit? */
	  if (fd != sd && FD_ISSET (fd, &readfds))
	    {	
	      if (readfunction(fd) == 0)	/* n-a putut fi trimis mesajul */
		{
		  printf ("S-a deconectat clientul cu descriptorul %d.\n",
			  fd);
		  fflush (stdout);
		  close (fd);		/* inchidem conexiunea cu clientul */
		  FD_CLR (fd, &actfds);/* scoatem si din multime */
		  
		}
	    }
	}			/* for */
    }				/* while */
}				/* main */

/* realizeaza primirea si retrimiterea unui mesaj unui client */
int
readfunction (int fd)
{
  //char buffer[100];		/* mesajul */
  char kilo[100];		/* mesajul */
  float kiloD;
  char height[100];		/* mesajul */
  float heightD;
  float bmi;
  char bmiSting[100];
  char result[100];		/* mesajul */
  int bytes;			/* numarul de octeti cititi/scrisi */

  bytes = read (fd, kilo, sizeof (kilo));
  if (bytes < 0)
    {
      perror ("Eroare la read() de la client.\n");
      return 0;
    }
  
  bytes = read (fd, height, sizeof (height));
  if (bytes < 0)
    {
      perror ("Eroare la read() de la client.\n");
      return 0;
    }
  
  //heightInt = (int)(height);
  //kiloInteger = (int)(kilo);
  heightD = atof(height);
  kiloD = atof(kilo);
  strcpy(result, " Kilo : ");
  strcat(result, kilo);
  strcat(result, " Height : ");
  strcat(result, height);
  //itoa()
  // BMI = (Weight in Kilograms / (Height in Meters x Height in Meters))
  bmi = (kiloD/(heightD*heightD))*10000;
  printf("BMI is %f\n",bmi);
  //result = (string)bmi
  //itoa(bmi, result,100);.
  sprintf(bmiSting, "%f", bmi);
  strcat(result, " Bmi : ");
  strcat(result, bmiSting);
  if (bytes && write (fd, result, bytes) < 0)
    {
      perror ("Eroare la write() catre client.\n");
      return 0;
    }
  
  return bytes;
}