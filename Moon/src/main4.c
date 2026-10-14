// #include <avr/io.h>
// #include <util/delay.h>

// int main()
// {
//     DDRD = 0xFF; // LED가 연결된 포트는 출력 설정

//     DDRG &= ~(1 << 2); // 포트 G에 핀 번호 한 개를 입력으로 설정

//     while(1)
//     {
//         if (PING & (1 << 2)) // 포트 G의 핀 번호 한 개가 HIGH 상태인지 확인
//         {
//             PORTD = 0xFF; // HIGH 상태이면 LED를 켬
//         }
//         else
//         {
//             PORTD = 0x00; // LOW 상태이면 LED를 끔
//         }
//     }
// }