// Tetris.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.

#include "stdafx.h" 
#include "iostream" 
#include <conio.h> 
#include <time.h> 
#include <windows.h> 
#include "block.h" 
#include "Log.h" 

#define _X(value) (value)*2 // BLOCK 의 가로 넓이 보정 
#define IS_ARROWKEY(key) (key==224)? true : false // 방향키 입력인가? 

#define STR_NEWGAME_OR_EXIT "! New game? (press 'Enter') or Exit?(press 'x') " 
#define STR_GAME_OVER "[GAME OVER]" 

#define MARGIN_X 2 
#define MARGIN_Y 2 
#define WIDTH 10 
#define HEIGHT 15 
#define BLOCK_CHAR "□" 
#define BORDER_CHAR "■" 
#define BG_CHAR "▦" 
#define BLANK_CHAR "  " 
#define START_X 4 
#define START_Y 0 
#define INIT_SPEED 11 
#define SLEEP_DELAY 100 

#define PREVIEW_BLOCK_X 13
#define PREVIEW_BLOCK_Y 4
#define PREVIEW_SIZE_XY 4

#define SCORE_X 13 
#define SCORE_Y 6 
#define SCORE_ONE_BLOCK 1
#define SCORE_DELETE_LINE 10

#define MAX_GAME_LEVEL 10
#define GAME_LEVEL_UP_GAP 5

#define K_SPACE 0x20 
#define K_UP 0x48 
#define K_LEFT 0x4b 
#define K_RIGHT 0x4d 
#define K_DOWN 0x50 
#define K_ENTER 0x0d 

enum {MOVE_LEFT,MOVE_RIGHT,MOVE_ROTATE,MOVE_DOWN,MOVE_DROP}; 
enum {DRAW_BOARD, DRAW_PREVIEW}; 

HANDLE CONSOLE_HANDLE; // OS 로부터 커서 객체 받아오기 
bool Board[WIDTH][HEIGHT] = {false,}; // 쌓여있는 블록을 저장하는 배열 (블록 있으면 true) 
bool drawBoard[WIDTH][HEIGHT]= {false,}; // 화면에 출력할때 사용하는 배열 (Board과 현재 내려오는 블록을 합쳐서 출력) 
bool cashDrawBoard[WIDTH][HEIGHT]= {false,}; // 바로 이전 프레임에 그렸던 drawBoard 정보를 저장하고 있음 
                                             // 현재 그려야 할 것과 비교해서 상태가 다른 픽셀만 화면에 출력하기 위함. 
bool drawPreview[PREVIEW_SIZE_XY][PREVIEW_SIZE_XY]= {false,}; // 미리보기 출력 배열 
bool cashDrawPreview[PREVIEW_SIZE_XY][PREVIEW_SIZE_XY]= {false,}; // 이전 프레임의 미리보기 출력상태를 저장 

bool isPlaying = true; // 현재 게임 진행중인가? 
block gCurBlock, gNextBlock; // 현재 내려오는 블럭과 다음 블럭의 정보를 가지고 있는 객체 
COORD gCurLocation = {START_X, START_Y}; // 현재 블럭의 위치 (이를 기반으로 블럭을 이동) 

int gBlockCnt = 1; // 나온 블럭 개수 
int gDeletedLineCnt = 0; // 삭제된 라인 개수 
int gScore = 0; // 점수 
int gGameLevel = 1; // 현 레벨 

void init(); // 초기화 
void initGame(); // 게임 기본 정보 초기화 
void initDrawBackGround(); // 배경 그리기 
void onDraw(int draw); // 그리기 (게임화면 + 미리보기 화면) 
void runFrame(); // 한프레임 마다 해야될 작업 

void setCurrentCursorPos(int x, int y); // 커서 이동 

bool isWall(int destX, int destY, int arrow); // 현재 블록이 벽과 충돌하는가? 
bool isCollision(int destX, int destY); // 현재 블록이 쌓여있는 블록들과 충돌하는가? 
void holdBlock(); // 블록 정지하고 쌓기 
void nextBlock(); // 다음 블록 블러오기 (게임오버 로직도 포함) 
void moveBlock(int mode); // mode 방향으로 블록이동 
void checkRemoveLine(); // 꽉찬라인 체크 + 삭제 
void gameOver(); // 게임 오버 
void calculateScore(int s); // 점수계산 
void updateState(); // 상태정보 업데이트 

int main(int argc, char* argv[])
{ 
    init(); 
    initGame(); 
    runFrame(); 
    return 0; 
} 

void init(){ 
    CONSOLE_HANDLE = GetStdHandle(STD_OUTPUT_HANDLE); 
    //LOG.setXY(0, 20); 

    // 커서 안보이게 
    CONSOLE_CURSOR_INFO curInfo; 
    GetConsoleCursorInfo(CONSOLE_HANDLE,&curInfo); 
    curInfo.bVisible= false; 
    SetConsoleCursorInfo(CONSOLE_HANDLE,&curInfo); 

    initDrawBackGround(); 
    calculateScore(gScore); 
} 

