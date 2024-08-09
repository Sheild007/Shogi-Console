
# Shogi Game

## Overview

This Shogi project is a console-based implementation of the traditional Japanese chess game, Shogi. It is built using C++ and provides a graphical interface in the console window to interact with the game. The project includes functionalities for drawing the game board, handling moves, capturing pieces, and managing game states.

![shogi](Shogi%20Console/shogi.PNG)

## Features

- **Game Board Drawing**: Draws the Shogi board with a graphical representation of pieces and captured areas.
- **Piece Movement**: Handles valid movements of Shogi pieces according to the game rules.
- **Capture Mechanism**: Allows capturing of pieces and displays captured pieces for both players.
- **Undo Feature**: Supports undoing the last move.
- **Promotion**: Manages piece promotion during the game.
- **File Operations**: Saves and loads game states from files.

![shogi2](Shogi%20Console/shogi2.PNG)

## Getting Started

### Prerequisites

- C++ compiler (e.g., g++, clang++)
- Windows operating system (due to the use of Windows-specific libraries like `windows.h`)

### Console Settings

To ensure the game displays correctly, configure your console with the following settings:

- **Font Raster**: 8x8
- **Width**: 160
- **Height**: 85

### Compilation

1. Ensure you have a C++ compiler installed.
2. Compile the project using the following command:
   ```bash
   g++ -o shogi shogi.cpp
   ```
   Replace `shogi.cpp` with the name of your source file if different.

### Running the Game

1. Run the compiled executable:
   ```bash
   ./shogi
   ```
2. Follow the on-screen instructions to interact with the game:
   - Choose to start a new game or load an existing game.
   - Enter player names.
   - Play the game by selecting positions and making moves.

## File Structure

- `shogi.cpp`: Contains the main game logic, board initialization, and game loop.
- `New.txt`: Contains the initial game board setup for a new game.
- `Load.txt`: Contains the saved game state for loading an existing game.

## Code Overview

### Key Components

- **Utilities**:
  - `getRowColbyLeftClick()`: Captures mouse click positions.
  - `gotoRowCol()`: Moves the cursor to a specific position on the console.
  - `SetClr()`: Sets the text color for console output.
  - `L2R()` and `T2B()`: Draws lines horizontally and vertically.

- **Board Drawing**:
  - `drawBox()`: Draws a box of a given size and color.
  - `DrawBoard()`: Draws the game board with alternating colors.
  - `DrawCapBoard()`: Draws the board for captured pieces.

- **Game Initialization**:
  - `init()`: Initializes the game board and player names.
  - `initLoad()`: Loads a game state from a file.

- **Game Mechanics**:
  - `TurnMessage()`: Displays the current player's turn.
  - `selectPosition()`: Allows the player to select a position on the board.
  - `updadteBoard()`: Updates the board with a move.
  - `PromotionMessage()`: Displays a message for piece promotion.

- **Legal Moves**:
  - Functions to check the legality of various piece moves (e.g., `isKingLegal()`, `isRookLegal()`).

- **Highlighting and Unhighlighting**:
  - `Highlight()`: Highlights valid move locations.
  - `UnHighlight()`: Removes highlights.

- **Check and Checkmate**:
  - `check()`: Checks for check condition.
  - `ChedkMate()`: Checks for checkmate.

- **Undo Functionality**:
  - `UndoMessage()`: Displays the undo option.
  - `UNDO()`: Undoes the last move.

- **Captured Pieces**:
  - `StoreCapturedPiece()`: Stores captured pieces and updates counts.

- **Graphics**:
  - Functions to draw specific pieces (e.g., `pawn()`, `king()`).

## Usage

1. **Starting a New Game**:
   - Choose option 1 from the menu.
   - Enter player names when prompted.
   - Play the game by selecting positions and making moves.

2. **Loading a Saved Game**:
   - Choose option 2 from the menu.
   - Provide the file name containing the saved game state.
   - Continue from where you left off.

3. **Exiting the Game**:
   - Choose option 0 from the menu to exit the game.

## Example

To start a new game:
1. Run the executable.
2. Select option 1 to start a new game.
3. Enter player names.
4. Play by selecting positions for your moves.

To load a saved game:
1. Run the executable.
2. Select option 2 to load a saved game.
3. Enter the file name containing the saved game state.

## Notes

- This project uses the Windows console API for graphical output.
- Ensure that `New.txt` and `Load.txt` files are correctly formatted as per the project's requirements.
- Adjust your console settings to the specified font raster, width, and height for the best experience.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgements

- The Shogi game rules and concepts are based on traditional Japanese chess.
- The project uses Windows-specific APIs for console graphics.

