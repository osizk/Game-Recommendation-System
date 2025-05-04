#ifndef LOGIN_H
#define LOGIN_H

int login(char *username);
int user_login(char *username,char *input_user,char *input_pass);
int admin_login(char *input_user,char *input_pass);
int user_register();
#endif