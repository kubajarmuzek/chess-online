#include <iostream>
#include <cmath> // For abs function

class ChessGame
{
public:
    char board[8][8];

    ChessGame()
    {
        // Initialize the board in the constructor or use resetGame function
        resetGame();
    }

    void printBoard()
    {
        std::cout << "  +-----------------+" << std::endl;
        for (int i = 7; i >= 0; --i)
        {
            std::cout << i + 1 << " | ";
            for (int j = 0; j < 8; ++j)
            {
                std::cout << board[i][j] << ' ';
            }
            std::cout << "|" << std::endl;
        }
        std::cout << "  +-----------------+" << std::endl;
        std::cout << "    a b c d e f g h" << std::endl;
    }

    char getPieceAt(int row, int col) const
    {
        if (row >= 0 && row < 8 && col >= 0 && col < 8)
        {
            return board[row][col];
        }
        else
        {
            return ' '; // Placeholder for an empty position
        }
    }

    std::string getBoardState()
    {
        std::string boardState;
        boardState += "  0 1 2 3 4 5 6 7\n"; // Column labels
        for (int i = 7; i >= 0; --i)
        {
            boardState += std::to_string(i) + " ";
            for (int j = 0; j < 8; ++j)
            {
                boardState += board[i][j];
                boardState += ' ';
            }
            boardState += std::to_string(i) + "\n"; // Row labels
        }
        boardState += "  0 1 2 3 4 5 6 7\n"; // Column labels

        return boardState;
    }

    int getWinner() const
    {
        bool player1KingAlive = false;
        bool player2KingAlive = false;

        // Check the status of kings
        for (int row = 0; row < 8; ++row)
        {
            for (int col = 0; col < 8; ++col)
            {
                char piece = getPieceAt(row, col);
                if (piece == 'k')
                {
                    // Player 1's king is still alive
                    player1KingAlive = true;
                }
                else if (piece == 'K')
                {
                    // Player 2's king is still alive
                    player2KingAlive = true;
                }
            }
        }

        // Check for a winner
        if (!player1KingAlive && player2KingAlive)
        {
            // Player 2 wins
            return 1;
        }
        else if (player1KingAlive && !player2KingAlive)
        {
            // Player 1 wins
            return 0;
        }

        // No winner yet
        return -1;
    }

