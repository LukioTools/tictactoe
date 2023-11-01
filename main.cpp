// Your First C++ Program
#include <iostream>
#include <ostream>
#include <sstream>
#include <vector>

#define log(smth) std::cout << smth << std::endl;

#define clear std::cout << "\ec" << std::flush;

namespace Color {
    enum Code {
        BG_RED      = 41,
        BG_GREEN    = 42,
        BG_BLUE     = 44,
        BG_DEFAULT  = 49
    };
    class Modifier {
        Code code;
    public:
        Modifier(Code pCode) : code(pCode) {}
        friend std::ostream&
        operator<<(std::ostream& os, const Modifier& mod) {
            return os << "\033[" << mod.code << "m";
        }
    };

    Color::Modifier red(Color::BG_RED);
    Color::Modifier green(Color::BG_GREEN);
    Color::Modifier defBG(Color::BG_DEFAULT);
}

class Vector2{
    public:
        int x,y;

        Vector2(){
            this->x = 0;
            this->y = 0;
        }
        
        Vector2(int _x, int _y): x(_x), y(_y) { }

        int ToElement(Vector2 size){
            return y*size.x+x;
        }
};

void generateMagicSquare(std::vector<int> &array, Vector2 size) {
    int y, x, count = 1, range;
    range = size.y*size.x;
    y = 0;
    x = size.x/2;
    array[Vector2(x,y).ToElement(size)] = count;

    while(count < range) {
        count++;
        if((y-1) < 0 && (x-1) < 0)      //6  when both row and column crosses the range
            y++;  
        else if((y-1) <0) {             //1, 8  when only row crosses range, set i to last row, and decrease j
            y = size.y-1;
            x--;
        }else if((x-1) < 0) {           //2, 7  when only col crosses range, set j to last column, and decrease i
            x = size.x-1;
            y--;  
        }else if(array[Vector2(x-1,y-1).ToElement(size)] != 0){    //3  when diagonal element is not empty, go to next row
            y++;
            }
        else{
            y--;                        // 4, 5
            x--;
        }
        array[Vector2(x,y).ToElement(size)] = count;
    }
}

int WinCheck(std::vector<int> &array,std::vector<int> &magic, Vector2 size)
{
    for (int i = 0; i < size.x; i++)
    {
        int VerticalNum = 0;
        int HorizontalNum = 0;
        for (int j = 0; j < size.y; j++)
        {
            VerticalNum += array[Vector2(i,j).ToElement(size)]*magic[Vector2(i,j).ToElement(size)];
            HorizontalNum += array[Vector2(j,i).ToElement(size)]*magic[Vector2(j,i).ToElement(size)];
        }
        if(VerticalNum == 15 || HorizontalNum == 15) {
            return 1;
        } 
        else if(VerticalNum == 30 || HorizontalNum == 30) {
            return 2;
        }
    }

    int num = 0;
    num += array[Vector2(0,0).ToElement(size)]*magic[Vector2(0,0).ToElement(size)];
    num += array[Vector2(1,1).ToElement(size)]*magic[Vector2(1,1).ToElement(size)];
    num += array[Vector2(2,2).ToElement(size)]*magic[Vector2(2,2).ToElement(size)];
    if(num == 15){ return 1; }else if(num == 30) return 2;

    num = 0;
    num += array[Vector2(2,0).ToElement(size)]*magic[Vector2(2,0).ToElement(size)];
    num += array[Vector2(1,1).ToElement(size)]*magic[Vector2(1,1).ToElement(size)];
    num += array[Vector2(0,2).ToElement(size)]*magic[Vector2(0,2).ToElement(size)];
    if(num == 15){return 1; }else if(num == 30) return 2;
    return 0;
}

void UI(Vector2 size, std::vector<int> board){
    std::stringstream output;
    for (int i = 0; i < size.y; i++)
    {
        for (int j = 0; j < size.x; j++)
        {
            output <<  " [" << board[Vector2(j,i).ToElement(size)] << "] ";
        }
        output << std::endl;
    }   
    std::cout << output.str();
}

void GenerateBoard(std::vector<int> &board, Vector2 boardSize){
    board = std::vector<int>(boardSize.x*boardSize.y, 0);
}

bool legalMove(Vector2 pos, std::vector<int> board, Vector2 size){
    if (board[pos.ToElement(size)]!=0)
    {
        clear;
        log(Color::red << "varattu, yritä uudelleen" << Color::defBG);
        return false;
    }

    if(board.size() < pos.ToElement(size)){
        clear;
        log(Color::red << "yli laidan, yritä uudelleen" << Color::defBG);
        return false;
    }
    return true;
}

int main() {

    std::vector<int> board, magicBoard;
    Vector2 size(3,3);
    int playerCount = 2, player = 0;

    GenerateBoard(board, size);
    GenerateBoard(magicBoard, size);
    generateMagicSquare(magicBoard, size);
    clear;

    while (true)
    {
        UI(size, board);
        log("pelaajan " << player + 1 << " vuoro ");
        Vector2 pos;
        std::cin >> pos.x;
        std::cin >> pos.y;

        if (!legalMove(pos, board, size)) continue;

        board[pos.ToElement(size)] = player + 1; 

        int winner = WinCheck(board, magicBoard, size);
        if(winner == 0){
            clear
        }else{
            clear;
            std:: cout << Color::green << "pelaaja " << winner << " voitti pelin" << Color::defBG;
            break;
        }

        player++;
        if (player >= playerCount) player = 0;
    }
    return 0;
}