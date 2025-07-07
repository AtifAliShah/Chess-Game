#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <algorithm>

using namespace std;

// Represents a piece on the board
enum PieceType { EMPTY, PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING };
enum Color { NONE, WHITE, BLACK };

struct Piece {
    PieceType type;
    Color color;
    bool hasMoved; // For castling and pawn double moves
    Piece(PieceType t = EMPTY, Color c = NONE) : type(t), color(c), hasMoved(false) {}
};

// Represents a position on the board
struct Position {
    int row, col;
    Position(int r = -1, int c = -1) : row(r), col(c) {} // Default to invalid position
    bool operator==(const Position& other) const { return row == other.row && col == other.col; }
};

// Represents a move
struct Move {
    Position from, to;
    PieceType promotion;
    Move() : from(-1, -1), to(-1, -1), promotion(EMPTY) {} // Default constructor
    Move(Position f, Position t, PieceType p = EMPTY) : from(f), to(t), promotion(p) {}
};

class ChessBoard {
private:
    Piece board[8][8];
    Color currentTurn;
    vector<Move> moveHistory;
    Position enPassantTarget; // Square vulnerable to en passant
    bool whiteCanCastleKingside, whiteCanCastleQueenside;
    bool blackCanCastleKingside, blackCanCastleQueenside;

public:
    ChessBoard() : currentTurn(WHITE), enPassantTarget(-1, -1),
                   whiteCanCastleKingside(true), whiteCanCastleQueenside(true),
                   blackCanCastleKingside(true), blackCanCastleQueenside(true) {
        initializeBoard();
    }

    void initializeBoard() {
        // Initialize empty squares
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                board[i][j] = Piece(EMPTY, NONE);

        // Initialize white pieces
        board[0][0] = board[0][7] = Piece(ROOK, WHITE);
        board[0][1] = board[0][6] = Piece(KNIGHT, WHITE);
        board[0][2] = board[0][5] = Piece(BISHOP, WHITE);
        board[0][3] = Piece(QUEEN, WHITE);
        board[0][4] = Piece(KING, WHITE);
        for (int j = 0; j < 8; ++j)
            board[1][j] = Piece(PAWN, WHITE);

        // Initialize black pieces
        board[7][0] = board[7][7] = Piece(ROOK, BLACK);
        board[7][1] = board[7][6] = Piece(KNIGHT, BLACK);
        board[7][2] = board[7][5] = Piece(BISHOP, BLACK);
        board[7][3] = Piece(QUEEN, BLACK);
        board[7][4] = Piece(KING, BLACK);
        for (int j = 0; j < 8; ++j)
            board[6][j] = Piece(PAWN, BLACK);
    }

    void printBoard() const {
        cout << "\n  a b c d e f g h\n";
        for (int i = 7; i >= 0; --i) {
            cout << i + 1 << " ";
            for (int j = 0; j < 8; ++j) {
                char c;
                switch (board[i][j].type) {
                    case PAWN:   c = (board[i][j].color == WHITE ? 'P' : 'p'); break;
                    case KNIGHT: c = (board[i][j].color == WHITE ? 'N' : 'n'); break;
                    case BISHOP: c = (board[i][j].color == WHITE ? 'B' : 'b'); break;
                    case ROOK:   c = (board[i][j].color == WHITE ? 'R' : 'r'); break;
                    case QUEEN:  c = (board[i][j].color == WHITE ? 'Q' : 'q'); break;
                    case KING:   c = (board[i][j].color == WHITE ? 'K' : 'k'); break;
                    default:     c = '.'; break;
                }
                cout << c << ' ';
            }
            cout << i + 1 << endl;
        }
        cout << "  a b c d e f g h\n";
        cout << (currentTurn == WHITE ? "White's turn" : "Black's turn") << endl;
    }

    bool isValidPosition(const Position& pos) const {
        return pos.row >= 0 && pos.row < 8 && pos.col >= 0 && pos.col < 8;
    }

    bool isPathClear(const Position& from, const Position& to) const {
        int dr = to.row - from.row, dc = to.col - from.col;
        int steps = max(abs(dr), abs(dc));
        if (steps <= 1) return true; // Single step or knight move

        int stepRow = dr ? dr / abs(dr) : 0;
        int stepCol = dc ? dc / abs(dc) : 0;

        Position current = from;
        for (int i = 1; i < steps; ++i) {
            current.row += stepRow;
            current.col += stepCol;
            if (board[current.row][current.col].type != EMPTY) return false;
        }
        return true;
    }

