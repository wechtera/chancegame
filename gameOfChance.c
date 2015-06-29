


/*IMPORTANT REMEMBER TO UPDATE THE JACKPOT TO TAKE IN THE JACKPOT SIZE TO BE ADDED INTO OTHER FUNCTION TOO*/

#include <stdio.h>
#include <string.h>
#include <fnct1.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>
#include "errorScripts.h"  //error messages brah

#define DATAFILE "/var/chance.data"  //file to store user data of cuse

//custom user struct to store information aboute users
struct user{
	int uid;
	int credits;
	int highscore;
	char name[100];
	int (*current_game);
	
}

//function prototypes:
int get_player_data();
void register_new_player();
void update_player_data();
void show_highscore();
void jackpot(int jackpot_size);
void input_name();
void print_cards(char *, char *, int);
int take_wager(int, int);
void play_the_game();
int pick_a_number();
int dealer_no_match();
int find_the_ace();
void fatal(char *);

//global vars
struct user player;  //player struct
					//means you can use player.<struct var>

int main(){
	/*var dictionary:  
	* last_game:  A check to see if the pointer is where it should be
	* choice:  current choice in game for if statements
	*/
	int choice, last_game;

	srand(time(0));  //seed the randomizer with the current time!

	if(get_player_data()== -1)  //try to read player data from file.
		register_new_player();  //if there is no data, register a new player

	while(choice != 7){
		printf("-=[ Game of Chance Menu ]=-\n");
		printf("1 - Play the pick a number game\n");
		printf("2 - Play the no match dealer game\n");
		printf("3 - Play the find the ace game\n");
		printf("4 - View current high score\n");
		printf("5 - Change your user name\n");
		printf("6 - Reset your account(100 cc)\n");
		printf("7 - Quit now (scrub)\n");
		printf("[Name: %s]\n", player.name);
		printf("[You have %u credits] ->  ", player.credits());
		scanf("%d", &choice);

		if((choice<1) || (choice>7))
			printf("\n [!!] Dear damaged child, the number %d is an invalid selection. \n\n", choice);
		else if(choice<4){
			if(choice !=last_game)  //check to see if pointer is already set or not!
				if (choice ==1)
					player.current_game = pick_a_number;  //point to current game!
				else if(choice ==2)
					player.current_game = dealer_no_match;
				else  //if not one or two must be three
					player.current_game = find_the_ace;
				last_game=choice;  //sets last game to choice for next time check
			}
			play_the_game();  //plays current game set in user struct current_game
		}
		else if(choice==4)
			show_highscore();
		else if(choice==5) {
			printf("\nChange user name\n");
			printf("Enter your new name:  ");
			input_name();
			printf("Your name has been changed to: %s \n\n", player.name);  //confirmation
		}
		else if(choice==6) {
			printf("\nYour account has been reset with 100 credits.\n\n");
			player.credits = 100;
			if (player.name== "Adam Wechter")  //once again if its me playing
				player.credits= 99999999999;
		}
	}
	update_player_data();
	printf("\nThanks for playing!  Bye.\n");
}

/* 
 * This function reads the player data for the current uid
 * from the file.  It returns -1 if it is unable to find player
 * data for the current uid
 */

int get_player_data() {
	/* Var dictionary:
	 *  fd:  File descriptor variable, a unique unsigned int that is the id of the file writing to
 	 *  uid: Unique user ID that tells if a root user, also used to find data
	 *  read_byes:  Well how many bytes you have to read derp
	 *
	 *  user entry:  entry will be what we use to call the struct variables
	 */
	int fd, uid, read_bytes;
	struct user entry;

	uid = getuid();  //built in to lib

	fd = open(DATAFILE, O_RDONLY); 
	if(fd == -1) //cant open file for that userid, mebe no existo?!
		return -1;
	read_bytes = read(fd, &entry, sizeof(struct user)) // read the first chunk
	while(entry.uid != uid && read_bytes>0){  //loop until proper uid is found.
		read_bytes = read(fd, &entry, sizeof(struct user)) // KEEP READING NUCKKA
	}
	close(fd);  //close the file
	if(read_bytes < sizeof(struct user))  //this means that the end of the file was reached
		return -1;
	else 
		player = entry;  //copy the read entry into the player struct 
	return 1;  //GREAT SUCCESS HIGH FIVE!!!!
}


/* This is the new user registration function.
 * It will create a new player account and append it to the file!
 */

