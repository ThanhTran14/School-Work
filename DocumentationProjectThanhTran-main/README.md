Github page: https://github.com/ThanhTran14/DocumentationProjectThanhTran 
 
TicTacToe

Documentation Project

By: Thanh Tran


Introduction

This is a documentation of a tictactoe program. This implementation is a Player vs Player so you need to have a friend to play with. Each player would get their turns and it would alternate until it either ends in a win, tie, or quit the program. This documentation only works with students from Appalachian State University.


How to install


Install needed files here: https://github.com/ThanhTran14/TicTacToe


How to Set up


1. Need to install either Putty or VSCode.

![Photos for Documentation](./Step1.jpg)

![Photos for Documentation](./Step1pt2.jpg)

2 (Putty). On Putty there is a header named "Host Name (or IP Address)" put in your userID@student2.cs.appstate.edu. Another window should pop up asking for your password. 

![Real Step 2](https://user-images.githubusercontent.com/97632664/165152707-46b8eaad-6c7e-4e9e-847d-f77fc0f747c5.jpg)

2 (VS Code). Download a extension called RemoteSSH. Click the green icon on the bottom left. There should be a drop window and hit "Connect to Host". Insert your userID@student2.cs.appstate.edu and password. It may ask multiple times to insert passwrod.


Extension > Search Bar > type in Remote SSH > install it > click green bottom left > Connect to Host > type in appstateuserID@student2.cs.appstate.edu > your password (default your BannerID) > hit Explorer > click ok (blue arrow) > new terminal (red arrow)

The terminal is like Putty part

Below are some photos
![Step3VS1R](https://user-images.githubusercontent.com/97632664/166091385-9ac2a54d-859c-434f-a600-2a0daeaabf5f.jpg)
![Step3VS2R](https://user-images.githubusercontent.com/97632664/166091404-9ab77904-ab1e-4fe5-b598-101df4b128fb.jpg)
![Step3VS3R](https://user-images.githubusercontent.com/97632664/166091418-f04aea12-bae4-4fa1-aed4-60304278899b.jpg)
![Step3VS4R](https://user-images.githubusercontent.com/97632664/166091422-23571abd-4e57-48da-be08-ed03e22eac3c.jpg)
![Step3VS5R](https://user-images.githubusercontent.com/97632664/166091448-79a779ac-beae-47af-a029-bdd61f1fab89.jpg)
![Step3VS6R](https://user-images.githubusercontent.com/97632664/166091467-dc72a197-d573-4b6c-b058-363cac699011.jpg)
![Step3VS67jpgR](https://user-images.githubusercontent.com/97632664/166091490-94cf5cda-3b3b-4cfc-af52-305bb90abb05.jpg)
![Step3VS8jpgR](https://user-images.githubusercontent.com/97632664/166091506-28f5ee47-0555-4cde-aac1-5961ea81854d.jpg)


(Optional) Can make a directory to make things organize. To do this: mkdir whatevernameyouwant

3. Either download the files as a zip (green arrow) and then move the files into the command prompt or git clone into your file (red arrow). If downloaded the zip install WinSCP to move files into your directory/command line. Need to unzip the files.
![](Step2.jpg)

When running WinSCP there should be a window. For "Hostname" enter student2.cs.appstate.edu. Then for user name enter your userID from Appstate and hit login. Then it would ask for your password.
![winSCP1](https://user-images.githubusercontent.com/97632664/166092078-a58bca33-4f9f-4850-a5f0-708da5560af5.jpg)
![passwordwinSCP](https://user-images.githubusercontent.com/97632664/166092096-77f168ba-ffc1-4eec-8a32-4da8f763d0ae.jpg)

Then move the unzipped files into your directory by holding left click and dragging your mouse to the other window.
![drag](https://user-images.githubusercontent.com/97632664/166092202-a30f990d-b771-4a63-81f8-0430cda15240.jpg)

To use git clone into your command use: git clone copyandpastetheredarrowcontent 
![clone](https://user-images.githubusercontent.com/97632664/166095190-2b8c5ae7-afcc-4a0b-ae29-f4bc56588332.jpg)


To use Tictactoe program


1. Use this line to compile the program : g++ -Wall -Werror -o tictactoe main.cc tictactoe.cc 
![compile](https://user-images.githubusercontent.com/97632664/166091865-10100696-dec4-4275-b4ab-fb345ade5d2c.jpg)

2. To run the program use: ./tictactoe

3. Enter Player 1 first name and age

![run](https://user-images.githubusercontent.com/97632664/166091951-7b580774-53ce-4be4-b383-6aefff93ac13.jpg)

4. Enter Player 2 first name and age

![player2](https://user-images.githubusercontent.com/97632664/166093926-b1943979-9256-4a9b-9e6c-1d9a1bac491e.jpg)

5. After Player info is entered, it would be Player 1 turn and board should print
![pic](https://user-images.githubusercontent.com/97632664/166094175-a366763b-eb7e-41d9-ab88-68755a230c4e.jpg)

6. Make a valid command which would be RXCX the R meaning Row and C meaning Column and X being a number between 1 - 3. 

![yourTurn](https://user-images.githubusercontent.com/97632664/166091966-caa61b10-b82c-43c3-bdb2-44639d51ab0a.jpg)

If invalid command the program will repeat until valid input.
Example of invalid inputs: 

![invalid](https://user-images.githubusercontent.com/97632664/166093985-23ce772a-b7f2-4230-a132-00b6bcf5fec0.jpg)

![invalid2](https://user-images.githubusercontent.com/97632664/166094025-a2dd5a88-6185-48b6-8c83-dba2d6e4e711.jpg)

7. The game will end if there is a tie or someone wins. To quit the program hit Control + C.


Functions of TicTacToe.

clearBoard()

It the method to clear the board 

![ClearBoard](https://user-images.githubusercontent.com/97632664/166091788-8a3a9c97-85ee-474a-ae6c-783284a2dc6e.jpg)

getPlayerInfo()

It the method that would ask for players information and it accepts your information through the keyboard

![PlayerInfo](https://user-images.githubusercontent.com/97632664/166091791-65836be6-1c0c-476c-a39b-d3854e480c62.jpg)

printBoard()

It is the method to print out the board so you can see how it looks like. 

![printBoard](https://user-images.githubusercontent.com/97632664/166091793-7e20a17e-2d76-4b99-952c-f5a28da3590b.jpg)

printTurnHeader() 

It is the method to let you know whoever turn it is.

![printPlayerInfo](https://user-images.githubusercontent.com/97632664/166091792-627ecdde-1b3d-4bdc-9e80-49b57303d0de.jpg)

getMove()

It is the method to accept your import and it would check if the move is valid. If someone already placed their O or X that it would say it invalid and ask you again to place your move. It would also checked if you enter the right format.

![getMove](https://user-images.githubusercontent.com/97632664/166091796-07c92352-6e44-45c7-a50a-9ab16e5c315e.jpg)

checkWin()

It is the method where it checks if there a winner.

![checkWin](https://user-images.githubusercontent.com/97632664/166091799-97b5a31b-3d84-4dce-8bf5-23d87569757d.jpg)

checkTie()

It is the method where it checks if there a tie.

![checkTie](https://user-images.githubusercontent.com/97632664/166091801-10858532-d777-489d-b4ab-a3a0c8975a99.jpg)

runGame()

It is the method that makes all these methods work together and in order. 

![runGame](https://user-images.githubusercontent.com/97632664/166091803-7b9cf793-3600-4fb1-b4b0-82eb2cdeceb3.jpg)
