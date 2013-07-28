#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_gpio.h"
#include "sysctl.h"
#include "hw_flash.h"
#include "hw_ints.h"
#include "timer.h"
#include "interrupt.h"
#include "gpio.h"

#include "main.h"
#include "touch.h"
#include "LTM024D130.h"
#include "ff.h"
#include "Bejeweled.h"

#define colornum 7
#define BGcolor BLACK

// define boundaries of these drawing rectangles
#define reorder_left    20
#define reorder_top     20
#define reorder_right   99
#define reorder_bottom  49

#define restart_left   140
#define restart_top     20
#define restart_right  219
#define restart_bottom  49

#define score_left      20
#define score_top       60
#define score_right    219
#define score_bottom    79

#define step            10


/*****************************************************/
// These are some basic functions to be used by other functions.


uint8 u8max(uint8 a, uint8 b)
{
    return((a > b) ? a : b);
}

uint8 u8min(uint8 a, uint8 b)
{
    return((a < b) ? a : b);
}

uint8 u8pow_mul(uint8 a, uint8 b, uint8 res)
{
  return((b > 1) ? u8pow_mul(a, b-1, res*a) : res);
}

uint8 u8pow(uint8 a, uint8 b)
{
    return((b == 0) ? 1 : u8pow_mul(a, b, a));
}

sint16 s16max(sint16 a, sint16 b)
{
    return((a > b) ? a : b);
}

sint16 s16min(sint16 a, sint16 b)
{
    return((a < b) ? a : b);
}

sint16 ClippX(sint16 pX)
{
    return(s16min(s16max(pX, 0), pXr-1));
}

sint16 ClippY(sint16 pY)
{
    return(s16min(s16max(pY, 0), pYr-1));
}

sint32 s32RoundDiv(sint32 a, sint32 b)
{
    sint32 c = a / b;
    sint32 d = a % b;
    
    return((d < b/2) ? c : c+1);
}

uint8 IsTouch(uint16 tX, uint16 tY)
{
    return((tX>=Xbl && tX<=Xul) && (tY>=Ybl && tY<=Yul) ? 1 : 0);
}

sint16 GetpX(uint16 tX, uint16 tY, uint8 isT)
{
    if(isT)
    {
        return(ClippX( (sint16)s32RoundDiv( ((sint32)tX-(blX+tlX)/2)*pXr, ((brX+trX)-(blX+tlX))/2 ) ));
    }
    else
    {
        return(pXr);
    }
}

sint16 GetpY(uint16 tX, uint16 tY, uint8 isT)
{
    if(isT)
    {
        return(ClippY( pYr - (sint16)s32RoundDiv( ((sint32)tY-blY)*pYr, (tlY-blY) ) ));
    }
    else
    {
        return(pYr);
    }
}

uint8 GetpXY(sint16 *pX, sint16 *pY)
{
    uint16 tX = ads7843_get_Y();
    uint16 tY = ads7843_get_X();
    uint8 isT = IsTouch(tX, tY);
        
    *pX = GetpX(tX, tY, isT);
    *pY = GetpY(tX, tY, isT);
    
    return(isT);
}


uint8 pX2BjH(sint16 pX)
{
    return((uint8)(pX/30));
}


uint8 pY2BjV(sint16 pY)
{
    return ((uint8)((pY-80)/30));
}


/*****************************************************/
// Boot up screen.


void Bootup(unsigned int *count_seed)
{
    sint16 pX = 240, pY = 320;
	char crystalstr[7];
	uint8 rnum;
    
        
	picture_Show("a1.ebm", 0, 0, 239, 319);
        
    while(!GetpXY(&pX, &pY))
	{
	    (*count_seed)++;
	}
	if   ( GetpXY(&pX, &pY))
	{
	    __screenFill(BGcolor);
	}
    while( GetpXY(&pX, &pY))
	{
        if(rnum>=10)
        {
            sprintf(crystalstr, "c%2d.ebm", rnum);
        }
        else
        {
            sprintf(crystalstr, "c0%1d.ebm", rnum);
        }
	    
	    picture_Show(crystalstr, 0, 40, 239, 279);
            
        rnum = (rnum+1)%40;
		
	    (*count_seed)++;
	}
	picture_Show("a2.ebm", 0, 0, 239, 319);
        
	delayMs(1000);
}


/*****************************************************/
// These are the functions deliver random numbers for other functions.


