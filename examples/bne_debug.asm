    LDX #$05      ; Load the X register with 5
Loop:
    DEX           ; Decrement the X register
    STA X
    BNE Loop      ; If X is not zero, branch back to 'Loop'
    ; Continue here when X reaches zero
    BRK           ; Break - end of program
