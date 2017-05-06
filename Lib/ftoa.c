/*-----------------------------------------------
          *数据转换库函数*
    功能：将整形数据转换为字符串
          将浮点数转换为字符串
     (解决printf函数无法输出浮点数问题)
    作者：苏晨珲 东秦光电直立1队
-------------------------------------------------*/
/*
 * 包含头文件
 */
#include "common.h"
//#include <stdio.h>
//#include <string.h>
#include "ftoa.h"
/*--------------------------------------------------

函数功能： 整数转换为字符串（输入整数，返回字符串地址）
例程：             i=itoa(data);

---------------------------------------------------*/
uint8 *itoa(int32 num)
{
    int32 i=0,isNegative=0;
    static uint8 s[15];//必须是全局变量或者是静态变量

    //如果是负数，先转为正数
    if((isNegative=num) < 0) num = -num;

    //从个位开始变为字符，直到最高位，最后应该反转
    do
    {
        s[i++] = num%10 + '0';
        num /= 10;
    }while(num > 0);

    //如果是负数，补上负号
    if(isNegative < 0) s[i++] = '-';

    //最后加上字符串结束符
    s[i] = '\0';

    /******翻转字符串******/
    //p指向s的头部
    uint8 *p = s;

    //q指向s的尾部
    uint8 *q = s;
    while(*q) ++q;
    q--;

    //交换移动指针，直到p和q交叉
    uint8 temp;
    while(q > p)
    {
        temp = *p;
        *p++ = *q;
        *q-- = temp;
    }

    return s;
}
/*-----------------------------------------------------------------------

函数功能：浮点数转换为字符串（输入浮点数和小数点精度，返回字符串地址）
例程：     printf("%s",ftoa(data,2))

------------------------------------------------------------------------*/
uint8 *ftoa(float num, uint8 precision)
{
    float floatpart;
    static uint8 s[15];//必须是全局变量或者是静态变量
    uint8 i,*p=s;

    if(num >= 0)
    {
        strcpy((int8 *)s,(const int8 *)itoa((int32)num));//转换整数部分
        floatpart = num;//取出小数部分（小于1的小数）
    }
    else if(num > -1)
    {
        strcpy((int8 *)s,"-0");
        floatpart = -num;
    }
    else
    {
        strcpy((int8 *)s,(const int8 *)itoa((int32)num));//转换整数部分
        floatpart = -num;//取出小数部分（小于1的小数）
    }

    while(*p) ++p;//指向字符串结束
    *(p++) = '.';//加上小数点

    for(i=0; i<precision; i++)
    {
        floatpart *= 10;
        *(p++) = (int32)floatpart%10 + '0';//转换小数部分
    }
    *p = '\0';

    return s;
}