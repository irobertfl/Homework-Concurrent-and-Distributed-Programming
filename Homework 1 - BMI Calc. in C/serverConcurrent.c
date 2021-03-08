#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>

#define PORT 23235

extern int errno;

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
  int socketDesc, client;		/* descriptori de socket */
  int fd;			/* descriptor folosit pentru 
				   parcurgerea listelor de descriptori */
  int nfds;			/* numarul maxim de descriptori */
  int len;			/* lungimea structurii sockaddr_in */

  if ((socketDesc = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("Eroare la socket().\n");
      return errno;
    }

  bzero (&server, sizeof (server));

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = htonl (INADDR_ANY);
  server.sin_port = htons (PORT);

  if (bind (socketDesc, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
      perror ("Eroare la bind().\n");
      return errno;
    }
  if (listen (socketDesc, 5) == -1)
    {
      perror ("Eroare la listen().\n");
      return errno;
    }
  
  FD_ZERO (&actfds);		
  FD_SET (socketDesc, &actfds);	

  tv.tv_sec = 1;
  tv.tv_usec = 0;
  
  /* valoarea maxima */
  nfds = socketDesc;

  printf ("Asteptam la portul %d...\n", PORT);
  fflush (stdout);
        
  while (1)
    {
      /* ajustare descriptori activi */
      bcopy ((char *) &actfds, (char *) &readfds, sizeof (readfds));

      if (select (nfds+1, &readfds, NULL, NULL, &tv) < 0)
	{
	  perror ("Eroare la select().\n");
	  return errno;
	}
      if (FD_ISSET (socketDesc, &readfds))
	{
	  len = sizeof (from);
	  bzero (&from, sizeof (from));

	  client = accept (socketDesc, (struct sockaddr *) &from, &len);

	  if (client < 0)
	    {
	      perror ("Eroare la accept().\n");
	      continue;
	    }

    if (nfds < client) 
      nfds = client;
    
	  FD_SET (client, &actfds);

	  printf("S-a conectat clientul cu numarul %d, de la adresa %s.\n",client, conv_addr (from));
      fflush (stdout);
	}
      for (fd = 0; fd <= nfds; fd++)
	    {
          if (fd != socketDesc && FD_ISSET (fd, &readfds))
            {	
              if (readAndCalculateBMI(fd) == 0)	/* n-a putut fi trimis mesajul */
              {
                printf ("S-a deconectat clientul cu descriptorul %d.\n",fd);
                fflush (stdout);
                close (fd);
                FD_CLR (fd, &actfds);
              }
            }
      }
    }	
}	
int readAndCalculateBMI (int fd)
{
  
  char kilo[100];		/* kilograme citite */
  float kiloD;
  char height[100];		/* inaltime citita */
  float heightD;
  float bmi;
  char bmiSting[100];
  char result[100];		/* rezultat afisat */
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
  // BMI = (Weight in Kilograms / (Height in Meters x Height in Meters))
  bmi = (kiloD/(heightD*heightD))*10000;
  printf("BMI is %f\n",bmi);
  //result = (string)bmi
  //itoa(bmi, result,100);.
  sprintf(bmiSting, "%f", bmi);
  strcat(result, " Bmi : ");
  strcat(result, bmiSting);
  //strcat(result, checkBMIValue(bmi));
  if (bytes && write (fd, result, bytes) < 0)
    {
      perror ("Eroare la write() catre client.\n");
      return 0;
    }
  return bytes;
}

// char * checkBMIValue (float bmi){
//   char bmiValue[100];
//   if(bmi < 15)  
//     {  
//         strcpy(bmiValue,"Your BMI category is: Starvation"); 
//     }  
//     else if(bmi >= 15.1 && bmi <= 17.5)  
//     {  
//         strcpy(bmiValue,"Your BMI category is: Anorexic");
//     }  
//     else if(bmi >= 17.6 && bmi <= 18.5)  
//     {  
//         strcpy(bmiValue,"Your BMI category is: Underweight");
//     }  
//     else if(bmi >= 18.6 && bmi <= 24.9)  
//     {  
//         strcpy(bmiValue,"Your BMI category is: Ideal");
//     }  
//     else if(bmi >= 25 && bmi <= 25.9)  
//     {  
//         strcpy(bmiValue,"Your BMI category is: Overweight");
//     }  
//     else if(bmi >= 30 && bmi <= 30.9)  
//     {  
//         strcpy(bmiValue,"Your BMI category is: Obese");
//     }  
//     else if(bmi >= 40)  
//     {  
//         strcpy(bmiValue,"Your BMI category is: Morbidly Obese");
//     }
//     else  
//     {  
//         strcpy(bmiValue,"NaN");
//     }

//     return bmiValue;
// }