void register_new_player() {
	int fd;  //file descriptor still durrrrh
	string passcode;  //lels

	printf("-=-={ New Player Registration }=-=-\n");
	printf("Enter your name:  ");
	input_name();

	player.uid = getuid();
	player.highscore = player.credits = 100


	//password duh every game needs password
	printf("Are you Special?!?! \n");
	scanf("%s", &passcode)
	if (passcode == "down down up up left right left right a b")
		player.highscore= player.credits = 999999999
	if(player.name = "Adam Wechter")  //if its me playing
		player.highscore = player.credits = 999999999999

	fd = open(DATAFILE, O_WRONLY|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR);  //perms for file
	if(fd = -1)  //error opening the file
		fatal("in register_new_player() whilst opening file");
	write(fd, &player, sizeof(struct user));
	close(fd);

	printf("\nWelcome to the Game of Chance %s.\n", player.name);
	printf("You have been given %u credits.\n", player.credits);
}

/* this function writes the current player data to the file.
 * it is used primarily for updating the credits after games.
 */

void update_player_data() {
	/* Var Dictionary
	 * fd: File directory
	 * i: incrementor used to run through the struct a few times
	 * read_uid: used to take the and store the uid in the different player structs 
	 * burned_byte:  kinda a sacrifice byte/char var used while looping through the struct to get through whole thing
	 */
	int fd, i , read_uid;
	char bunred_byte;

	fd = open(DATAFILE, O_RDONLY);
	if(fd== -1)  //if open fails here something is srsly fukked
		fatal("in update_player_data() when opening");
	read(fd, &read_uid, 4);  //gets only the 4 byte long uid which is required
	while(read_uid != player.uid) {
		for(i=0; i < sizeof(struct user) -4; i++)  //read through the rest of the struct.
			read(fd, &burned_byte, 1);  //burnedbyte used as a sacrifice
		read(fd, &read_uid, 4);  //read the uid from the next struct after getting through the last
	}
	write(fd, &(player.credits), 4);   //CREDIT update
	write(fd, &(player.highscore), 4); //HIGHSCORE update
	write(fd, &(player.name), 100);    //NAME update

	close(fd);
}

/* 
 * This function will display the current high
 * score and the name of the person who set it
 */
void show_highscore(){
	//local var dictionary, all very obvious
	unsigned int top_score = 0;
	char top_name[100];
	struct user entry;
	int fd;

	printf("\n================| HIGH SCORE |================");
	fd = open(DATAFILE, O_RDONLY);
	if(fd == -1)
		fatal("in show_highscore() while opening file.");  //der
	while(read(fd, &entry, sizeof(struct))>0) {  //loop until end of file
		if(entry.highscore > top_score) {  //if a top score is found
			top_score = entry.highscore;   //set top score
			strcpy(top_name, entry.name);  //and sets top_name to the entries name
		}
	}
	close(fd);
	if(top_score > player.highscore)
		printf("%s has the high score of %u credits!\n", top_name, top_score);
	else
		printf("You have the high score of %u credits!\n", player.highscore);
	printf("==============================================\n\n");
}

/* 
 * This function awards a jack pot for the pick a number game
 */
void jackpot(int jackpot_size){
	printf("*+*+*+*+*+*+* JACKPOT *+*+*+*+*+*+*");
	printf("\nYOU HAVE WON THE JACK POT OF: %u", jackpot_size)  //adding a random jackpot size here
	player.credits += jackpot_size;
}

/* 
 * This function is used to input and update the player name
 * since scanf("%s", &whatever) will stop at the first space (LAME)
 */
void input_name(){
	/* Var Dictionary
	 * input character:  used to flush with new lines to flush out the whole ting
	 * name_ptr: used to go through the character array
	 */
	//Local Variable lib
	char *name_ptr, input_char ='\n';

	while(input=='n')  //Flush any left overs
		scanf("%c", &input_char);  //newline chars

	name_ptr = (char *) &(player.name);  //name_ptr = player name's address
	/*lop to move into name field*/
	while(input_char !='\n') {     //Loop until newline
		*name_ptr = input_char;    //put the input char into name field
		scanf("%c", &input_char);  //gets the next char you put in
		name_ptr++;				   //increments name ptr through the array
	}
	*name_ptr = 0; 				   //Terminates the string and array
}

/* 
 * Thisfunction prints the 3 cards used for the Find the Ace game. 
 * It expects a message to display, a pointer to the cards array, 
 * and the card the user has picked as input.  If the user_pick is
 * -1, then the selection numbers are displayed.
 */
