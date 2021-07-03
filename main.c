#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <fcntl.h>
#include <sysexits.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <time.h>  
#include <locale.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define MAXCHAR 1000

#include <assert.h>
#include <wchar.h>
#include <wctype.h>
#include "Converter/utf8_to_cp1251.h"




int min(int a, int b, int c)
{	
	if(a <= b && a <= c)
	{
		return a;
	}
	else if(b <= a && b <= c)
	{
		return b;
	}
	else if(c <= a && c <= b)
	{
		return c;
	}
}

int levenshtein(char *s1,unsigned int s1len, char *s2, int s2len) {
    unsigned int x, y;
    unsigned int matrix[s2len+1][s1len+1];
    matrix[0][0] = 0;
    for (x = 1; x <= s2len; x++)
        matrix[x][0] = matrix[x-1][0] + 1;
    for (y = 1; y <= s1len; y++)
        matrix[0][y] = matrix[0][y-1] + 1;
    for (x = 1; x <= s2len; x++)
        for (y = 1; y <= s1len; y++)
            matrix[x][y] = min(matrix[x-1][y] + 1, matrix[x][y-1] + 1, matrix[x-1][y-1] + (s1[y-1] == s2[x-1] ? 0 : 1));

    return(matrix[s2len][s1len]);
}


void delete_timestamps (char* path_to_file,char * folder_name,char * file_name,int size){
    FILE *fp;
    char str[size];
    setlocale(LC_ALL, "Rus");
	
    //unsigned int phrase_len;
    //phrase_len = strlen(phrase);
    // char new_file_name="result.txt";
    FILE *fptr;
    
   // use appropriate location if you are using MacOS or Linux
    
   
    //char* filename = "c:\\temp\\test.txt";
    //printf("%s\n",filename);
    fp = fopen(path_to_file, "r");
    char n_file[MAXCHAR];
    strcpy( n_file, "../new_Files/");
    strcat( n_file, folder_name);
    strcat(n_file, "/");
    struct stat st = {0};
    if (stat(n_file, &st) == -1) { // check if directory exist
      mkdir(n_file, 0700); // creating a directory
    }
    strcat( n_file, file_name);
    //printf("%s" , n_file);
    fptr = fopen(n_file , "w");
    
    if (fp == NULL){
        printf("Could not open file %s",file_name);  
    }
    size_t result = fread (str,1,size,fp);
    if (result != size){return;} //{fputs ("Reading error",stderr); exit (3);}
    //printf("NEWLINE:");

    
    int is_end = 0;
    //int is_numder = 1;
    int i = 0;
    while(!is_end){
      if(isdigit(str[i])){
	while(str[i] != '\n'){
	  i++;
	}
      }
      if(str[i] != '\n'){
	fprintf(fptr,"%c",towupper(str[i]));
      }
      
      if(str[i] == '\0') break;
      i++;
    }
    //sleep(2000);
    int index = 0;
    //fprintf(fptr,"%s",str);
    fclose(fp);
    fclose(fptr);

}