uint8 Bj_1random7(void)
{
    return(rand()%7 + 1);
}


uint8 Bj_0random7(void)
{
    return(rand()%8);
}


uint8 Bj_0random19(void)
{
    return(rand()%20);
}


void Bj_orandom_p1(uint8 onum, uint8 *ran)
{
    *ran = rand()%onum;
}


void Bj_orandom_p2(uint8 o[64], uint8 *onum, uint8 ran)
{
    (*onum)--;
    
    for(uint8 i=ran; i<(*onum); i++)
    {
        o[i] = o[i+1];
    }
}


/*****************************************************/
// These are the functions "Rearrangement" that rearrange the gems in current gem box without 3 or above same gems in a line.


uint8 Bj_reorder_availableH(uint8 Bj[8][8], uint8 ci, uint8 cj, uint8 n)
{
    uint8 valid = 1;
	
    if(cj<2)
    {
        valid = 1;
    }
    else if(n == Bj[ci][cj-1] && n == Bj[ci][cj-2])
    {
        valid = 0;
    }
    
	return(valid);
}


uint8 Bj_reorder_availableV(uint8 Bj[8][8], uint8 ci, uint8 cj, uint8 n)
{
    uint8 valid = 1;
	
    if(ci<2)
    {
        valid = 1;
    }
    else if(n == Bj[ci-1][cj] && n == Bj[ci-2][cj])
    {
        valid = 0;
    }
    
	return(valid);
}


uint8 Bj_reorder_available(uint8 Bj[8][8], uint8 ci, uint8 cj, uint8 n)
{
    uint8 valid = 1;
	
    valid = u8min(u8min(valid, Bj_reorder_availableH(Bj, ci, cj, n)), Bj_reorder_availableV(Bj, ci, cj, n));
	
	return(valid);
}


void Bj_reorder(uint8 Bj[8][8])
{
    uint8 temp1[64], temp2[8][8];
    uint8 c = 0, ci, cj;
    uint8 onum, ran = 0;
    
    while(c < 65)
    {
        c = 0, ci = 0, cj = 0;
        onum = 64;
        
        memcpy(temp1, Bj, 64*sizeof(uint8));
    
        while(c < 64)
        {
            
            Bj_orandom_p1(onum, &ran);
            
            if(Bj_reorder_available(temp2, ci, cj, temp1[ran]))
            {
                temp2[ci][cj] = temp1[ran];
                Bj_orandom_p2(temp1, &onum, ran);
                
                if(c == 63)
                {
                    c = 65;
                }
                else
                {
                    c++;
                    ci = c / 8;
                    cj = c % 8;
                }
            }
            else if(c == 63)
            {
                c = 64;
            }
        }
    }
    
    memcpy(Bj, temp2, 64*sizeof(uint8));
}


/*****************************************************/
// These are the functions "Display" controll what to be displayed on LCD in specific conditions.


void Bj_display_reorder(uint32 reorder)
{
    char reorderstr[5];
    
    sprintf(reorderstr, "%5d", reorder);
    RECTANGLE reorderRect = {reorder_left, reorder_top, reorder_right, reorder_bottom, 0, 0};
    
    rectFill((RECTANGLE *)&reorderRect, GREEN);
    writeString((uint8*)reorderstr, reorder_left+10, reorder_top+10, RED);
}


void Bj_display_restart()
{
    char restartstr[7];
    
    sprintf(restartstr, "Restart");
    RECTANGLE restartRect = {restart_left, restart_top, restart_right, restart_bottom, 0, 0};
    
    rectFill((RECTANGLE *)&restartRect, BLUE);
    writeString((uint8*)restartstr, restart_left+10, restart_top+10, YELLOW);
}


void Bj_display_score(uint32 score, uint32 level)
{
    char scorestr[28];
    
    sprintf(scorestr, "Score:%8d  Level:%3d", score, level);
    RECTANGLE scoreRect = {score_left, score_top, score_right, score_bottom, 0, 0};
    
    rectFill((RECTANGLE *)&scoreRect, BGcolor);
    writeString((uint8*)scorestr, score_left, score_top, YELLOW);
}


uint16 Bj_display_getcolor(uint8 Bjnum)
{
    return(Bjnum==0 ? BGcolor : Bjnum==1 ? RED : Bjnum==2 ? GREEN : Bjnum==3 ? BLUE : Bjnum==4 ? CYAN : Bjnum==5 ? MAGENTA : Bjnum==6 ? YELLOW : LGRAY);
}


