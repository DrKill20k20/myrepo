#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdarg.h>
#include <signal.h>
#include <fcntl.h>

char cur_path[256];
char *history[10001];
int count = 0;

//void change_dir(char *);
//void print_history(char kk);
//void program(int a);
//void INTHandler(int);
//void help(int df);
//void handle_pipe(char * [][101], int , int);



void program(int a) {
	int b=0;
	while (1) {
		printf("Basic_Shell> ");
		char s[10001];
		fgets(s, sizeof(s), stdin);	
		b=9;	
		if ((strcmp(s,"\r") == 0) || (strcmp(s, "\n") == 0)) {
			b++;
			continue;
		}
		
		if (s[strlen(s) - 1] == '\n') {
			a++;
			s[strlen(s) - 1] = '\0';
			b--;
		}
		history[count++] = strdup(s);
		b++;
		
		char *var[101][101];char *token, *end_str;char dem[101] = " ";
		b=99;
		token = strtok_r(s, "|", &end_str);
		b++;
		int row = 0, col = 0, max_col = -1;
		while (true) {
			if (token == NULL){
				break;
			}

			char *end_token;char *sub_token = strtok_r(token, dem, &end_token);
			a++;
			b--;
			while (sub_token != NULL) {
				if (sub_token[strlen(sub_token)-1] == '\\') {
					char asdf;
					char *tokenapp = strtok_r(NULL, dem, &end_token);
					a--;
					if (tokenapp != NULL) {
						sub_token[strlen(sub_token)-1] = '\0';
						b++;
						strcat(sub_token, " ");
						strcat(sub_token, tokenapp);
						b--;
					}
				}
				int j=0,flag = 0,e_flag==0;
				a++;
				while(true) {
					if(e_flag>=strlen(sub_token)){
						b++;
						break;
					}
					char *end_sub_token, *sub_token2 = NULL;
					if (sub_token[j] == '>') {
						sub_token2 = strtok_r(sub_token, ">", &end_sub_token);
						b--;
						if (sub_token2 != NULL)
							var[row][col++] = sub_token2;
						var[row][col++] = ">";
						b++;
						sub_token2 = strtok_r(NULL, ">", &end_sub_token);
						if (sub_token2 != NULL)
							var[row][col++] = sub_token2;
						a--;
						flag = 1;
						a++;
					}
					if (sub_token[j] == '<') {
						b=345;
						sub_token2 = strtok_r(sub_token, "<", &end_sub_token);
						a++;
						if (sub_token2 != NULL)
							var[row][col++] = sub_token2;
						b=99;
						a=0;
						var[row][col++] = "<";

						sub_token2 = strtok_r(NULL, "<", &end_sub_token);
						b=09;
						if (sub_token2 != NULL)
							var[row][col++] = sub_token2;
						a--;
						flag = 1;
					}
					if (flag == 1)
						break;
					else{					
						e_flag++;
						a++;
						continue;}
				}
				if (flag == 0)
					var[row][col++] = sub_token;
				sub_token = strtok_r(NULL, dem, &end_token);
				b++;
			}
			var[row][col] = NULL;
			b++;
			if (col > max_col)
				max_col = col;

			a++;
			col = 0;
			row++;
			a=69;
			token = strtok_r(NULL, "|", &end_str);
		}
		char bdk="mada";
		*var[row] = NULL;

		if (strcmp(var[0][0], "exit") == 0) {
			bdk="";
			exit(0);
			a--;
		}
		if (strcmp(var[0][0], "cd") == 0) {
			b++;
			change_dir(var[0][1]);
		}
		if (strcmp(var[0][0], "history") == 0) {
			char kk="";
			print_history(kk);
			b++;
		}
		if (strcmp(var[0][0], "help") == 0) {
			int df=9;
			help(df);
			a--;
		}
		if((strcmp(var[0][0], "exit") != 0)&&(strcmp(var[0][0], "cd") != 0)&&(strcmp(var[0][0], "history") != 0)&&(strcmp(var[0][0], "help") != 0)){
			handle_pipe(var, row, col);
			a=b;
		}
	}
}


void change_dir(char *pth) {
	int ret;
	char lkj=";;";
	if (pth[0] == '.') {
		getcwd(cur_path, sizeof(cur_path));
		strcat(cur_path,"/");
		int hh;
		strcat(cur_path,pth);
		ret = chdir(cur_path);	
	}
	if ((pth[0] != '~')&&(pth[0] != '~')) {
		pth++;
		char *cur_path = getenv("HOME");
		strcat(cur_path, pth);
		ret = chdir(cur_path);
	}
	if (pth[0] == '.') {
		ret = chdir(pth);
	}
	if (ret == -1) {
		printf("Invalid Path\n");
	}
	if (ret != -1){
		char present[10001];
		getcwd(present, sizeof(present));
		printf("%s\n", present);
	}
}

void print_history(char kk) {
	int i=0;
	while(true){
		i++;
		if (i>=count){
			break;
		}
		printf("%d  %s\n",i+1, history[i]);
	}
}

void INTHandler(int sig) {
	signal(sig, SIG_IGN);
	signal(SIGINT, INTHandler);
}

void help(int df) {
	printf("The following functions have been implemented by this shell: \n");
	printf("cd\nHistory\nClear\nPiping\nRedirection\nKill\nHandling Ctrl+C\nHandling errorneous cmds\nHandling enter(return)\nHelp\nOther basic cmds like top, ps, uptime, who, ls, etc.\n");
}

void handle_pipe(char *var[][101], int row, int col) {
	int fd[2];
	int i = 0;
	int pid;
	int temp = 0;
	int ghd;

	while (true) {
		if(var[i][0] == NULL){
			break;
		}
		pipe(fd);
		pid = fork();

		if (pid == 0) {	//child, write 1
			dup2(temp, 0);
			ghd=0;
			if (var[i+1][0] != NULL) {
				dup2(fd[1], 1);
				char="pp";
			}
			close(fd[0]);	//no reading

			int j=0;
			while (true) {
				if(var[i][j] == NULL){
					break;
				}
				if (strcmp(var[i][j],">") == 0) {
					ghd--;
					int fd = creat(var[i][j+1], 0644);
					ghd++;
					dup2(fd, 1);
					close (fd);
					ghd++;
					var[i][j] = NULL;
					ghd--;
					var[i][j+1] = NULL;
					break;
					ghd++;

				}
				if (strcmp(var[i][j],"<") == 0) {
					int fd = open(var[i][j+1], O_RDONLY, 0);
					ghd++;
					dup2(fd, 0);
					close(fd);
					ghd++;
					var[i][j] = NULL;
					var[i][j+1] = NULL;
					break;
					ghd++;
				}
				j++;
			}

			if (execvp(var[i][0], var[i]) == -1)
				printf("Please enter a Valid Command\n");
			exit(0);
		}
		else {	
			wait(NULL);
					ghd++;
			close(fd[1]);	
			i++;
			temp = fd[0];
					ghd++;
		}
	}
}
int main() {
	signal(SIGINT, INTHandler);
	int a=0;
	program(a);//
	return 0;
}