void initGame(){ 
    gBlockCnt = 1; // 나온 블럭 개수 
    gDeletedLineCnt = 0; // 삭제된 라인 개수 
    gScore = 0; // 점수 
    gGameLevel = 1; // 현 레벨 

    gCurLocation.X = START_X; 
    gCurLocation.Y = START_Y; 

    for(int i = 0 ; i < WIDTH ; i ++){ 
        for(int j= 0 ; j < HEIGHT ; j++){ 
            Board[i][j]=false; 
        } 
    } 

    setCurrentCursorPos(WIDTH+2, HEIGHT-1); 
    printf("             "); 
    setCurrentCursorPos(WIDTH+2, HEIGHT); 
    printf("                                               "); 
} 

void initDrawBackGround(){ 
    // 좌표 초기화 
    int w= WIDTH+2; 
    int h = HEIGHT; 

    for(int i = 0 ; i < HEIGHT; i++){ 
        setCurrentCursorPos(-1, i); 
        printf("%s",BORDER_CHAR); 
        setCurrentCursorPos(w-2, i); 
        printf("%s",BORDER_CHAR); 
    } 
    for(int i = 0 ; i < WIDTH+2; i++){ 
        setCurrentCursorPos(i-1, h); 
        printf("%s",BORDER_CHAR); 
    } 
    for(int h = 0 ; h < HEIGHT ; h++){ 
        for(int w= 0 ; w < WIDTH ; w++){ 
            setCurrentCursorPos(w, h); 
            printf("%s",BG_CHAR); 
        } 
    } 

    setCurrentCursorPos(PREVIEW_BLOCK_X,PREVIEW_BLOCK_Y); 
    printf("Next block"); 

/* 
    for(int i = 0; i < PREVIEW_SIZE_XY ; i++){ 
        setCurrentCursorPos(PREVIEW_BLOCK_X-1, PREVIEW_BLOCK_Y+i); 
        printf("%s",BORDER_CHAR); 
        setCurrentCursorPos(PREVIEW_BLOCK_X+PREVIEW_SIZE_XY, PREVIEW_BLOCK_Y+i); 
        printf("%s",BORDER_CHAR); 
    } 
    */ 
} 

void runFrame(){ 
    int speed = 0; 
    isPlaying= false; 
    setCurrentCursorPos(WIDTH+2, HEIGHT); 
    printf("%s",STR_NEWGAME_OR_EXIT); 
    while(!isPlaying){ 
        if(_kbhit()){ 
            byte key = _getch(); 
            switch(key){ 
            case K_ENTER: //enter
                nextBlock(); 
                initGame(); 
                updateState(); 
                onDraw(DRAW_PREVIEW); 
                onDraw(DRAW_BOARD); 
                  isPlaying = true; 
            break; 
            case 'x': 
                return; 
            } 
        } 

        while(isPlaying){ 
            if(_kbhit()){ 
                byte key = _getch(); 
                if(IS_ARROWKEY(key)) {// 방향키 입력에 따른 특수코드 필터링 (224) 
                    key = _getch(); 
                } 
                switch(key){ 
                case K_UP :  
                    moveBlock(MOVE_ROTATE); 
                    break; 
                case K_LEFT :  
                    moveBlock(MOVE_LEFT); 
                    break; 
                case K_RIGHT :  
                    moveBlock(MOVE_RIGHT); 
                    break; 
                case K_DOWN :  
                    moveBlock(MOVE_DOWN); 
                    speed = 1; 
                    break; 
                case K_SPACE: 
                    moveBlock(MOVE_DROP); 
                    break;     
                } 
                onDraw(DRAW_BOARD); 
                if(!isPlaying) break; 
            } 

            // 타이머로 바꿀것. 
            if(isPlaying && speed++ >= (INIT_SPEED-gGameLevel)){ 
                speed = 1; 
                moveBlock(MOVE_DOWN); 
                onDraw(DRAW_BOARD); 
            } 

            Sleep(SLEEP_DELAY); 
        } 
        Sleep(SLEEP_DELAY); 
    } 
} 

void holdBlock(){ 
    COORD *loc = gCurBlock.getCoords(); 
    for(int i=0 ; i < BLOCK_PIXEL_CNT; i++){ 
        if(gCurLocation.Y + loc[i].Y < 0) continue;
        Board[gCurLocation.X + loc[i].X][gCurLocation.Y + loc[i].Y] = true; 
    } 
} 

