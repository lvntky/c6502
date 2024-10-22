
; 8a 99
; 00 02
; 48 e8
; c8 c0
; 10 d0
; f5 68
; 99 00
; 02 c8
; c0 20
; d0 f7  
  
  LDX #$00 ; a2 00
  LDY #$00 ; a0 00
firstloop:
  TXA
  STA $0200,Y
  PHA
  INX
  INY
  CPY #$10
  BNE firstloop ;loop until Y is $10
secondloop:
  PLA
  STA $0200,Y
  INY
  CPY #$20      ;loop until Y is $20
  BNE secondloop