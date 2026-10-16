// #include "button.h"



// Button btnSpeed;
// Button btnRotate;
// uint8_t speed = 0;
// uint8_t rotate = 0;

// void ButtonSystemInit()
// {
//     // 버튼 1 : 풍속 변경
//     buttonInit(&btnSpeed, &BUTTON_DDR, &BUTTON_PIN, BUTTON_ON);

//     // 버튼 2 : 회전 ON / OFF
//     buttonInit(&btnRotate, &BUTTON_DDR, &BUTTON_PIN, BUTTON_OFF);
// }

// void buttonInit(Button *button, volatile uint8_t *ddr, volatile uint8_t *pin, uint8_t pinNum)
// {
//     button->ddr = ddr;
//     button->pin = pin;
//     button->btnPinnumber = pinNum;
//     button->prevState = RELEASED;
//     *button->ddr &= ~(1 << button->btnPinnumber);
// }

// uint8_t buttonGetState(Button *button)
// {
//     static uint8_t debounceCount = 0;

//     uint8_t curState = *button->pin & (1 << button->btnPinnumber);

//     // 현재 상태와 이전 확정 상태가 같으면 디바운스 초기화
//     if (curState == button->prevState)
//     {
//         debounceCount = 0;
//         return NO_ACT;
//     }

//     // 상태가 달라진 상태가 50ms 동안 유지되는지 확인
//     debounceCount++;

//     if (debounceCount < 50)
//     {
//         return NO_ACT;
//     }

//     debounceCount = 0;

//     // 50ms 동안 상태 변화가 유지됨 → 실제 상태 변경으로 확정
//     if (curState == PUSHED)
//     {
//         button->prevState = PUSHED;
//         return ACT_PUSH;
//     }
//     else
//     {
//         button->prevState = RELEASED;
//         return ACT_RELEASE;
//     }
// }

// void ButtonMain()
// {

// }