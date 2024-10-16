#include <graphics.h>
#include <conio.h>
#include <dos.h>
#include <stdio.h>

#define BOARD_SIZE 8
#define CELL_SIZE 50

// Colors for the chessboard
#define WHITE_CELL LIGHTGRAY
#define BLACK_CELL DARKGRAY
#define SELECTED_CELL GREEN
#define POSSIBLE_MOVE_CELL LIGHTBLUE

// Define chess piece types
#define EMPTY 0
#define PAWN 1
#define ROOK 2
#define KNIGHT 3
#define BISHOP 4
#define QUEEN 5
#define KING 6

// 2D array to store the board state (0 = empty, 1 = pawn, etc.)
int board[BOARD_SIZE][BOARD_SIZE] = {
    {ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK},
    {PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN},
    {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
    {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
    {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
    {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
    {PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN},
    {ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK}
};

// Function to draw the chessboard and pieces
void drawBoard(int selectedRow, int selectedCol, int highlightMoves) {
    int x, y;

    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            // Calculate position of the cell
            x = col * CELL_SIZE;
            y = row * CELL_SIZE;

            // Set the color of the cell (alternate colors)
            if ((row + col) % 2 == 0) {
                setfillstyle(SOLID_FILL, WHITE_CELL);
            } else {
                setfillstyle(SOLID_FILL, BLACK_CELL);
            }

            // Highlight selected cell
            if (row == selectedRow && col == selectedCol) {
                setfillstyle(SOLID_FILL, SELECTED_CELL);
            }

            // Highlight possible moves for selected piece
            if (highlightMoves && board[row][col] == EMPTY) {
                setfillstyle(SOLID_FILL, POSSIBLE_MOVE_CELL);
            }

            // Draw the cell
            bar(x, y, x + CELL_SIZE, y + CELL_SIZE);

            // Draw the chess piece
            if (board[row][col] != EMPTY) {
                setcolor(WHITE); // Default piece color (can add more customization)
                switch (board[row][col]) {
                    case PAWN:
                        setcolor(YELLOW); // Pawn color
                        circle(x + CELL_SIZE / 2, y + CELL_SIZE / 2, 10); // Simple circle for pawn
                        floodfill(x + CELL_SIZE / 2, y + CELL_SIZE / 2, YELLOW);
                        break;
                    case ROOK:
                        setcolor(RED); // Rook color
                        rectangle(x + 10, y + 10, x + CELL_SIZE - 10, y + CELL_SIZE - 10); // Rook shape
                        break;
                    case KNIGHT:
                        setcolor(BLUE); // Knight color
                        rectangle(x + 15, y + 15, x + CELL_SIZE - 15, y + CELL_SIZE - 15); // Knight shape
                        break;
                    case BISHOP:
                        setcolor(CYAN); // Bishop color
                        circle(x + CELL_SIZE / 2, y + CELL_SIZE / 2, 15); // Bishop circle
                        floodfill(x + CELL_SIZE / 2, y + CELL_SIZE / 2, CYAN);
                        break;
                    case QUEEN:
                        setcolor(MAGENTA); // Queen color
                        rectangle(x + 5, y + 5, x + CELL_SIZE - 5, y + CELL_SIZE - 5); // Queen shape
                        break;
                    case KING:
                        setcolor(GREEN); // King color
                        rectangle(x + 20, y + 20, x + CELL_SIZE - 20, y + CELL_SIZE - 20); // King shape
                        break;
                }
            }
        }
    }
}

// Function to highlight possible moves (currently only for pawns, extendable)
void highlightMoves(int row, int col) {
    if (board[row][col] == PAWN) {
        if (row > 0 && board[row - 1][col] == EMPTY) {
            drawBoard(row - 1, col, 1); // Highlight move forward for pawn
        }
    }
}

// Function to animate the movement of a piece from one cell to another
void animatePieceMove(int startX, int startY, int endX, int endY, int piece) {
    int x = startX, y = startY;

    while (x != endX || y != endY) {
        // Clear old position
        drawBoard(x / CELL_SIZE, y / CELL_SIZE, 0);

        // Draw the piece at the new position
        drawBoard(x / CELL_SIZE, y / CELL_SIZE, 0);
        drawBoard(x, y, piece);

        // Move the piece slowly to the target
        if (x < endX) x += 5;
        if (x > endX) x -= 5;
        if (y < endY) y += 5;
        if (y > endY) y -= 5;

        delay(50); // Delay for animation
    }

    // Final draw of the piece at the target position
    drawBoard(endX / CELL_SIZE, endY / CELL_SIZE, piece);
}

// Function to move pieces
void movePiece(int *selectedRow, int *selectedCol) {
    int isSelected = 0; // Flag to indicate if a piece is selected
    int selectedPieceRow = -1, selectedPieceCol = -1; // Store selected piece position
    char key;

    while (1) {
        drawBoard(*selectedRow, *selectedCol, 0);
        delay(100);

        // Get user input for movement
        if (kbhit()) {
            key = getch();
            if (key == 27) { // ESC key to exit
                break;
            }

            switch (key) {
                case 72: // Up arrow
                    if (*selectedRow > 0) (*selectedRow)--;
                    break;
                case 80: // Down arrow
                    if (*selectedRow < BOARD_SIZE - 1) (*selectedRow)++;
                    break;
                case 75: // Left arrow
                    if (*selectedCol > 0) (*selectedCol)--;
                    break;
                case 77: // Right arrow
                    if (*selectedCol < BOARD_SIZE - 1) (*selectedCol)++;
                    break;
                case 13: // Enter key to select or move piece
                    if (!isSelected) {
                        // First enter key press, select the piece
                        if (board[*selectedRow][*selectedCol] != EMPTY) {
                            isSelected = 1;
                            selectedPieceRow = *selectedRow;
                            selectedPieceCol = *selectedCol;
                            highlightMoves(selectedPieceRow, selectedPieceCol);
                        }
                    } else {
                        // Second enter key press, move the piece
                        if (board[*selectedRow][*selectedCol] == EMPTY) {
                            // Move the selected piece to the new position
                            board[*selectedRow][*selectedCol] = board[selectedPieceRow][selectedPieceCol];
                            board[selectedPieceRow][selectedPieceCol] = EMPTY;
                            isSelected = 0; // Reset selection
                        }
                    }
                    break;
            }
        }
    }
}

int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "C:\\Turboc3\\BGI");

    int selectedRow = 0, selectedCol = 0;

    // Main game loop
    movePiece(&selectedRow, &selectedCol);

    closegraph();
    return 0;
}
