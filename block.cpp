#include "block.h" 
#include "Log.h" 

enum E_TYPE {A,B,C,D,E,F}; 


void block::set(COORD *coord, int x, int y){ 
    (*coord).X = x; 
    (*coord).Y = y; 
} 

block::block(void) 
{ 
    T_BLOCK A; 
    allocMemory(&A,2); 

    // ■■■■ 
    set(&A.coord[0][0],0,0); 
    set(&A.coord[0][1],1,0); 
    set(&A.coord[0][2],2,0); 
    set(&A.coord[0][3],3,0); 
    A.size[0].X = 4; 
    A.size[0].Y = 1; 
     
    // ■ 
    // ■ 
    // ■  
    // ■ 
    set(&A.coord[1][0],0,-3); 
    set(&A.coord[1][1],0,-2); 
    set(&A.coord[1][2],0,-1); 
    set(&A.coord[1][3],0,0); 
    A.size[1].X = 1; 
    A.size[1].Y = 4; 

    // ------------------------------ 

    T_BLOCK B; 
    allocMemory(&B,1); 

    // ■■ 
    // ■■ 
    set(&B.coord[0][0],0,0); 
    set(&B.coord[0][1],1,0); 
    set(&B.coord[0][2],0,-1); 
    set(&B.coord[0][3],1,-1); 
    B.size[0].X = 2; 
    B.size[0].Y = 2; 

    // ------------------------------ 

    T_BLOCK C; 
    allocMemory(&C,2); 

    //   ■■ 
    // ■■ 
    set(&C.coord[0][0],0,0); 
    set(&C.coord[0][1],1,0); 
    set(&C.coord[0][2],1,-1); 
    set(&C.coord[0][3],2,-1); 
    C.size[0].X = 3; 
    C.size[0].Y = 2; 

    // ■ 
    // ■■ 
    //   ■ 
    set(&C.coord[1][0],1,0); 
    set(&C.coord[1][1],0,-1); 
    set(&C.coord[1][2],1,-1); 
    set(&C.coord[1][3],0,-2); 
    C.size[1].X = 2; 
    C.size[1].Y = 3; 

    // ------------------------------- 

    T_BLOCK D; 
    allocMemory(&D,2); 

    // ■■ 
    //   ■■ 
    set(&D.coord[0][0],1,0); 
    set(&D.coord[0][1],2,0); 
    set(&D.coord[0][2],0,-1); 
    set(&D.coord[0][3],1,-1); 
    D.size[0].X = 3; 
    D.size[0].Y = 2; 

    //   ■  
    // ■■ 
    // ■ 
    set(&D.coord[1][0],0,0); 
    set(&D.coord[1][1],0,-1); 
    set(&D.coord[1][2],1,-1); 
    set(&D.coord[1][3],1,-2); 
    D.size[1].X = 2; 
    D.size[1].Y = 3; 

    // ------------------------------ 

    T_BLOCK E; 
    allocMemory(&E,4); 

    // ■ 
    // ■■■ 
    set(&E.coord[0][0],0,0); 
    set(&E.coord[0][1],1,0); 
    set(&E.coord[0][2],2,0); 
    set(&E.coord[0][3],0,-1); 
    E.size[0].X = 3; 
    E.size[0].Y = 2; 

    // ■■ 
    // ■ 
    // ■ 
    set(&E.coord[1][0],0,0); 
    set(&E.coord[1][1],0,-1); 
    set(&E.coord[1][2],0,-2); 
    set(&E.coord[1][3],1,-2); 
    E.size[1].X = 2; 
    E.size[1].Y = 3; 

    // ■■■ 
    //     ■ 
    set(&E.coord[2][0],2,0); 
    set(&E.coord[2][1],0,-1); 
    set(&E.coord[2][2],1,-1); 
    set(&E.coord[2][3],2,-1); 
    E.size[2].X = 3; 
    E.size[2].Y = 2; 

    //   ■  
    //   ■ 
    // ■■ 
    set(&E.coord[3][0],0,0); 
    set(&E.coord[3][1],1,0); 
    set(&E.coord[3][2],1,-1); 
    set(&E.coord[3][3],1,-2); 
    E.size[3].X = 2; 
    E.size[3].Y = 3; 

    // ---------------------------------- 

    T_BLOCK F; 
    allocMemory(&F,4); 

    //     ■ 
    // ■■■ 
    set(&F.coord[0][0],0,0); 
    set(&F.coord[0][1],1,0); 
    set(&F.coord[0][2],2,0); 
    set(&F.coord[0][3],2,-1); 
    F.size[0].X = 3; 
    F.size[0].Y = 2; 

    // ■ 
    // ■ 
    // ■■ 
    set(&F.coord[1][0],0,0); 
    set(&F.coord[1][1],1,0); 
    set(&F.coord[1][2],0,-1); 
    set(&F.coord[1][3],0,-2); 
    F.size[1].X = 2; 
    F.size[1].Y = 3; 

    // ■■■ 
    // ■ 
    set(&F.coord[2][0],0,0); 
    set(&F.coord[2][1],0,-1); 
    set(&F.coord[2][2],1,-1); 
    set(&F.coord[2][3],2,-1); 
    F.size[2].X = 3; 
    F.size[2].Y = 2; 

    // ■■ 
    //   ■ 
    //   ■ 
    set(&F.coord[3][0],1,0); 
    set(&F.coord[3][1],1,-1); 
    set(&F.coord[3][2],0,-2); 
    set(&F.coord[3][3],1,-2); 
    F.size[3].X = 2; 
    F.size[3].Y = 3; 

    //---------------------------------- 
    T_BLOCK G; 
    allocMemory(&G,4); 

    //   ■ 
    // ■■■ 
    set(&G.coord[0][0],0,0); 
    set(&G.coord[0][1],1,0); 
    set(&G.coord[0][2],2,0); 
    set(&G.coord[0][3],1,-1); 
    G.size[0].X = 3; 
    G.size[0].Y = 2; 

    // ■ 
    // ■■ 
    // ■ 
    set(&G.coord[1][0],0,0); 
    set(&G.coord[1][1],0,-1); 
    set(&G.coord[1][2],1,-1); 
    set(&G.coord[1][3],0,-2); 
    G.size[1].X = 2; 
    G.size[1].Y = 3; 

    // ■■■ 
    //   ■ 
    set(&G.coord[2][0],1,0); 
    set(&G.coord[2][1],0,-1); 
    set(&G.coord[2][2],1,-1); 
    set(&G.coord[2][3],2,-1); 
    G.size[2].X = 3; 
    G.size[2].Y = 2; 

    //   ■ 
    // ■■ 
    //   ■ 
    set(&G.coord[3][0],1,0); 
    set(&G.coord[3][1],0,-1); 
    set(&G.coord[3][2],1,-1); 
    set(&G.coord[3][3],1,-2); 
    G.size[3].X = 2; 
    G.size[3].Y = 3; 

    ///////////////////////////////// 

    Blocks[0] = A; 
    Blocks[1] = B; 
    Blocks[2] = C; 
    Blocks[3] = D; 
    Blocks[4] = E; 
    Blocks[5] = F; 
    Blocks[6] = G; 

    srand((unsigned)time(NULL)); // 난수 발생 
} 