void Bj_display_getgemstr(char gemstr[7], uint8 Bjnum, uint8 rnum)
{
    rnum = rnum%20;
    
    if(rnum>=10)
    {
        switch(Bjnum)
        {
            case 1: sprintf(gemstr, "r%2d.ebm", rnum); break;
            case 2: sprintf(gemstr, "g%2d.ebm", rnum); break;
            case 3: sprintf(gemstr, "b%2d.ebm", rnum); break;
            case 4: sprintf(gemstr, "o%2d.ebm", rnum); break;
            case 5: sprintf(gemstr, "p%2d.ebm", rnum); break;
            case 6: sprintf(gemstr, "y%2d.ebm", rnum); break;
            case 7: sprintf(gemstr, "w%2d.ebm", rnum); break;
            default: sprintf(gemstr, "r%2d.ebm", rnum);
        }
    }
    else
    {
        switch(Bjnum)
        {
            case 1: sprintf(gemstr, "r0%1d.ebm", rnum); break;
            case 2: sprintf(gemstr, "g0%1d.ebm", rnum); break;
            case 3: sprintf(gemstr, "b0%1d.ebm", rnum); break;
            case 4: sprintf(gemstr, "o0%1d.ebm", rnum); break;
            case 5: sprintf(gemstr, "p0%1d.ebm", rnum); break;
            case 6: sprintf(gemstr, "y0%1d.ebm", rnum); break;
            case 7: sprintf(gemstr, "w0%1d.ebm", rnum); break;
            default: sprintf(gemstr, "r0%1d.ebm", rnum);
        }
    }
}


void Bj_display_part(uint8 Bj[8][8], uint8 r[8][8], uint8 H, uint8 V)
{
	if(Bj[V][H] != 0)
	{
        char gemstr[7];
		
	    Bj_display_getgemstr(gemstr, Bj[V][H], r[V][H]);
        
	    picture_Show(gemstr, H*30, V*30+80, H*30+29, V*30+80+29); 
	}
	else
	{
	    RECTANGLE BjRect = {H*30, V*30+80, H*30+29, V*30+80+29, 0, 0};
            
        rectFill((RECTANGLE *)&BjRect, BGcolor);
	}
}


void Bj_display_part_free(uint8 Bjnum, uint8 rnum, uint16 Hmin, uint16 Vmin, uint16 Hmax, uint16 Vmax)
{
    if(Bjnum != 0)
	{
        char gemstr[7];
	    
	    Bj_display_getgemstr(gemstr, Bjnum, rnum);
        
        picture_Show(gemstr, Hmin, Vmin, Hmax, Vmax);
	}
	else
	{
	    RECTANGLE BjRect = {Hmin, Vmin, Hmax, Vmax, 0, 0};
            
        rectFill((RECTANGLE *)&BjRect, BGcolor);
	}
}


void Bj_display_highlight(uint8 Bj[8][8], uint8 r[8][8], uint8 H, uint8 V)
{
    r[V][H] = (r[V][H]+1)%20;
	
    Bj_display_part(Bj, r, H, V);
}


void Bj_display_main(uint8 Bj[8][8], uint8 r[8][8])
{
    for(uint8 i=0; i<8; i++)
    {
        for(uint8 j=0; j<8; j++)
        {
            Bj_display_part(Bj, r, j, i);
            //Bj_display_highlight(Bj, r, j, i);
        }
    }
}


void Bj_display(uint8 Bj[8][8], uint8 r[8][8], uint32 score, uint32 reorder, uint32 level)
{
    __screenFill(BGcolor);
    
    Bj_display_reorder(reorder);
    Bj_display_restart();
    Bj_display_score(score, level);
    Bj_display_main(Bj, r);
}


