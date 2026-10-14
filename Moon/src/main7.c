// #include <ap.h>

// volatile uint8_t test = 0; //전역변수

// ISR(INT0_vect)
// {
//     PORTB = 0xff;
// }

// ISR(INT1_vect)
// {
//     PORTB = 0x00;
// }

// int main()
// {
//     cli(); //전역 인터럽트 비활성화 
//     DDRD &= ~((1 << PD0) | (1 << PD1)); //INT 0, 1번 입력으로 설정
    
//     PORTD &= ~((1 << PD0) | (1 << PD1)); //외부 풀업이니 내부꺼를 사용하지 않는다

//     DDRA = 0xFF;     // 불켜지는거 볼라구..
//     PORTA = 0x00;     // 초기에 꺼지게

//     EICRA |= (1<<ISC00) | (1<<ISC01) | (1<<ISC11) | (1<<ISC10); //하강엣지에서 인터럽트 발생
//     EIMSK |= (1<<INT0) | (1<<INT1);        // INT0, INT1 인터럽트 활성화

//     // 혹시 설정중에 발생할 수 있는 인터럽트 플래그 제거용
//     EIFR |= (1<<INTF0) | (1<<INTF1);



//     sei();// 전역 인터럽트 활성화 SREG |= (1 << 7); 

//     while(1)
//     {
        
//     }
// }