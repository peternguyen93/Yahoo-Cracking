/*
 *	Function.h
 *	Author : Peter Nguyen
 **/ 

int send_request(char *username, char *password);
char* check_response();
void cracking(char *db_pass,char *db_account,char *db_out);