    bool isLegalMove(const Move& move) const {
        if (!isValidPosition(move.from) || !isValidPosition(move.to)) return false;
        Piece piece = board[move.from.row][move.from.col];
        if (piece.color != currentTurn || piece.type == EMPTY) return false;

        Position delta(move.to.row - move.from.row, move.to.col - move.from.col);
        int dr = abs(delta.row), dc = abs(delta.col);

        switch (piece.type) {
            case PAWN: {
                int direction = (piece.color == WHITE ? 1 : -1);
                // Normal move
                if (dc == 0 && dr == 1 && delta.row == direction && board[move.to.row][move.to.col].type == EMPTY)
                    return true;
                // Double move
                if (dc == 0 && dr == 2 && delta.row == 2 * direction && !piece.hasMoved &&
                    board[move.to.row][move.to.col].type == EMPTY &&
                    board[move.from.row + direction][move.from.col].type == EMPTY)
                    return true;
                // Capture
                if (dr == 1 && dc == 1 && delta.row == direction &&
                    (board[move.to.row][move.to.col].color == (piece.color == WHITE ? BLACK : WHITE)))
                    return true;
                // En passant
                if (dr == 1 && dc == 1 && delta.row == direction && move.to == enPassantTarget)
                    return true;
                return false;
            }
            case KNIGHT:
                return (dr == 2 && dc == 1) || (dr == 1 && dc == 2);
            case BISHOP:
                return dr == dc && isPathClear(move.from, move.to);
            case ROOK:
                return (dr == 0 || dc == 0) && isPathClear(move.from, move.to);
            case QUEEN:
                return (dr == dc || dr == 0 || dc == 0) && isPathClear(move.from, move.to);
            case KING: {
                if (dr <= 1 && dc <= 1) return true;
                // Castling
                if (piece.color == WHITE && !piece.hasMoved && dr == 0 && dc == 2) {
                    if (move.to.col == 6 && whiteCanCastleKingside &&
                        board[0][5].type == EMPTY && board[0][6].type == EMPTY &&
                        board[0][7].type == ROOK && !board[0][7].hasMoved)
                        return true;
                    if (move.to.col == 2 && whiteCanCastleQueenside &&
                        board[0][1].type == EMPTY && board[0][2].type == EMPTY && board[0][3].type == EMPTY &&
                        board[0][0].type == ROOK && !board[0][0].hasMoved)
                        return true;
                }
                if (piece.color == BLACK && !piece.hasMoved && dr == 0 && dc == 2) {
                    if (move.to.col == 6 && blackCanCastleKingside &&
                        board[7][5].type == EMPTY && board[7][6].type == EMPTY &&
                        board[7][7].type == ROOK && !board[7][7].hasMoved)
                        return true;
                    if (move.to.col == 2 && blackCanCastleQueenside &&
                        board[7][1].type == EMPTY && board[7][2].type == EMPTY && board[7][3].type == EMPTY &&
                        board[7][0].type == ROOK && !board[7][0].hasMoved)
                        return true;
                }
                return false;
            }
            default:
                return false;
        }
    }

    bool makeMove(const Move& move) {
        if (!isLegalMove(move)) return false;

        Piece piece = board[move.from.row][move.from.col];
        enPassantTarget = Position(-1, -1); // Reset en passant target

        // Handle castling
        if (piece.type == KING && abs(move.to.col - move.from.col) == 2) {
            int rookCol = (move.to.col > move.from.col ? 7 : 0);
            int newRookCol = (move.to.col > move.from.col ? 5 : 3);
            int row = (piece.color == WHITE ? 0 : 7);
            board[row][newRookCol] = board[row][rookCol];
            board[row][newRookCol].hasMoved = true;
            board[row][rookCol] = Piece(EMPTY, NONE);
        }

        // Handle en passant
        if (piece.type == PAWN && move.to == enPassantTarget) {
            int capturedRow = (piece.color == WHITE ? move.to.row - 1 : move.to.row + 1);
            board[capturedRow][move.to.col] = Piece(EMPTY, NONE);
        }

        // Set en passant target for pawn double moves
        if (piece.type == PAWN && abs(move.to.row - move.from.row) == 2) {
            int targetRow = (piece.color == WHITE ? move.from.row + 1 : move.from.row - 1);
            enPassantTarget = Position(targetRow, move.from.col);
        }

        // Move piece
        board[move.to.row][move.to.col] = piece;
        board[move.to.row][move.to.col].hasMoved = true;
        board[move.from.row][move.from.col] = Piece(EMPTY, NONE);

        // Handle pawn promotion
        if (piece.type == PAWN && (move.to.row == 0 || move.to.row == 7)) {
            board[move.to.row][move.to.col].type = move.promotion == EMPTY ? QUEEN : move.promotion;
        }

        // Update castling availability
        if (piece.type == KING) {
            if (piece.color == WHITE) {
                whiteCanCastleKingside = whiteCanCastleQueenside = false;
            } else {
                blackCanCastleKingside = blackCanCastleQueenside = false;
            }
        }
        if (piece.type == ROOK) {
            if (piece.color == WHITE) {
                if (move.from.col == 0) whiteCanCastleQueenside = false;
                if (move.from.col == 7) whiteCanCastleKingside = false;
            } else {
                if (move.from.col == 0) blackCanCastleQueenside = false;
                if (move.from.col == 7) blackCanCastleKingside = false;
            }
        }

        moveHistory.push_back(move);
        currentTurn = (currentTurn == WHITE ? BLACK : WHITE);
        return true;
    }

