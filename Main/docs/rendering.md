# 부분 더블 버퍼링

ATmega128A는 SRAM이 4,096바이트다. 240×320 RGB565 화면 버퍼는 한 장에
153,600바이트, 두 장에 307,200바이트이므로 내부 SRAM에 저장할 수 없다.
메모리 사양: [Microchip 데이터시트](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-8151-8-bit-AVR-ATmega128A_Datasheet.pdf).

현재 구현은 480픽셀 RGB565 버퍼 두 개, 총 1,920바이트를 사용한다.
240픽셀 너비에서는 2줄, 104픽셀 버튼에서는 4줄을 한 띠로 처리한다.

1. back 버퍼를 지정한 배경색으로 초기화한다.
2. 배경, 테두리, 아이콘, 글자를 back 버퍼에 합성한다.
3. 이전에 완성한 front 버퍼를 TFT로 전송한다.
4. 두 버퍼의 역할을 바꾸고 다음 띠를 처리한다.
5. 마지막에 남은 front 버퍼까지 전송하고 반환한다.

버퍼는 전송이 완료된 뒤 재사용한다. 동적 메모리를 할당하지 않는다.
`DrawCanvas`는 헤더, 배경, 카드 네 개를 한 프레임에 합성한다.
이후 `DrawDirtyCanvas`는 실제로 달라진 픽셀 구간만 갱신한다. 별도의 `DrawUI` 호출은
필요하지 않다. `DrawButton`은 그리기만 수행하며 dirty 해제는 Canvas가 담당한다.

## 변경 영역 감지

각 버튼은 마지막으로 표시한 위치, 표시 여부, 색상, 아이콘, 단계 및 문자열을
`rendered`에 보관한다. `DrawDirtyCanvas`는 현재 표시 속성과 비교하므로 setter를
통하지 않은 필드 변경과 문자열 버퍼의 내용 변경도 감지한다.

라벨/제목/하단 안내 또는 풍속 막대만 바뀌면 해당 세로 영역만 비교한다.
위치, 표시 여부, 배경이나 활성 상태가 바뀌면 이전/새 카드 영역을 비교한다.
이 영역을 띠로 나눠 버퍼 0에는 이전 화면을, 버퍼 1에는 새 화면을 합성한다.
각 행에서 RGB565 값이 달라진 연속 구간만 TFT에 보낸다. 동일한 픽셀은 전송하지 않는다.
이전/새 Canvas를 동일한 순서로 합성하므로 이동·숨김·겹침 시 배경도 복원한다.

전체 프레임 버퍼는 저장하지 않는다. 두 픽셀 버퍼는 합계 1,920바이트이며,
현재 GUI 펌웨어의 정적 SRAM 사용량은 2,742바이트다. 나머지는 스택 등에서 사용한다.
카드의 label/title/hint는 각각 최대 15자를 표시하고 이전 문자열도 복사해 보관한다.
상태가 바뀌지 않으면 합성과 SPI 전송을 생략한다.
직접 TFT에 별도 화면을 그린 후 GUI로 복귀할 때는 `DrawCanvas`로 다시 동기화한다.

## 하드웨어 SPI

TFT 전송은 ATmega128A의 PB2(MOSI), PB1(SCK), PB0(SS/CS)를 사용한다.
PB0를 출력으로 유지하고 `SPCR = SPE | MSTR`, `SPSR = SPI2X`로 설정한다.
Mode 0, MSB first, CPU 16MHz 기준 SCK 8MHz이며 각 바이트의 SPIF 완료를 기다린다.
`TFT_SPI_DOUBLE_SPEED=0`으로 컴파일하면 SCK 4MHz로 낮출 수 있다.
SPIF를 읽은 다음 SPDR을 읽어 완료 플래그를 지우고, 완료 이후 CS를 해제한다.
PB4/PB5의 팬/서보 PWM과 PORTA의 터치 소프트웨어 SPI는 유지한다.
핀 배치 및 분주 설정은 위 Microchip 데이터시트의 18장/24장을 따른다.

## 카드 디자인