void Bj_display_exchange(uint8 Bj[8][8], uint8 r[8][8], uint8 aH, uint8 aV, uint8 bH, uint8 bV, uint8 inverse)
{
    uint8 i;
    uint16 aBjnum = inverse ? Bj[bV][bH] : Bj[aV][aH];
    uint16 bBjnum = inverse ? Bj[aV][aH] : Bj[bV][bH];
    uint8  *arnum;
    uint8  *brnum;
	        arnum = inverse ? &r[bV][bH] : &r[aV][aH];
	        brnum = inverse ? &r[aV][aH] : &r[bV][bH];
	
    if(aV == bV)
    {
        for(i=step; i<=30; i=i+step)
        {
            *arnum = (bH>aH) ? (*arnum+1)%20 : (*arnum-1)%20;
            *brnum = (bH>aH) ? (*brnum+1)%20 : (*brnum-1)%20;
            
            Bj_display_part_free(bBjnum, *brnum, bH*30-i*(bH-aH), bV*30+80, bH*30+29-i*(bH-aH), bV*30+80+29);
            Bj_display_part_free(aBjnum, *arnum, aH*30+i*(bH-aH), aV*30+80, aH*30+29+i*(bH-aH), aV*30+80+29);
			
		    if(i<=15)
			{
                RECTANGLE bkRect1 = {u8min(aH, bH)*30+i -step, aV*30+80, u8min(aH, bH)*30+i -1     , aV*30+80+29, 0, 0};
                rectFill((RECTANGLE *)&bkRect1, BGcolor);
                RECTANGLE bkRect2 = {u8max(aH, bH)*30+30-i   , bV*30+80, u8max(aH, bH)*30+29-i+step, bV*30+80+29, 0, 0};
                rectFill((RECTANGLE *)&bkRect2, BGcolor);
			}
			
			//delayMs(10);
        }
    }
    else
    {
        for(i=step; i<=30; i=i+step)
        {
            *arnum = (bV>aV) ? (*arnum+1)%20 : (*arnum-1)%20;
            *brnum = (bV>aV) ? (*brnum+1)%20 : (*brnum-1)%20;
			
		    Bj_display_part_free(bBjnum, *brnum, bH*30, bV*30+80-i*(bV-aV), bH*30+29, bV*30+80+29-i*(bV-aV));
			Bj_display_part_free(aBjnum, *arnum, aH*30, aV*30+80+i*(bV-aV), aH*30+29, aV*30+80+29+i*(bV-aV));
			
		    if(i<=15)
			{
                RECTANGLE bkRect1 = {aH*30, u8min(aV, bV)*30+80+i -step, aH*30+29, u8min(aV, bV)*30+80+i -1     , 0, 0};
                rectFill((RECTANGLE *)&bkRect1, BGcolor);
                RECTANGLE bkRect2 = {bH*30, u8max(aV, bV)*30+80+30-i   , bH*30+29, u8max(aV, bV)*30+80+29-i+step, 0, 0};
                rectFill((RECTANGLE *)&bkRect2, BGcolor);
            }
			
			//delayMs(10);
        }
    }
}


/*****************************************************/
// Score and level calculation functions.


void Bj_score(uint32 *score, uint32 level, uint32 multihit, uint8 count)
{
    *score = *score + (5+5*level) * (uint32)u8pow(2, (count-3)) * (multihit);
}


uint32 Bj_level_cal(uint32 level, uint32 res)
{
    res = res + level * 500;
    return((level==1) ? res : Bj_level_cal(level-1, res));
}


void Bj_level(uint32 score, uint32 *level, uint32 *reorder)
{
	if(score >= Bj_level_cal(*level, 0))
	{
	    (*level)++;
		(*reorder)++;
	}
}


/*****************************************************/
// Movement in touch screen results in exchanging two adjacent gems.
// These functions "Movement" check whether there is available match around moved gems, then determine if the movement is permitted.