void checkRemoveLine(){ 
    int checkLine_cnt = gCurBlock.getSize().Y; 
    int checkLine = gCurLocation.Y; 
    int existCnt; 
     
    int deleteLine[4] = {-1,}; 
    int deleteLineCnt = 0;
    for(int i = 0 ; i <  checkLine_cnt ; i++){ 
        existCnt = 0; 
        for(int x = 0 ; x < WIDTH ;x++){ 
            if(Board[x][checkLine] == false){ 
                break; 
            }else{ 
                existCnt++; 
            } 
            if(existCnt == WIDTH){ 
                deleteLine[deleteLineCnt] = checkLine; 
                deleteLineCnt++; 
            } 
        } 
        checkLine--; 
    } 

    if(deleteLineCnt == 0) return;
    int baseLine = deleteLine[0];
    int moveLine =baseLine-1; 
    int nonExistCnt=0; 
    int i = 1; 
    while(nonExistCnt < WIDTH){ 
        nonExistCnt = 0; 
        if(i != deleteLineCnt){ // 지워야할 라인이 2줄이상 남은 상태라면, 
            while(moveLine == deleteLine[i]){ // 지금 옴길 라인이 삭제될 라인이라면, 
                moveLine--; // 다음 라인으로 넘어감 
                i++; // 다음 삭제될 라인 인덱스로 넘어감 
            } 
        } 
        for(int x = 0 ; x <WIDTH ; x++){ 
            Board[x][baseLine] = Board[x][moveLine];// 픽셀이동 
            if(Board[x][moveLine] == false) nonExistCnt++; 
        } 
        moveLine--; 
        baseLine--; 
    } 
    while(moveLine != baseLine){ 
        for(int x= 0 ; x <WIDTH ; x++){ 
            if(Board[x][baseLine]) Board[x][baseLine]= false; 
        } 
        baseLine--; 
    } 
    gDeletedLineCnt += deleteLineCnt; 
    calculateScore(SCORE_DELETE_LINE*deleteLineCnt); 
} 

void moveBlock(int mode){ 
    int gap; 
    switch(mode){ 
    case MOVE_LEFT:  
        if(!isWall(gCurLocation.X-1, gCurLocation.Y, mode) &&  
            !isCollision(gCurLocation.X-1, gCurLocation.Y)){
            gCurLocation.X--; 
        } 
        break; 

    case MOVE_RIGHT: 
        if(!isWall(gCurLocation.X+1, gCurLocation.Y,mode) && 
            !isCollision(gCurLocation.X+1, gCurLocation.Y)){
            gCurLocation.X++; 
        } 
        break; 

    case MOVE_ROTATE: 
        gCurBlock.rotation(CW); 
        // 오른쪽 벽 확인? 
        // - 벽아니면 블록 충돌체크? 
        //    - Y: ccw 회전후 종료 
        //    - n: 종료  
        // - 벽이면 왼쪽으로 이동후 충돌체크? 
        //    - y: ccw 회전후 종료 
        //    - n: 종료 
        gap = WIDTH - (gCurBlock.getSize().X+gCurLocation.X); 
        if(gap >= 0){ 
            if(isCollision(gCurLocation.X,gCurLocation.Y)){ 
                gCurBlock.rotation(CCW); 
            } 
        }else{ 
            if(isCollision(gCurLocation.X+gap,gCurLocation.Y)){ 
                gCurBlock.rotation(CCW); 
            }else{ 
                gCurLocation.X+=gap; 
            } 
        } 
        break; 

    case MOVE_DOWN: 
        if(!isWall(gCurLocation.X, gCurLocation.Y+1, mode) &&  
            !isCollision(gCurLocation.X, gCurLocation.Y+1)){
            gCurLocation.Y++; 
        }else{ 
            holdBlock(); 
            checkRemoveLine(); 
            nextBlock(); 
        } 
        break; 

    case MOVE_DROP: 
        while(!isWall(gCurLocation.X, gCurLocation.Y+1, mode) &&  
            !isCollision(gCurLocation.X, gCurLocation.Y+1)){
            gCurLocation.Y++; 
        } 
        holdBlock(); 
        checkRemoveLine(); 
        nextBlock(); 
        break; 
    } 
} 

void nextBlock(){ 
    gBlockCnt++; 
    calculateScore(SCORE_ONE_BLOCK); 
    gCurLocation.X = START_X; 
    gCurLocation.Y = START_Y; 
    gCurBlock.set(gNextBlock.getShape(),gNextBlock.getRotation()); 
    gNextBlock.random(); 

    for(int i = 0 ; i < gCurBlock.getSize().X ; i++){ 
        if(Board[START_X+i][0]){ 
            gameOver(); 
            return; 
        } 
    } 

    onDraw(DRAW_PREVIEW); 
} 

void setCurrentCursorPos(int x, int y)        
{ 
    COORD pos={_X(MARGIN_X+1+x), MARGIN_Y+y}; 
    SetConsoleCursorPosition(CONSOLE_HANDLE, pos); 
} 

