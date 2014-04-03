#include <apricot/cookie.h>
#include <apricot/conf.h>
#include <apricot/log.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>

void cookie_create(char * dest)
{
  char buf[255];
  char * tmp;
  int fd;
  
  /* on recupere le nom du dossier courant du serveur car on sait que le
  dossier des cookies se trouve dans le meme dossier que le serveur */
  if(!getcwd(buf, 254))
  {
  	log_error("failed to get current working directory in cookie_create");
  	strcpy(dest, "");
  	return;
  }
  
  strncat(buf, "/cookies", 254);
  
  /* on genere un nom de fichier temporaire pour le cookie
  si ça marche on crée le fichier avec les permissions les plus
  réduites possibles
  */
  tmp = tempnam(buf, "c");
  
  if(tmp == NULL)
	strcpy(dest, "");
  else
  {
  	if((fd = creat(tmp, O_RDWR | S_IRUSR | S_IWUSR)) == -1)
  	{
  		strcpy(dest, "");
  		log_error("cookie file creation failed %s", tmp);
  	}
  	else
  	{
  		strcpy(dest, tmp);
  		close(fd);
  	}
  }
}

void cookie_remove_all()
{
	char entryname[1024];
	char dirname[512];
	
	if(getcwd(dirname, 254))
	{
		strcat(dirname, "/cookies/");
		
		DIR * d = opendir(dirname);
		
		if(!d)
		{
			log_error("cookie directory not found %s", dirname);
			return;
		}
			
		struct dirent * element;
		
		while((element = readdir(d)))
		{
			/* si le fichier dans le dossier n'est pas un cookie 
			* on le saute	
			*/
			if(element->d_name[0] != 'c')
				continue;
		
			strcpy(entryname, dirname);
			strcat(entryname, element->d_name);
			
			if(unlink(entryname) < 0)
			{
				log_error("failed to remove cookie %s %s", entryname, strerror(errno));
			}
		}
	}
}
