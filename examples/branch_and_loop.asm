    LDX #$00         ; Initialize X register to 0
    LDY #$00         ; Initialize Y register to 0
loop_start:
    TXA              ; Transfer X to A (Accumulator)
    STA $0200,Y      ; Store the value of A (X) at address $0200 + Y
    INX              ; Increment X
    INY              ; Increment Y
    CPX #$0A         ; Compare X with 10 (decimal)
    BNE loop_start   ; If X is not equal to 10, branch back to loop_start
done:
    BRK              ; Break - halt execution
