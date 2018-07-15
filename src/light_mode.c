#include "light_mode.h"


//----------------------------------------------------------------------------------
// Varable Definitions


//----------------------------------------------------------------------------------
// Function Declarations
void add_back(uint8_t r, uint8_t g, uint8_t b);
void add_point(uint8_t r, uint8_t g, uint8_t b, int pointSize, int pointPosition);


void LIGHT_MODE_set_index(int32_t index)
{
    G_ModeIndex = index;
    STATE_CTRL_set_state(STATE_LIGHT_MODE_INDICATE);
}

int32_t LIGHT_MODE_get_index(void)
{
    return G_ModeIndex;
}

void LIGHT_MODE_index_prev(void)
{
    if(G_ModeIndex == 0){
        for(uint8_t i=15; i>0; i--){
            if(G_pModeTable->table[i] != (uint8_t)0xFF){
                G_ModeIndex = i;
                break;
            }
        }
    }else{
        G_ModeIndex --;
    }
    
    STATE_CTRL_set_state(STATE_LIGHT_MODE_INDICATE);
}

void LIGHT_MODE_index_next(void)
{
    if(G_ModeIndex == 15){
        G_ModeIndex = 0;
    }else{
        G_ModeIndex ++;
        if(G_pModeTable->table[G_ModeIndex] == 0xFF){
            G_ModeIndex = 0;
        }
    }
    
    STATE_CTRL_set_state(STATE_LIGHT_MODE_INDICATE);
}

void LIGHT_MODE_loop(uint8_t isNeedInit)
{
    switch(G_pModeTable->table[G_ModeIndex]){
        case LM_STATIC_LIGHT_0:
            if(isNeedInit){
                DISP_add_back(G_pStaticLight->color[0].R, G_pStaticLight->color[0].G, G_pStaticLight->color[0].B);
                DISP_update();
            }
            break;

        case LM_STATIC_LIGHT_1:
            if(isNeedInit){
                DISP_add_back(G_pStaticLight->color[1].R, G_pStaticLight->color[1].G, G_pStaticLight->color[1].B);
                DISP_update();
            }
            break;

        case LM_STATIC_LIGHT_2:
            if(isNeedInit){
                DISP_add_back(G_pStaticLight->color[2].R, G_pStaticLight->color[2].G, G_pStaticLight->color[2].B);
                DISP_update();
            }
            break;

        case LM_STATIC_LIGHT_3:
            if(isNeedInit){
                DISP_add_back(G_pStaticLight->color[3].R, G_pStaticLight->color[3].G, G_pStaticLight->color[3].B);
                DISP_update();
            }
            break;

        case LM_STATIC_LIGHT_4:
            if(isNeedInit){
                DISP_add_back(G_pStaticLight->color[4].R, G_pStaticLight->color[4].G, G_pStaticLight->color[4].B);
                DISP_update();
            }
            break;

        case LM_STATIC_LIGHT_5:
            if(isNeedInit){
                DISP_add_back(G_pStaticLight->color[5].R, G_pStaticLight->color[5].G, G_pStaticLight->color[5].B);
                DISP_update();
            }
            break;

        case LM_IMG_DISP_0:
            IMG_DISP_loop(G_pIMG0);
            break;

        case LM_IMG_DISP_1:
            IMG_DISP_loop(G_pIMG1);
            break;

        case LM_IMG_DISP_2:
            IMG_DISP_loop(G_pIMG2);
            break;

        case LM_IMG_DISP_3:
            IMG_DISP_loop(G_pIMG3);
            break;

        case LM_IMG_DISP_4:
            IMG_DISP_loop(G_pIMG4);
            break;

        case LM_IMG_DISP_5:
            IMG_DISP_loop(G_pIMG5);
            break;

        case LM_INTERACT_0:
            if (isNeedInit){
                INTERACT_DISP_reset();   
            }else{
                INTERACT_DISP_0();
            }
            break;

        case LM_INTERACT_1:
            if (isNeedInit){
                INTERACT_DISP_reset();   
            }else{
                INTERACT_DISP_1();
            }
            break;

        case LM_DYNAMIC_0:
            DYNAMIC_DISP_0(isNeedInit);
            break;

        case LM_DYNAMIC_1:
            DYNAMIC_DISP_1(isNeedInit);
            break;

        case LM_DYNAMIC_2:
            DYNAMIC_DISP_2(isNeedInit);
            break;

        case LM_DYNAMIC_3:
            DYNAMIC_DISP_3(isNeedInit);
            break;

        case LM_DYNAMIC_4:
            DYNAMIC_DISP_4(isNeedInit);
            break;

        case LM_BIKE_0:
            BIKE_DISP_0(isNeedInit);
            break;

        default:
            break;
    }
}
