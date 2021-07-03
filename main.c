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
#define MAX_WORDS 10
#define MAX_LETTERS 15
#include <assert.h>
#include <wchar.h>
#include <wctype.h>
#include "Converter/utf8_to_cp1251.h"

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

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

int levenshtein(char *s1, char *s2) {
    int s1len = strlen(s1);
    int s2len = strlen(s2);
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

float compare_phrases(char * phr_1, char * phr_2){
  float match_words = 0;
  char all_words_1[MAX_WORDS][MAX_LETTERS];
  char all_words_2[MAX_WORDS][MAX_LETTERS];
  char * token = strtok(phr_1, " ");
  int id_1=0;
  while( token != NULL ) {
     
    // printf( "%s\n", token); //printing each token
     strcpy(all_words_1[id_1],token);
     id_1++;
     token = strtok(NULL, " ");
  }
  char * token_2 = strtok(phr_2, " ");
  int id_2=0;
  while( token_2 != NULL ) {
    // printf( "%s\n", token_2); //printing each token
     strcpy(all_words_2[id_2],token_2);
     id_2++;
     token_2 = strtok(NULL, " ");
  }
  for(int i = 0;i < id_1;i++){
    for(int j = 0; j < id_2; j++){
      if(levenshtein(all_words_1[i], all_words_2[j]) <= 1){
	match_words++;
	break;
      }
    }
  }
  return match_words/max(id_1,id_2);
}

int find_phrase(char*path,int size,char * phrase){
  FILE *fp;
  ssize_t read;
  size_t len = 0;
  char * line = NULL;
  fp = fopen(path, "r");
  int cnt = 0;
  while (fscanf(fp, "%[^\n]", line) ) {
        float num = compare_phrases(line,phrase);
        printf("%s",line);
	printf("%f\n",num);
	cnt++;
   }
  
  printf("%d",cnt);
  fclose(fp);
  
  return 0;

}
void iterate_files(char * phrase){
    printf("Iterating...\n");
    double time_spent = 0.0;  
    clock_t begin = clock();
    char folder_name[255];
    strcpy( folder_name, "../new_Files");
    DIR *folder;
    struct dirent * entry;
    int files = 0;
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

        char path_to_subfolder[255];
	strcpy( path_to_subfolder, "../new_Files/"); // making path to str file
        files++;
	strcat( path_to_subfolder, entry->d_name );
	strcat(path_to_subfolder,"/");
	DIR * subfolder;
	struct dirent *file;
	subfolder = opendir(path_to_subfolder);
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
	   find_phrase(path_to_file,size,phrase);
	}
    }
    
    closedir(folder);

    clock_t end = clock();
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    printf("The elapsed time is %f seconds\n", time_spent);
    
}

int main()
{
  
    char * query = "и ты брут сын мой";
    char * query_par = (char *)malloc(17);
    convertUtf8ToCp1251(query, query_par);      
    char * query_2 = "и вы брут сыр мой";
    char * query_par_2 = (char *)malloc(14);
    convertUtf8ToCp1251(query_2, query_par_2);
    // printf("%f\n",(float)compare_phrases(query_par, query_par_2));
    double time_spent = 0.0;  
    clock_t begin = clock();    
    //printf("DISTANCE:%d\n",levenshtein(query_par, 17, query_par_2,14));  
    //compare_phrases(query_par,query_par_2);
    // update_subtitles();
    //iterate_files(query_par);
    find_phrase("../new_Files/1/A Bronx Tale-CD2.srt",15730,query_par);
    clock_t end = clock();
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    printf("The elapsed time is %f seconds\n", time_spent);
    return 0;    
}
 
       
