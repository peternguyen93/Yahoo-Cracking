/*
 * 	Yahoo Cracking
 * 	Require : libcurl
 * 	Peter Nguyen
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<getopt.h>

const char __version__[] = "0.1 alpha";
const char __author__[] = "Peter Nguyen";

void header();
void version();
void help();

int main(int argc, char **argv){
	int opt;
	char *account_file=NULL;
	char *password_file=NULL;
	char *outfile=NULL;
	
	while(1){
		static struct option long_options[] ={
			{"crack",required_argument,0,'c'},
			{"dump",required_argument,0,'d'},
			{"output",required_argument,0,'o'},
			{"help",no_argument,0,'h'},
			{"about",no_argument,0,'a'},
			{0, 0, 0, 0}
		};
		int option_index = 0;
		if((opt = getopt_long(argc,argv,"c:d:o:h:a",long_options,&option_index)) == -1){
			break;
		}
		switch(opt){
			case 'c':
				account_file = optarg;
				break;
			case 'd':
				password_file = optarg;
				break;
			case 'o':
				outfile = optarg;
				break;
			case 'h':
				help();
				break;
			case 'a':
				version();
				break;
			default:
				fprintf(stderr,"[?] Usage : %s -h to help\n",argv[0]);
				break;
		}
	}
	if(account_file && password_file){
		header();
		cracking(password_file,account_file,outfile);
	}else{
		fprintf(stderr,"[!] Error you must have file consist of password and file consist of account\n");
		fprintf(stderr,"[?] Usage : %s -c <account_file> -d <password_file>\n",argv[0]);
	}
	
	return 0;
}

void header(){
	int i;
	for(i = 0; i < 80; i++){
		fputs("-",stdout);
	}
	fputs("\n",stdout);
	version();
	for(i = 0; i < 80; i++){
		fputs("-",stdout);
	}
	fputs("\n\n\r",stdout);
}

void version(){
	fprintf(stdout,"\tCracking Yahoo - Version : %s - Author : %s\n",__version__,__author__);
}

void help(){
	fputs("\t-c : cracking account lists. -c <account file>\n",stdout);
	fputs("\t-d : dump password file. -d <password list>\n",stdout);
	fputs("\t-o : file to save result.\n",stdout);
	fputs("\t-h : help\n",stdout);
	fputs("\t-a : about this tool\n",stdout);
}