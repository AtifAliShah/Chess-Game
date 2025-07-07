Chess Game
A console-based two-player chess game implemented in C++ that supports standard chess rules, including castling, en passant, and pawn promotion. Players input moves using algebraic notation (e.g., e2e4). The game is designed to run in a terminal or IDE like Dev-C++ and provides robust input validation with clear error messages.
Features

Standard Chess Rules: Supports moves for all pieces (pawn, knight, bishop, rook, queen, king).
Special Moves:
Castling (kingside: e1g1, queenside: e1c1 for White).
En passant for pawns.
Pawn promotion (e.g., e7e8q to promote to a queen).


Console Interface: Displays the board with ranks (1-8) and files (a-h). White pieces are uppercase (P, N, B, R, Q, K), black pieces are lowercase (p, n, b, r, q, k), and empty squares are dots (.).
Input Validation: Handles invalid inputs with descriptive error messages (e.g., "Invalid format", "No piece at starting position").
Game End: Simplified checkmate detection (ends when a king is captured).

Limitations

Checkmate detection is simplified (triggers when the king is captured, not full checkmate logic).
No detection for stalemate, threefold repetition, or 50-move rule.
No check validation (moves putting your own king in check are allowed).
Console-based; no graphical interface.

Prerequisites

A C++ compiler supporting C++11 or later (e.g., MinGW for Dev-C++, g++, clang++).
Dev-C++ (version 5.11 or newer, preferably Embarcadero or Orwell Dev-C++) for Windows users.
Basic knowledge of chess and algebraic notation.

Installation

Clone the Repository:
git clone https://github.com/your-username/Chess-Game.git
cd Chess-Game


File Structure:

chess.cpp: The main source file containing the game logic.
README.md: This documentation file.



Compilation and Running
Using Dev-C++

Open Dev-C++:
Ensure you have Dev-C++ installed (preferably a recent version like Embarcadero Dev-C++).


Create a Project:
Go to File -> New -> Project.
Select "Console Application" (C++), name it (e.g., "Chess"), and save.


Add the Source File:
Replace the default main.cpp with chess.cpp from this repository.


Enable C++11:
Go to Tools -> Compiler Options.
Check "Add the following commands when calling the compiler".
Add: -std=c++11.
Click OK.


Compile and Run:
Press F11 (Compile & Run).
The console will display the chess board and prompt for moves.



Using Command Line (g++ or clang++)

Navigate to the project directory:cd Chess-Game


Compile the code:g++ chess.cpp -o chess -std=c++11


Run the game:./chess  # Linux/Mac
chess.exe  # Windows



How to Play

Start the Game:
Run the compiled program. The board is displayed with White to move first.


Enter Moves:
Use algebraic notation (e.g., e2e4 to move from e2 to e4).
For pawn promotion, append the piece type: e7e8q (queen), e7e8r (rook), e7e8b (bishop), e7e8n (knight).
For castling: e1g1 (White kingside), e1c1 (White queenside), e8g8 (Black kingside), e8c8 (Black queenside).


Exit:
Type quit to end the game.


Error Handling:
Invalid inputs (e.g., e2, 12e4, e2i9) show error messages like "Input too short" or "Move out of board bounds".
Illegal moves (e.g., moving a bishop like a knight) show "Illegal move".



Example Gameplay
Welcome to Console Chess!
Enter moves in algebraic notation (e.g., e2e4, or e7e8q for promotion to queen).
Type 'quit' to exit.

  a b c d e f g h
8 r n b q k b n r 8
7 p p p p p p p p 7
6 . . . . . . . . 6
5 . . . . . . . . 5
4 . . . . . . . . 4
3 . . . . . . . . 3
2 P P P P P P P P 2
1 R N B Q K B N R 1
  a b c d e f g h
White's turn
Enter move: e2e4

Troubleshooting

Compiler Error: ‘cout’ was not declared:
Ensure #include <iostream> and using namespace std; are at the top of chess.cpp.


Compiler Error: ‘vector’ is not a member of ‘std’:
Add -std=c++11 in Dev-C++ Compiler Options or command line.


Console Closes Immediately:
The code includes cin.get() to pause the console. Ensure it’s present before return 0 in main.


Runtime Crash:
Check input format (e.g., e2e4). Share the input causing the crash for debugging.


Dev-C++ Issues:
Update to a newer version (e.g., Embarcadero Dev-C++).
Verify -std=c++11 is set in Compiler Options.



Future Improvements

Add full checkmate and stalemate detection.
Implement check validation to prevent illegal moves.
Support draw conditions (threefold repetition, 50-move rule).
Create a graphical interface using SFML or SDL.
Add move history display.

Contributing
Contributions are welcome! To contribute:

Fork the repository.
Create a branch (git checkout -b feature-branch).
Commit changes (git commit -m "Add feature").
Push to the branch (git push origin feature-branch).
Open a pull request.

License
This project is licensed under the MIT License. See the LICENSE file for details.
Contact
For issues or suggestions, open an issue on GitHub or contact [your-email@example.com].