void update_subtitles(){
  //setlocale(LC_ALL, "Rus");
    printf("Update Subtitles...\n");
  //making clock to see execution time
    double time_spent = 0.0;  
    clock_t begin = clock();
    // Path to directory with subtitles
    char folder_name[255];
    strcpy( folder_name, "../Files");
    
    DIR *folder;
    struct dirent * entry;
    int files = 0;
    
    //char folder_name[] = "./Files";

    // open directory with subtitles 
    folder = opendir(folder_name);
    if(folder == NULL)
    {
        perror("Unable to read directory");
        return;
    }
    
    while( (entry=readdir(folder)) ) // iterating in directory
    {
      
      if(!strcmp(entry->d_name,".") || !strcmp(entry->d_name,"..")){
     	  continue;
        }
      //printf("%s\n", entry->d_name);
        char path_to_subfolder[255];
	strcpy( path_to_subfolder, "../Files/"); // making path to str file
        files++;
	strcat( path_to_subfolder, entry->d_name );
	strcat(path_to_subfolder,"/");
	DIR * subfolder;
	struct dirent *file;
	subfolder = opendir(path_to_subfolder);
	
	//	printf("FILENAME:%s\n",path_to_subfolder);
	while((file=readdir(subfolder))){
	  if(!strcmp(file->d_name,".") || !strcmp(file->d_name,"..")){
     	     continue;
          }
	   char path_to_file[255];
	   
           strcpy( path_to_file, path_to_subfolder);
	   struct stat st;
	   int size;
	   strcat( path_to_file, file->d_name );
	
	   if (stat (path_to_file, &st) == 0) size = st.st_size; // SIZE OF FILE
	   printf("%s\n", entry->d_name);
	   struct stat st_f = {0};
	   char upd_file_path[255];
	   
	   strcpy(upd_file_path,"../new_Files/");
	   strcat(upd_file_path,entry->d_name);
	   if (stat(upd_file_path, &st) == -1) { // check if directory exist
     	       mkdir(upd_file_path, 0700); // creating a directory
     	    }
	   strcat(upd_file_path,"/");
	   strcat(upd_file_path, file->d_name);
	   
	   if (stat(upd_file_path, &st_f) != -1) { // check if file exist and not need to update
	       continue; 
	   }
	   printf("%s\n",upd_file_path);
	   printf("update...\n");
	   char command[255];
	   strcpy(command,"python3 ../PyFiles/prepare_file.py \"");
	   //strcat(command,);
	   strcat(command, path_to_file);
	   strcat(command,"\" \"");
	   strcat(command,upd_file_path);
	   strcat(command,"\"\n");
	   printf("%s",command);
	   system(command);
	   //delete_timestamps( path_to_file,entry->d_name,file->d_name,size);
	   // remove(path_to_file);                                                    //TODO
	}
	

    }
    printf("end\n");
    closedir(folder);

    clock_t end = clock();
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    printf("The elapsed time is %f seconds\n", time_spent);
    
}
int find_shift(char * str){
    
    return 1;
}
int is_phrase(char* path, char * phrase,int size){
    FILE *fp;
    char *str = (char *)malloc(size);
    unsigned int phrase_length = strlen(phrase);
    fp = fopen(path, "r");
    struct stat st = {0};
   
    if (fp == NULL){
        printf("Could not open file %s",path);  
    }
    size_t result = fread (str,1,size,fp);
    if (result != size){fputs ("Reading error",stderr); exit (3);}
    
    int is_end = 0;
    int cnt = 0;
    while(*str != '\0'){
      //printf("%c",*str);
      
      str++;
      cnt++;
      if(levenshtein(str,phrase_length,phrase,phrase_length) < 100){
	return 1;
      }
      if(cnt >= size - phrase_length){
	break;
      }
      
    }
    //printf("%d\n",i);
    fclose(fp);
    
  return 0;
}
void find(){

  //   update_subtitles();
  //setlocale(LC_ALL, "Rus");

  //making clock to see execution time
    double time_spent = 0.0;  
    clock_t begin = clock();
    // Path to directory with subtitles
    char folder_name[255];
    strcpy( folder_name, "../Files");
    
    DIR *folder;
    struct dirent * entry;
    int files = 0;
    
    //char folder_name[] = "./Files";

    // open directory with subtitles 
    folder = opendir(folder_name);
    if(folder == NULL)
    {
        perror("Unable to read directory");
        return;
    }
    
    while( (entry=readdir(folder)) ) // iterating in directory
    {
      
      if(!strcmp(entry->d_name,".") || !strcmp(entry->d_name,"..")){
     	  continue;
        }
      //printf("%s\n", entry->d_name);
        char path_to_subfolder[255];
	strcpy( path_to_subfolder, "../new_Files/"); // making path to str file
        files++;
	strcat( path_to_subfolder, entry->d_name );
	strcat(path_to_subfolder,"/");
	DIR * subfolder;
	struct dirent *file;
	subfolder = opendir(path_to_subfolder);
	
	//	printf("FILENAME:%s\n",path_to_subfolder);
	while((file=readdir(subfolder))){
	  if(!strcmp(file->d_name,".") || !strcmp(file->d_name,"..")){
     	     continue;
          }
	   char path_to_file[255];
           strcpy( path_to_file, path_to_subfolder);
	   
	   struct stat st;
	   int size;
	   strcat( path_to_file, file->d_name );
	   if (stat (path_to_file, &st) == 0) size = st.st_size; // SIZE OF FILE
	   printf("%s\n", path_to_file);
	   
	}

    }
    printf("end\n");
    closedir(folder);

    clock_t end = clock();
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    printf("The elapsed time is %f seconds\n", time_spent);
    
}
int compare_phrases(char * phr_1, char * phr_2){
  
  return 1;
}
int main()
{
  
    char * query = "и ты брут сын мой";
    char * query_par = (char *)malloc(17);
    convertUtf8ToCp1251(query, query_par);      
    char * query_2 = "и ты брут сын";
    char * query_par_2 = (char *)malloc(14);
    convertUtf8ToCp1251(query_2, query_par_2);
    compare_phrases(query_par, query_par_2);
    double time_spent = 0.0;  
    clock_t begin = clock();    
    printf("DISTANCE:%d\n",levenshtein(query_par, 17, query_par_2,14));  
    //compare_phrases(query_par,query_par_2);
    // update_subtitles();
    clock_t end = clock();
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    printf("The elapsed time is %f seconds\n", time_spent);
    return 0;    
}
 
       
