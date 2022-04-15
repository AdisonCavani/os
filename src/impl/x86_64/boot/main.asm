global start

section .text
bits 32
start:
  ; print green `OK` to screen
  mov dword [0xb8000], 0x2f4b2f4f ; 2f = green
  hlt