    void resetGame()
    {
        char initialBoard[8][8] = {
            {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'},
            {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
            {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'}};

        // Copy values from initialBoard to the board array
        for (int i = 0; i < 8; ++i)
        {
            for (int j = 0; j < 8; ++j)
            {
                board[i][j] = initialBoard[i][j];
            }
        }
    }

    // Function to make a move
    int makeMove(int startRow, int startCol, int destRow, int destCol)
    {
        // Check if the coordinates are within the valid range
        if (isValidCoordinate(startRow, startCol) && isValidCoordinate(destRow, destCol))
        {
            char piece = board[startRow][startCol];

            // Check validity based on the type of piece
            switch (std::tolower(piece))
            {
            case 'p': // Pawn
                if (isValidPawnMove(startRow, startCol, destRow, destCol))
                {
                    board[startRow][startCol] = ' ';
                    board[destRow][destCol] = piece;
                }
                else
                {
                    std::cout << "Invalid move for a pawn." << std::endl;
                    return 0;
                }
                break;

            case 'r': // Rook
                if (isValidRookMove(startRow, startCol, destRow, destCol))
                {
                    board[startRow][startCol] = ' ';
                    board[destRow][destCol] = piece;
                }
                else
                {
                    std::cout << "Invalid move for a rook." << std::endl;
                    return 0;
                }
                break;

            case 'b': // Bishop
                if (isValidBishopMove(startRow, startCol, destRow, destCol))
                {
                    board[startRow][startCol] = ' ';
                    board[destRow][destCol] = piece;
                }
                else
                {
                    std::cout << "Invalid move for a bishop." << std::endl;
                    return 0;
                }
                break;

            case 'n': // Knight
                if (isValidKnightMove(startRow, startCol, destRow, destCol))
                {
                    board[startRow][startCol] = ' ';
                    board[destRow][destCol] = piece;
                }
                else
                {
                    std::cout << "Invalid move for a knight." << std::endl;
                    return 0;
                }
                break;

            case 'q': // Queen
                if (isValidQueenMove(startRow, startCol, destRow, destCol))
                {
                    board[startRow][startCol] = ' ';
                    board[destRow][destCol] = piece;
                }
                else
                {
                    std::cout << "Invalid move for a queen." << std::endl;
                    return 0;
                }
                break;

            case 'k': // King
                if (isValidKingMove(startRow, startCol, destRow, destCol))
                {
                    board[startRow][startCol] = ' ';
                    board[destRow][destCol] = piece;
                }
                else
                {
                    std::cout << "Invalid move for a king." << std::endl;
                    return 0;
                }
                break;

            default:
                std::cout << "Invalid piece." << std::endl;
                return 0;
            }
        }
        else
        {
            std::cout << "Invalid coordinates." << std::endl;
        }
    }

    bool convertInput(const std::string &input, int &startRow, int &startCol, int &destRow, int &destCol)
    {
        if (input.length() != 4)
        {
            return false; // Invalid input length
        }

        // Convert column letter to index
        startCol = input[0] - 'a';
        destCol = input[2] - 'a';

        // Convert row number to index
        startRow = input[1] - '1';
        destRow = input[3] - '1';

        // Check if indices are within the valid range
        return isValidCoordinate(startRow, startCol) && isValidCoordinate(destRow, destCol);
    }

    bool isGameOver()
    {
        // Basic example: Check if the king of either side is captured (checkmate)
        bool whiteKingPresent = false;
        bool blackKingPresent = false;

        for (int i = 0; i < 8; ++i)
        {
            for (int j = 0; j < 8; ++j)
            {
                if (board[i][j] == 'K')
                {
                    whiteKingPresent = true;
                }
                else if (board[i][j] == 'k')
                {
                    blackKingPresent = true;
                }
            }
        }

        return !(whiteKingPresent && blackKingPresent);
    }

private:
    bool isValidCoordinate(int row, int col)
    {
        return row >= 0 && row < 8 && col >= 0 && col < 8;
    }

    bool isValidPawnMove(int startRow, int startCol, int destRow, int destCol)
    {
        // Ensure the destination coordinates are within the board limits
        if (!isValidCoordinate(startRow, startCol) || !isValidCoordinate(destRow, destCol))
        {
            return false;
        }

        char pawn = board[startRow][startCol];
        char destPiece = board[destRow][destCol];

        // Check for initial two-square move
        if (startRow == 1 && destRow == 3 && pawn == 'P' && destPiece == ' ' && board[2][destCol] == ' ')
        {
            return true;
        }
        else if (startRow == 6 && destRow == 4 && pawn == 'p' && destPiece == ' ' && board[5][destCol] == ' ')
        {
            return true;
        }

        // Check for regular one-square move
        if ((pawn == 'P' && destRow == startRow + 1 && destCol == startCol && destPiece == ' ') ||
            (pawn == 'p' && destRow == startRow - 1 && destCol == startCol && destPiece == ' '))
        {
            return true;
        }

        // Check for capturing diagonally
        if ((pawn == 'P' && destRow == startRow + 1 && std::abs(destCol - startCol) == 1 && std::islower(destPiece)) ||
            (pawn == 'p' && destRow == startRow - 1 && std::abs(destCol - startCol) == 1 && std::isupper(destPiece)))
        {
            return true;
        }

        // Check for en passant
        if ((pawn == 'P' && startRow == 4 && destRow == 5 && std::abs(destCol - startCol) == 1 && destPiece == 'p') ||
            (pawn == 'p' && startRow == 3 && destRow == 2 && std::abs(destCol - startCol) == 1 && destPiece == 'P'))
        {
            // Implement en passant logic here
            // For simplicity, let's assume the move is valid
            return true;
        }

        return false;
    }

    bool isValidRookMove(int startRow, int startCol, int destRow, int destCol)
    {
        // Implement rook movement rules
        // For example, you might check if the rook is moving along a row or a column.
        return startRow == destRow || startCol == destCol;
    }

    // Function to check if the destination is empty or has an opponent's piece
    bool isDestinationEmptyOrOpponent(char piece, int destRow, int destCol)
    {
        return board[destRow][destCol] == ' ' || (islower(piece) && isupper(board[destRow][destCol])) || (isupper(piece) && islower(board[destRow][destCol]));
    }

    // Function to check if the move is valid for a bishop
    bool isValidBishopMove(int startRow, int startCol, int destRow, int destCol)
    {
        return std::abs(destRow - startRow) == std::abs(destCol - startCol) && isDestinationEmptyOrOpponent(board[startRow][startCol], destRow, destCol);
    }

    // Function to check if the move is valid for a knight
    bool isValidKnightMove(int startRow, int startCol, int destRow, int destCol)
    {
        int rowDiff = std::abs(destRow - startRow);
        int colDiff = std::abs(destCol - startCol);
        return (rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2) && isDestinationEmptyOrOpponent(board[startRow][startCol], destRow, destCol);
    }

    // Function to check if the move is valid for a queen
    bool isValidQueenMove(int startRow, int startCol, int destRow, int destCol)
    {
        return (startRow == destRow || startCol == destCol || std::abs(destRow - startRow) == std::abs(destCol - startCol)) && isDestinationEmptyOrOpponent(board[startRow][startCol], destRow, destCol);
    }

    // Function to check if the move is valid for a king
    bool isValidKingMove(int startRow, int startCol, int destRow, int destCol)
    {
        return (std::abs(destRow - startRow) <= 1 && std::abs(destCol - startCol) <= 1) && isDestinationEmptyOrOpponent(board[startRow][startCol], destRow, destCol);
    }
};
