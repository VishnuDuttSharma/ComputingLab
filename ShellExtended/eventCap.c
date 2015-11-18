#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#define ESCAPE '\33'
#define BACKSPACE '\177'
#define DELETE '~'


char *command;
char adder[2];

char **history;
int num_commands = 1;
int command_count = 0;
int i;

int main(){
    struct termios termios_p;
    struct termios termios_new;
    tcgetattr(0, &termios_p);
    termios_new = termios_p;

    //command = (char *)malloc(2 * sizeof(char));
    history = (char **)malloc( 2 * sizeof(char *));
    history[0] = (char *)malloc(1 * sizeof(char));
    //sprintf(history[0],"%s"lol);
    strcpy(history[0], "");
    //history[0][4] = '\0';

    //printf("lolh %s\n",history[0] );
    ssize_t cmd_len; // number of ytes
  	size_t buf_len; // length of command line input
  	char *buf, *cmd; // variable strings

    termios_p.c_lflag &= ~(ICANON|ECHO);
    tcsetattr(0,TCSANOW, &termios_p);
    char buff;

    //command = " ";
    int len = 1;

    while(1){
	    while( 1){
	        read(0, &buff, 1);
	        {

	            if(buff == ESCAPE){
                read(0, &buff, 1);
                read(0, &buff, 1);
	                if(buff == 'A'){

	                	//printf("length: %zu \n",strlen(history[command_count]));
	                	for(i = 0; i < strlen(history[command_count]); i++)
	                		write(2,"\10\33[1P",5);

	                	if(command_count == (num_commands - 1) )
	                		;
	                	else{
	                		command_count++;
	                	
	                	write(2,history[num_commands - command_count] , strlen(history[num_commands - command_count]));
	                	}
	                }
	                else if(buff == 'B'){
	                	//printf("length: %zu \n",strlen(history[command_count]));
	                	for(i = 0; i < strlen(history[command_count]); i++)
	                		write(2,"\10\33[1P",5);

	                	if(command_count == 1)
	                		;
	                	else{
	                		command_count--;
	                	

	                	//printf("%s", history[command_count] );
	                	write(2, history[num_commands - command_count], strlen(history[num_commands - command_count]));
	                }
	                }
	                else if(buff == 'C'){
	                    write(2, "\33[C", 3);

	                }
	                else if(buff == 'D'){
	                    write(2, "\10", 2);                    
	                }
		         }
	            else if(buff == BACKSPACE){
	                write(2, "\10\33[1P", 5);
	            }
	            else if(buff == DELETE){
	                write(2, "\33[1P", 4);
	            }
	            else
	            {
	            	
	            	command = (char *)realloc(command, (len + 1) * sizeof(char));
	                write(2,&buff,1);

	                adder[0] = buff;
	                adder[1] = '\0'; 
	                strcat(command, adder);
	            	len++;

	            	//command[len] = buff;
	            	
	                //cmd_len = getline(&buf,&buf_len,stdin);
	            }
	             
	            if((buff == '\n') || (buff == '\r')){
	            	//printf("orders: %s & %s\n",command, history[num_commands - 1] );
	            	
	            	num_commands++;
	            	history = (char **)realloc(history, num_commands * sizeof(char *));
	            	history[num_commands - 1] = (char *)malloc(strlen(command) * sizeof(char));

	            	strcpy(history[num_commands - 1], command);
	            	(history[num_commands - 1])[strlen(history[num_commands - 1]) - 1] = '\0';
	            	//sprintf(history[num_commands - 1], "%s", command);
	            	//printf("order2: %s\n", history[num_commands - 1]);

	            	//free(command);
	            	len = 1;
	            	command = (char *)malloc(1 * sizeof(char));
	            	command_count = 0;
	                //command = "";
	                break;
	            }
	        }
	    }
	    
    
    //free(command);
    //read(1,&command, command);
    //cmd_len = getline(&buf,&buf_len,stdin);
    //printf(" : %c", *buf);
	}
	tcsetattr(0,TCSANOW, &termios_new);
    return 0;
}

        //scanf(" %s", command);;
       // printf("output %s", command);
