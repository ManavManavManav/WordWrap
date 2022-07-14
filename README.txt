Manav Gohil mrg225
Manankumar Patel mpp128

We are going to test if the program wrap the txt to given width. If no file name is given, the program will read from standard input
and print to standard ouput. If a presented file name is a regular file, the program reads from the file and prints to standard ouput.
If the presented file name is a directory, the program open each regular file in the directory and write the wrapped in a new file 
created in the directory. The name of each output file will be prefixed with "wrap.", such as file name "wow" will become "wrap.wow".
To determine if the program is working as inteded, we tested each scenerio vigoursly. 
First, we test the scenerio where no file name was presented.
Second, we test the scenerio where a regular file was passed and see if it was wrapped to the width provided by the user.
Third, we checked if the program run if a non regular file was passed. 
Fourth, we tested if a file name is a directory. While ignoring all other subdirectories and non regular file, it goes through every 
single regular file and creates the new file with inteded worp wrap. 