uint8 Bj_moveH(uint8 Bj[8][8], uint8 r[8][8], uint8 aH, uint8 aV, uint8 bH, uint8 bV, uint32 *score, uint32 *level, uint32 *multihit)
{
    // aV == bV
    // aH  < bH
    uint8 avalid = 0, bvalid = 0, count = 1;
	uint8 i;
	
	
    if(aH >= 2)
    {
        count = 1;
        for(i=1; (i<=  aH && Bj[bV][bH]==Bj[aV][aH-i]); i++)
    	{
    	    count++;
    	}
    	if(count >= 3)
    	{
            for(i=1; i<count; i++)
    	    {
                Bj[aV][aH-i] = 0;
				
				Bj_display_part(Bj, r, aH-i, aV);
    	    }

            (*multihit)++;
            Bj_score(score, *level, *multihit, count);
            avalid = 1;
    	}
    }
    
    count = 1;
    for(i=1; (i<=  aV && Bj[bV][bH]==Bj[aV-i][aH]); i++)
    {
        count++;
    }
    for(i=1; (i<=7-aV && Bj[bV][bH]==Bj[aV+i][aH]); i++)
    {
        count++;
    }
    if(count >= 3)
    {
        for(i=1; (i<=  aV && Bj[bV][bH]==Bj[aV-i][aH]); i++)
        {
            Bj[aV-i][aH] = 0;
				
			Bj_display_part(Bj, r, aH, aV-i);
        }
        for(i=1; (i<=7-aV && Bj[bV][bH]==Bj[aV+i][aH]); i++)
        {
            Bj[aV+i][aH] = 0;
				
			Bj_display_part(Bj, r, aH, aV+i);
        }
    
        (*multihit)++;
        Bj_score(score, *level, *multihit, count);
        avalid = 1;
    }
    
    
    if(bH <= 5)
    {
        count = 1;
        for(i=1; (i<=7-bH && Bj[aV][aH]==Bj[bV][bH+i]); i++)
    	{
    	    count++;
    	}
    	if(count >= 3)
    	{
            for(i=1; i<count; i++)
    	    {
                Bj[bV][bH+i] = 0;
				
			    Bj_display_part(Bj, r, bH+i, bV);
    	    }

            (*multihit)++;
            Bj_score(score, *level, *multihit, count);
            bvalid = 1;
    	}
    }
    
    count = 1;
    for(i=1; (i<=  bV && Bj[aV][aH]==Bj[bV-i][bH]); i++)
    {
        count++;
    }
    for(i=1; (i<=7-bV && Bj[aV][aH]==Bj[bV+i][bH]); i++)
    {
        count++;
    }
    if(count >= 3)
    {
        for(i=1; (i<=  bV && Bj[aV][aH]==Bj[bV-i][bH]); i++)
        {
            Bj[bV-i][bH] = 0;
				
			Bj_display_part(Bj, r, bH, bV-i);
        }
        for(i=1; (i<=7-bV && Bj[aV][aH]==Bj[bV+i][bH]); i++)
        {
            Bj[bV+i][bH] = 0;
				
			Bj_display_part(Bj, r, bH, bV+i);
        }
    
        (*multihit)++;
        Bj_score(score, *level, *multihit, count);
        bvalid = 1;
    }
    
    
    if(avalid)
	{
	    if(bvalid)
		{
			Bj[bV][bH] = 0;
		}
		else
		{
		    Bj[bV][bH] = Bj[aV][aH];
			 r[bV][bH] =  r[aV][aH];
		}
        Bj[aV][aH] = 0;
		
	    Bj_display_part(Bj, r, aH, aV);
	    Bj_display_part(Bj, r, bH, bV);
	}
	else
	{
	    if(bvalid)
		{
		    Bj[aV][aH] = Bj[bV][bH];
			 r[aV][aH] =  r[bV][bH];
            Bj[bV][bH] = 0;
		    
			Bj_display_part(Bj, r, aH, aV);
			Bj_display_part(Bj, r, bH, bV);
		}
	}
    
    
    return(u8max(avalid, bvalid));
}


