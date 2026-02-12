.text
.global req1

req1:
    bl led0On
    bl led0Off
    bl led0Toggle
    bl led0Toggle
    bx lr