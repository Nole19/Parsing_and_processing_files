#include <stdbool.h>
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
#define MAX_WORDS 25
#define MAX_LETTERS 25
#include <assert.h>
#include <wchar.h>
#include <wctype.h>
#include "Converter/utf8_to_cp1251.h"
#define ALPHABET_SIZE 33
#define ARRAY_SIZE(a) sizeof(a)/sizeof(a[0])
#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

int get_index(char symbol){
  if(symbol == ' '){
    return -2;
  }
  if((int)symbol == -72){
    return 32;
  }
  if((int)symbol >= -32 && (int)symbol <= -1){
    return (int)symbol + 32;
  }
  return -1;
}
struct TrieNode{
  unsigned int id_film;
  struct TrieNode * children[ALPHABET_SIZE];
  bool is_end;
};
struct TrieNode * getNode(){
  struct TrieNode *pNode = NULL;
  pNode = (struct TrieNode *)malloc(sizeof(struct TrieNode));
  if(pNode){
    int i;
    pNode->is_end = false;
    for(i=0;i<ALPHABET_SIZE;i++){
      pNode->children[i] = NULL;
   }
  }
  return pNode;
}

  void insert(struct TrieNode*root,const char *key){
  int level;
  int length = strlen(key);
  int index;
  struct TrieNode * pCrawl = root;
  for(level =0; level < length;level++){
    index = get_index(key[level]);
    if(index < 0 || index >= ALPHABET_SIZE) continue;
    if(!pCrawl->children[index]){
      pCrawl->children[index] = getNode();
    }
    
    pCrawl = pCrawl->children[index];
    
  }
  pCrawl->is_end = true;
}

bool search(struct TrieNode * root,const  char * key){
  int level;
  int length = strlen(key);
  int index;
  struct TrieNode * pCrawl = root;

  for(level = 0;level<length;level++){
    index = get_index(key[level]);
    if(index < 0 || index >= ALPHABET_SIZE) continue;
    if(!pCrawl->children[index]){
      return false;
    }
    pCrawl = pCrawl->children[index];
  }
  return pCrawl->is_end;
}

void free_all(struct TrieNode * curs){
  int i;
  if(!curs) return;

  for(i=0;i<ALPHABET_SIZE;i++){
    free_all(curs->children[i]);
  }
  free(curs);
}

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

int build_tries(char*path,int size,struct TrieNode ** trie_array,int * trie_index,int id_film){

  FILE *fp;
  //char str[size];
  
  //FILE * fw;
 
  //fw = fopen("./check.txt","a");
  
  fp = fopen(path, "r");
  char *str = (char *)malloc(size);
  size_t result = fread (str,1,size,fp);
  if (result != size){fputs ("Reading error",stderr); exit (3);}  
  char word[180];
  int index = 0;
  int cnt_b = 0;
  int line_counter=0;
  struct TrieNode *root = getNode();
  root->id_film = id_film;
  while(cnt_b != size){
    if(*str == '\r'){
      word[index] = '\0';
      index = 0;
      insert(root, word);
      str++;
      cnt_b++;
      line_counter++;
      if(line_counter == 10){
	  trie_array[*trie_index] = root;
	  printf("%d\n",*trie_index);
	  (*trie_index)++;
	  root = getNode();
	  root->id_film = id_film;
	  line_counter = 0;
      } 
     }
    else if(*str == ' '){
      word[index] = '\0';
      index = 0;
      //printf("%s\n",word);
      insert(root, word);
    }
    else{
      word[index] = *str;
      index++;
    }
    cnt_b++;
    str++;
  }
  trie_array[*trie_index] = root;
  printf("%d\n",*trie_index);
  (*trie_index)++;
  
  //free_all(root);
  // printf("%d\n",cnt_b);
  // size_t result = fread (str,1,size,fp);
  //if (result != size){fputs ("Reading error",stderr); exit (3);}
  //char * line = strtok_r( str, " ",&saveptr);
  //  fprintf(fw,"[%s]\n",phrase);
  int cnt = 0;
  /* while( line != NULL )
    {
      //char * tmp_phrase = (char *)malloc(strlen(phrase));
      //strcpy(tmp_phrase,phrase);
      //float num = compare_phrases(line,tmp_phrase);
      cnt++;
      //fprintf(fw,"%s %f \n",phrase,num);
      line = strtok_r( NULL, " ",&saveptr);
      //printf("%s\n",line);
      //free(tmp_phrase);
    }
  */
  fclose(fp);
  //fclose(fw);
  return 0;

}
void iterate_files(struct TrieNode ** trie_array,int* trie_index){
    printf("Iterating...\n");
    
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
	   
	   build_tries(path_to_file,size,trie_array,trie_index,atoi(entry->d_name));
	}
	closedir(subfolder);
	//free(path_to_subfolder);
    }
    
    closedir(folder);

   
    
}

void display(FILE *fptr,struct TrieNode* root, char str[], int level)
{
    // If node is leaf node, it indicates end
    // of string, so a null character is added
    // and string is displayed
    if (root->is_end) 
    {
        str[level] = '\0';
        fprintf(fptr,"%s\n",str);
    }
  
    int i;
    for (i = 0; i < 33; i++) 
    {
        // if NON NULL child is found
        // add parent key to str and
        // call the display function recursively
        // for child node
        if (root->children[i]) 
        {
            str[level] = i - 32;
            display(fptr,root->children[i], str, level + 1);
        }
    }
}
char * enter_line(void) {
    char * line = malloc(100), * linep = line;
    size_t lenmax = 100, len = lenmax;
    int c;

    if(line == NULL)
        return NULL;

    for(;;) {
        c = fgetc(stdin);
        if(c == EOF)
            break;

        if(--len == 0) {
            len = lenmax;
            char * linen = realloc(linep, lenmax *= 2);

            if(linen == NULL) {
                free(linep);
                return NULL;
            }
            line = linen + (line - linep);
            linep = linen;
        }

        if((*line++ = c) == '\n')
            break;
    }
    *line = '\0';
    return linep;
}
int main()
{
    
    
    int in = 0;
    int * trie_index = &in;
    struct TrieNode * trie_array[1000000];
    update_subtitles();
    
    // char * query = "больше";
   
    iterate_files(trie_array,trie_index);
   
    while(true){
      char words[255][255];
      int words_count = 0;
      int word_index = 0;
      char* query  = enter_line();
      char * query_par = (char *)malloc(strlen(query));
      convertUtf8ToCp1251(query, query_par);
   
      while(*query_par != '\0'){
	if(*query_par == ' '){
	  words[words_count][word_index] = '\0';
	  word_index = 0;
	  words_count++;
	}
	else if(*query_par == '\n'){
	  words[words_count][word_index] = '\0';	  
	  words_count++;
	  break;
	}
	else{
	  words[words_count][word_index] = *query_par;
	  word_index++;
	}
	query_par++;
      }
      FILE * fw = fopen("ch.txt","w");
      for(int i = 0; i < words_count;i++){
	fprintf(fw,"%s\n",words[i]);
      }
      fclose(fw);
      double time_spent = 0.0;  
      clock_t begin = clock();
      for(int i=0;i< *trie_index;i++){
	int good_word_count = 0;
        for(int i =0;i<words_count;i++){
	  if(search(trie_array[i],words[i])){
	    printf("please");
	    good_word_count++;
	  }
	}
	  if(good_word_count != 0){
	    printf("%d\n",good_word_count);
	  }
      }
      clock_t end = clock();
      time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
      printf("The elapsed time is %f seconds\n", time_spent);
      printf("done\n");
  
    }
    return 0;    
}