void onDraw(int draw){ 
    COORD *c; 

    switch(draw){ 
    case DRAW_BOARD: 
        c = gCurBlock.getCoords(); 
        for(int i = 0; i < BLOCK_PIXEL_CNT ; i++){ 
            if(gCurLocation.Y + c[i].Y < 0){ 
                continue; 
            } 
            drawBoard[gCurLocation.X + c[i].X][gCurLocation.Y + c[i].Y] = true; 
        } 

        for(int h = 0 ; h < HEIGHT ; h++){ 
            for(int w = 0 ; w < WIDTH ; w++){ 
                if(Board[w][h] == true) 
                    drawBoard[w][h] = true; 

                if(cashDrawBoard[w][h] != drawBoard[w][h]){ 
                    if(drawBoard[w][h] == true){ 
                        setCurrentCursorPos(w, h); 
                        printf("%s",BLOCK_CHAR); 
                        drawBoard[w][h] = false; 
                        cashDrawBoard[w][h] = true; 
                    }else{ 
                        setCurrentCursorPos(w, h); 
                        printf("%s",BG_CHAR);     
                        cashDrawBoard[w][h] = false; 
                    } 
                }else{ 
                    if(drawBoard[w][h] == true) drawBoard[w][h] = false; 
                } 
            } 
        } 
        break; 

    case DRAW_PREVIEW: 
        c = gNextBlock.getCoords(); 
        for(int i = 0 ; i < BLOCK_PIXEL_CNT ; i ++){ 
            drawPreview[c[i].X][PREVIEW_BLOCK_Y-1+c[i].Y] = true; 
        } 
        for(int h= 0 ; h < PREVIEW_SIZE_XY ; h++){ 
            for(int w= 0 ; w < PREVIEW_SIZE_XY ; w++){ 
                if(cashDrawPreview[w][h] != drawPreview[w][h]){ 
                    if(drawPreview[w][h] == true){ 
                        setCurrentCursorPos(PREVIEW_BLOCK_X+w, h); 
                        drawPreview[w][h] =false; 
                        cashDrawPreview[w][h] = true; 
                        printf("%s",BLOCK_CHAR); 
                    }else{ 
                        setCurrentCursorPos(PREVIEW_BLOCK_X+w, h); 
                        printf("%s", BLANK_CHAR);     
                        cashDrawPreview[w][h] = false; 
                    } 
                }else{ 
                    if(drawPreview[w][h] == true) drawPreview[w][h] = false; 
                } 
            } 
        } 
        break; 
    } 
} 

bool isWall(int destX, int destY, int arrow){ 
    switch(arrow){ 
    case MOVE_LEFT:  
        if(destX < 0) 
            return true; 
        break; 
    case MOVE_RIGHT:  
        if(destX+gCurBlock.getSize().X > WIDTH) 
            return true; 
        break; 
    case MOVE_DROP:  
    case MOVE_DOWN:  
        if(destY >= HEIGHT) 
            return true; 
        break; 
    } 
    return false; 
} 

bool isCollision(int destX, int destY){ 
    COORD *pix = gCurBlock.getCoords(); 
    COORD result; 
    for(int i = 0 ; i < BLOCK_PIXEL_CNT ; i ++){ 
        result.X = destX+pix[i].X; 
        result.Y = destY+pix[i].Y; 
        if(result.Y < 0){ 
            continue; 
        } 
        if(Board[result.X][result.Y] == true){ 
            return true; 
        } 
    } 

    return false; 
} 

void calculateScore(int s){
    gScore += s; 
    if(gDeletedLineCnt > gGameLevel*GAME_LEVEL_UP_GAP){ 
        if(gGameLevel!= MAX_GAME_LEVEL) gGameLevel++; 
    } 
    updateState(); 
} 

void updateState(){ 
    setCurrentCursorPos(SCORE_X,SCORE_Y); 
    printf("Score : %d\t\t\t",gScore); 
    setCurrentCursorPos(SCORE_X,SCORE_Y+1); 
    printf("Level : %d\t\t\t",gGameLevel); 
    setCurrentCursorPos(SCORE_X,SCORE_Y+2); 
    printf("Block Count : %d\t\t\t", gBlockCnt); 
    setCurrentCursorPos(SCORE_X,SCORE_Y+3); 
    printf("Deleted Line Count: %d\t\t\t",gDeletedLineCnt); 
} 

void gameOver(){ 
    isPlaying = false; 
    setCurrentCursorPos(WIDTH+2, HEIGHT-1); 
    printf("%s",STR_GAME_OVER); 
    setCurrentCursorPos(WIDTH+2, HEIGHT); 
    printf("%s",STR_NEWGAME_OR_EXIT); 
}