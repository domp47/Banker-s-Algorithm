#include <stdio.h>
#include <string.h>
#include <stdbool.h>
/*
 * Banker's Algorithm using given state files
 * Domenic Perriccioli
 * v1.0 - 03/24/2017
 * C99 Standard
 */


void bankers(char*); //method to run the Banker's Algorithm

int main(int argc,char* argv[]){

    char* file;

    if(argc > 1){
        file = argv[1];
    }else{
        return 0;
    }

    bankers(file);

    return 0;
}
void bankers(char* f){
    int nProc;  //number of processes
    int nRes;   //number of resources

    FILE *State;
    State = fopen(f,"r");// opens chosen file
    char buf[100];      //char array for file contents

    if(!State){//if no file end
        printf("Error - File not found.\n");
    }

    fgets(buf,100,State);   //get first line
    sscanf(buf,"%d",&nProc);   //set number of processes

    fgets(buf,100,State);   //get second line
    sscanf(buf,"%d",&nRes);    //set number of resources

    int totRes[nRes];       //array for total resources

    fgets(buf,100,State);   //get third line

    char *token;        //char token to get delimited numbers
    token = strtok(buf,",");    //break up the line delimited by ,

    for(int i = 0;i < nRes; i++){
        sscanf(token,"%d",&totRes[i]);    //convert char to int

        token = strtok(NULL,",");   //get next delimited number
    }

    int availRes[nRes]; //array for available resources

    fgets(buf,100,State);   //get fourth line

    token = strtok(buf,",");    //break up the line delimited by ,

    for(int i = 0;i < nRes; i++){   //load available  resource array
        sscanf(token,"%d",&availRes[i]);    //convert char to int

        token = strtok(NULL,",");   //get next delimited number
    }

    int cAllocation[nProc][nRes];

    fgets(buf,100,State);   //get fifth line

    token = strtok(buf,",");    //break up the line delimited by ,

    for(int y = 0; y < nProc; y++){ //load current allocation array
        for(int x = 0; x < nRes; x++){
            sscanf(token,"%d",&cAllocation[y][x]);    //convert char to int

            token = strtok(NULL,",");    //break up the line delimited by ,
        }
        fgets(buf,100,State);   //get next line
        token = strtok(buf,",");    //break up the line delimited by ,
    }

    int requests[nProc][nRes];

    for(int y = 0; y < nProc; y++){ //load requests array
        for(int x = 0; x < nRes; x++){
            sscanf(token,"%d",&requests[y][x]);    //convert char to int

            token = strtok(NULL,",");    //break up the line delimited by ,
        }
        fgets(buf,100,State);   //get next line
        token = strtok(buf,",");    //break up the line delimited by ,
    }

    fclose(State);  //close state file

    bool done[nProc];   //boolean array to detect if processes are complete

    for(int i = 0;i < nProc; i++){  //set them all to false
        done[i] = false;
    }

    int compProcesses = 0;   //counts number of completed processes

    for(int p = 0; p < nProc; p++){ //
        for(int y = 0;y < nProc; y++){
            if(!done[y]){   //if this process hasn't been run check if it can
                bool enough = true; //detect if enough resources for this process

                for(int x = 0;x < nRes; x++) {//checks all the resources in process
                    if (requests[y][x] > availRes[x]){//if not enough resources indicate it
                        enough = false;
                        break;
                    }
                }

                if(enough){//if there was enough resources run that process
                    compProcesses++;    //increment completed processes

                    for(int x = 0; x < nRes; x++){//add the resources from the process to available
                        availRes[x]+=cAllocation[y][x];
                        requests[y][x] = -1;
                    }

                    done[y] = true; //set this process as complete

                    printf("Executing process %d\n",y);

                    break;
                }
            }
        }
    }

    if(compProcesses==nProc){//if equal then we completed all processes
        printf("Safe state.\n");
    }
    else{
        printf("Unsafe state.\n");
    }
}