void print_cards(char *message, char *cards, int user_pick){
	//local variable lib:
	int i; //is used to go through this thing

	printf("\n\t*** %s ***\n", message);  //msg print
	printf("      \t._.\t._.\t._.\n");    //card tops
	printf("Cards:\t|%c|\t|%c|\t|%c|\n\t", cards[0], cards[1], cards[2]);
	if(user_pick==-1)
		printf(" 1 \t 2 \t 3\n");
	else{
		for(i=0; i < user_pick; i++)
			printf("\t");
		printf(" ^-- your pick\n");
	}
}

/* ------------Take the Wager Function------------
 * This function inputs wagers for both the No Match dealer and the 
 * Find the Ace games.  It expects the available credits and the
 * previous wager as its args the Previous wager is only important
 * for the secondwager in find the ace game.  the function returns -1
 * if the wager is too big or too little, and it returns the wager
 * amount otherwise
 */
 int take_wager(int available_credits, int previous_wager){
 	//local var lib
 	int wager, total_wager;  //total wager used for ace game

 	printf("How many of your %d credits would you like to wager?  ", available_credits);
 	scanf("%d". &wager);
 	if(wager <1){        //make sure they put somethin on the line
 		printf("Invalid wager amount (too low or negative)");
 		return -1;
 	}
 	/*IMPORTANT*/
 	//ADDED CODE THIS ELSE IF, MAY BE INCORRECT TO BE TESTED!!!
 	/*IMPORTANT*/
 	else if(wager> available_credits){
 		printf("Invalid wager(too much)");
 		return -1;
 	}
 	total_wager = previous_wager+wager;  //total wager
 	if(total_wager> available_credits){  //double checks and confirms available credits
 		printf("Your total wager of %d is more than you have!\n", total_wager);
 		printf("You only have %d credits available, try again!\n", available_credits);
 		return -1;
 	}
 	return wager;
 }

 /* 
  * This function contains a loop to allow the current game to be
  * played again.  It also writes the new credit totals to file
  * after each game is played!
  */
void play_the_game(){
  	//Local var lib
  	int play_again =1;
  	int (*game) ();
  	char selection;  //yes or no character

  	while(play_again){
  		printf("\n[DEBUG] current_game pointer @ 0x%08x\n", player.current_game);
  		if(player.currentgame() != -1){ //if the game plays without error and new hs is set update hs
  			if(player.credits > player.highscore)
  				player.highscore = player.credits;  //update hs here
  			printf("\nYou now have %u credits\n", player.credits);
  			update_player_data();  //updates new credits here brah!
  			printf("Would you like to play again? (y/n):  ");
  			selection = '\n';
  			while(selection =='\n')   //flush new lines here
  				scanf("%c", &selection);
  			if(selection != 'y'| selection !='Y' | selection != 'n' | selection != 'N'){
  				printf("[DEBUG] invalid selection in play_the_game function"); //how can i loop it back around?
  				play_again = 0; //make it 0 to automatically go to menu!
  			}
  			if(selection == 'n')
  				play_again =0;
 		}
  	}
  	else 
  		play_again = 0;  //means some error happened on the inside
}

/* 
 * This function is the Pick a number game.
 * it returns -1 if the player doesnt have enough 
 * credits.  Random jackpot size was added extra.
 */
int pick_a_number(){
	//local var lib
	int pick, winning_number, jackpot_size;  

	srand(time(0));						 //reset random seed every time
	jackpot_size = (rand()%125) + 75;    //75-200 jackpot size
	printf("\n######## Pick a Number ##########\n");
	printf("This game costs 10 credits to play.  Simply pick a number\n");
	printf("between 1 and 20, and if you pick the winning number, you\n");
	printf("will win the jackpot of %d credits!!\n\n", jackpot_size);
	winning_number = (rand()%20) +1; 	//pick a number between 1-20
	if(player.credits < 10){
		printf("You only have %d credits.  That's not enough to play!\n\n", player.credits);
		return -1;  //not enough credits!
	}
	player.credits -= 10;  //deduct here
	printf("10 credits have been deducted from your account. \n");
	printf("Pick an integer between 1 and 20, No refunds given:  ");
	scanf("%d", pick);

	printf("\n ...............");
	printf("\nThe winning number is: %d", winning_number);
	if(pick== winning_number)
		jackpot(jackpot_size);
	else
		printf("Sorry, you didn't win :(");
	return 0;
}

