# include <stdio.h>
# include <stdlib.h>
int copy_file(char [],char []);
int copy_file(char FileSou [],char FileDest []){
	FILE *sou = fopen(FileSou,"r");
	FILE *dest = fopen(FileDest,"w");
	char c;
	if (sou == NULL ){
		fclose(dest);
		printf("Failed to open file\n");
        return 1;
	}
	if (dest == NULL ){ 
		fclose(sou);
		printf("Failed to open file\n");
        return 1;
	}

	while ((c = fgetc(sou)) != EOF) {
		fputc(c,dest);
	}
	
	fclose(sou);
	fclose(dest);
	return 0;
}
int main (){
	int flag ;
	char sou [] = "./test.txt";
	char dest [] = "./testT.txt";
	
	flag = copy_file(sou,dest);
	return 0;
}