    bool parseMove(const string& input, Move& move) const {
        if (input.length() < 4) {
            cout << "Error: Input too short. Use format like 'e2e4'.\n";
            return false;
        }
        if (!isalpha(input[0]) || !isdigit(input[1]) || !isalpha(input[2]) || !isdigit(input[3])) {
            cout << "Error: Invalid format. Use algebraic notation (e.g., 'e2e4').\n";
            return false;
        }

        int fromCol = tolower(input[0]) - 'a';
        int fromRow = input[1] - '1';
        int toCol = tolower(input[2]) - 'a';
        int toRow = input[3] - '1';

        if (fromCol < 0 || fromCol > 7 || fromRow < 0 || fromRow > 7 ||
            toCol < 0 || toCol > 7 || toRow < 0 || toRow > 7) {
            cout << "Error: Move out of board bounds (a1-h8).\n";
            return false;
        }

        move.from = Position(fromRow, fromCol);
        move.to = Position(toRow, toCol);
        move.promotion = EMPTY;

        if (input.length() == 5 && board[fromRow][fromCol].type == PAWN &&
            (toRow == 7 || toRow == 0)) {
            switch (tolower(input[4])) {
                case 'q': move.promotion = QUEEN; break;
                case 'r': move.promotion = ROOK; break;
                case 'b': move.promotion = BISHOP; break;
                case 'n': move.promotion = KNIGHT; break;
                default:
                    cout << "Error: Invalid promotion piece. Use 'q', 'r', 'b', or 'n'.\n";
                    return false;
            }
        } else if (input.length() > 5) {
            cout << "Error: Input too long. Use 'e2e4' or 'e7e8q' for promotion.\n";
            return false;
        }

        // Check if there's a piece to move
        if (board[fromRow][fromCol].type == EMPTY) {
            cout << "Error: No piece at starting position " << input[0] << input[1] << ".\n";
            return false;
        }
        // Check if the piece belongs to the current player
        if (board[fromRow][fromCol].color != currentTurn) {
            cout << "Error: Cannot move opponent's piece at " << input[0] << input[1] << ".\n";
            return false;
        }

        return true;
    }

    bool isCheckmate() const {
        // Simplified: Check if king is captured (not full checkmate detection)
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                if (board[i][j].type == KING && board[i][j].color == currentTurn)
                    return false;
        return true;
    }
};

int main() {
    ChessBoard game;
    string input;
    Move move; // Safe declaration with default constructor

    cout << "Welcome to Console Chess!\nEnter moves in algebraic notation (e.g., e2e4, or e7e8q for promotion to queen).\nType 'quit' to exit.\n";
    while (true) {
        game.printBoard();
        cout << "Enter move: ";
        getline(cin, input);
        if (input == "quit") break;

        if (!game.parseMove(input, move)) {
            cout << "Please try again.\n";
            continue;
        }

        if (!game.makeMove(move)) {
            cout << "Illegal move. Check the rules and try again.\n";
            continue;
        }

        if (game.isCheckmate()) {
            game.printBoard();
            cout << (game.currentTurn == WHITE ? "Black" : "White") << " wins by checkmate!\n";
            break;
        }
    }
    cout << "Game ended.\n";
    return 0;
}
