/** FILENAME:    AMStartup.c
 *
 * NAME:        Celina Tala, Jeff Liu, Willem Klein Wassink
 *
 *
 * DESCRIPTION : This is the AMStartup.c file where the program connects with the server 
 *                and starts threads
 *              
 *
 * PUBLIC FUNCTIONS:
 *      
 *
 */

#include "../amazing.h"
#include "../avatar/messages.h"
#include "../avatar/avatar.h"
#include "../output/logfile.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>	  
#include <netdb.h>	
#include <string.h>
#include <time.h>
#include <getopt.h>
#include <pthread.h>

int main(int argc, char *argv[])
{
    //Variable declarations
    char *nAvatars_input;
    char *Difficulty_input;
    char *Hostname;
    char *program;
    int c;
    char *usageMessage = "./AMStartup -n nAvatar -d Difficulty -h Hostname\n";
    int nAvatars;
    int Difficulty;
    opterr = 0; //we set this as zero so getopt doesn't print an error message
    int err = 0;

    //setting the options arguments
    while ((c = getopt(argc, argv, "n:d:h:")) != -1) {
        switch (c) {
            case 'n':
                if (optarg[0] == '-' && err == 0 ) {
                    optopt = 'n';
                    err = 1;
                } else if (err == 0) {
                    nAvatars_input = optarg;
                    break;
                }
            case 'd':
                if (optarg[0] == '-' && err == 0) {
                    optopt = 'n';
                    err = 1;
                } else if (err == 0){
                    Difficulty_input = optarg;
                    break;
                }
            case 'h':
                if (optarg[0] == '-' && err == 0) {
                    optopt = 'n';
                    err = 1;
                } else if (err == 0){
                    Hostname = optarg;
                    break;
                }
            default:
                if (optopt == 'd' || optopt == 'n' || optopt == 'h') {
                    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                } else {
                    fprintf(stderr, "Unknown option\n");
                }
                exit(1);
        }
    }

    //if we dont have the correct number of options
    if (optind != 7) {
        fprintf(stderr, "%s", usageMessage);
        exit(2);
    }
    //making sure the options are integers
    if ((sscanf(nAvatars_input, "%d", &nAvatars) != 1 || sscanf(Difficulty_input, "%d", &Difficulty) != 1)){
        fprintf(stderr, "Please input integer for parameters\n");
        exit(3);
    }
    program = argv[0];

    //Opening our socket
    int comm_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (comm_sock < 0) {
        perror("opening socket");
        exit(4);
    }
    struct sockaddr_in server;  // address of the server
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(AM_SERVER_PORT));

    struct hostent *hostp = gethostbyname(Hostname);
    if (hostp == NULL) {
        fprintf(stderr, "%s: unknown host '%s'\n", program, Hostname);
        exit(5);
    }  
    memcpy(&server.sin_addr, hostp->h_addr_list[0], hostp->h_length);

    // 3. Connect the socket to that server   
    if (connect(comm_sock, (struct sockaddr *) &server, sizeof(server)) < 0) {
        perror("connecting stream socket");
        exit(6);
    }
    printf("Connected!\n");
    //The AM_INIT message the client sends to the server after parameter validation
    AM_Message init_message;
    memset(&init_message, 0, sizeof(AM_Message));
    init_message.type = htonl(AM_INIT);
    init_message.init.nAvatars = htonl(nAvatars);
    init_message.init.Difficulty = htonl(Difficulty);
    printf("Try to send AM_INIT message to the server now \n");
    if (write(comm_sock, &init_message, sizeof(AM_Message)) < 0) {
        fprintf(stderr, "error\n");
        exit(7);
    }
    printf("Sent\n");

    //The message we receive from the server
    AM_Message server_message;  
    int receive = read(comm_sock, &server_message, sizeof(AM_Message));

    //If it is less than or equal to 0, then the connection failed
    if (receive <= 0) {
        fprintf(stderr, "Connection to Server Closed\n");
        exit(8);
    } 
    //if it returns an error message
    if (IS_AM_ERROR(ntohl(server_message.type))) {
        errorMessage(NULL, server_message);
        exit(9);
    } 

    int width = ntohl(server_message.init_ok.MazeWidth);
    int height = ntohl(server_message.init_ok.MazeHeight);
    int mazeport = ntohl(server_message.init_ok.MazePort);
    close(comm_sock);
    //starting to write our logfile
    char *username = getenv("USER");
    if (username == NULL) {
        fprintf(stderr, "Failed to get username\n");
        exit(10);
    }

    char *logfile = malloc(strlen("../logOutput/Amazing___.log") + strlen(username) + 2 * sizeof(int) + 1);
    if (logfile == NULL) {
        fprintf(stderr, "Failed to allocate memory for file\n");
        exit(10);
    }
    sprintf(logfile, "../logOutput/Amazing_%s_%d_%d.log", username, Difficulty, nAvatars);

    FILE *fp;
    fp = fopen(logfile, "w");
    if (fp == NULL) {
        fprintf(stderr, "error opening file\n");
        exit(11);
    }
    time_t curtime;
    time(&curtime);
    fprintf(fp, "%s %d %s", username, mazeport, ctime(&curtime));
    fprintf(fp, "*****************************************\n");
    fclose(fp);

    pthread_t threads[nAvatars];
    int rc;
    for (int i = 0; i < nAvatars; i++) {
        avatar_p *parameter = clientParameters(i, nAvatars, Difficulty, Hostname, mazeport, logfile);
        rc = pthread_create(&threads[i], NULL, avatar, (void *)parameter);
        if (rc) {
            printf("Error:unable to create thread, %d\n", rc);
            exit(12);
        }
    }
    for (int i = 0; i < nAvatars; i++) {
        pthread_join(threads[i], NULL);
    }
    // exitGame(logfile, *finalMessage);
    free(logfile);
    return 0;
}