/* 
 * This is the No match dealer game.
 * it returns -1 if the player has 0 credits.
 */
int dealer_no_match(){
	//local var lib
	/* Var Dictionary
	 * i:  used as an incrementor for for loops, both fill and comparison
	 * j:  also used as an incrementor for loop looking for matches
	 * nunmber[16]: holds our 16 random numbers we will use
	 * match = Stores if there are matches in the array
	 */
	int i, j, numbers[16], wager = -1, match = -1; 

	printf("\n:::::::: No Match Dealer ::::::::");
	printf("In this game, you can wager up to all of your credits.\n");
	printf("The dealer will deal out 16 random numbers between 0 and 99\n");
	printf("If there are no matches among them, you double your money!\n");

	if(player.credits <0){
		printf("You have %d credits.\n", player.credits);
		printf("This is not enough credits to play please try again.\n\n");
		return -1;
	}
	while(wager == -1)
		wager = take_wager(player.credits, 0);
	printf("\t\t::: Dealing out 16 random numbers :::\n");
	for(i=0; i < 16; i++) {
		numbers[i] = rand() % 100; //pic ran num
		printf("\%2d\t", numbers[i]);
		if(i%8) {
			printf("\n");
		}
		for(i=0; i < 15; i++) {
			j = i+1;
			while(j < 16) {
				if(numbers[i] == numbers[j]) 
					match = numbers[i];
				j++
			}
		}
	}

	if(match != -1) {
		printf("The dealer matched the number %d!\n", match);
		printf("You lose %d credits.\n", wager);
		player.credits -= wager;
	}
	else {
		printf("no matches found! You win %d credits!\n", wager);
		player.credits += wager;
	}
	return 0;
}

/*
 *find the ace
 * returns -1 if player has 0 credits
 */

int find_the_ace() {
	int i, ace, total_wager;
	int invalid_choice, pick=-1, wager_one = -1, wager_two = -1;
	char choice_two, cards[3] = {'X', 'X', 'X'};

	ace = rand()%3; //place ace

	printf("********** Find the ace game *************");
	printf("In this game you can wager up to all of your credits.\n");
	printf("If you find the ace, you will win your wager.\n");
	printf("After choosing a card, one of the queens will be revealed.\n");
	printf("At this point, you can change selection to a different card\n");
	printf("or increase your wager.\n");

	if(player.credits == 0) {
		printf("You don't have any credits to wager!\n\n");
		return -1;
	}

	while(wager_one == -1) {
		wager_one = take_wager(player.credits, 0);
	}

	print_cards("Dealing cards", cards, -1);
	pick = -1;

	while((pick < 1) || (pick > 3)) { //dont allow invalid picks
		printf("select a card: 1, 2, or 3   ");
		scanf("%d", &pick);
	}

	pick--;   //adjust for offset
	i=0;
	while(i == ace || i == pick) {  //find queen
		i++;
	}

	cards[i] = 'Q';
	print_cards("Revealing a Queen", cards, pick);
	invalid_choice=1;
	while(invalid_choice) {
		printf("would you like to : \n [c]hange your pick \t or  \t [i]ncrease your wager?\n");
		printf("Select c or i\n");
		choice_two = '\n';
		while(choice_two == '\n')  //get rid of extra nl
			scanf("%c", &choice_two);
		if(choice_two == 'i') {  //increase wager
			invalid_choice = 0;
			while(wager_two == -1)  //loop til you get new wager
				wager_two = take_wager(player.credits, wager_one);
		}
		if(choice_two == 'c') {
			i = invalid_choice = 0;
			while(i == pick || cards [i] == 'Q') {
				i++;
			}
			pick = i;
			printf("Your card pick has been changed to card %d.\n", pick+1);
		}	
	}

	for(i=0; i < 3; i++) {
		if(ace == i)
			cards[i] = 'A';
		else
			cards[i] = 'Q';
	}
	print_cards("End result", cards, pick);

	if(pick == ace) { //win
		printf("You have won  %d credits from your first wager\n", wager_one);
		player.credits += wager_one;
		if(wager_two != -1) {
			printf("You have an addtional %d credits from wager two.\n", wager_two);
			player.credits += wager_two;
		}
	}
	else {
		printf("You have lost %d credits!\n", wager_one);
		player.credits -= wager_one;
		if(wager_two != -1) {
			printf("You have lost an additional %d credits from your second wager!\n", wager_two );
			player.credits -= wager_two;
		}
	}
	return 0;
}








