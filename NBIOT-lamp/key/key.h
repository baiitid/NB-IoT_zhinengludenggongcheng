
#ifndef __KEY_H
#define __KEY_H

#define	KEY2_GPIO	GPIOC
#define	KEY2_GPIO_PIN	GPIO_PIN_13
#define	KEY3_GPIO	GPIOB
#define	KEY3_GPIO_PIN	GPIO_PIN_1
#define	KEY4_GPIO	GPIOB
#define	KEY4_GPIO_PIN	GPIO_PIN_5

#define KEY2 2
#define KEY3 3
#define KEY4 4

#define KEY_UP			1
#define KEY_DOWN		0

void keys_init(void);
unsigned char key_even(unsigned char keyNum);


#endif /* __KEY_H */