block::~block(void) 
{ 
    for(int i = 0 ; i < BLOCK_SHAPE_CNT ;i++ ){ 
        COORD **s = Blocks[i].coord; 
        for(int j= 0 ; j < Blocks[i].rotate ; j++ ){ 
            delete [] s[j];  
        } 
        delete[] s;  delete[] Blocks[i].size;​​​​
    } 
} 

void block::allocMemory(T_BLOCK *block, int size){ 
    block->rotate = size; 
    block->coord = new COORD*[size]; 
    block->size = new COORD[size]; 
    for(int i = 0 ; i < size ; i ++){ 
        block->coord[i] = new COORD[BLOCK_PIXEL_CNT]; 
    } 
} 

int block::getRotation(){ 
    return gCurRotation; 
} 

int block::getShape(){ 
    return gCurShape; 
} 

void block::random(){ 
    //gCurShape = 0; 
    //gCurRotation = 1; 
    gCurShape = rand()% BLOCK_SHAPE_CNT; 
    gCurRotation = rand()% Blocks[gCurShape].rotate; 
} 

COORD block::getSize(){ 
    return Blocks[gCurShape].size[gCurRotation]; 
} 

COORD *block::getCoords(){ 
    return Blocks[gCurShape].coord[gCurRotation]; 
} 

void block::rotation(int ckwise){ 
    switch(ckwise){ 
    case CW: 
        if(gCurRotation == Blocks[gCurShape].rotate-1){ 
            gCurRotation = 0; 
        }else{ 
            gCurRotation++; 
        } 
        break; 
    case CCW: 
        if(gCurRotation == 0){ 
            gCurRotation = Blocks[gCurShape].rotate-1; 
        }else{ 
            gCurRotation--; 
        } 
        break; 
    } 
     
} 

void block::set(int shape, int rotation){ 
    gCurRotation = rotation; 
    gCurShape = shape; 
} 

COORD *block::getCoords(int rotation){ 
    return Blocks[gCurShape].coord[rotation]; 
} 

COORD *block::getCoords(int shape, int rotation){ 
    return Blocks[shape].coord[rotation]; 
}