uint8 Bj_moveV(uint8 Bj[8][8], uint8 r[8][8], uint8 aH, uint8 aV, uint8 bH, uint8 bV, uint32 *score, uint32 *level, uint32 *multihit)
{
    // aH == bH
    // aV  < bV
    uint8 avalid = 0, bvalid = 0, count = 1;
	uint8 i;
	
	
    if(aV >= 2)
    {
        count = 1;
        for(i=1; (i<=  aV && Bj[bV][bH]==Bj[aV-i][aH]); i++)
    	{
    	    count++;
    	}
    	if(count >= 3)
    	{
            for(i=1; i<count; i++)
    	    {
                Bj[aV-i][aH] = 0;
				
				Bj_display_part(Bj, r, aH, aV-i);
    	    }

            (*multihit)++;
            Bj_score(score, *level, *multihit, count);
            avalid = 1;
    	}
    }
    
    count = 1;
    for(i=1; (i<=  aH && Bj[bV][bH]==Bj[aV][aH-i]); i++)
    {
        count++;
    }
    for(i=1; (i<=7-aH && Bj[bV][bH]==Bj[aV][aH+i]); i++)
    {
        count++;
    }
    if(count >= 3)
    {
        for(i=1; (i<=  aH && Bj[bV][bH]==Bj[aV][aH-i]); i++)
        {
            Bj[aV][aH-i] = 0;
				
			Bj_display_part(Bj, r, aH-i, aV);
        }
        for(i=1; (i<=7-aH && Bj[bV][bH]==Bj[aV][aH+i]); i++)
        {
            Bj[aV][aH+i] = 0;
				
			Bj_display_part(Bj, r, aH+i, aV);
        }
    
        (*multihit)++;
        Bj_score(score, *level, *multihit, count);
        avalid = 1;
    }
    
    
    if(bV <= 5)
    {
        count = 1;
        for(i=1; (i<=7-bV && Bj[aV][aH]==Bj[bV+i][bH]); i++)
    	{
    	    count++;
    	}
    	if(count >= 3)
    	{
            for(i=1; i<count; i++)
    	    {
                Bj[bV+i][bH] = 0;
				
			    Bj_display_part(Bj, r, bH, bV+i);
    	    }

            (*multihit)++;
            Bj_score(score, *level, *multihit, count);
            bvalid = 1;
    	}
    }
    
    count = 1;
    for(i=1; (i<=  bH && Bj[aV][aH]==Bj[bV][bH-i]); i++)
    {
        count++;
    }
    for(i=1; (i<=7-bH && Bj[aV][aH]==Bj[bV][bH+i]); i++)
    {
        count++;
    }
    if(count >= 3)
    {
        for(i=1; (i<=  bH && Bj[aV][aH]==Bj[bV][bH-i]); i++)
        {
            Bj[bV][bH-i] = 0;
				
			Bj_display_part(Bj, r, bH-i, bV);
        }
        for(i=1; (i<=7-bH && Bj[aV][aH]==Bj[bV][bH+i]); i++)
        {
            Bj[bV][bH+i] = 0;
				
			Bj_display_part(Bj, r, bH+i, bV);
        }
    
        (*multihit)++;
        Bj_score(score, *level, *multihit, count);
        bvalid = 1;
    }
    
    
    if(avalid)
	{
	    if(bvalid)
		{
			Bj[bV][bH] = 0;
		}
		else
		{
		    Bj[bV][bH] = Bj[aV][aH];
			 r[bV][bH] =  r[aV][aH];
		}
        Bj[aV][aH] = 0;
		
	    Bj_display_part(Bj, r, aH, aV);
	    Bj_display_part(Bj, r, bH, bV);
	}
	else
	{
	    if(bvalid)
		{
		    Bj[aV][aH] = Bj[bV][bH];
			 r[aV][aH] =  r[bV][bH];
            Bj[bV][bH] = 0;
		    
			Bj_display_part(Bj, r, aH, aV);
			Bj_display_part(Bj, r, bH, bV);
		}
	}
	
    
    return(u8max(avalid, bvalid));
}


uint8 Bj_move(uint8 Bj[8][8], uint8 r[8][8], uint8 aH, uint8 aV, uint8 bH, uint8 bV, uint32 *score, uint32 *level, uint32 *multihit)
{
    if(Bj[aV][aH] == Bj[bV][bH])
	{
	    return(0);
	}
	else if(aV == bV)
	{
	    if(aH < bH)
		{
		    return(Bj_moveH(Bj, r, aH, aV, bH, bV, score, level, multihit));
		}
		else
		{
		    return(Bj_moveH(Bj, r, bH, bV, aH, aV, score, level, multihit));
		}
	}
	else if(aH == bH)
	{
	    if(aV < bV)
		{
		    return(Bj_moveV(Bj, r, aH, aV, bH, bV, score, level, multihit));
		}
		else
		{
		    return(Bj_moveV(Bj, r, bH, bV, aH, aV, score, level, multihit));
		}
	}
	
    return(0);
}


/*****************************************************/
// After matching gems, these functions "Dropping" will fill those blank spaces in gem box through generating new gems and dropping down the gems in the air.


