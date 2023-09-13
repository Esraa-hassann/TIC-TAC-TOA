#include <iostream>
#include <vector>
using namespace std;
// The board size
const int BOARD_SIZE = 3;
// The player and AI markers
const char PLAYER_MARKER = 'X';
const char AI_MARKER = 'O';
// Function to print the board
void printBoard(const vector<vector<char>>& board)
{
    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        for (int j = 0; j < BOARD_SIZE; ++j)
        {
            cout << board[i][j] << ' ';
        }
        cout << endl;
    }
}
// Function to check if a player has won
bool checkWin(const vector<vector<char>>& board, char marker)
{
    // Check rows and columns
    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        if (board[i][0] == marker && board[i][1] == marker && board[i][2] == marker)
            return true;
        if (board[0][i] == marker && board[1][i] == marker && board[2][i] == marker)
            return true;
    }
    // Check diagonals
    if (board[0][0] == marker && board[1][1] == marker && board[2][2] == marker)
        return true;
    if (board[0][2] == marker && board[1][1] == marker && board[2][0] == marker)
        return true;

    return false;
}

// Function to check if the board is full
bool boardFull(const vector<vector<char>>& board)
{
    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        for (int j = 0; j < BOARD_SIZE; ++j)
        {
            if (board[i][j] == ' ')
                return false;
        }
    }
    return true;
}

// Heuristic function to evaluate the score of a board state
int evaluateBoard(const vector<vector<char>>& board) 
{
    if (checkWin(board, AI_MARKER))
        return 10;
    if (checkWin(board, PLAYER_MARKER))
        return -10;
    return 0;
}

// Minimax function with alpha-beta pruning to find the optimal move for the AI
int minimax(vector<vector<char>>& board, int depth, bool isMaximizing, int alpha, int beta) {
    int score = evaluateBoard(board);

    // Base cases for terminal states
    if (score == 10)
        return score - depth;
    if (score == -10)
        return score + depth;
    if (boardFull(board))
        return 0;

    // If maximizing player's turn
    if (isMaximizing) {
        int maxScore = -1000;
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                if (board[i][j] == ' ') {
                    board[i][j] = AI_MARKER;
                    maxScore = max(maxScore, minimax(board, depth + 1, !isMaximizing, alpha, beta));
                    board[i][j] = ' ';
                    alpha = max(alpha, maxScore);
                    if (beta <= alpha)
                        break;
                }
            }
        }
        return maxScore;
    }
    // If minimizing player's turn
    else {
        int minScore = 1000;
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                if (board[i][j] == ' ') {
                    board[i][j] = PLAYER_MARKER;
                    minScore = min(minScore, minimax(board, depth + 1, !isMaximizing, alpha, beta));
                    board[i][j] = ' ';
                    beta = min(beta, minScore);
                    if (beta <= alpha)
                        break;
                }
            }
        }
        return minScore;
    }
}

// Function to find the best move for the AI
pair<int, int> findBestMove(vector<vector<char>>& board)
{
    int bestScore = -1000;
    pair<int, int> bestMove = make_pair(-1, -1);

    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] == ' ') {
                board[i][j] = AI_MARKER;
                int moveScore = minimax(board, 0, false, -1000, 1000);
                board[i][j] = ' ';
                if (moveScore > bestScore) {
                    bestScore = moveScore;
                    bestMove = make_pair(i, j);
                }
            }
        }
    }

    return bestMove;
}

// Function to play the game
void playGame() {
    vector<vector<char>> board(BOARD_SIZE, vector<char>(BOARD_SIZE, ' '));

    cout << "Welcome to Tic-Tac-Toe!" << endl;
    cout << "You are playing as " << PLAYER_MARKER << " against the AI (" << AI_MARKER << ")." << endl;

    printBoard(board);

    bool playerTurn = true;

    while (true) {
        if (playerTurn) {
            int row, col;
            cout << "Enter the row (0-2) and column (0-2) to place your marker: ";
            cin >> row >> col;

            if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE || board[row][col] != ' ') {
                cout << "Invalid move! Try again." << endl;
                continue;
            }

            board[row][col] = PLAYER_MARKER;
        }
        else {
            cout << "AI's turn:" << endl;

            pair<int, int> bestMove = findBestMove(board);
            board[bestMove.first][bestMove.second] = AI_MARKER;

            cout << "AI placed its marker at row " << bestMove.first << ", col " << bestMove.second << endl;
        }

        printBoard(board);

        if (checkWin(board, PLAYER_MARKER)) {
            cout << "You won!" << endl;
            break;
        }
        else if (checkWin(board, AI_MARKER)) {
            cout << "AI won!" << endl;
            break;
        }
        else if (boardFull(board)) {
            cout << "It's a draw!" << endl;
            break;
        }

        playerTurn = !playerTurn;
    }
}

// Main function
int main()
{
    playGame();
    return 0;
}
