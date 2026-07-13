global outb  ; make the label outb visible to the outside file

; outb:  send a byte to an IO port
; stack: [esp + 8] the data byte
;        [esp + 4] the I/O port 
;        [esp    ] return address 
outb:
	mov al, [esp + 8]   ; move the data to be sent to the al register
	mov dx, [esp + 4]   ; move the address of I/O port to the dx register 
  out dx, al          ; send the data to the port
  ret                 ; return to the calling function

global inb  ; make the label inb visible to the outside file

; inb:   returns a byte from given IO port
; stack: [esp + 4] The address of the I/O port 
;        [esp    ] The return address 
inb:
	mov dx, [esp + 4]   ; move the address of I/O port to the dx register 
  in  al, dx          ; read a byte from I/O port and store it in the al register    
  ret                 ; return the read byte
