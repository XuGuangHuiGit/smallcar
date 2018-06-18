#include "stm32f10x.h"


#define LCD_DC_CLR()	GPIO_ResetBits(GPIOC,GPIO_Pin_6)
#define LCD_DC_SET()	GPIO_SetBits(GPIOC,GPIO_Pin_6)

#define LCD_RST_CLR()	GPIO_ResetBits(GPIOC,GPIO_Pin_4)
#define LCD_RST_SET()	GPIO_SetBits(GPIOC,GPIO_Pin_4)

void LCD_P8x16Str(u8 x,u8 y,u8 *ch);
void LCD_P6x8Str(u8 x,u8 y,u8 *ch);
void LCD_PutPixel(u8 x,u8 y);
void LCD_CLS(void);
void LCD_Set_Pos(u8 x, u8 y);
void LCD_Init(void) ;
void OLED_Init(void) ;
void LCD_Fill(u8 bmp_data);
void LCD_Set_Pos(u8 x, u8 y);
void LCD_WrDat(u8 data);
void LCD_WrCmd(u8 cmd);
