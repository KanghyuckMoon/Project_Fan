# 부분 더블 버퍼링

ATmega128A는 SRAM이 4,096바이트다. 240×320 RGB565 화면 버퍼는 한 장에
153,600바이트, 두 장에 307,200바이트이므로 내부 SRAM에 저장할 수 없다.
메모리 사양: [Microchip 데이터시트](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-8151-8-bit-AVR-ATmega128A_Datasheet.pdf).

현재 구현은 480픽셀 RGB565 버퍼 두 개, 총 1,920바이트를 사용한다.
240픽셀 너비에서는 2줄, 120픽셀 버튼에서는 4줄을 한 띠로 처리한다.

1. back 버퍼를 지정한 배경색으로 초기화한다.
2. 배경, 테두리, 글자, 구분선을 back 버퍼에 합성한다.
3. 이전에 완성한 front 버퍼를 TFT로 전송한다.
4. 두 버퍼의 역할을 바꾸고 다음 띠를 처리한다.
5. 마지막에 남은 front 버퍼까지 전송하고 반환한다.

버퍼는 전송이 완료된 뒤 재사용한다. 동적 메모리를 할당하지 않는다.
기존 dirty 버튼 갱신을 유지하며 `DrawCanvas` / `DrawDirtyCanvas`가 각 버튼의
완성된 픽셀만 보낸다. 구분선도 같은 버퍼에 합성하므로 별도의 `DrawUI` 호출은
필요하지 않다. `DrawButton`은 그리기만 수행하며 dirty 해제는 Canvas가 담당한다.

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

콜백은 띠마다 다시 실행된다. 그리기 함수만 호출하고, 상태 변경, 라벨 변경,
입출력, 지연을 수행하지 않아야 한다. `DrawRect`, `DrawChar`, `DrawString`은
현재 띠에 클리핑된다. `TFT_Data`, `TFT_Command`, `SPI_Write` 또는 CS/DC 직접
제어는 사용하지 않는다. ISR에서는 호출할 수 없고 중첩 RenderRegion 호출은 무시된다.

## 한계와 검증

실제 TFT 색상 테스트는 `src/main.c`의 `AP_RENDER_TEST`로 선택한다.
기본값 `1`에서는 `testInit()` / `testMain()`이 실행되며 화면 전체를
빨강 → 초록 → 파랑 → 노랑 → 흰색 → 검정 순서로 반복 표시한다.
모든 프레임은 `TFT_RenderRegion`의 부분 더블 버퍼를 통과한다.
각 프레임 전송이 끝난 뒤 500ms 유지하므로 전환 간격에는 전송 시간도 포함된다.
기존 선풍기 동작으로 돌아가려면 `AP_RENDER_TEST`를 `0`으로 변경한다.

소프트웨어 SPI를 사용하므로 합성과 전송은 순차 실행된다. 두 버퍼가 있어도
CPU와 전송이 병렬 실행되지는 않는다. 전체 화면을 원자적으로 교체하지 않으며,
패널의 스캔과 동기화하지 않으므로 화면 찢어짐 방지를 보장하지 않는다.
합성이 추가되므로 실제 속도 향상은 보드에서 측정해야 한다.

`python3 tests/run_rendering_tests.py`로 호스트 회귀 테스트를 실행한다.
GPIO 전송만 테스트용 SPI 디코더로 교체하고 실제 렌더링 코드를 사용한다.
직접 렌더링과의 픽셀 일치, 픽셀 중복 전송 여부, dirty 처리, 라벨 축소,
문자 확대, 영역 클리핑, 마지막 띠와 버퍼 재사용을 검사한다.
