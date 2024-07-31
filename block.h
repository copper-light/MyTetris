#pragma once 

#include <windows.h> 
#include <ctime> 

#define BLOCK_SHAPE_CNT 7
#define BLOCK_PIXEL_CNT 4

enum ROTATOIN {CW,CCW}; 

typedef struct _BLOCK{ 
    int rotate; // 회전 개수 
    COORD **coord; // 블럭 픽셀들의 좌표 
    COORD *size; // 블럭의 가로세로 크기 
} T_BLOCK; 

class block 
{ 
public: 
    block(void); 
    ~block(void); 
    void random(); // 다음 블럭 랜덤 형성 
    void rotation(int ckwise); // 회전 
    COORD getSize(); // 현재 블럭의 가로 세로 사이즈 
    COORD *getCoords(); // 현재 블럭 픽셀의 위치들 
    COORD *getCoords(int rotation); 
    COORD *getCoords(int shape, int rotation);
    int getRotation();  
    int getShape(); 
    void set(int shape, int rotation);

private: 
    void set(COORD *coord, int x, int y); // 좌표 저장 
    void allocMemory(T_BLOCK *block, int size); // 블럭에 메모리 할당 

    T_BLOCK Blocks[BLOCK_SHAPE_CNT]; 
    int gCurShape; // 현재 블럭의 번호 
    int gCurRotation; // 현재 블럭의 회전 번호 
};