240×320 화면에 어두운 배경과 104×104 둥근 카드를 배치한다.
전원은 민트, 회전은 파랑, 풍속은 앰버로 강조한다.
자동 모드는 ON/OFF 표시만 전환하며 실제 자동 제어는 수행하지 않는다.
`Button`의 `title`, `hint`, `accent`, `icon`으로 카드 내용을 지정한다.
`SetButtonActive`와 `SetButtonLevel`은 상태 변경 시 자동으로 dirty 표시를 추가한다.
기타 디자인 필드를 직접 수정해도 다음 `DrawDirtyCanvas`에서 감지한다.
영문 대문자/숫자 글꼴과 기능 아이콘은 `PROGMEM`으로 플래시에 저장한다.
터치는 누른 뒤 뗄 때까지 한 번만 실행되며, 카드 사이 여백은 버튼 영역에 포함하지 않는다.

## 사용자 정의 영역

```c
static void PaintStatus(void *context)
{
    (void)context;
    DrawRect(0, 0, 240, 40, WHITE);
    DrawString(120, 12, "LEVEL 1", 2, BLACK, WHITE);
}

// x1, y1은 포함하지 않는 끝 좌표다.
TFT_RenderRegion(0, 0, 240, 40, WHITE, PaintStatus, NULL);
```

사용자 정의 화면도 `TFT_RenderChangedRegion(..., painter, before, after)`로
이전/새 상태를 전달하면 같은 방식으로 변경 픽셀만 전송할 수 있다.

콜백은 띠마다 다시 실행된다. 그리기 함수만 호출하고, 상태 변경, 라벨 변경,
입출력, 지연을 수행하지 않아야 한다. `DrawRect`, `DrawRoundRect`, `DrawChar`, `DrawString`은
현재 띠에 클리핑된다. `TFT_Data`, `TFT_Command`, `SPI_Write` 또는 CS/DC 직접
제어는 사용하지 않는다. ISR에서는 호출할 수 없고 중첩 RenderRegion 호출은 무시된다.

## 한계와 검증

실제 TFT 색상 테스트는 `src/main.c`의 `AP_RENDER_TEST`로 선택한다.
값이 `1`이면 `testInit()` / `testMain()`이 실행되며 화면 전체를
빨강 → 초록 → 파랑 → 노랑 → 흰색 → 검정 순서로 반복 표시한다.
모든 프레임은 `TFT_RenderRegion`의 부분 더블 버퍼를 통과한다.
각 프레임 전송이 끝난 뒤 500ms 유지하므로 전환 간격에는 전송 시간도 포함된다.
현재 기본값 `0`에서는 새 GUI를 사용하는 선풍기 앱이 실행된다.

하드웨어 SPI도 SPIF를 기다리는 동기식 전송이므로 합성과 전송은 순차 실행된다. 두 버퍼가 있어도
CPU와 전송이 병렬 실행되지는 않는다. 전체 화면을 원자적으로 교체하지 않으며,
패널의 스캔과 동기화하지 않으므로 화면 찢어짐 방지를 보장하지 않는다.
합성이 추가되므로 실제 속도 향상은 보드에서 측정해야 한다.

`python3 tests/run_rendering_tests.py`로 호스트 회귀 테스트를 실행한다.
바이트 전송만 테스트용 SPI 디코더로 교체하고 실제 렌더링 코드를 사용한다.
직접 렌더링과의 픽셀 일치, 픽셀 중복 전송 여부, dirty 처리, 라벨 축소,
문자 확대, 영역 클리핑, 마지막 띠와 버퍼 재사용을 검사한다.
실제 `apInit` / `apMain`에 가상 터치를 입력해 카드 상태와 길게 누르기도 검사한다.
차이 비교 테스트는 모든 픽셀을 검사하여 달라진 픽셀만 정확히 한 번 전송했는지 검증한다.
실제 SPI_Write 함수의 SPDR 기록 및 GPIO 보존도 별도로 검사한다.
호스트 테스트는 전기적인 SCK 파형/주파수를 재현하지 않으므로 보드에서 확인해야 한다.
`python3 tests/run_rendering_tests.py --preview-dir build/preview`를 실행하면 실제
렌더링 코드의 대기/동작 화면을 PPM 파일로 내보낸다.
