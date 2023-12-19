/* LED CGI Program
 *
 *
 * Note :
 *   PORT NAME[PIN] = GPIO [id]	
 *   PORTA[ 0]      = gpio[ 0x00]
 *   PORTA[ 1]      = gpio[ 0x01]	  
 *                  :
 *   PORTA[31]      = gpio[ 0x1F]
 *   PORTB[ 0]      = gpio[ 0x20]
 *                  :
 *   PORTB[31]      = gpio[ 0x3F]
 *                  :
 *                  :
 *                  :
 *   PORTI[ 0]      = gpio[ 0xC0]
 *                  :
 *                  :
 *   PORTI[31]      = gpio[ 0xDF]
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#if 0
//For NK-980IOT board
#define LED1_NUM	0x28 //PB8--->LED3
#define LED2_NUM	0x2D //PB13--->LED5
#endif

//For chili board
#define LED1_NUM	0x4B //PC11--->LED1
#define LED2_NUM	0x43 //PC3--->LED2

#if 0		
//For NuDesign-EtherU and NuDesign-EtherD
#define LED1_NUM	0x6C //PD12
#define LED2_NUM	0x6D //PD13
#define LED3_NUM	0x6E //PD14
#endif	

void SetLedValue(int dev_num, int value)
{
	FILE *fp;
    char str[256];
	int gpio_number = LED2_NUM + dev_num*8;

	if(dev_num >1 || value > 1 || value <0)
		return;
	sprintf(str,"/sys/class/gpio/gpio%d/direction", gpio_number);
	printf("%s\n",str);
	if ((fp = fopen(str, "w")) == NULL) {
		printf("1\n");
		//linux equivalent code "echo pin_num > export" to export the pin
		if ((fp = fopen("/sys/class/gpio/export", "w")) == NULL) {
			printf("Cannot open export file.\n");
			exit(1);
		}
		fprintf(fp, "%d", gpio_number);
		fclose(fp);

		//linux equivalent code "echo out > direction" to set the pin as an output  
		sprintf(str,"/sys/class/gpio/gpio%d/direction",gpio_number);
		printf("%s\n",str);
		if ((fp = fopen(str, "w")) != NULL) {
			fprintf(fp, "out");
			fclose(fp);
		}
	}
	else{
		printf("%s exist \n",str);
		fclose(fp);
	}

	sprintf(str,"/sys/class/gpio/gpio%d/value",gpio_number);
	if ((fp = fopen(str, "w")) != NULL) {
		printf("open %s value = %d\n",str,value);
		fprintf(fp, "%d", value);
		fclose(fp);
	}
}

// int main(){
// 	int n, value;
// 	scanf("%d %d",&n,&value);
// 	SetLedValue(n,value);
// }