void Bj_drop_column(uint8 Bj[8][8], uint8 r[8][8], uint8 V, uint8 H, uint8 count)
{
    sint16 i;
	sint8 j;
	
    if(V > count-1)
	{
        for(j=V-count; j>=0; j--)
        {
            for(i=step; i<=count*30; i=i+step)
            {
                r[j][H] = (r[j][H]+1)%20;
                Bj_display_part_free(Bj[j][H], r[j][H], H*30, j*30+80+i, H*30+29, j*30+80+i+29);
                
                RECTANGLE bkRect = {H*30, j*30+80+i-step, H*30+29, j*30+80+i-1, 0, 0};
            	if(i > 1)
            	{
            	    rectFill((RECTANGLE *)&bkRect, BGcolor);
                }
            }
        }
	    
	    for(i=V; i>=0; i--)
	    {
	        if(i-count >= 0)
	        {
	            Bj[i][H] = Bj[i-count][H];
				 r[i][H] =  r[i-count][H];
	        }
	        else
	        {
	            Bj[i][H] = 0;
	        }
	    }
	}
	else
	{
	    for(i=V; i>=0; i--)
		{
		    Bj[i][H] = Bj_1random7();
			 r[i][H] = Bj_0random19();
		}
		
		sint16 top = -count*30, VB;
		
        for(j=count-1; j>=0; j--)
        {
		    for(i=step; i<=80-top; i=i+step)
		    {
                VB  = top+j*30+i+29;
                if(VB >= 0)
                {
                    r[j][H] = (r[j][H]+1)%20;
                    Bj_display_part_free(Bj[j][H], r[j][H], H*30, (uint16)s16max(VB-29, 0), H*30+29, (uint16)VB);
                }
		    	if(s16max(VB-29-1, 0) >= 0)
		    	{
		            RECTANGLE bkRect = {H*30, (uint16)s16max(VB-29-step, 0), H*30+29, (uint16)s16max(VB-29-1, 0), 0, 0};
		    	    rectFill((RECTANGLE *)&bkRect, BGcolor);
		        }
		    }
        }
	}
}


uint8 Bj_drop(uint8 Bj[8][8], uint8 r[8][8])
{
    uint8 enable = 0, count, bottom, flag;
	
    for(uint8 j=0; j<8; j++)
	{
	    count = 0;
		flag = 0;
		
	    for(sint8 i=7; (i>=0 && flag<2); i--)
		{
		    if(Bj[i][j] == 0)
			{
			    count++;
				flag = 1;
            }
			else if(flag == 1)
			{
			    flag = 2;
			}
			if(count == 1 && flag == 1)
			{
			    bottom = i;
			}
		}
		if(count > 0)
		{
            Bj_drop_column(Bj, r, bottom, j, count);
			
            enable = 1;
		}
	}

    return(enable);
}


/*****************************************************/
// After dropping gems, this function "Checking" will check whether there is available match in the whole gem box.
// If there is available match, the function "Dropping" will be called again by function "Action".


void Bj_check(uint8 Bj[8][8], uint8 r[8][8], uint32 *score, uint32 *level, uint32 *multihit)
{
    uint8 V, H, i;
	uint8 count;
	uint8 temp[8][8];
	
	memcpy(temp, Bj, 64*sizeof(uint8));
	
	for(V=0; V<=7; V++)
	{
	    for(H=0; H<=5; H++)
		{
		    if(temp[V][H] != 0)
			{
			    count = 1;
				
				for(i=H+1; (i<=7 && Bj[V][H]==Bj[V][i]); i++)
				{
				    count++;
				}
				
				if(count >= 3)
				{
				    for(i=H; i<H+count; i++)
					{
					    temp[V][i] = 0;
					       r[V][i] = 0;
					}
					
					(*multihit)++;
			        Bj_score(score, *level, *multihit, count);
				}
			}
		}
	}
	
	for(H=0; H<=7; H++)
	{
	    for(V=0; V<=5; V++)
		{
		    if(Bj[V][H] != 0)
			{
			    count = 1;
				
				for(i=V+1; (i<=7 && Bj[V][H]==Bj[i][H]); i++)
				{
				    count++;
				}
				
				if(count >= 3)
				{
				    for(i=V; i<V+count; i++)
					{
					    Bj[i][H] = 0;
					     r[i][H] = 0;
					}
					
					(*multihit)++;
			        Bj_score(score, *level, *multihit, count);
				}
			}
		}
	}
	
	for(V=0; V<=7; V++)
	{
	    for(H=0; H<=7; H++)
		{
		    if(temp[V][H] == 0 || Bj[V][H] == 0)
			{
			    Bj[V][H] = 0;
                            
			    Bj_display_part(Bj, r, H, V);
			}
		}
	}
}


/*****************************************************/
// Function "Initialization".
// It will generate gem box with random gems, rearrange them, and display all the things on screen.


