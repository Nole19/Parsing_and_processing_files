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

#define MAXCHAR 1000

char* substr(const char *src, int m, int n)
{
    // get the length of the destination string
    int len = n - m;
 
    // allocate (len + 1) chars for destination (+1 for extra null character)
    char *dest = (char*)malloc(sizeof(char) * (len + 1));
 
    // extracts characters between m'th and n'th index from source string
    // and copy them into the destination string
    for (int i = m; i < n && (*(src + i) != '\0'); i++)
    {
        *dest = *(src + i);
        dest++;
    }
 
    // null-terminate the destination string
    *dest = '\0';
 
    // return the destination string
    return dest - len;
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
    unsigned int x, y, s1len, s2len;
    s1len = strlen(s1);
    s2len = strlen(s2);
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

int find_white(char * text, int first_index,int length_phrase){
	int shift = 0;
	while(text[shift] != ' ' && text[shift] != '\n'){
		shift++;
	}
	return shift;
}

void search_sub_2(char * filename,int size,char * phrase){
    FILE *fp;
    char str[size];

    unsigned int phrase_len;
    phrase_len = strlen(phrase);
    // char new_file_name="result.txt";
    FILE *fptr;
   // use appropriate location if you are using MacOS or Linux
    fptr = fopen("result.txt","a");
   
    //char* filename = "c:\\temp\\test.txt";
    //printf("%s\n",filename);
    fp = fopen(filename, "r");
    if (fp == NULL){
        printf("Could not open file %s",filename);  
    }
    size_t result = fread (str,1,size,fp);
    if (result != size) {fputs ("Reading error",stderr); exit (3);}
    //printf("NEWLINE:");
    // printf("%s", str);
    //sleep(2000);
    int index = 0;
    
    fclose(fp);
    fclose(fptr);

}
int
main()
{

  // int dist;
    //dist=levenshtein("первое правило бойцовского клуба","Первое правило бойцовского клуба");
    // printf("distance : %d\n",dist);
    //    return 0;
  // setlocale(LC_ALL, "Rus");
    double time_spent = 0.0;
    clock_t begin = clock();
    char folder_name[255];
    strcpy( folder_name, "./Files");
    DIR *folder;
    struct dirent *entry;
    int files = 0;
    //char folder_name[] = "./Files";
    
    folder = opendir(folder_name);
    if(folder == NULL)
    {
        perror("Unable to read directory");
        return(1);
    }

    while( (entry=readdir(folder)) )
    {

        char folder_path[255];
	strcpy( folder_path, "./Files/");
        files++;
	//printf("%d\n",files);
	if(files < 3) continue;
	// strcat( fn, argv[1);

	strcat( folder_path, entry->d_name );
	//strcat(folder_name,entry->d_name);
	 struct stat st;
	int size;
        if (stat (folder_path, &st) == 0) size = st.st_size;
   
	search_sub_2(folder_path,size,"В Голливуд? Что ты там забыл?");

    }
    printf("end");
    closedir(folder);

    clock_t end = clock();
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    printf("The elapsed time is %f seconds", time_spent);
    
    return 0;
}
 
       
