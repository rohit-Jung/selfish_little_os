global outb  ; make the label outb visible to the outside file

; outb:  send a byte to an IO port
; stack: [esp + 8] the data byte
;        [esp + 4] the I/O port 
;        [esp    ] return address 
outb:
	mov al, [esp + 8]   ; move the data to be sent to the al register
	mov dx, [esp + 4]   ; move the register of I/O port to the dx register 
  out dx, al          ; send the data to the port
  ret