void Bj_initial(uint8 Bj[8][8], uint8 r[8][8], uint32 *score, uint32 *reorder, uint32 *level)
{
    for(uint8 i=0; i<8; i++)
    {
        for(uint8 j=0; j<8; j++)
        {
            Bj[i][j] = Bj_1random7();
             r[i][j] = Bj_0random19();
        }
    }
    
    Bj_reorder(Bj);
    
    *score = 0;
    *reorder = 1;
    *level = 1;
    
    Bj_display(Bj, r, *score, *reorder, *level);
}


/*****************************************************/
// Function "Action".
// Main part of this Bejeweled program.
// It will get and judge the action in touch screen, then implement highlight, exchanging, rearrangement or restarting.


uint8 Bj_action(uint8 Bj[8][8], uint8 r[8][8], uint32 *score, uint32 *reorder, uint32 *level, uint8 *state)
{
    sint16 pX = 240, pY = 320;
    uint8  BjH, BjV, sH, sV;
    uint32 multihit = 0;
    
    if(*state < 64)
    {
        sH = *state % 8;
        sV = *state / 8;
		
        Bj_display_highlight(Bj, r, sH, sV);
    }
    if(GetpXY(&pX, &pY))
    {
        if(pY >= 80 && pY < 320)
        {
            BjH = pX2BjH(pX);
            BjV = pY2BjV(pY);
			
            if(*state >= 64)
            {
                Bj_display_highlight(Bj, r, BjH, BjV);
				
                *state = BjV * 8 + BjH;
                return(0);
            }
            
            if(*state != BjV * 8 + BjH)
            {
                sH = *state % 8;
                sV = *state / 8;
				
                if(((BjV == sV) && (BjH == sH+1 || BjH == sH-1)) || ((BjH == sH) && (BjV == sV+1 || BjH == sV-1)))
                {
                    Bj_display_exchange(Bj, r, sH, sV, BjH, BjV, 0);
                    
                    if(Bj_move(Bj, r, sH, sV, BjH, BjV, score, level, &multihit))
                    {
					    Bj_level(*score, level, reorder);
                        Bj_display_score(*score, *level);
						
                        //delayMs(250);
						
						while(Bj_drop(Bj, r))
						{
						    Bj_display_reorder(*reorder);
                            Bj_display_restart();
                            Bj_display_score(*score, *level);
							
						    while(Bj_drop(Bj, r))
							{
							    Bj_display_reorder(*reorder);
                                Bj_display_restart();
                                Bj_display_score(*score, *level);
							}
							
                            //delayMs(500);
							
						    Bj_check(Bj, r, score, level, &multihit);
							Bj_level(*score, level, reorder);
                            Bj_display_score(*score, *level);
							
                            //delayMs(250);
						}
                        
                        *state = 64;
                        return(0);
                    }
                    else
                    {
                        Bj_display_exchange(Bj, r, sH, sV, BjH, BjV, 1);
						
                        *state = 64;
                        return(0);
                    }
                }
                else
                {
                    Bj_display_highlight(Bj, r, BjH, BjV);
					
                    *state = BjV * 8 + BjH;
                    return(0);
                }
            }
            else
            {
                return(0);
            }
        }
        else
        {
			if(pX>=reorder_left && pX<=reorder_right && pY>=reorder_top && pY<=reorder_bottom)
			{
                if((*state == 100) && (*reorder > 0))
                {
                    Bj_reorder(Bj);
                    *reorder = *reorder - 1;
                                        
                    Bj_display_reorder(*reorder);
                    Bj_display_main(Bj, r);
                }
				else
				{
				    *state = 100;
					
				    delayMs(500);
				}
				
                return(0);
			}
            else if(pX>=restart_left && pX<=restart_right && pY>=restart_top && pY<=restart_bottom)
			{
				if(*state == 200)
				{
                    return(1);
			    }
				else
				{
				    *state = 200;
				    
				    delayMs(500);
				    return(0);
				}
			}
        }
    }
    
    return(0);
}


/*****************************************************/
// Top function of this Bejeweled program, including initialization, action and restarting.


void Bejeweled(unsigned int *count_seed)
{
    // Bj[Vertical][Horizontal]
    uint8  Bj[8][8], r[8][8];
    uint8  state=64;
    uint32 score, reorder, level;
    
    srand(*count_seed);

    while(1)
    {
        Bj_initial(Bj, r, &score, &reorder, &level);

        while(!Bj_action(Bj, r, &score, &reorder, &level, &state))
        {
            //delayMs(50);
        }
    }
}
