#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//struct
struct song{
    char title[32];
    char artist[32];
    int mins;
    int secs;
    float dance;
};

//editSong method edits an aspect of the default struct according to user input
void editSong(struct song* defSongs){
    printf("Which attribute do you wish to edit? [artist, title, duration, danceability]: ");
    char *attribute = malloc(sizeof(char) * 32);
    scanf("%s", attribute);
    if(strcmp(attribute, "artist") == 0){
        char *inputArtist = malloc(sizeof(char) * 32);
        printf("Enter artist name: ");
        scanf("%s", inputArtist);
        strcpy(defSongs->artist, inputArtist);
        free(inputArtist);
    }
    if(strcmp(attribute, "title") == 0){
        char *inputTitle = malloc(sizeof(char) * 32);
        printf("Enter artist name: ");
        scanf("%s", inputTitle);
        strcpy(defSongs->title, inputTitle);
        free(inputTitle);
    }
    if(strcmp(attribute, "duration") == 0){
        int inputMins;
        int inputSecs;
        printf("Enter duration in 'min:secs' format: ");
        scanf("%d:%d", &inputMins, &inputSecs);
        defSongs->mins = inputMins;
        defSongs->secs = inputSecs;
    }
    if(strcmp(attribute, "danceability") == 0){
        float inputDance;
        printf("Enter danceability between 0-1: ");
        scanf("%f", &inputDance);
        defSongs->dance = inputDance;
    }
    free(attribute);
}
//main method first prints out the default song list, then asks the user to edit and
//calls on editSong method
int main(){
    struct song defSongs[3];
    //titles
    strcpy(defSongs[0].title, "The News");
    strcpy(defSongs[1].title, "Kerosene!");
    strcpy(defSongs[2].title, "Come On Eileen");
    //artists
    strcpy(defSongs[0].artist, "Paramore");
    strcpy(defSongs[1].artist, "Yves Tumor");
    strcpy(defSongs[2].artist, "Dexys Midnight Runners");
    //duration
    defSongs[0].mins = 2;
    defSongs[0].secs = 39;
    defSongs[1].mins = 4;
    defSongs[1].secs = 55;
    defSongs[2].mins = 3;
    defSongs[2].secs = 27;
    //danceability
    defSongs[0].dance = 1;
    defSongs[1].dance = 0.3;
    defSongs[2].dance = 0.5;
    
    printf("Welcome to Selin's song list!\n");
    for(int i = 0; i < 3; i++){
        printf("%d) %-20s artist: %-20s duration: %d:%-16d danceability %.1f\n", i, defSongs[i].title, defSongs[i].artist, defSongs[i].mins, defSongs[i].secs, defSongs[i].dance);
    }

    printf("Which song would you like to edit? [0,1,2]: ");
    int pickedSong;
    scanf("%d", &pickedSong);
    if(pickedSong > 2){
	    printf("Must pick between 0,1,2!\n");
	    return 0;
    }
    editSong(&defSongs[pickedSong]);
    
    //prints again after editSong method is complete and struct is updated
    for(int i = 0; i < 3; i++){
        printf("%d) %-20s artist: %-20s duration: %d:%-16d danceability %.1f\n", i, defSongs[i].title, defSongs[i].artist, defSongs[i].mins, defSongs[i].secs, defSongs[i].dance);
    }

    return 0;
}
