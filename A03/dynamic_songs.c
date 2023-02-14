/*----------------------------------------------
 * Author: Selin Butun
 * Date: February 8, 2023
 * Description: Program that reads songs from a text file and prints
 * out the appended version.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct song{
    char title[32];
    char artist[32];
    int mins;
    int secs;
    float dance;
    float energy;
    float tempo;
    float valence;
};


int main(){
    FILE *infile;
    const char* delim = ",";

    // Open file
    infile = fopen("songlist.csv", "r");
    if (infile == NULL){
        printf("Cannot open file \n");
        exit(0);
    }

    // Read contents from file
    char attribute[1024];
    char *temp = NULL;
    fgets(attribute, 1024, infile);
    int size = atoi(strtok(attribute, delim));
    struct song *songsList = malloc(size * sizeof(struct song));
    fgets(attribute, 1024, infile);
    int index = 0;
    
    while (fgets(attribute, 1024, infile) != NULL){
        temp = strtok(attribute, delim); 
        strcpy(songsList[index].title, temp);//title
        temp = strtok(NULL,",");
        strcpy(songsList[index].artist, temp);//artist
        temp = strtok(NULL,",");
        float tempNum = atof(temp); //number for mins and secs
        int minute = tempNum / 60000;
        songsList[index].mins = minute;
        int seconds = (tempNum - (minute * 60000))/1000;
        songsList[index].secs = seconds;
        temp = strtok(NULL,",");
        tempNum = atof(temp); //number for danceability
        songsList[index].dance = tempNum;
        temp = strtok(NULL,",");
        tempNum = atof(temp); //number for energy
        songsList[index].energy = tempNum;
        temp = strtok(NULL,",");
        tempNum = atof(temp); //tempo
        songsList[index].tempo = tempNum;
        temp = strtok(NULL,",");
        tempNum = atof(temp); //valence
        songsList[index].valence = tempNum;

        index++;
    }
    for(int i = 0; i < size; i++){
        printf("%d) %-27s artist: %-16s duration: %d:%-10d"
		"D: %-10.3f E: %-10.3f T: %-10.3f V: %-10.3f\n",
	       	i, songsList[i].title, songsList[i].artist, songsList[i].mins, 
		songsList[i].secs, songsList[i].dance, songsList[i].energy,
	       	songsList[i].tempo, songsList[i].valence);
    }
    
    free(songsList);
    songsList = NULL;
    fclose(infile);
    return 0;
}

