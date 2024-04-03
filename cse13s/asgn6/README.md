# Assignment 6
The purpose of this assignment is to create a firewall to filter text, detect banned words (badspeak), and convert any old words (oldspeak) into new words (newspeak). 

The program will read in multiple files--badspeak.txt, oldspeak.txt, and newspeak.txt--to create a lexicon of badspeak and oldspeak/newspeak translations. 

Then, the program will read in text from a citizen. If a word from the text is possibly badspeak or oldspeak, a Bloom filter will be used to carry out a few different actions: send the citizen to joycamp by accusing them of thoughtcrime if they used badspeak, help the citizen practice Rightspeak by providing oldspeak/newspeak pairs, or refrain from any disciplinary action when a false positive was detected.

## Build
	$ make all

## Run
	$ ./banhammer

### Program arguments
	-h <size>: Specifies that the hash table with have <size> entries. The default is 10000.
	-f <size>: Specifies that the Bloom filter will have <size> entries. The default is 2^20.
	-m: Enables the move-to-front rule. Everytime ll_lookup is called, the node to lookup will be moved to the front of the list if it is present.

## Clean
	$ make clean

## Format
	$ make format
